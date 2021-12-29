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

int IsServer(const Socket& sock, const std::vector<Server>& servers) {
    int i;
    for (i = 0; i < servers.size(); i++) {
        if (sock == servers[i].getSocket())
            return i;
    }
    return -1;
}

Server& ClientInServ(const Socket& sock, const std::vector<Server>& servers) {
    int i;
    for (i = 0; i < servers.size(); i++) {
        if (servers[i].find(sock))
            break;
    }
    return (const_cast<Server&>(servers[i]));
}

void handle_readables(const set_t& to_read, std::vector<Server>& servers,
                      set_t& input, set_t& output, Select& selector) {
    std::string recv;
    set_t::iterator it;
    Socket client;
    int i;
    for (it = to_read.begin(); it != to_read.end(); it++) {
        if ((i = IsServer(*it, servers)) >= 0) {
            client = servers[i].Accept();
            std::cout << "<- New cliend connected with fd " << client.get_fd()
                      << std::endl;
            input.insert(client);
        } else {
            Socket tmp_cli = *it;
            try {
                recv = tmp_cli.Recv();
                // std::cout << "<- Recv data from client " << it->get_fd() << std::endl;
                // std::cout << recv << "\n";
            } catch (std::exception& ex) {
                std::cout << ex.what() << std::endl;
                clear_resourse(input, output, tmp_cli, selector);
                // continue;
            }
            ClientInServ(tmp_cli, servers).HandleCliReq(recv, tmp_cli);
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
    std::string file;
    std::string header;
    Socket tmp_cli;
    for (it = to_write.begin(); it != to_write.end(); it++) {
        tmp_cli = *it;
        waitpid(Server::_cli_pids[tmp_cli], NULL, 0);
        std::ifstream   answer(Server::_cli_ans[tmp_cli].c_str());
        std::string     line;
        try {
            // std::cout << "-> Sending data to client " << it->get_fd() << std::endl;
            while (std::getline(answer, line)) {
                tmp_cli.Send(line + "\r\n");
                if (line.find("Connection") != std::string::npos) { /* последний заголовок */
                    // std::getline(answer, line);
                    tmp_cli.Send("\r\n");
                }
            }
            tmp_cli.Send("\0");
            // tmp_cli.Send("\r\n");
            // char buf[1000];
            // int n, sum_byte = 0;
            // while (answer) {
            //     answer.read(buf, 999);
            //     buf[answer.gcount()] = '\0';
            //     // if (answer.gcount() < 1000) {
            //     //     buf[answer.gcount() + 1] = '\0';
            //     // }
            //     tmp_cli.Send(buf);
            // }
            
            output.erase(tmp_cli);
            selector.Remove(tmp_cli, WRITE);
            answer.close();
            // pause();
            remove(file.c_str());
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
            clear_resourse(input, output, tmp_cli, selector);
        }
    }
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
