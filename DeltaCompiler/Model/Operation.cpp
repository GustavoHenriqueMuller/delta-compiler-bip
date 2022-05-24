#include "Operation.h"

OperationCategory Operation::getOperationCategory() {
    switch (type) {
        // Arit low
        case ADD:
        case SUBTRACT:
            return CATEGORY_ARIT_LOW;

        // Arit high
        case MULTIPLY:
        case DIVIDE:
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

        // Attribution
        case ATTRIBUTION:
        case INCREMENT_ATTRIBUTION:
        case DECREMENT_ATTRIBUTION:
            return CATEGORY_ATTRIBUTION;
    }
}

OperationType getOperationTypeFromTokenId(TokenId tokenId) {
    switch (tokenId) {
        // Arithmetic
        case t_PLUS:
            return ADD;
        case t_MINUS:
            return SUBTRACT;
        case t_MULT:
            return MULTIPLY;
        case t_DIV:
            return DIVIDE;
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

OperationType getRightUnaryOperationTypeFromTokenId(TokenId tokenId) {
    switch (tokenId) {
        case t_INCREMENT:
            return INCREMENT_RIGHT;
        case t_DECREMENT:
            return DECREMENT_RIGHT;
    }
}

OperationType getLeftUnaryOperationTypeFromTokenId(TokenId tokenId) {
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

Operation getBinaryOperationFromAttributionType(OperationType attributionType) {
    switch (attributionType) {
        case INCREMENT_ATTRIBUTION:
            return Operation(ADD, "+");
        case DECREMENT_ATTRIBUTION:
            return Operation(SUBTRACT, "-");
    }
}
