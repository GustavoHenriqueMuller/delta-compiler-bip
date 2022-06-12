#ifndef OPERATION_H
#define OPERATION_H

#include "Type.h"
#include "../Gals/Constants.h"

#include <string>

enum OperationType {
    // Arithmetic
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
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
    ASSIGNMENT,
    INCREMENT_ASSIGNMENT,
    DECREMENT_ASSIGNMENT,
    MULTIPLICATION_ASSIGNMENT,
    DIVISION_ASSIGNMENT,
    MOD_ASSIGNMENT,
    BIT_OR_ASSIGNMENT,
    BIT_XOR_ASSIGNMENT,
    BIT_AND_ASSIGNMENT,
    BIT_LS_ASSIGNMENT,
    BIT_RS_ASSIGNMENT
};

enum OperationCategory {
    CATEGORY_ARIT_LOW,
    CATEGORY_ARIT_HIGH,
    CATEGORY_RELATIONAL,
    CATEGORY_BIT,
    CATEGORY_UNARY_RIGHT_MUTABLE,
    CATEGORY_UNARY_LEFT,
    CATEGORY_UNARY_LEFT_MUTABLE,
    CATEGORY_ASSIGNMENT
};

class Operation {
public:
    Operation() {}
    Operation(OperationType type, std::string lexeme): type(type), lexeme(lexeme) {}

    OperationType type;
    std::string lexeme;
};

OperationType getOperationTypeFromTokenId(const TokenId &tokenId);
OperationType getRightUnaryOperationTypeFromTokenId(const TokenId &tokenId);
OperationType getLeftUnaryOperationTypeFromTokenId(const TokenId &tokenId);
OperationType getAssignmentOperationTypeFromTokenId(const TokenId &tokenId);
Operation getBinaryOperationFromAssignmentType(const OperationType &attributionType);
OperationCategory getOperationCategory(const OperationType &operationType);

#endif // OPERATION_H
