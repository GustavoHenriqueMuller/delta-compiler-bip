#ifndef CONSTANTS_H
#define CONSTANTS_H

enum TokenId 
{
    EPSILON  = 0,
    DOLLAR   = 1,
    t_INT_DEC = 2,
    t_INT_HEX = 3,
    t_INT_BIN = 4,
    t_FLOAT = 5,
    t_STRING = 6,
    t_CHAR = 7,
    t_BOOLEAN = 8,
    t_VAR_INT = 9,
    t_VAR_FLOAT = 10,
    t_VAR_DOUBLE = 11,
    t_VAR_STRING = 12,
    t_VAR_CHAR = 13,
    t_VAR_BOOLEAN = 14,
    t_VAR_VOID = 15,
    t_CONST = 16,
    t_REF = 17,
    t_IF = 18,
    t_ELSE = 19,
    t_ELSEIF = 20,
    t_WHILE = 21,
    t_DO = 22,
    t_FOR = 23,
    t_WHEN = 24,
    t_IS = 25,
    t_RETURN = 26,
    t_DEFAULT = 27,
    t_PRINT = 28,
    t_INPUT = 29,
    t_AND = 30,
    t_OR = 31,
    t_NOT = 32,
    t_GREATER = 33,
    t_SMALLER = 34,
    t_GREATER_EQ = 35,
    t_SMALLER_EQ = 36,
    t_EQ = 37,
    t_DIF = 38,
    t_INCREMENT_ATTRIBUTION = 39,
    t_DECREMENT_ATTRIBUTION = 40,
    t_INCREMENT = 41,
    t_DECREMENT = 42,
    t_PLUS = 43,
    t_MINUS = 44,
    t_MULT = 45,
    t_DIV = 46,
    t_MOD = 47,
    t_ATTRIBUTION = 48,
    t_BIT_AND = 49,
    t_BIT_OR = 50,
    t_BIT_XOR = 51,
    t_BIT_NOT = 52,
    t_BIT_RS = 53,
    t_BIT_LS = 54,
    t_LSP = 55,
    t_RSP = 56,
    t_LSB = 57,
    t_RSB = 58,
    t_LSC = 59,
    t_RSC = 60,
    t_DOT = 61,
    t_END = 62,
    t_COLON = 63,
    t_COMMA = 64,
    t_ID = 65
};

const int STATES_COUNT = 152;

extern int SCANNER_TABLE[STATES_COUNT][256];

extern int TOKEN_STATE[STATES_COUNT];

extern const char *SCANNER_ERROR[STATES_COUNT];

const int FIRST_SEMANTIC_ACTION = 120;

const int SHIFT  = 0;
const int REDUCE = 1;
const int ACTION = 2;
const int ACCEPT = 3;
const int GO_TO  = 4;
const int ERROR  = 5;

extern const int PARSER_TABLE[323][204][2];

extern const int PRODUCTIONS[132][2];

extern const char *PARSER_ERROR[323];

#endif
