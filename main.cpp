#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Select.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "answer.hpp"
#include "header.hpp"

#define QUEUE 10

void getAutoIndex(const std::string &path, const std::string &uri_path);

void clear_resourse(set_t& input, set_t& output, Socket& sock,
                    Select& selector) {
    shutdown(sock.get_fd(), SHUT_RDWR);
    sock.Close();
    selector.Remove(sock, READ);
    selector.Remove(sock, WRITE);
    if (input.find(sock) != input.end()) {
        input.erase(sock);
    }
    if (output.find(sock) != output.end()) {
        output.erase(sock);
    }
}

int check_serv(const Socket& sock, const std::vector<Server>& servers) {
    int i;
    for (i = 0; i < servers.size(); i++) {
        if (sock.get_fd() == servers[i].getSocket().get_fd()) return i;
    }
    return -1;
}


char** set_http_env_vars(std::map<std::string, std::string>& http_req, const Server& server) {
    std::vector<std::string> env_vars = Server::_env_vars;
    std::string tmp;

    tmp = "SERVER_PROTOCOL=HTTP/1.1";
    env_vars.push_back(tmp);
    tmp = "REQUEST_METHOD=" + http_req["Type"];
    env_vars.push_back(tmp);
    tmp = "HTTP_VERSION=" + http_req["Version"];
    env_vars.push_back(tmp);
    tmp = "SERVER_NAME=" +
          http_req["Host"].substr(0, http_req["Host"].find(":"));
    env_vars.push_back(tmp);
    tmp = "SERVER_PORT=" +
          http_req["Host"].substr(http_req["Host"].find(":") + 1,
                                  http_req["Host"].length());
    env_vars.push_back(tmp);
    tmp = "HTTP_CONNECTION=" + http_req["Connection"];
    env_vars.push_back(tmp);
    tmp = "PATH_INFO=" + http_req["Path"];
    env_vars.push_back(tmp);
    tmp = "HTTP_USER_AGENT=" + http_req["User-Agent"];
    env_vars.push_back(tmp);
    tmp = "HTTP_ACCEPT=" + http_req["Accept"];
    env_vars.push_back(tmp);
    tmp = "HTTP_CONTENT_TYPE=" + http_req["Content-Type"];
    env_vars.push_back(tmp);
    tmp = "HTTP_CONTENT_LENGTH=" + http_req["Content-Length"];
    env_vars.push_back(tmp);
    tmp = "QUERY_STRING=" + http_req["Body"];
    env_vars.push_back(tmp);
    tmp = "CLI_MAX_BODY_SIZE=" + ft::to_string(server.GetClientMaxBodySize());
    env_vars.push_back(tmp);
    /* add QUERY_STRING */
    int size = env_vars.size();
    char** env = new char*[size + 1];
    env[size] = NULL;
    for (int i = 0; i < size; i++) {
        env[i] = const_cast<char*>(env_vars[i].c_str());
    }
    return env;
}

std::string    handle_client_request(const std::string& recv, const Server& server) {
    std::map<std::string, std::string> http_req = parse_header(recv, const_cast<char *>(&recv[recv.length()]));
    char** env = set_http_env_vars(http_req, server);
    int pid = fork();
    if (pid == 0) {
        int fd = open("output.txt", O_TRUNC | O_CREAT | O_WRONLY, 0777);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execve("./cgi/cgi", NULL, env);
    }
    delete [] env;
    while (wait(NULL) > 0);
    std::ifstream   infile("output.txt");
    std::string response;
    for (std::string line; std::getline(infile, line); ) {
        response.append(line + "\r\n");
    }
    return response;
}

void handle_readables(const set_t& to_read, std::vector<Server>& servers,
                      set_t& input, set_t& output, Select& selector) {
    std::string recv;
    set_t::iterator it;
    Socket client;
    int i;
    for (it = to_read.begin(); it != to_read.end(); it++) {
        if ((i = check_serv(*it, servers)) >= 0) {
            client = servers[i].Accept();
            // std::cout << "<- New cliend connected with fd " << client.get_fd()
            //           << std::endl;
            input.insert(client);
        } else {
            Socket tmp_cli = *it;
            try {
                recv = tmp_cli.Recv();
                std::cout << "<- Recv data from client " << it->get_fd() << std::endl;
                std::cout << recv << "\n";
            } catch (std::exception& ex) {
                std::cout << ex.what() << std::endl;
                clear_resourse(input, output, tmp_cli, selector);
                break;
            }
            Server::_conn_cli[tmp_cli] = handle_client_request(recv, servers[check_serv(tmp_cli, servers)]);
            if (recv.length()) {
                if (output.find(tmp_cli) == output.end()) {
                    output.insert(tmp_cli);
                }
            }
        }
    }
}

void handle_writeables(const set_t& to_write, set_t& output, set_t& input,
                       Select& selector) {
    set_t::iterator it;
    Socket tmp_cli;
    std::ifstream file;
    std::string line;
    for (it = to_write.begin(); it != to_write.end(); it++) {
        tmp_cli = *it;
        try {
            // std::cout << "-> Sending data to client " << it->get_fd() << std::endl;
            tmp_cli.Send(Server::_conn_cli[tmp_cli]);
            output.erase(tmp_cli);
            selector.Remove(tmp_cli, WRITE);
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
            clear_resourse(input, output, tmp_cli, selector);
        }
    }
}

void server_shutdown(int s) {
    std::cout << "Server is shutting down..." << std::endl;
    exit(1);
}

std::vector<std::string> parse_envp(char** envp) {
    std::vector<std::string> env_vec;
    for (int i = 0; envp[i]; i++) {
        env_vec.push_back(envp[i]);
    }
    return env_vec;
}

int main(int argc, char** argv, char** envp) {
    if (argc < 2) {
        std::cout << "[Usage]: ./webserv config_file" << std::endl;
        return 1;
    }
    getAutoIndex("/Users/vlados_paperos/Documents/21school/webserv", "/Users/vlados_paperos/Documents/21school/webserv");
    std::string configname(argv[1]);
    std::vector<Server> servers;
    try {
        servers = parse_config(configname);
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    Server::_env_vars = parse_envp(envp);
    Select selector;
    set_t input, output;
    std::pair<set_t, set_t> ready;
    struct sigaction sigpipe = {SIG_IGN};
    sigaction(SIGPIPE, &sigpipe, NULL);

    for (std::vector<Server>::iterator start = servers.begin();
         start != servers.end(); start++) {
        input.insert(start->getSocket());
    }
    std::cout << "Server started. Waiting for clients..." << std::endl;
    while (!input.empty()) {
        ready = selector.Selector(input, output);
        handle_readables(ready.first, servers, input, output, selector);
        handle_writeables(ready.second, output, input, selector);
    }
    return 0;
}
