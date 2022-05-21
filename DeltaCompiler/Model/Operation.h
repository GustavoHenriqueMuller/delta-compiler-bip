#ifndef OPERATION_H
#define OPERATION_H

#include "Type.h"
#include "../Gals/Constants.h"

#include <string>

enum OperationType {
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    COMP,
    BIT,
    INCREMENT_RIGHT,
    DECREMENT_RIGHT,
    MINUS_INVERSION,
    BIT_NOT,
    NOT,
    INCREMENT_LEFT,
    DECREMENT_LEFT,
    ATTRIBUTION,
    INCREMENT_ATTRIBUTION,
    DECREMENT_ATTRIBUTION
};

enum AttributionResult {
    ATT_OK, // Ok
    ATT_ER, // Error
    ATT_PL  // Precision Loss
};

class Operation {
public:
    OperationType operationType;
    std::string lexeme;

    Operation() {}
    Operation(OperationType operationType, std::string lexeme): operationType(operationType), lexeme(lexeme) {}
};

class OperationManager {
public:
    static Primitive checkBinaryOperation(Type type1, Type type2, Operation operation);
    static Primitive checkUnaryOperation(Type type1, Operation operation);
    static AttributionResult checkImplicitCast(Type type1, Type type2);
    static AttributionResult checkAttribution(Type type1, Type type2, Operation operation);

private:
    static Primitive OPERATION_TABLE[7][7][7];
    static Primitive UNARY_OPERATION_TABLE[7][7];
    static AttributionResult ATTRIBUTION_TABLE[7][7];
};

OperationType getOperationTypeFromTokenId(TokenId tokenId);
OperationType getRightUnaryOperationTypeFromTokenId(TokenId tokenId);
OperationType getLeftUnaryOperationTypeFromTokenId(TokenId tokenId);
Operation getBinaryOperationFromAttributionType(OperationType attributionType);

#endif // OPERATION_H
