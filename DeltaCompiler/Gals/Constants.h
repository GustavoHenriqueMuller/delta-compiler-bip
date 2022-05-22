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
    t_AND = 28,
    t_OR = 29,
    t_NOT = 30,
    t_GREATER = 31,
    t_SMALLER = 32,
    t_GREATER_EQ = 33,
    t_SMALLER_EQ = 34,
    t_EQ = 35,
    t_DIF = 36,
    t_INCREMENT_ATTRIBUTION = 37,
    t_DECREMENT_ATTRIBUTION = 38,
    t_INCREMENT = 39,
    t_DECREMENT = 40,
    t_PLUS = 41,
    t_MINUS = 42,
    t_MULT = 43,
    t_DIV = 44,
    t_MOD = 45,
    t_ATTRIBUTION = 46,
    t_BIT_AND = 47,
    t_BIT_OR = 48,
    t_BIT_XOR = 49,
    t_BIT_NOT = 50,
    t_BIT_RS = 51,
    t_BIT_LS = 52,
    t_LSP = 53,
    t_RSP = 54,
    t_LSB = 55,
    t_RSB = 56,
    t_LSC = 57,
    t_RSC = 58,
    t_DOT = 59,
    t_END = 60,
    t_COLON = 61,
    t_COMMA = 62,
    t_ID = 63
};

const int STATES_COUNT = 144;

extern int SCANNER_TABLE[STATES_COUNT][256];

extern int TOKEN_STATE[STATES_COUNT];

extern const char *SCANNER_ERROR[STATES_COUNT];

const int FIRST_SEMANTIC_ACTION = 118;

const int SHIFT  = 0;
const int REDUCE = 1;
const int ACTION = 2;
const int ACCEPT = 3;
const int GO_TO  = 4;
const int ERROR  = 5;

extern const int PARSER_TABLE[314][199][2];

extern const int PRODUCTIONS[130][2];

extern const char *PARSER_ERROR[314];

#endif
