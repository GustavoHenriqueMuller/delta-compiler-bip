#ifndef UTILS_H
#define UTILS_H

#include "Model/Symbol.h"

#include <string>
#include <iostream>

namespace Utils {
    void printBar();
    int lexemeToInt(const std::string &lexeme);
    bool lexemeToBoolean(const std::string &lexeme);
    std::string mangleFunction(const Symbol &function);
}

#endif // UTILS_H
