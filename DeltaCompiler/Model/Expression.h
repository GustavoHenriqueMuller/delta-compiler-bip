#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Type.h"

#include <string>

class Expression {
public:
    Type type;
    Expression(Type type): type(type) {}
};

#endif // EXPRESSION_H
