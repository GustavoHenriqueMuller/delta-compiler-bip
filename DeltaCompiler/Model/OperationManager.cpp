#include "OperationManager.h"

Primitive OperationManager::OPERATION_TABLE[7][7][7] = {
    //              INT                           FLOAT                         DOUBLE                        STRING                        CHAR                          BOOLEAN                       VOID
    //              PLU MIN MUL DIV MOD REL BIT   PLU MIN MUL DIV MOD REL BIT   PLU MIN MUL DIV MOD REL BIT   PLU MIN MUL DIV MOD REL BIT   PLU MIN MUL DIV MOD REL BIT   PLU MIN MUL DIV MOD REL BIT   PLU MIN MUL DIV MOD REL BIT
    /* INT     */ {{INT,INT,INT,FLO,INT,BOO,INT},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}},
    /* FLOAT   */ {{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}},
    /* DOUBLE  */ {{DOU,DOU,DOU,DOU,ERR,BOO,ERR},{DOU,DOU,DOU,DOU,ERR,BOO,ERR},{DOU,DOU,DOU,DOU,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}},
    /* STRING  */ {{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{STR,ERR,ERR,ERR,ERR,BOO,ERR},{STR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}},
    /* CHAR    */ {{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{STR,ERR,ERR,ERR,ERR,BOO,ERR},{STR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}},
    /* BOOLEAN */ {{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{FLO,FLO,FLO,FLO,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,BOO,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}},
    /* VOID    */ {{ERR,ERR,ERR,ERR,ERR,ERR,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR},{ERR,ERR,ERR,ERR,ERR,ERR,ERR}}
};

Primitive OperationManager::UNARY_OPERATION_TABLE[7][7] = {
    //             IR  DR  MI  BN  NO  IL  DL
    /* INT     */ {INT,INT,INT,INT,BOO,INT,INT},
    /* DOUBLE  */ {DOU,DOU,DOU,ERR,BOO,DOU,DOU},
    /* FLOAT   */ {FLO,FLO,FLO,ERR,BOO,FLO,FLO},
    /* STRING  */ {ERR,ERR,ERR,ERR,BOO,ERR,ERR},
    /* CHAR    */ {ERR,ERR,ERR,ERR,BOO,ERR,ERR},
    /* BOOLEAN */ {ERR,ERR,ERR,BOO,BOO,ERR,ERR},
    /* VOID    */ {ERR,ERR,ERR,ERR,ERR,ERR,ERR}
};

AttributionResult OperationManager::ATTRIBUTION_TABLE[7][7] = {
    //             INT    FLOAT  DOUBLE STRING CHAR   BOOL   VOID
    /* INT     */ {ATT_OK,ATT_OK,ATT_OK,ATT_ER,ATT_ER,ATT_OK,ATT_ER},
    /* DOUBLE  */ {ATT_PL,ATT_OK,ATT_OK,ATT_ER,ATT_ER,ATT_ER,ATT_ER},
    /* FLOAT   */ {ATT_PL,ATT_OK,ATT_PL,ATT_ER,ATT_ER,ATT_ER,ATT_ER},
    /* STRING  */ {ATT_ER,ATT_ER,ATT_ER,ATT_OK,ATT_ER,ATT_ER,ATT_ER},
    /* CHAR    */ {ATT_ER,ATT_ER,ATT_ER,ATT_OK,ATT_OK,ATT_ER,ATT_ER},
    /* BOOLEAN */ {ATT_ER,ATT_ER,ATT_ER,ATT_ER,ATT_ER,ATT_OK,ATT_ER},
    /* VOID    */ {ATT_ER,ATT_ER,ATT_ER,ATT_ER,ATT_ER,ATT_ER,ATT_ER}
};

Primitive OperationManager::checkBinaryOperation(Type type1, Type type2, Operation operation) {
    OperationCategory category = operation.getOperationCategory();
    int operationIndex;

    switch (category) {
        case CATEGORY_ARIT_LOW: {
            switch (operation.type) {
                case ADD:
                    operationIndex = 0;
                    break;
                case SUBTRACT:
                    operationIndex = 1;
                    break;
            }
            break;
        }
        case CATEGORY_ARIT_HIGH: {
            switch (operation.type) {
                case MULTIPLY:
                    operationIndex = 2;
                    break;
                case DIVIDE:
                    operationIndex = 3;
                    break;
                case MOD:
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

    return OperationManager::OPERATION_TABLE[type1.primitive][type2.primitive][operationIndex];
}

Primitive OperationManager::checkUnaryOperation(Type type1, Operation operation) {
    return OperationManager::UNARY_OPERATION_TABLE[type1.primitive][operation.type - 18];
}

AttributionResult OperationManager::checkImplicitCast(Type type1, Type type2) {
    if (type1.isArray != type2.isArray) {
        return ATT_ER;
    }

    return OperationManager::ATTRIBUTION_TABLE[type1.primitive][type2.primitive];
}

AttributionResult OperationManager::checkAttribution(Type type1, Type type2, Operation operation) {
    Type rightType = type2;
    OperationType attributionType = operation.type;

    if (attributionType == INCREMENT_ATTRIBUTION || attributionType == DECREMENT_ATTRIBUTION) {
        Primitive result = checkBinaryOperation(type1, type2, getBinaryOperationFromAttributionType(attributionType));

        if (result == ERR) {
            return ATT_ER;
        } else {
            rightType.primitive = result;
        }
    }

    return checkImplicitCast(type1, rightType);
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
