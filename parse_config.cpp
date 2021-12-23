#include <fstream>
#include <string>
#include <vector>

#include "Server.hpp"
#include "header.hpp"

typedef std::vector<std::string> vec_str;

void check_braces(const vec_str& v) {
    int count = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] == "{")
            count++;
        else if (v[i] == "}")
            count--;
        if (count > 2 || count < -2) {
            std::cout << "error with brackets\n";
            exit(1);
        }
    }
    if (count != 0) {
        std::cout << "error with brackets\n";
        exit(1);
    }
}

std::vector<Server> parse_config(const std::string& configfile) {
    std::vector<Server> v_serv;
    std::fstream config(configfile);
    if (config.is_open()) {
        std::string s_config;
        std::vector<std::string> v_config;

        while (config >> s_config) {
            if (s_config[0] != '#') v_config.push_back(s_config);
        }

        for (size_t i = 0; i < v_config.size(); i++) {
            if (v_config[i].back() == ';') v_config[i].pop_back();
        }

        check_braces(v_config);

        for (size_t i = 0; i < v_config.size(); i++) {
            if (v_config[i] == "server") {
                //данные для сервера
                i += 2;
                Server serv;
                std::string listen, port;
                while (i < v_config.size() &&
                       (v_config[i] != "location" && v_config[i] != "}")) {
                    if (v_config[i] == "listen") {
                        listen = v_config[++i];
                    }
                    if (v_config[i] == "port") {
                        port = v_config[++i];
                    }
                    if (v_config[i] == "error_page") {
                        ++i;
                        int code_err = std::stoi(v_config[i]);
                        std::string pat_err = v_config[++i];
                        serv[code_err] = pat_err;
                    }
                    if (v_config[i] == "client_max_body_size")
                        serv.SetClientMaxBodySize(std::stoi(v_config[++i]));
                    if (v_config[i] == "mime_conf_path")
                        serv.mime_conf_path = v_config[++i];
                    if (v_config[i] == "server_name")
                        serv.server_name = v_config[++i];
                    ++i;
                }
                try {
                    serv.SocketListen(listen, port);
                } catch (...) {
                    throw;
                }

                //данные для локаций
                while (i < v_config.size() && v_config[i] != "server") {
                    location_t v_loc;

                    v_loc.methods.reserve(4);
                    v_loc.methods.push_back(false);
                    v_loc.methods.push_back(false);
                    v_loc.methods.push_back(false);
                    v_loc.methods.push_back(false);
                    v_loc.autoindex = true;
                    v_loc.file_upload = false;
                    v_loc.path = "";
                    v_loc.root = "";
                    v_loc.cgi_pass = "";
                    v_loc._return = "";
                    v_loc.index = "";

                    if (v_config[i] == "location") {
                        v_loc.path = v_config[++i];
                        i += 2;
                        while (v_config[i] != "}") {
                            if (v_config[i] == "root")
                                v_loc.root = v_config[++i];
                            if (v_config[i] == "client_max_body_size")
                                v_loc.client_max_body_size =
                                    std::stoi(v_config[++i]);
                            if (v_config[i] == "autoindex") {
                                if (v_config[++i] == "on")
                                    v_loc.autoindex = true;
                                else
                                    v_loc.autoindex = false;
                            }
                            if (v_config[i] == "file_upload") {
                                if (v_config[++i] == "on")
                                    v_loc.file_upload = true;
                                else
                                    v_loc.file_upload = false;
                            }
                            if (v_config[i] == "return")
                                v_loc._return = v_config[++i];
                            if (v_config[i] == "cgi_pass")
                                v_loc.cgi_pass = v_config[++i];
                            if (v_config[i] == "index")
                                v_loc.index = v_config[++i];
                            if (v_config[i] == "methods") {
                                ++i;
                                while (v_config[i] == "GET" ||
                                       v_config[i] == "POST" ||
                                       v_config[i] == "DELETE" ||
                                       v_config[i] == "PUT") {
                                    if (v_config[i] == "GET")
                                        v_loc.methods[GET] = true;
                                    if (v_config[i] == "POST")
                                        v_loc.methods[POST] = true;
                                    if (v_config[i] == "DELETE")
                                        v_loc.methods[DELETE] = true;
                                    if (v_config[i] == "PUT")
                                        v_loc.methods[PUT] = true;
                                    ++i;
                                }
                                --i;  // прошли методы и встретили что-то
                                      // другое, поэтому --
                            }
                            ++i;
                        }
                        serv.PBLocation(v_loc);
                    }
                    ++i;
                }
                v_serv.push_back(serv);
                if (i < v_config.size() && v_config[i] == "server") --i;
            }
        }
    } else {
        std::cout << "can't open the config file!\n";
        exit(1);
    }
    config.close();
    return v_serv;
}
