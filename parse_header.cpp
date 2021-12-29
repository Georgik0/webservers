#include <iostream>
#include <map>
#include <string>
#include "answer.hpp"
#include <cstring>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

std::map<std::string, std::string> parse_header(const std::string &str, char *msg_end) {
    std::map<std::string, std::string> http_request;

    // std::cout << "parse_header: start\n";
    if (str.size() < 1)
        return http_request;

    const char *msg = str.c_str();
    const char *head = msg;
    const char *tail = msg;

    //std::cout <<strlen(head) <<"!!!!!!!!!!!!!!!!!!!!!!1parse_header2: start\n";
    // Find request type
    // std::cout << "parse_header1: start\n";
    try {
    while (tail != msg_end && *tail != ' ') ++tail;
    http_request["Type"] = std::string(head, tail);

    //std::cout << "parse_header2: start\n";
    // Find path
    while (tail != msg_end && *tail == ' ') ++tail;
    head = tail;
    //std::cout << "parse_header3: start\n";
    while (tail != msg_end && *tail != ' ') ++tail;
    http_request["Path"] = std::string(head, tail);

    //std::cout << "parse_header4: start\n";
    // Find HTTP version
    while (tail != msg_end && *tail == ' ') ++tail;
    head = tail;
    //std::cout << "parse_header5: start\n";
    while (tail != msg_end && *tail != '\r') ++tail;
    http_request["Version"] = std::string(head, tail);
    if (tail != msg_end) tail+=2;  // skip '\r'
    // // TODO: what about the trailing '\n'?

    // // Map all headers from a key to a value
    //std::cout << "parse_header6: start\n";
    head = tail;
    //std::cout << *head << " parse_header11: start\n";
    //std::cout << *head <<(head == 0) << (*head != '\r') << " parse_header12: start\n";
    while (head && head != msg_end && *head != '\r') {
        //std::cout << "parse_header10: start\n";
        while (tail != msg_end && *tail != '\r') {
            ++tail;
        }
        //std::cout << "parse_header7: start\n";
        const char *colon = (const char*)memchr(head, ':', std::string(head, tail).size());
        if (colon == NULL) {
            // TODO: malformed headers, what should happen? по идее сюда не должен зайти, но если зашел, то скорее всго это тело, а не голова запроса
            break ;
        }
        //std::cout << "parse_header8: start\n";
        const char *value = colon + 1;
        while (value != tail && *value == ' ') ++value;
        http_request[ std::string(head, colon) ] = std::string(value, tail);
        head = tail + 2;
        tail = head;
        //std::cout << "parse_header9: start\n";
        // TODO: what about the trailing '\n'?
    }
    // std::cout << "!: " <<msg << " parse_header101: start\n";
    // //std::cout << strlen(msg) << strlen(head + 1)<< "parse_header78: start\n";
    // std::cout << "parse_header: 000--\n";
    // //std::cout << std::string(head + 1, msg + str.size())<< "\n";
    // std::cout << (int)(*head)<<"parse_header: 111--\n";

    if (http_request[ "Type" ] == "POST") {
        while (*head == '\r' || *head == '\n')
            head++;
        std::string path = http_request["Path"].substr(1, http_request["Path"].size());
        std::cout << "path: " <<path << std::endl;
        int cont_len = std::stoi(http_request["Content-Length"]);

        struct stat statbuf;
        int fd = -5;
        if (!(stat(path.c_str(), &statbuf) == 0)) {
            //std::cout << "path:: " << path << "\n";
                fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 00755);
                //fd = -10;
                http_request[ "http_status" ] = "201";
            }
        else {
            fd = open(path.c_str(), O_RDWR | O_TRUNC);
            http_request[ "http_status" ] = "200";
        }
        std::cout << "fd: " << fd  << " cont len = " << cont_len << "\n";

        std::string body;
        int written_bytes = 0;
        while (fd > 0 && written_bytes < cont_len) {
            std::cout << "cycle\n";
            if (cont_len - written_bytes < 1000){
                std::cout << "check1\n";
                body = std::string(head, head + cont_len - written_bytes);
                std::cout << "check2\n";
                written_bytes = cont_len;
            }
            else {
                std::cout << "check3\n";
                body = std::string(head, head + 1000);
                std::cout << "check4\n";
                written_bytes += 1000;
                head += 1000;
            }
            std::cout << "check5\n";
            if (body.length() <= 0){
                http_request[ "http_status" ] = "400"; //Bad Request
                break ;
            }
            else
                std::cout << "body: " << write(fd, body.c_str(), body.length()) << "\n";
        }
        if (fd > 0)
            close(fd);
    // if ((int)(*head) && head )//&& str.size() > strlen(head + 1))
    //     http_request[ "Body" ] = std::string(head + 1, msg + str.size());
    // std::cout << "parse_header79: start\n";
        }

        if ((int)(*head) && head )//&& str.size() > strlen(head + 1))
            http_request[ "Body" ] = std::string(head + 1, msg + str.size());
    } catch (std::exception &ex) {
        std::cout << "parse_header: exception! " << ex.what() << std::endl;
    }


    // Determine if successful
    // std::cout << "То что лежит в мапе\n";
    // std::cout << " <******************************************************\n";
    // std::cout << http_request[ "Type" ] << std::endl;
    // std::cout << http_request[ "Path" ] << std::endl;
    // std::cout << http_request[ "Version" ] << std::endl;
    // std::cout << http_request[ "Host" ] << std::endl;
    // std::cout << http_request[ "Connection" ] << std::endl;
    // std::cout << http_request[ "sec-ch-ua" ] << std::endl;
    // std::cout << http_request[ "User-Agent" ] << std::endl;
    // std::cout << http_request[ "Accept-Language" ] << std::endl;
    // std::cout << http_request[ "Accept-Encoding" ] << std::endl;
    // std::cout << http_request[ "Connection" ] << std::endl;
    // std::cout << http_request[ "Accept" ] << std::endl;
    // std::cout << "То что лежит в теле" << std::endl;
    // std::cout << http_request[ "Body" ] << std::endl;
     return http_request;
}
