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
