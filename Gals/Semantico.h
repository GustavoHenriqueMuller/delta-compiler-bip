#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "Token.h"
#include "SemanticError.h"
#include "Semantico.h"
#include "Constants.h"

#include "../Model/Type.h"
#include "../Model/Scope.h"
#include "../Model/Symbol.h"
#include "../Model/Expression.h"
#include "../Model/Operation.h"
#include "../Logger/Errors.h"
#include "../Logger/Logger.h"
#include "../Logger/Warnings.h"
#include "../Utils/JsonBuilder.h"

#include <stack>

class Semantico {
public:
    Semantico(Logger &logger);

    void executeAction(int action, const Token *token) throw (SemanticError);
    void popScope();
    std::string getScopesJson();

private:
    void doAttribution();
    void doOperation();
    void doUnaryOperation();
    Symbol* getSymbolByName(std::string name);

    void saveScope(const Scope &scope);
    int getScopeId();

    // Logger
    Logger &logger;

    // JsonBuilder
    JsonBuilder jsonBuilder;

    // Scopes and expressions
    std::vector<Scope> scopes;
    std::stack<Expression> expressions;
    std::stack<Operation> operations;
    int scopeCounter = 0;

    // Reading names and indexes from identifiers
    std::string identifierName;
    Type identifierType;

    // Declaration / Attribution
    std::vector<std::string> leftIdentifierNames;
    Type leftType;
    Operation attributionOperation;

    // Functions
    std::string functionName;
    int amountFunctionParameters = 0;

    // "When-is" statement
    std::stack<Type> whenExpressionTypes;
};

#endif
