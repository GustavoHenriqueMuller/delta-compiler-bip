#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H

#include "Operation.h"

enum AttributionResult {
    ATT_OK, // Ok
    ATT_ER, // Error
    ATT_PL  // Precision Loss
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

#endif // OPERATION_MANAGER_H
