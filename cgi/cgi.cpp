//
// Created by egor on 18.12.2021.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <time.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
//#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

std::string error_page(std::string err_str);
typedef std::map<std::string, std::string> mss;
static void answer_page(mss &answer, std::string err_str);

std::ostream &operator<<(std::ostream& os, std::string s) {
    write(1, s.c_str(), s.length());
    return os;
}

static void setLength(mss &answer) {
    int len = 0;
    std::string path = answer["Path"];
    std::string line;
    if (path != "/")
    {
        std::ifstream   file;
        file.open(("." + path).c_str());
        if (file.is_open()) {
            while (std::getline(file, line)) {
                len += line.length();
            }
            file.close();
        }
    }
    std::ostringstream ss;
    ss << len;
    answer["Content-Length"] = ss.str();
}

static void result_answer(mss &answer) {
    std::ifstream file;
    file.open(("." + answer["Path"]).c_str(), std::ios::out);
    if (file.is_open()) {
        answer["Code"] = "200 OK";
        std::cout << answer["Version"] << " " << answer["Code"] << std::string("\r\n");
        std::cout << std::string("Date: ") << answer["Date"] << std::string("\r\n");
        std::cout << std::string("Server: ") << answer["Server"] << std::string("\r\n");
        setLength(answer);
        std::cout << std::string("Content-Length: ") << answer["Content-Length"] << std::string("\r\n");
        std::cout << std::string("Connection: ") << answer["Connection"] << std::string("\n\n");
        std::string buf;
        while (std::getline(file, buf))
            std::cout << buf << std::string("\n");
        file.close();
    } else {
        answer["Code"] = "404";
        answer_page(answer, "404 NOT FOUND");
    }
}

static void status_code(mss &answer) {

}

static void method_delete(mss &answer) {
    int rm = remove(answer["Path"].c_str());
    if (rm != 0) {
        answer["Code"] = "404"; /* Ресурс не найден */
        answer_page(answer, "404 NOT FOUND");
    } else {
        answer["Code"] = "204"; /* метод DELETE выполнен, но тело сообщения отсутствует */
        answer_page(answer, "204 DELETE completed, no content");
    }
}

static void method_post(mss &answer) {
    char *len = getenv("HTTP_CONTENT_LENGTH");
    int lenght = atoi(len);
    char buf[lenght];
    int bytes = read(STDIN_FILENO, buf, lenght);

    if (bytes < 0) {
        std::cerr << "Error read in method post\n";
        return;
    }
    answer["Query_string"] = std::string(buf);
}

static void method_get(mss &answer) {
    int result;
    struct stat info;
    std::string path = answer["Path"];
    /* можно добавить запись ошибок в stderr */
    if (path == "/") {
        answer["Code"] = "200 OK";
        answer_page(answer, "Default page");
    } else if ((result = stat(("." + path).c_str(), &info)) == -1) {
        answer["Code"] = "404";
        answer_page(answer, "404 NOT FOUND");
    } else if (!(info.st_mode & S_IFMT)) {
        answer["Code"] = "404";
        answer_page(answer, "404 NOT FOUND");
    } else if (!(info.st_mode & S_IRUSR)) {
        answer["Code"] = "403"; /* нет доступа */
        answer_page(answer, "403 FORBIDDEN");
    }
    if (answer["Code"] == "")
        result_answer(answer);
//    else
//        only_header(answer);
}

static void answer_page(mss &answer, std::string err_str) {
//    answer["Code"] = "405"; /* метод нельзя применить на сервере */
    std::ostringstream ss;
    std::string body = error_page(err_str);
    ss << body.length();
    answer["Content-Length"] = ss.str();

//    write(STDOUT_FILENO, answer["Version"].c_str(), answer["Version"].length());
    std::cout << answer["Version"] << " " << answer["Code"] << std::string("\r\n");
    std::cout << std::string("Date: ") << answer["Date"] << std::string("\r\n");
    std::cout << std::string("Server: ") << answer["Server"] << std::string("\r\n");
    std::cout << std::string("Content-Length: ") << answer["Content-Length"] << std::string("\r\n");
    std::cout << std::string("Connection: ") << answer["Connection"] << std::string("\n\n");
    std::cout << body << std::endl;
}

static void selcetMethod(mss &answer) {
    int i = 0;
    std::string type = getenv("REQUEST_METHOD");
    answer["Path"] = getenv("PATH_INFO");
    std::string types[] = {"GET", "POST", "DELETE"};
    void (*methods[])(mss&) = {method_get, method_post, method_delete};
    // std::cout << "search method\n";
    for (; i < 3; ++i) {
        if (type == types[i]) {
            methods[i](answer);
            return;
        }
    }
    answer["Code"] = "405"; /* метод нельзя применить на сервере */
    answer_page(answer, "405 METHOD NOT ALLOWED"); /* попадаем сюда, если не нашли допустимый метод */
}

static void setTime(mss &answer) {
    time_t      now = time(0);
    struct tm   tstruct;
    char        buf[1024];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tstruct);
    answer["Date"] = std::string(buf);
}

int main(int argc, char **argv, char **env) {
//    int fd = open("output.txt", O_TRUNC | O_CREAT | O_WRONLY, 0777);
//    dup2(fd, STDOUT_FILENO);
//    close(fd);
//    __gnu_cxx::stdio_filebuf<char> fb(1, std::ios::out);
//    std::ostream gg(&fb);
    std::map<std::string, std::string>  answer;
//    char *version = getenv("HTTP_VERSION");
//    if (version == NULL) {
//        std::cout << std::string("HTTP_VERSION is NULL\n");
//    } else {
//        std::cout << "все ок: " << std::string(version);
//    }
//    std::string test((char *)getenv("HTTP_VERSION"));
//    std::cout << test << "\n";

    answer["Version"] = getenv("HTTP_VERSION");
    answer["Server"] = "Webmasters";
    answer["Connection"] = "Close";
    write(STDOUT_FILENO, answer["Version"].c_str(), answer["Version"].length());
    setTime(answer);
    selcetMethod(answer);
}
