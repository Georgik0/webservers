//
// Created by Stephane Kitsch on 11/18/21.
//

#include <sys/select.h> /* FD_SETSIZE */
#include <sys/types.h> /* ssize_t fd_set */
#include <sys/socket.h> /* socklen_t - непрозрачный целочисленный тип без знака длиной не менее 32 бит.*/
#include <netinet/in.h>	/* sockaddr_in{} and other Internet defns */

#define	MAXLINE		4096	/* max text line length */

int main(int argc, char **argv) {
    int i, maxi, maxfd, listenfd, connfd, sockft;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

    }

}