#ifndef _PARSER_CONFIG_HPP_
#define _PARSER_CONFIG_HPP_

#include <string>
#include <iostream>
#include <vector>

typedef std::string str;
typedef std::vector<std::string> vec_str;
typedef std::vector<bool> vec_bool;

enum Methods
{
    GET,
    POST,
    DELETE,
};

class ParserConfig
{
private:
public:
    // for each server
    int     listen;
    str     host;
    str     server_name;
    vec_str error_pages;

    //for each location
    str     location;
    str     root;
    str     index;
    bool     autoindex;
    str     post_body;
    str     cgi;
    str     cgi_ext;
    vec_bool methods;


public:
    ParserConfig();
    void setRoot(const std::string &r);
    void clear_loc();
    void clear_serv();
    ParserConfig(const ParserConfig &p);
    ParserConfig& operator= (const ParserConfig &p);
    ~ParserConfig();
};

ParserConfig::ParserConfig():cgi(), methods(), listen(0), host(), server_name(), error_pages(),
    location(), root(), index(), autoindex(), post_body(), cgi_ext()
{
    methods.reserve(3);
    methods.push_back(false);
    methods.push_back(false);
    methods.push_back(false);
}

ParserConfig::ParserConfig(const ParserConfig &p):
    listen(p.listen), host(p.host),
    server_name(p.server_name), error_pages(p.error_pages),
    root(p.root),cgi(p.cgi), methods(p.methods),
    location(p.location), index(p.index), autoindex(p.autoindex), post_body(p.post_body), cgi_ext(p.cgi_ext)
{
}

ParserConfig& ParserConfig::operator= (const ParserConfig &p)
{
    if (this == &p)
        return *this;

    listen = p.listen;
    host = p.host;
    server_name = p.server_name;
    error_pages = p.error_pages;

    return *this;
}


ParserConfig::~ParserConfig()
{
}

void ParserConfig::setRoot(const std::string &r)
{
    root = r;
}
void ParserConfig::clear_loc()
{
    std::string t;
    post_body = t;
    cgi = t;
    vec_str v;
    vec_bool bV ;
    bV.reserve(3);
    bV.push_back(false);
    bV.push_back(false);
    bV.push_back(false);
    methods = bV;
    location = t;
    root = t;
    index = t;
    autoindex = false;
    cgi_ext = t;
}
void ParserConfig::clear_serv()
{
    std::string t;
    listen = 0;
    host = t;
    server_name = t;
    vec_str v;
    error_pages = v;
}

#endif
