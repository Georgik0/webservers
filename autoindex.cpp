#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>

void getAutoIndex(const std::string &path, const std::string &uri_path) {

    DIR           *dp;
    struct dirent *di_struct;
    int           i = 0;
    std::string   table;
    std::ofstream ai("auto.html");

    dp = opendir(path.data());
    table += "<!DOCTYPE html>";
    table += "<h1>" + uri_path + "</h1>";
    table += "<table>";
    // table += "<tr> <th>File name</th> <th>File size</th> <th>Last modified</th> </tr>";
    if (dp != NULL) {
        while ((di_struct = readdir(dp)) != nullptr) {
            struct stat s;
            stat(std::string(path + "/" + di_struct->d_name).data(), &s);
            table += "<tr>";
            table += "<td><a href=\"" + uri_path;
            table += di_struct->d_name;
            if (s.st_mode & S_IFDIR)
                table += "/";
            table += "\">" + std::string(di_struct->d_name) + "</a></td>";
            //table += "<td>" + file.getSizeInMb() + "</td>"; вот это можно выкинуть
            //table += "<td>" + file.getTimeModified() + "</td>";
            table += "</tr>";
            i++;
        }
        closedir(dp);
    }
    table += "</table>";
    ai << table;
}
