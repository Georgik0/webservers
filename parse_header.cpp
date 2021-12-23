#include <iostream>
#include <map>
#include <string>
#include "answer.hpp"
#include <cstring>

std::map<std::string, std::string> parse_header(const std::string &str, char *msg_end) {
    std::map<std::string, std::string> http_request;

    std::cout << "parse_header: start\n";
    const char *msg = str.c_str();
    const char *head = msg;
    const char *tail = msg;

    // Find request type
    std::cout << "parse_header1: start\n";
    while (tail != msg_end && *tail != ' ') ++tail;
    http_request["Type"] = std::string(head, tail);

    std::cout << "parse_header2: start\n";
    // Find path
    while (tail != msg_end && *tail == ' ') ++tail;
    head = tail;
    std::cout << "parse_header3: start\n";
    while (tail != msg_end && *tail != ' ') ++tail;
    http_request["Path"] = std::string(head, tail);

    std::cout << "parse_header4: start\n";
    // Find HTTP version
    while (tail != msg_end && *tail == ' ') ++tail;
    head = tail;
    std::cout << "parse_header5: start\n";
    while (tail != msg_end && *tail != '\r') ++tail;
    http_request["Version"] = std::string(head, tail);
    if (tail != msg_end) tail+=2;  // skip '\r'
    // // TODO: what about the trailing '\n'?
    
    // // Map all headers from a key to a value
    std::cout << "parse_header6: start\n";
    head = tail;
    while (head != msg_end && *head != '\r') {
        while (tail != msg_end && *tail != '\r') {
            ++tail;
        }
        const char *colon = (const char*)memchr(head, ':', std::string(head, tail).size());
        if (colon == NULL) {
            // TODO: malformed headers, what should happen? по идее сюда не должен зайти, но если зашел, то скорее всго это тело, а не голова запроса
            break ;
        }
        const char *value = colon + 1;
        while (value != tail && *value == ' ') ++value;
        http_request[ std::string(head, colon) ] = std::string(value, tail);
        head = tail + 2;
        tail = head;
        // TODO: what about the trailing '\n'?
    }
    http_request[ "Body" ] = std::string(head + 1, msg + strlen(msg));

    std::cout << "parse_header: end\n";
    return http_request;
    // Determine if successful
    std::cout << "То что лежит в мапе\n";
    std::cout << " <******************************************************\n";
    std::cout << http_request[ "Type" ] << std::endl; 
    std::cout << http_request[ "Path" ] << std::endl; 
    std::cout << http_request[ "Version" ] << std::endl; 
    std::cout << http_request[ "Host" ] << std::endl; 
    std::cout << http_request[ "Connection" ] << std::endl; 
    std::cout << http_request[ "sec-ch-ua" ] << std::endl; 
    std::cout << http_request[ "User-Agent" ] << std::endl; 
    std::cout << http_request[ "Accept-Language" ] << std::endl; 
    std::cout << http_request[ "Accept-Encoding" ] << std::endl; 
    std::cout << http_request[ "Connection" ] << std::endl; 
    std::cout << http_request[ "Accept" ] << std::endl; 
    std::cout << "То что лежит в теле" << std::endl; 
    std::cout << http_request[ "Body" ] << std::endl;
}
