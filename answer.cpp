//
// Created by Stephane Kitsch on 12/13/21.
//

#include <iostream>
#include <string>
#include <map>
#include <sys/stat.h>
#include <time.h>
#include <sstream>
#include <fstream>

typedef std::map<std::string, std::string> mss;

static void setCode(mss &http_request, struct stat &info, mss &answer) {
    int i = 0;
    std::string types[] = {"GET", "POST", "DELETE"};
    for (; i < 3; ++i) {
        if (http_request["Type"] == types[i])
            break;
    }
    if (i == 3) {
        answer["Code"] = "405"; /* метод нельзя применить на сервере */
        return;
    }
    if (http_request["Path"] == "/") {
        answer["Code"] = "200";
        return;
    }
    int result;
    if ((result = stat(("." + http_request["Path"]).c_str(), &info)) == -1) {
        answer["Code"] = "404";
        std::cout << "stat error\n";
        return;
    }
    if (!(info.st_mode & S_IFMT)) {
        answer["Code"] = "404";
        std::cout << "S_IFMT error\n";
        return;
    }
    if (!(info.st_mode & S_IRUSR)) {
        answer["Code"] = "403"; /* нет доступа */
        return;
    }
    // std::cout << "info.st_size = " << info.st_size << std::endl;
    answer["Code"] = "200 OK";
}

static void setTime(mss &answer) {
    time_t      now = time(0);
    struct tm   tstruct;
    char        buf[1024];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tstruct);
    answer["Date"] = std::string(buf);
}

static void setLength(mss &http_request, mss &answer) {
    int len = 0;
    std::string line;
    if (http_request["Path"] != "/")
    {
        std::ifstream   file;
        file.open("." + http_request["Path"]);
        if (file.is_open()) {
            while (std::getline(file, line)) {
                len += line.length();
            }
        }
    }
    std::ostringstream ss;
    ss << len;
    answer["Content-Length"] = ss.str();
}

std::string getAnswer(mss &http_request) {
    return "";
    std::string str_ans;
    std::map<std::string, std::string>  answer;
    answer["Version"] = "HTTP/1.1";
    struct stat info;

    setCode(http_request, info, answer);
    answer["Server"] = "Webmasters";
    setTime(answer);
    answer["Connection"] = "Close";
    setLength(http_request, answer);
    str_ans = answer["Version"] + " " + answer["Code"] + "\n\r" + "Server: " + answer["Server"] + "\n\r" + "Date: " + answer["Date"] + "\n\r" + "Connection: " + answer["Connection"] + "\n\r" + "Content-Length: " + answer["Content-Length"] + "\n\n";
    return str_ans;
}
