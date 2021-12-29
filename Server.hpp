#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <set>
#include <string>
#include <map>
#include <vector>
#include <fcntl.h>

#include "Socket.hpp"

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

class Server {
    private:
        Socket                          _server;
        std::vector<location_t>         _locations;
        std::map<int, std::string>      _err_pages;
        int                             _cli_max_body_size;
        std::string                     _listen;
        std::string                     _port;
        std::string                     CgiPhp;
        std::string                     CgiTxt;
        std::set<Socket>                _clients;
        char** _SetEnv(std::map<std::string, std::string>& http_req);
        void   _SetLocation(std::vector<std::string>& env_vars, const std::string& path, std::map<std::string, std::string>& http_req);
        const location_t& _GetLocation(const std::string& path) const;

    public:
        std::string                             mime_conf_path; // ???
        std::string                             server_name;    // ???
        static std::map<Socket, std::string>    _cli_ans;
        static std::map<Socket, int>            _cli_pids;
        static std::vector<std::string>         _env_vars;

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
        bool  find(const Socket& client) const;
        int GetClientMaxBodySize() const;
        void SetClientMaxBodySize(size_t size);
        void  HandleCliReq(const std::string& recv, const Socket& client);
        void setCgiPhp(std::string const & path);
        void setCgiTxt(std::string const & path);
        std::string const &getCgiPhp() const;
        std::string const &getCgiTxt() const;
        ~Server();

};

bool operator<(const Server& lhs, const Server& rhs);
std::vector<Server> parse_config(const std::string& configfile);

#endif  // __SERVER_HPP
