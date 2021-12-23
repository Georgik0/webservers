#include "parser_config.hpp"
#include <fstream>

void print_config(const std::vector<std::vector<ParserConfig> > &v_serv)
{
    for (size_t i = 0; i < v_serv.size(); i++)
    {
        std::cout << v_serv[i][0].listen  << " " ;
        std::cout << v_serv[i][0].host << " " ;
        std::cout << v_serv[i][0].server_name << " " ;
        for (size_t k = 0; k < v_serv[i][0].error_pages.size(); k++)
        {
            std::cout << v_serv[i][0].error_pages[k] << " " ;
        }
        for (size_t j = 0; j < v_serv[i].size(); j++)
        {
            std::cout << v_serv[i][j].location << " " ;
            std::cout << v_serv[i][j].root << " " ;
            std::cout << v_serv[i][j].index << " " ;
            std::cout << v_serv[i][j].autoindex << " " ;
            std::cout << v_serv[i][j].post_body << " " ;
            std::cout << v_serv[i][j].cgi << " " ;
            std::cout << v_serv[i][j].cgi_ext << " " ;
            for (size_t k = 0; k < v_serv[i][j].methods.size(); k++)
            {
                std::cout << v_serv[i][j].methods[k] << " ! " ;
            }
            std::cout << std::endl;
        }
    }
}

void print_serv(const ParserConfig &v_serv)
{
    std::cout << "SERVER start\n" << std::endl;
    std::cout << v_serv.listen  << " " ;
    std::cout << v_serv.host << " " ;
    std::cout << v_serv.server_name << std::endl ;
    for (size_t k = 0; k < v_serv.error_pages.size(); k++)
    {
        std::cout << v_serv.error_pages[k] << " " ;
    }
    std::cout << "SERVER end\n" << std::endl;
}

int main()
{
    std::fstream config("config.file");
    std::vector<std::vector<ParserConfig> > v_serv;
    if (config.is_open())
    {
        std::string s_config;
        std::vector<std::string> v_config;

        while(config >> s_config)
            v_config.push_back(s_config);

        // for (std::string i:v_config)
        //     std::cout << i << std::endl;

        for (size_t i = 0; i < v_config.size(); i++)
        {
            //std::cout << v_config[i] <<"1\n";
            if(v_config[i] == "server")
            {

                i++;
                ParserConfig serv;
                while (i < v_config.size() && (v_config[i] != "location" && v_config[i] != "server"))
                {
                    if (v_config[i] == "host")
                        serv.host = v_config[++i];
                    if (v_config[i] == "listen"){
                        serv.listen = std::stoi(v_config[++i]);
                        std::cout << serv.listen  <<" listen\n";
                    }
                    if (v_config[i] == "server_name")
                    {

                        serv.server_name = v_config[++i];
                        std::cout << serv.server_name  <<" serv name\n";
                    }
                    if (v_config[i] == "error_pages")
                        serv.error_pages.push_back(v_config[++i]);
                    ++i;
                }
                print_serv(serv);
                std::vector<ParserConfig> v_loc;

                ParserConfig loc;
                serv.root = "fffffffffff";
                while(v_config[i] == "location")
                {
                    loc = serv;
                    //std::cout << serv.host << "1!!!!!!!!!!!!!!11\n";
                    loc.location = v_config[++i];
                    while (i < v_config.size() && (v_config[i] != "location" && v_config[i] != "server"))
                    {
                        if (v_config[i] == "root"){
                            loc.setRoot(v_config[++i]);
                            //std::cout << serv.root << "----------------------\n";
                        }
                        if (v_config[i] == "index")
                            loc.index = v_config[++i];
                        if (v_config[i] == "autoindex")
                        {
                            if (v_config[++i] == "ON")
                                loc.autoindex = true;
                            else
                                loc.autoindex = false;
                        }
                        if (v_config[i] == "post_body")
                            loc.post_body = v_config[++i];
                        if (v_config[i] == "cgi")
                            loc.cgi = v_config[++i];
                        if (v_config[i] == "cgi_ext")
                            loc.cgi_ext = v_config[++i];
                        if (v_config[i] == "methods")
                        {
                            i++;
                            while (i < v_config.size())
                            { 
                                if (v_config[i] == "POST")
                                    loc.methods[POST] = true;
                                else if (v_config[i] == "GET")
                                    loc.methods[GET] = true;
                                else if (v_config[i] == "DELETE")
                                    loc.methods[DELETE] = true;
                                else
                                    break ;
                                ++i;
                            }
                            break ;
                        }
                        ++i;
                    }
                    //std::cout << serv.root << "************!!!!!!!!!!!!!11\n";
                    //std::cout << v_config[i] <<"3\n";
                    v_loc.push_back(loc);
                    loc.clear_loc();
                    //std::cout << serv.root << "************!!!!!!!!!!!!!11\n";
                    //std::cout << v_loc.back().root << "99999************\n";
                    //std::cout << v_loc.back().host << "888989************\n";
                }
                //std::cout << v_config[i] <<"4\n";
                //std::cout << v_loc[0].root << "99999************!!!!!!!!!!!!!11\n";
                v_serv.push_back(v_loc);
                serv.clear_serv();
                if (v_config[i] == "server")
                    --i;
            }
        }


    }
    else{
        std::cout << "can't open the config file!\n";
        exit(1);
    }
    std::cout <<"общее кол-во серверов: " << v_serv.size() <<"\n";
    for (size_t i = 0; i < v_serv.size(); i++)
    {
        std::cout <<"общее кол-во location на "<< i + 1 << " сервере: " << v_serv[i].size() <<"\n";
    }
    print_config(v_serv);
    config.close();
    return 0;
}
