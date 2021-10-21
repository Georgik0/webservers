//
// Created by Stephane Kitsch on 9/27/21.
//

//#include <netinet/in.h>
//#include <netinet/udp.h>
//#include <sys/uio.h>
//#include <sys/types.h>
//#include <netinet/tcp.h>
//#include "unp.h"

#include <fcntl.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <unistd.h> // read()

//#include "sys/un.h"
#include <arpa/inet.h> // struct sockaddr_in
#include <sys/socket.h> // socket()
#include <strings.h> // bzero()
#include <iostream>

#define MAXLINE 4096
#define	SA	struct sockaddr

int main(int argc, char **argv) {
    int     sockfd, n;
    struct sockaddr_in servaddr;
    char    recvline[MAXLINE + 1];

    if (argc != 2) {
        std::cout << "Error argc" << std::endl;
        return 0;
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // socket() - создает потоковый (SOCK_STREAM) сокет Интернета (AF_INET); AF_INET - показывает, что мы работаем с сетевыми сокетами (сокеты для обмена данными по сети)
        std::cout << "Error socket" << std::endl;       // SOCK_STREAM - указывает, что мы будем исполоьзовать сокет для работы с протоколом tcp
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13); // преобразовывает двоичный номер порта в требуемый формат
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) // преобразует ip адрес в двоичный формат
        std::cout << "Error inet_pton" << std::endl;
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        std::cout << "Error connect" << std::endl;

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0 ) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
            std::cout << "Error fputs" << std::endl;
    }
    if (n < 0)
        std::cout << "Error read" << std::endl;

    exit(0);
}