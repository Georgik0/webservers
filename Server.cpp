#include "Server.hpp"
#include "Socket.hpp"
#include "header.hpp"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <sys/fcntl.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <fstream>

std::map<Socket, std::string> Server::_cli_ans;
std::map<Socket, int> Server::_cli_pids;
std::vector<std::string> Server::_env_vars;
void getAutoIndex(const std::string &path, const std::string &uri_path);
// Initiate server socket for future
Server::Server() : _server() {

}

// Copy all data
Server::Server(const Server& other) :
    _server(other._server),
    _err_pages(other._err_pages),
    _locations(other._locations), 
    _cli_max_body_size(other._cli_max_body_size),
    _clients(other._clients),
    _listen(other._listen),
    _port(other._port) {
}

Server& Server::operator=(const Server& other) {
    if (this == &other)
        return *this;
    _server = other._server;
    _err_pages = other._err_pages;
    _locations = other._locations;
    _cli_max_body_size = other._cli_max_body_size;
    _listen = other._listen;
    _port = other._port;
    _clients = other._clients;
    return *this;
}

// Making socket to listen
void    Server::SocketListen(const std::string& host, const std::string& port) {
    // Запомнить на будущее?
    _listen = host;
    _port = port;
    try {
        _server.Setsockopt();
        _server.Bind(host, port);
        _server.Listen(QUEUE);
    } catch (...) {
        throw;
    }
}

const Socket& Server::getSocket() const {
    return _server;
}

void    Server::PBLocation(const location_t& location) {
    _locations.push_back(location);
}

Socket    Server::Accept() {
    Socket  client;
    try {
        client = _server.Accept();
    } catch (...) {
        throw;
    }
    _cli_ans.insert(std::make_pair(client, ""));
    _clients.insert(client);
    return (client);
}

std::string& Server::operator[](const int err_code) {
    return (_err_pages[err_code]);
}

bool  Server::find(const Socket& client) const {
    if (_clients.find(client) != _clients.end())
        return true;
    return false;
}


int Server::GetClientMaxBodySize() const {
    return _cli_max_body_size;
}

void Server::SetClientMaxBodySize(size_t size) {
    _cli_max_body_size = size;
}

const location_t& Server::_GetLocation(const std::string& path) const {
    for (int i = 0; i < _locations.size(); i++) {
        if (path.compare(_locations[i].path) == 0) {
            return _locations[i];
        }
    }
    return _locations[0];
}

void   Server::_SetLocation(std::vector<std::string>& env_vars, const std::string& path, std::map<std::string, std::string>& http_req) {
    location_t loc = _GetLocation(path);
    std::string tmp;
    tmp = "LOC_ROOT=" + loc.root;
    env_vars.push_back(tmp);
    tmp = "GET=" + std::string((loc.methods[GET] ? "ON" : "OFF"));
    env_vars.push_back(tmp);
    tmp = "POST=" + std::string((loc.methods[POST] ? "ON" : "OFF"));
    env_vars.push_back(tmp);
    tmp = "DELETE=" + std::string((loc.methods[DELETE] ? "ON" : "OFF"));
    env_vars.push_back(tmp);
    tmp = "AUTOINDEX=" + std::string((loc.autoindex ? "ON" : "OFF"));
    env_vars.push_back(tmp);
    if (loc.autoindex) {
        getAutoIndex(http_req["Path"], http_req["Path"]);
        tmp = std::string("PATH_INFO=") + std::string("/auto.html");
    } else {
        tmp = "PATH_INFO=" + http_req["Path"];
    }
    env_vars.push_back(tmp);
    tmp = "FILE_UPLOAD=" + std::string((loc.file_upload ? "ON" : "OFF"));
    env_vars.push_back(tmp);
    tmp = "INDEX_HTML=" + loc.index;
    env_vars.push_back(tmp);
}

char** Server::_SetEnv(std::map<std::string, std::string>& http_req) {
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
   // tmp = "PATH_INFO=" + http_req["Path"];
    //env_vars.push_back(tmp);
    tmp = "HTTP_USER_AGENT=" + http_req["User-Agent"];
    env_vars.push_back(tmp);
    tmp = "HTTP_ACCEPT=" + http_req["Accept"];
    env_vars.push_back(tmp);
    tmp = "HTTP_CONTENT_TYPE=" + http_req["Content-Type"];
    env_vars.push_back(tmp);
    tmp = "HTTP_CONTENT_LENGTH=" + http_req["Content-Length"];
    env_vars.push_back(tmp);
    // tmp = "QUERY_STRING=" + http_req["Body"];
    env_vars.push_back(tmp);
    tmp = "CLI_MAX_BODY_SIZE=" + ft::to_string(GetClientMaxBodySize());
    env_vars.push_back(tmp);
    for (std::map<int, std::string>::iterator it = _err_pages.begin(); it != _err_pages.end(); it++) {
        tmp = ft::to_string(it->first) + "=" + it->second;
        env_vars.push_back(tmp);
    }
    _SetLocation(env_vars, http_req["Path"], http_req);
    int size = env_vars.size();
    char** env = new char*[size + 1];
    env[size] = NULL;
    for (int i = 0; i < size; i++) {
        env[i] = strdup(env_vars[i].c_str()); 
        // std::cout << env[i] << "\n";
    }
    return env;
}

void  Server::HandleCliReq(const std::string& recv, const Socket& client) {
    std::string file("client_" + ft::to_string(client.get_fd()));
    int pid = fork();
    if (pid == 0) {
        std::map<std::string, std::string> http_req = parse_header(recv, const_cast<char *>(&recv[recv.length()]));
        std::string body("body_" + ft::to_string(client.get_fd()));
        int fdb = open(body.c_str(), O_TRUNC | O_CREAT | O_RDWR, 0777);
        write(fdb, http_req["Body"].c_str(), http_req["Body"].length());
        dup2(fdb, STDIN_FILENO);
        close(fdb);
        char** env = _SetEnv(http_req);
        int fd = open(file.c_str(), O_TRUNC | O_CREAT | O_WRONLY, 0777);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execve("./cgi/cgi", NULL, env);
    }
    _cli_pids[client] = pid;
    _cli_ans[client] = file;
}

Server::~Server() { }

bool operator<(const Server& lhs, const Server& rhs) { return lhs.getSocket() < rhs.getSocket(); }
