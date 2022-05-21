#ifndef SYMBOL_H
#define SYMBOL_H

#include "Type.h"
#include "Parameter.h"

#include <string>
#include <vector>

class Symbol {
public:
    Symbol(Type type, std::string name): type(type), name(name) {}

    Type type;
    std::string name;

    bool isInitialized = false;
    bool isUsed = false;
    bool isInDeclaration = true;

    bool isFunction = false;
    std::vector<Parameter> parameters;
};

#endif // SYMBOL_H
