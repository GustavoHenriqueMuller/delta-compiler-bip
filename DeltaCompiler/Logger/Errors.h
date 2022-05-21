#ifndef ERRORS_H
#define ERRORS_H

#include "../Gals/SemanticError.h"

class ConstMutationError : public SemanticError {
public:
    ConstMutationError(const std::string &identifier)
      : SemanticError("Trying to mutate const identifier '" + identifier + "'", -1) { }
};


class DuplicateIdentifierError : public SemanticError {
public:
    DuplicateIdentifierError(const std::string &identifier)
      : SemanticError("Identifier '" + identifier + "' already exists", -1) { }
};


class IdentifierIsNotAFunctionError : public SemanticError {
public:
    IdentifierIsNotAFunctionError(const std::string &identifier)
      : SemanticError("Identifier '" + identifier + "' cannot be called, because it is not a function", -1) { }
};


class IdentifierNotFoundError : public SemanticError {
public:
    IdentifierNotFoundError(const std::string &identifier)
      : SemanticError("Identifier '" + identifier + "' not found", -1) { }
};


class IncompatibleAttributionTypesError : public SemanticError {
public:
    IncompatibleAttributionTypesError(const std::string &assignedName, const std::string &type1String, const std::string &type2String, const std::string &attributionLexeme)
      : SemanticError("Cannot attribute value of type '" + type1String + "' to identifer '" + assignedName + "' of type '" + type2String + "' for attribution operator '" + attributionLexeme + "'", -1) { }
};


class IncompatibleOperationTypesError : public SemanticError {
public:
    IncompatibleOperationTypesError(const std::string &op, const std::string &type1String, const std::string &type2String)
      : SemanticError("Incompatible types for '" + op + "' operator: '" + type1String + "' and '" + type2String +  "'", -1) { }
};

class IncompatibleUnaryOperationTypeError : public SemanticError {
public:
    IncompatibleUnaryOperationTypeError(const std::string &op, const std::string &typeString)
      : SemanticError("Incompatible type for '" + op + "' unary operator: '" + typeString + "'", -1) { }
};


class InvalidArrayIndexError : public SemanticError {
public:
    InvalidArrayIndexError(const std::string &arrayName, const std::string &indexType)
      : SemanticError("Invalid index type for array '" + arrayName + "': '" + indexType + "', expected 'int'", -1) { }
};


class InvalidArraySizeInitializerError : public SemanticError {
public:
    InvalidArraySizeInitializerError(const std::string &type)
      : SemanticError("Size initializer of " + type + " array must be 'int'", -1) { }
};


class InvalidExpressionForBlockError : public SemanticError {
public:
    InvalidExpressionForBlockError(const std::string &typeString, const std::string &blockName, const std::string &expectedType)
      : SemanticError("Invalid expression type '" + typeString + "' for '" + blockName + "' block, expected '" + expectedType + "'", -1) { }
};


class InvalidFunctionParameterError : public SemanticError {
public:
    InvalidFunctionParameterError(const std::string &functionName, int paramPos, const std::string &paramType, const std::string &givenType)
      : SemanticError("Expected '" + paramType + "' for function '" + functionName + "' at parameter " + std::to_string(paramPos) + ", got '" + givenType + "'", -1) { }
};


class InvalidFunctionParameterQuantityError : public SemanticError {
public:
    InvalidFunctionParameterQuantityError(const std::string &functionName, int paramQnt, int givenParamQnt)
      : SemanticError("Expected " + std::to_string(paramQnt) + " parameters for function '" + functionName + "', got " + std::to_string(givenParamQnt), -1) { }
};


class InvalidFunctionReturnTypeError : public SemanticError {
public:
    InvalidFunctionReturnTypeError(const std::string &returnType, const std::string &givenReturnType)
      : SemanticError("Expected return of type '" + returnType + "', got '" + givenReturnType + "'", -1) { }
};


class MissingArrayIndexError : public SemanticError {
public:
    MissingArrayIndexError(const std::string &arrayName)
      : SemanticError("Missing array index for accessing '" + arrayName + "'", -1) { }
};


class MissingConstInitializationError : public SemanticError {
public:
    MissingConstInitializationError(const std::string &identifier)
      : SemanticError("Missing initialization for const identifier '" + identifier + "'", -1) { }
};


class MissingReturnStatementError : public SemanticError {
public:
    MissingReturnStatementError(const std::string &type)
      : SemanticError("Missing return statement, expected '" + type + "' return") { }
};


class ReadingNonArrayAsArrayError : public SemanticError {
public:
    ReadingNonArrayAsArrayError(const std::string &identifier)
      : SemanticError("Reading non-array identifier '" + identifier + "' as array", -1) { }
};

#endif // ERRORS_H
