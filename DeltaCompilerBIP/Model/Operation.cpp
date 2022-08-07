#include "Operation.h"

OperationType getOperationTypeFromTokenId(const TokenId& tokenId) {
    switch (tokenId) {
        // Arithmetic
        case t_PLUS:
            return OP_ADDITION;
        case t_MINUS:
            return OP_SUBTRACTION;

        // Relational
        case t_OR:
            return OP_OR;
        case t_AND:
            return OP_AND;
        case t_GREATER:
            return OP_GREATER;
        case t_SMALLER:
            return OP_SMALLER;
        case t_GREATER_EQ:
            return OP_GREATER_EQ;
        case t_SMALLER_EQ:
            return OP_SMALLER_EQ;
        case t_EQ:
            return OP_EQUAL;
        case t_DIF:
            return OP_DIFFERENT;

        // Bit
        case t_BIT_OR:
            return OP_BIT_OR;
        case t_BIT_XOR:
            return OP_BIT_XOR;
        case t_BIT_AND:
            return OP_BIT_AND;
        case t_BIT_LS:
            return OP_BIT_LS;
        case t_BIT_RS:
            return OP_BIT_RS;
    }
}

OperationType getRightUnaryOperationTypeFromTokenId(const TokenId& tokenId) {
    switch (tokenId) {
        case t_INCREMENT:
            return OP_INCREMENT_RIGHT;
        case t_DECREMENT:
            return OP_DECREMENT_RIGHT;
    }
}

OperationType getLeftUnaryOperationTypeFromTokenId(const TokenId& tokenId) {
    switch (tokenId) {
        case t_MINUS:
            return OP_MINUS_INVERSION;
        case t_BIT_NOT:
            return OP_BIT_NOT;
        case t_NOT:
            return OP_NOT;
        case t_INCREMENT:
            return OP_INCREMENT_LEFT;
        case t_DECREMENT:
            return OP_DECREMENT_LEFT;
    }
}

OperationType getAssignmentOperationTypeFromTokenId(const TokenId& tokenId) {
    switch (tokenId) {
        case t_ASSIGNMENT:
            return OP_ASSIGNMENT;
        case t_INCREMENT_ASSIGNMENT:
            return OP_INCREMENT_ASSIGNMENT;
        case t_DECREMENT_ASSIGNMENT:
            return OP_DECREMENT_ASSIGNMENT;
        case t_BIT_OR_ASSIGNMENT:
            return OP_DECREMENT_ASSIGNMENT;
        case t_BIT_XOR_ASSIGNMENT:
            return OP_BIT_XOR_ASSIGNMENT;
        case t_BIT_AND_ASSIGNMENT:
            return OP_BIT_AND_ASSIGNMENT;
        case t_BIT_LS_ASSIGNMENT:
            return OP_BIT_LS_ASSIGNMENT;
        case t_BIT_RS_ASSIGNMENT:
            return OP_BIT_RS_ASSIGNMENT;
    }
}

Operation getBinaryOperationFromAssignmentType(const OperationType& attributionType) {
    switch (attributionType) {
        case OP_INCREMENT_ASSIGNMENT:
            return Operation(OP_ADDITION, "+");
        case OP_DECREMENT_ASSIGNMENT:
            return Operation(OP_SUBTRACTION, "-");
        case OP_BIT_OR_ASSIGNMENT:
            return Operation(OP_BIT_OR, "|");
        case OP_BIT_XOR_ASSIGNMENT:
            return Operation(OP_BIT_XOR, "^");
        case OP_BIT_AND_ASSIGNMENT:
            return Operation(OP_BIT_AND, "&");
        case OP_BIT_LS_ASSIGNMENT:
            return Operation(OP_BIT_LS, "<<");
        case OP_BIT_RS_ASSIGNMENT:
            return Operation(OP_BIT_RS, ">>");
    }
}

OperationCategory getOperationCategory(const OperationType& operationType) {
    switch (operationType) {
        // Arit low
        case OP_ADDITION:
        case OP_SUBTRACTION:
            return CATEGORY_ARIT;

        // Relational
        case OP_OR:
        case OP_AND:
        case OP_GREATER:
        case OP_SMALLER:
        case OP_GREATER_EQ:
        case OP_SMALLER_EQ:
        case OP_EQUAL:
        case OP_DIFFERENT:
            return CATEGORY_RELATIONAL;

        // Bit
        case OP_BIT_OR:
        case OP_BIT_XOR:
        case OP_BIT_AND:
        case OP_BIT_LS:
        case OP_BIT_RS:
            return CATEGORY_BIT;

        // Unary right mutable
        case OP_INCREMENT_RIGHT:
        case OP_DECREMENT_RIGHT:
            return CATEGORY_UNARY_RIGHT_MUTABLE;

        // Unary left
        case OP_MINUS_INVERSION:
        case OP_BIT_NOT:
        case OP_NOT:
            return CATEGORY_UNARY_LEFT;

        // Unary left mutable
        case OP_INCREMENT_LEFT:
        case OP_DECREMENT_LEFT:
            return CATEGORY_UNARY_LEFT_MUTABLE;

        // Assignment
        case OP_ASSIGNMENT:
        case OP_INCREMENT_ASSIGNMENT:
        case OP_DECREMENT_ASSIGNMENT:
        case OP_BIT_OR_ASSIGNMENT:
        case OP_BIT_XOR_ASSIGNMENT:
        case OP_BIT_AND_ASSIGNMENT:
        case OP_BIT_LS_ASSIGNMENT:
        case OP_BIT_RS_ASSIGNMENT:
            return CATEGORY_ASSIGNMENT;
    }
}
