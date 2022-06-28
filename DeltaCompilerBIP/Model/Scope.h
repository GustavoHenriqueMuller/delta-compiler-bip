#ifndef SCOPE_H
#define SCOPE_H

#include "Symbol.h"

#include <vector>

struct Scope {
    Scope(int id): id(id) {}
    Scope(int id, Type returnType): id(id), returnType(returnType) {}

    const int id;
    std::vector<Symbol> symbolList;
    bool hasReturned = false;
    Type returnType = Type(PRIMITIVE_VOID);
};

#endif // SCOPE_H
