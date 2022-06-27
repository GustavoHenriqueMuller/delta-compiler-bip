#ifndef SYMBOL_H
#define SYMBOL_H

#include "Type.h"
#include "Parameter.h"

#include <string>
#include <vector>

class Symbol {
public:
    Symbol() {}
    Symbol(Type type, std::string name, int scopeId): type(type), name(name), scopeId(scopeId) {}

    std::string getMangledName() const;

    Type type;
    std::string name;
    int scopeId;

    bool isInitialized = false;
    bool isUsed = false;
    bool isInDeclaration = true;

    bool isFunction = false;
    std::vector<Symbol> parameters;
};

#endif // SYMBOL_H
