#include "Server.hpp"
#include "Socket.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <utility>

std::map<Socket, std::string> Server::_conn_cli;
std::vector<std::string> Server::_env_vars;

// Initiate server socket for future
Server::Server() : _server() {

}

// Copy all data
Server::Server(const Server& other) :
    _server(other._server),
    _err_pages(other._err_pages),
    _locations(other._locations) {

}

Server& Server::operator=(const Server& other) {
    if (this == &other)
        return *this;
    _server = other._server;
    _err_pages = other._err_pages;
    _locations = other._locations;
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
    _conn_cli.insert(std::make_pair(client, ""));
    return (client);
}

std::string& Server::operator[](const int err_code) {
    return (_err_pages[err_code]);
}

int Server::GetClientMaxBodySize() const {
    return _cli_max_body_size;
}

void Server::SetClientMaxBodySize(size_t size) {
    _cli_max_body_size = size;
}

Server::~Server() { }

bool operator<(const Server& lhs, const Server& rhs) { return lhs.getSocket() < rhs.getSocket(); }
