#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H

#include "Operation.h"

enum AssignmentResult {
    ASSIGNMENT_OK,
    ASSIGNMENT_ER,
    ASSIGNMENT_PL
};

class OperationManager {
public:
    static Primitive checkBinaryOperation(const Type& type1, const Type& type2, const Operation& operation);
    static Primitive checkUnaryOperation(const Type& type1, const Operation& operation);
    static AssignmentResult checkImplicitCast(const Type& type1, const Type& type2);
    static AssignmentResult checkAssignment(const Type& type1, const Type& type2, const Operation& operation);

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
    static AssignmentResult ASSIGNMENT_TABLE[7][7];
};

#endif // OPERATION_MANAGER_H
