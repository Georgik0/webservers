//
// Created by Stephane Kitsch on 10/15/21.
//

/* getaddrinfo() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h> // struct sockaddr_in

#include "../utils_src/net_write_read.hpp"

void err_sys(const char* x) {
    perror(x);
    exit(1);
}

/* срт. 720 */
int Tcp_listen(const char *host, const char *serv, socklen_t *addrlenp) {
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // надёжная потокоориентированная служба (сервис) или потоковый сокет (в данном случае TCP)

    /* getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result) (стр. 348)
     * возвращает список структур sockaddr через указатель result
     * hostname - либо имя узла либо строка адреса
     * service - либо имя службы, либо строка, содержащая десятичный номер порта
     * hints - либо пустой указатель, либо указатель на структуру addrinfo*/
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        std::cout << "Error getaddrinfo" << std::endl;
        return 1;
    }
    ressave = res;

    do {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
//        std::cout << "listenfd = " << listenfd << std::endl;
        if (listenfd < 0)
            continue; /* ошибка, пробуем следующий адрес */

        /* setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) (стр. 223)
         * sockfd - ссылается на открытый дескриптор сокета
         * level - определяет, каким кодом должен интерпретироваться параметр
         * optval - это указатель на переменную, из которой извлекается новое значение параметра функции */
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
            std::cout << "Error setsockopt" << std::endl;
            return 1;
        }

        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break; /* успех */

        /* ошибка при вызове bind, закрываем сокет и пробуем следующий адрес */
        if (close(listenfd) < 0) {
            std::cout << "Error close" << std::endl;
            return 1;
        }
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)
        err_sys("tcp_listen error");

    if (listen(listenfd, LISTENQ) < 0)
        err_sys("Error listen");
    if (addrlenp)
        *addrlenp = res->ai_addrlen; /* возвращает размер адреса протокола */

    freeaddrinfo(ressave);

    return listenfd;
}

void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];
    for (;;) {
        if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
            std::cout << "Прочитал: " << buf << std::endl;
            return; /* Если клиент закрывает соединение, то при получении клиентского сегмента FIN функция read возвращает 0 */
        } else {
            std::cout << "[" << sockfd << "] " << "Прочитал: " << buf << std::endl;
        }

        std::string answer(buf);
        answer = "\"" + answer + "\"" + " - server response";
        if (writen(sockfd, answer.c_str(), answer.length()) < 0){
            err_sys("Error write");
        } else {
            std::cout << "Ответил: " << answer << std::endl;
        }
    }
}

static void *doit(void *);

int main(int argc, char **argv) {
    int listenfd, connfd;
    pthread_t tid;
    socklen_t addrlen, len;
    struct sockaddr *cliaddr;

    if (argc == 2)
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    else if (argc == 3)
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    else {
        std::cout << "err argc" << std::endl;
        return 1;
    }

    if ( (cliaddr = (struct sockaddr *)malloc(addrlen)) == NULL) {
        std::cout << "err malloc" << std::endl;
        return 1;
    }
    for (;;) {
        len = addrlen;
        if ((connfd = accept(listenfd, cliaddr, &len)) < 0) {
            std::cout << "Error accept" << std::endl;
            return 1;
        }
        std::cout << "Server accept, connfd before pthread = " << connfd << std::endl;
        if (pthread_create(&tid, NULL, doit, static_cast<void *>(&connfd)) != 0) {
            std::cout << "Error ptrhead" << std::endl;
            exit(1);
        }
    }
}

static void *doit(void *arg) {
    /* pthread_detach - делает поток отсоединенным
     * Отсоединенный поток напоминает процесс-демон: когда он завершается,
     * все занимаемые им ресурсы освобождаются и мы не можем отслеживать его завершения
     * pthread_self - возвращает идентификатор потока */
    if (pthread_detach(pthread_self()) != 0) {
        std::cout << "Error pthread_detach" << std::endl;
        exit(1);
    }

    std::cout << "Кто-то подключился [" << *static_cast<int *>(arg) << "]" << std::endl;
//    std::cout << "After static_cast<int *>" << *static_cast<int *>(arg) << std::endl;

    str_echo(*static_cast<int *>(arg));

    if (close(*static_cast<int *>(arg)) < 0) {
        std::cout << "Error close in doit" << std::endl;
        exit(1);
    }
    return NULL;
}