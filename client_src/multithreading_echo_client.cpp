//
// Created by Stephane Kitsch on 10/20/21.
//

#include <arpa/inet.h>
#include <iostream>
#include <cstring>

#include <stdio.h>
#include <unistd.h>

#include "../utils_src/net_write_read.hpp"

#define MAXLINE 4096
#define	SA	struct sockaddr

int main(int argc, char **argv) {
    int sockfd;//, n;
    struct sockaddr_in servaddr;
//    char recvline[MAXLINE + 1];

    if (argc != 2) {
        std::cout << "Error argc" << std::endl;
        return 0;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        std::cout << "Error socket" << std::endl;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        std::cout << "Error inet_pton" << std::endl;

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        std::cout << "Error connect" << std::endl;
    else
        std::cout << "Start connect" << std::endl;

    std::string buf_string;
    while (getline(std::cin, buf_string)) {
        writen(sockfd, buf_string.c_str(), buf_string.length());

    }

}
