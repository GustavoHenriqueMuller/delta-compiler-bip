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

    void saveToFile(std::string content, std::string filePath) {
        std::ofstream file;
        file.open(filePath, std::ofstream::out | std::ofstream::trunc);
        file << content;
        file.close();
    }

    std::string getAsmFilePath(std::string sourceFilePath) {
        return sourceFilePath.replace(sourceFilePath.find_last_of('.') + 1, 6, "asm");
    }

    std::string getSimplifiedName(std::string filePath) {
        return filePath.substr(filePath.find_last_of('/') + 1, filePath.size());
    }
}

#endif
