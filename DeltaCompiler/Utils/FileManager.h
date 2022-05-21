#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <string>
#include <sstream>

namespace FileManager {
    std::string getFileContent(std::string filepath) {
        std::ifstream file;
        std::string line;
        std::stringstream content;

        file.open(filepath.c_str());

        while(getline(file, line))
            content << line << "\n";

        file.close();
        return content.str();
    }
}

#endif
