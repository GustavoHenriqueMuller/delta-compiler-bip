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
    enum PrimitiveReduced {
        R_INT,
        R_FLO,
        R_DOU,
        R_STR,
        R_CHA,
        R_BOO,
        R_VOI,
        R_ERR
    };

    static PrimitiveReduced OPERATION_TABLE[7][7][7];
    static PrimitiveReduced UNARY_OPERATION_TABLE[7][7];
    static AttributionResult ATTRIBUTION_TABLE[7][7];
};

#endif // OPERATION_MANAGER_H
