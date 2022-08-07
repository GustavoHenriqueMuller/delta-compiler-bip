#ifndef CONSTANTS_H
#define CONSTANTS_H

enum TokenId 
{
    EPSILON  = 0,
    DOLLAR   = 1,
    t_INT_DEC = 2,
    t_INT_HEX = 3,
    t_INT_BIN = 4,
    t_CHAR = 5,
    t_BOOLEAN = 6,
    t_VAR_INT = 7,
    t_VAR_CHAR = 8,
    t_VAR_BOOLEAN = 9,
    t_VAR_VOID = 10,
    t_CONST = 11,
    t_REF = 12,
    t_IF = 13,
    t_ELSE = 14,
    t_ELSEIF = 15,
    t_WHILE = 16,
    t_DO = 17,
    t_FOR = 18,
    t_WHEN = 19,
    t_IS = 20,
    t_RETURN = 21,
    t_DEFAULT = 22,
    t_PRINT = 23,
    t_INPUT = 24,
    t_AND = 25,
    t_OR = 26,
    t_NOT = 27,
    t_GREATER = 28,
    t_SMALLER = 29,
    t_GREATER_EQ = 30,
    t_SMALLER_EQ = 31,
    t_EQ = 32,
    t_DIF = 33,
    t_INCREMENT_ASSIGNMENT = 34,
    t_DECREMENT_ASSIGNMENT = 35,
    t_MULTIPLICATION_ASSIGNMENT = 36,
    t_DIVSION_ASSIGNMENT = 37,
    t_MOD_ASSIGNMENT = 38,
    t_BIT_OR_ASSIGNMENT = 39,
    t_BIT_XOR_ASSIGNMENT = 40,
    t_BIT_AND_ASSIGNMENT = 41,
    t_BIT_LS_ASSIGNMENT = 42,
    t_BIT_RS_ASSIGNMENT = 43,
    t_INCREMENT = 44,
    t_DECREMENT = 45,
    t_PLUS = 46,
    t_MINUS = 47,
    t_MULT = 48,
    t_DIV = 49,
    t_MOD = 50,
    t_ASSIGNMENT = 51,
    t_BIT_AND = 52,
    t_BIT_OR = 53,
    t_BIT_XOR = 54,
    t_BIT_NOT = 55,
    t_BIT_LS = 56,
    t_BIT_RS = 57,
    t_LSP = 58,
    t_RSP = 59,
    t_LSB = 60,
    t_RSB = 61,
    t_LSC = 62,
    t_RSC = 63,
    t_DOT = 64,
    t_END = 65,
    t_COLON = 66,
    t_COMMA = 67,
    t_ID = 68
};

const int STATES_COUNT = 139;

extern int SCANNER_TABLE[STATES_COUNT][256];

extern int TOKEN_STATE[STATES_COUNT];

extern const char *SCANNER_ERROR[STATES_COUNT];

const int FIRST_SEMANTIC_ACTION = 125;

const int SHIFT  = 0;
const int REDUCE = 1;
const int ACTION = 2;
const int ACCEPT = 3;
const int GO_TO  = 4;
const int ERROR  = 5;

extern const int PARSER_TABLE[357][629][2];

extern const int PRODUCTIONS[136][2];

extern const char *PARSER_ERROR[357];

#endif
