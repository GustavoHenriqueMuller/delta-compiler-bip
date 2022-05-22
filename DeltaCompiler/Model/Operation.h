#ifndef OPERATION_H
#define OPERATION_H

#include "Type.h"
#include "../Gals/Constants.h"

#include <string>

enum OperationType {
    // Arithmetic
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MOD,

    // Relational
    OR,
    AND,
    GREATER,
    SMALLER,
    GREATER_EQ,
    SMALLER_EQ,
    EQUAL,
    DIFFERENT,

    // Bit
    BIT_OR,
    BIT_XOR,
    BIT_AND,
    BIT_LS,
    BIT_RS,

    // Unary right
    INCREMENT_RIGHT,
    DECREMENT_RIGHT,

    // Unary left
    MINUS_INVERSION,
    BIT_NOT,
    NOT,
    INCREMENT_LEFT,
    DECREMENT_LEFT,

    // Attribution
    ATTRIBUTION,
    INCREMENT_ATTRIBUTION,
    DECREMENT_ATTRIBUTION
};

enum OperationCategory {
    CATEGORY_ARIT_LOW,
    CATEGORY_ARIT_HIGH,
    CATEGORY_RELATIONAL,
    CATEGORY_BIT,
    CATEGORY_UNARY_RIGHT_MUTABLE,
    CATEGORY_UNARY_LEFT,
    CATEGORY_UNARY_LEFT_MUTABLE,
    CATEGORY_ATTRIBUTION
};

class Operation {
public:
    OperationType type;
    std::string lexeme;

    Operation() {}
    Operation(OperationType type, std::string lexeme): type(type), lexeme(lexeme) {}
    OperationCategory getOperationCategory();
};

#endif // OPERATION_H
