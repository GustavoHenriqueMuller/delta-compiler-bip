#include "OperationManager.h"

#include <iostream>

OperationManager::PrimitiveReduced OperationManager::OPERATION_TABLE[7][7][7] = {
    //              INT                                         FLOAT                                       DOUBLE                                      STRING                                      CHAR                                        BOOLEAN                                     VOID
    //              ADD   MIN   MUL   DIV   MOD   REL   BIT     ADD   MIN   MUL   DIV   MOD   REL   BIT     ADD   MIN   MUL   DIV   MOD   REL   BIT     ADD   MIN   MUL   DIV   MOD   REL   BIT     ADD   MIN   MUL   DIV   MOD   REL   BIT     ADD   MIN   MUL   DIV   MOD   REL   BIT     ADD   MIN   MUL   DIV   MOD   REL   BIT
    /* INT     */ {{R_INT,R_INT,R_INT,R_FLO,R_INT,R_BOO,R_INT},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}},
    /* FLOAT   */ {{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}},
    /* DOUBLE  */ {{R_DOU,R_DOU,R_DOU,R_DOU,R_ERR,R_BOO,R_ERR},{R_DOU,R_DOU,R_DOU,R_DOU,R_ERR,R_BOO,R_ERR},{R_DOU,R_DOU,R_DOU,R_DOU,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}},
    /* STRING  */ {{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_STR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_STR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}},
    /* CHAR    */ {{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_STR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_STR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}},
    /* BOOLEAN */ {{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_FLO,R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}},
    /* VOID    */ {{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR},{R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}}
};

OperationManager::PrimitiveReduced OperationManager::UNARY_OPERATION_TABLE[7][7] = {
    //             IR  DR  MI  BN  NO  IL  DL
    /* INT     */ {R_INT,R_INT,R_INT,R_INT,R_BOO,R_INT,R_INT},
    /* DOUBLE  */ {R_DOU,R_DOU,R_DOU,R_ERR,R_BOO,R_DOU,R_DOU},
    /* FLOAT   */ {R_FLO,R_FLO,R_FLO,R_ERR,R_BOO,R_FLO,R_FLO},
    /* STRING  */ {R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR,R_ERR},
    /* CHAR    */ {R_ERR,R_ERR,R_ERR,R_ERR,R_BOO,R_ERR,R_ERR},
    /* BOOLEAN */ {R_ERR,R_ERR,R_ERR,R_BOO,R_BOO,R_ERR,R_ERR},
    /* VOID    */ {R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR,R_ERR}
};

AssignmentResult OperationManager::ASSIGNMENT_TABLE[7][7] = {
    //             INT    FLOAT  DOUBLE STRING CHAR   BOOL   VOID
    /* INT     */ {ASSIGNMENT_OK,ASSIGNMENT_OK,ASSIGNMENT_OK,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_OK,ASSIGNMENT_ER},
    /* DOUBLE  */ {ASSIGNMENT_PL,ASSIGNMENT_OK,ASSIGNMENT_OK,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER},
    /* FLOAT   */ {ASSIGNMENT_PL,ASSIGNMENT_OK,ASSIGNMENT_PL,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER},
    /* STRING  */ {ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_OK,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER},
    /* CHAR    */ {ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_OK,ASSIGNMENT_OK,ASSIGNMENT_ER,ASSIGNMENT_ER},
    /* BOOLEAN */ {ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_OK,ASSIGNMENT_ER},
    /* VOID    */ {ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_ER,ASSIGNMENT_OK}
};

Primitive OperationManager::checkBinaryOperation(const Type& type1, const Type& type2, const Operation& operation) {
    OperationCategory category = getOperationCategory(operation.type);
    int operationIndex;

    switch (category) {
        case CATEGORY_ARIT_LOW: {
            switch (operation.type) {
                case OP_ADDITION:
                    operationIndex = 0;
                    break;
                case OP_SUBTRACTION:
                    operationIndex = 1;
                    break;
            }
            break;
        }
        case CATEGORY_ARIT_HIGH: {
            switch (operation.type) {
                case OP_MULTIPLICATION:
                    operationIndex = 2;
                    break;
                case OP_DIVISION:
                    operationIndex = 3;
                    break;
                case OP_MOD:
                    operationIndex = 4;
                    break;
            }
            break;
        }
        case CATEGORY_RELATIONAL: {
            operationIndex = 5;
            break;
        }
        case CATEGORY_BIT: {
            operationIndex = 6;
            break;
        }
    }

    return (Primitive) OperationManager::OPERATION_TABLE[type1.primitive][type2.primitive][operationIndex];
}

Primitive OperationManager::checkUnaryOperation(const Type& type1, const Operation& operation) {
    if (type1.isArray) {
        return PRIMITIVE_ERR;
    }

    return (Primitive) OperationManager::UNARY_OPERATION_TABLE[type1.primitive][operation.type - 18];
}

AssignmentResult OperationManager::checkImplicitCast(const Type& type1, const Type& type2) {
    if (type1.isArray != type2.isArray || type1.arraySize != type2.arraySize) {
        return ASSIGNMENT_ER;
    }

    return OperationManager::ASSIGNMENT_TABLE[type1.primitive][type2.primitive];
}

AssignmentResult OperationManager::checkAssignment(const Type& type1, const Type& type2, const Operation& operation) {
    Type rightType = type2;
    OperationType attributionType = operation.type;

    if (attributionType == OP_INCREMENT_ASSIGNMENT || attributionType == OP_DECREMENT_ASSIGNMENT) {
        Primitive result = checkBinaryOperation(type1, type2, getBinaryOperationFromAssignmentType(attributionType));

        if (result == R_ERR) {
            return ASSIGNMENT_ER;
        } else {
            rightType.primitive = result;
        }
    }

    return checkImplicitCast(rightType, type1);
}
