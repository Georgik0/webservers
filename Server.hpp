#ifndef __SERVER_HPP
#define __SERVER_HPP

#include "Socket.hpp"
#include <string>
#include <map>
#include <vector>

enum Methods {
    GET,
    POST,
    DELETE,
    PUT,
};

#define QUEUE 10

typedef struct location_s {
    std::string                 path;
    std::string                 root;
    std::string                 cgi_pass;
    std::string                 _return;
    std::string                 index;
    int                         client_max_body_size;
    bool                        autoindex;
    bool                        file_upload;
    std::vector<bool>           methods;
}               location_t;

typedef struct answer_s {
    std::string     header;
    std::string     path;
}               answer_t;

class Server {
    private:
        Socket                          _server;
        std::map<int, std::string>      _err_pages;
        std::vector<location_t>         _locations;
        int                             _cli_max_body_size;
        std::string                     _listen;
        std::string                     _port;

    public:
        std::string                     mime_conf_path;
        std::string                     server_name;
        static std::map<Socket, std::string>   _conn_cli;
        static std::vector<std::string> _env_vars;

    public:
        Server();
        Server(const Server& other);
        Server& operator=(const Server& other);
        void    SocketListen(const std::string& host, const std::string& port);
        const Socket& getSocket() const;
        void    PBLocation(const location_t& location);
        void    PBErrPages(const std::string& err_page);
        Socket  Accept();
        std::string&    operator[](const int err_code);
        bool        find(const Socket& client);
        int GetClientMaxBodySize() const;
        void SetClientMaxBodySize(size_t size);
        ~Server();
};

bool operator<(const Server& lhs, const Server& rhs);
std::vector<Server> parse_config(const std::string& configfile);

#endif  // __SERVER_HPP
