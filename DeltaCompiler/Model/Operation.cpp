#include "Operation.h"

OperationType getOperationTypeFromTokenId(const TokenId &tokenId) {
    switch (tokenId) {
        // Arithmetic
        case t_PLUS:
            return ADDITION;
        case t_MINUS:
            return SUBTRACTION;
        case t_MULT:
            return MULTIPLICATION;
        case t_DIV:
            return DIVISION;
        case t_MOD:
            return MOD;

        // Relational
        case t_OR:
            return OR;
        case t_AND:
            return AND;
        case t_GREATER:
            return GREATER;
        case t_SMALLER:
            return SMALLER;
        case t_GREATER_EQ:
            return GREATER_EQ;
        case t_SMALLER_EQ:
            return SMALLER_EQ;
        case t_EQ:
            return EQUAL;
        case t_DIF:
            return DIFFERENT;

        // Bit
        case t_BIT_OR:
            return BIT_OR;
        case t_BIT_XOR:
            return BIT_XOR;
        case t_BIT_AND:
            return BIT_AND;
        case t_BIT_LS:
            return BIT_LS;
        case t_BIT_RS:
            return BIT_RS;
    }
}

OperationType getRightUnaryOperationTypeFromTokenId(const TokenId &tokenId) {
    switch (tokenId) {
        case t_INCREMENT:
            return INCREMENT_RIGHT;
        case t_DECREMENT:
            return DECREMENT_RIGHT;
    }
}

OperationType getLeftUnaryOperationTypeFromTokenId(const TokenId &tokenId) {
    switch (tokenId) {
        case t_MINUS:
            return MINUS_INVERSION;
        case t_BIT_NOT:
            return BIT_NOT;
        case t_NOT:
            return NOT;
        case t_INCREMENT:
            return INCREMENT_LEFT;
        case t_DECREMENT:
            return DECREMENT_LEFT;
    }
}

OperationType getAssignmentOperationTypeFromTokenId(const TokenId &tokenId) {
    switch (tokenId) {
        case t_ASSIGNMENT:
            return ASSIGNMENT;
        case t_INCREMENT_ASSIGNMENT:
            return INCREMENT_ASSIGNMENT;
        case t_DECREMENT_ASSIGNMENT:
            return DECREMENT_ASSIGNMENT;
        case t_MULTIPLICATION_ASSIGNMENT:
            return MULTIPLICATION_ASSIGNMENT;
        case t_DIVSION_ASSIGNMENT:
            return DIVISION_ASSIGNMENT;
        case t_MOD_ASSIGNMENT:
            return MOD_ASSIGNMENT;
        case t_BIT_OR_ASSIGNMENT:
            return DECREMENT_ASSIGNMENT;
        case t_BIT_XOR_ASSIGNMENT:
            return BIT_XOR_ASSIGNMENT;
        case t_BIT_AND_ASSIGNMENT:
            return BIT_AND_ASSIGNMENT;
        case t_BIT_LS_ASSIGNMENT:
            return BIT_LS_ASSIGNMENT;
        case t_BIT_RS_ASSIGNMENT:
            return BIT_RS_ASSIGNMENT;
    }
}

Operation getBinaryOperationFromAssignmentType(const OperationType &attributionType) {
    switch (attributionType) {
        case INCREMENT_ASSIGNMENT:
            return Operation(ADDITION, "+");
        case DECREMENT_ASSIGNMENT:
            return Operation(SUBTRACTION, "-");
        case MULTIPLICATION_ASSIGNMENT:
            return Operation(MULTIPLICATION, "*");
        case DIVISION_ASSIGNMENT:
            return Operation(DIVISION, "/");
        case MOD_ASSIGNMENT:
            return Operation(MOD, "%");
        case BIT_OR_ASSIGNMENT:
            return Operation(BIT_OR, "|");
        case BIT_XOR_ASSIGNMENT:
            return Operation(BIT_XOR, "^");
        case BIT_AND_ASSIGNMENT:
            return Operation(BIT_AND, "&");
        case BIT_LS_ASSIGNMENT:
            return Operation(BIT_LS, "<<");
        case BIT_RS_ASSIGNMENT:
            return Operation(BIT_RS, ">>");
    }
}

OperationCategory getOperationCategory(const OperationType &operationType) {
    switch (operationType) {
        // Arit low
        case ADDITION:
        case SUBTRACTION:
            return CATEGORY_ARIT_LOW;

        // Arit high
        case MULTIPLICATION:
        case DIVISION:
        case MOD:
            return CATEGORY_ARIT_HIGH;

        // Relational
        case OR:
        case AND:
        case GREATER:
        case SMALLER:
        case GREATER_EQ:
        case SMALLER_EQ:
        case EQUAL:
        case DIFFERENT:
            return CATEGORY_RELATIONAL;

        // Bit
        case BIT_OR:
        case BIT_XOR:
        case BIT_AND:
        case BIT_LS:
        case BIT_RS:
            return CATEGORY_BIT;

        // Unary right mutable
        case INCREMENT_RIGHT:
        case DECREMENT_RIGHT:
            return CATEGORY_UNARY_RIGHT_MUTABLE;

        // Unary left
        case MINUS_INVERSION:
        case BIT_NOT:
        case NOT:
            return CATEGORY_UNARY_LEFT;

        // Unary left mutable
        case INCREMENT_LEFT:
        case DECREMENT_LEFT:
            return CATEGORY_UNARY_LEFT_MUTABLE;

        // Assignment
        case ASSIGNMENT:
        case INCREMENT_ASSIGNMENT:
        case DECREMENT_ASSIGNMENT:
        case MULTIPLICATION_ASSIGNMENT:
        case DIVISION_ASSIGNMENT:
        case MOD_ASSIGNMENT:
        case BIT_OR_ASSIGNMENT:
        case BIT_XOR_ASSIGNMENT:
        case BIT_AND_ASSIGNMENT:
        case BIT_LS_ASSIGNMENT:
        case BIT_RS_ASSIGNMENT:
            return CATEGORY_ASSIGNMENT;
    }
}
