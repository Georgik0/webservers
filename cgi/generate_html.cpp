//
// Created by egor on 21.12.2021.
//

#include <iostream>

std::string error_page(std::string err_str) {
    std::string begin = "<!DOCTYPE html>\n"
                       "<html lang=\"en\">\n"
                       "<head>\n"
                       "    <meta charset=\"UTF-8\">\n"
                       "    <title>Error</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "<h2 align=\"center\" style=\"color:Black\">";
    std::string end = "</h2>\n"
                      "</body>\n"
                      "</html>";
    return begin + err_str + end;
}
