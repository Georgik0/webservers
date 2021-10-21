//
// Created by Stephane Kitsch on 10/21/21.
//

#ifndef NETWORK_PROGRAMMING_NET_WRITE_READ_HPP
#define NETWORK_PROGRAMMING_NET_WRITE_READ_HPP

#define MAXLINE 4096
#define	LISTENQ	1024 /* максимальное кол-во клиентских соединений */
#define	SA	struct sockaddr

ssize_t writen(int fd, const void *vptr, size_t n);

#endif //NETWORK_PROGRAMMING_NET_WRITE_READ_HPP
