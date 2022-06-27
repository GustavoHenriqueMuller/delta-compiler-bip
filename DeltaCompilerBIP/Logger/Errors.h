#ifndef ERRORS_H
#define ERRORS_H

#include "../Model/Symbol.h"
#include "../Model/Type.h"
#include "../Model/Operation.h"
#include "../Gals/SemanticError.h"

#include "vector"

static std::string parameterTypesToString(const std::vector<Type> &parameterTypes) {
    std::string result = "";

    for (int i = 0; i < parameterTypes.size(); i++) {
        result += parameterTypes[i].toString();

        if (i < parameterTypes.size() - 1) {
            result += ", ";
        }
    }

    return result;
}

static std::string parameterTypesToString(const Symbol &symbol) {
    std::string result = "";

    for (int i = 0; i < symbol.parameters.size(); i++) {
        result += symbol.parameters[i].type.toString();

        if (i < symbol.parameters.size() - 1) {
            result += ", ";
        }
    }

    return result;
}

class ConstMutationError : public SemanticError {
public:
    ConstMutationError(const std::string &identifier)
      : SemanticError("Trying to mutate const identifier '" + identifier + "'", -1) { }
};


class IdentifierAlreadyExistsError : public SemanticError {
public:
    IdentifierAlreadyExistsError(const std::string &identifier)
      : SemanticError("Identifier '" + identifier + "' already exists", -1) { }
};


class FunctionIdentifierAlreadyExistsError : public SemanticError {
public:
    FunctionIdentifierAlreadyExistsError(const Symbol &symbol)
      : SemanticError("Function identifier '" + symbol.name + "(" + parameterTypesToString(symbol) + ")' already exists", -1) { }
};


class IdentifierNotFoundError : public SemanticError {
public:
    IdentifierNotFoundError(const std::string &identifier)
      : SemanticError("Identifier '" + identifier + "' not found", -1) { }
};


class FunctionIdentifierNotFoundError : public SemanticError {
public:
    FunctionIdentifierNotFoundError(const std::string &identifier, const std::vector<Type> &parameterTypes)
      : SemanticError("Function identifier '" + identifier + "(" + parameterTypesToString(parameterTypes) + ")' not found", -1) {}
};


class IncompatibleAssignmentTypesError : public SemanticError {
public:
    IncompatibleAssignmentTypesError(const std::string &identifier, const Type &expressionType, const Type &identifierType, const Operation &assignment)
      : SemanticError("Cannot attribute value of type '" + expressionType.toString() + "' to identifer '" + identifier + "' of type '" + identifierType.toString() + "' for assignment operator '" + assignment.lexeme + "'", -1) { }
};


class IncompatibleOperationTypesError : public SemanticError {
public:
    IncompatibleOperationTypesError(const Operation &op, const Type &type1, const Type &type2)
      : SemanticError("Incompatible types for '" + op.lexeme + "' operator: '" + type1.toString() + "' and '" + type2.toString() +  "'", -1) { }
};

class IncompatibleUnaryOperationTypeError : public SemanticError {
public:
    IncompatibleUnaryOperationTypeError(const std::string &op, const Type &type)
      : SemanticError("Incompatible type for '" + op + "' unary operator: '" + type.toString() + "'", -1) { }
};


class InvalidArrayIndexError : public SemanticError {
public:
    InvalidArrayIndexError(const std::string &arrayName, const Type &indexType)
      : SemanticError("Invalid index type for array '" + arrayName + "': '" + indexType.toString() + "', expected 'int'", -1) { }
};


class InvalidExpressionForBlockError : public SemanticError {
public:
    InvalidExpressionForBlockError(const Type &type, const std::string &blockName, const Type &expectedType)
      : SemanticError("Invalid expression type '" + type.toString() + "' for '" + blockName + "' block, expected '" + expectedType.toString() + "'", -1) { }
};


class InvalidFunctionParameterError : public SemanticError {
public:
    InvalidFunctionParameterError(const std::string &functionName, int paramPos, const Type &paramType, const Type &givenType)
      : SemanticError("Expected '" + paramType.toString() + "' for function '" + functionName + "' at parameter " + std::to_string(paramPos) + ", got '" + givenType.toString() + "'", -1) { }
};


class InvalidFunctionReturnTypeError : public SemanticError {
public:
    InvalidFunctionReturnTypeError(const Type &returnType, const Type &givenReturnType)
      : SemanticError("Expected return of type '" + returnType.toString() + "', got '" + givenReturnType.toString() + "'", -1) { }
};


class MissingConstInitializationError : public SemanticError {
public:
    MissingConstInitializationError(const std::string &identifier)
      : SemanticError("Missing initialization for const identifier '" + identifier + "'", -1) { }
};


class MissingReturnStatementError : public SemanticError {
public:
    MissingReturnStatementError(const Type &type)
      : SemanticError("Missing return statement, expected '" + type.toString() + "' return") { }
};


class ReadingNonArrayAsArrayError : public SemanticError {
public:
    ReadingNonArrayAsArrayError(const std::string &identifier)
      : SemanticError("Reading non-array identifier '" + identifier + "' as array", -1) { }
};

#endif // ERRORS_H
