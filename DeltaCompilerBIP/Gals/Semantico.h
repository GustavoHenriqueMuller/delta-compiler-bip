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
#include "../Model/Action.h"
#include "../Model/OperationManager.h"
#include "../Logger/Errors.h"
#include "../Logger/Logger.h"
#include "../Logger/Warnings.h"
#include "../Utils/JsonBuilder.h"
#include "../Utils/Utils.h"
#include "../Generator/Generator.h"
#include "../ConsoleParser/ConsoleParser.h"

#include <stack>

class Semantico {
public:
    Semantico(Logger &logger, Generator &generator, ConsoleParser &consoleParser);
    void executeAction(int action, const Token *token) throw (SemanticError);
    void popScope();
    std::string getScopesJson();

private:
    void doAssignment();
    void doOperation();
    void doUnaryOperation();

    Symbol* getSymbolByName(const std::string &name);

    Symbol* findAppropriateFunctionCall();
    bool isSymbolAppropriateForFunctionCall(const Symbol &symbol);
    bool declaredFunctionExists();

    void saveScope(const Scope &scope);
    int getScopeId();

    // Logger and Generator
    Logger &logger;
    Generator &generator;
    ConsoleParser &consoleParser;

    // JsonBuilder
    JsonBuilder jsonBuilder;

    // Scopes and expressions
    std::vector<Scope> scopes;
    std::stack<Expression> expressions;
    std::stack<Operation> operations;
    unsigned int scopeCounter = 0;

    // Reading names and indexes from identifiers
    std::stack<std::string> identifierNames;
    std::stack<Type> identifierTypes;

    // Declaration / Assignment
    std::vector<std::string> leftIdentifierNames;
    Type leftType;
    Operation assignmentOperation;

    // Functions
    Symbol functionDeclaration;
    std::stack<std::string> functionCallNames;
    std::stack<std::vector<Type>> functionCallParameterTypes;

    // "When-is" statement
    std::stack<Type> whenExpressionTypes;
    std::stack<int> whenIsIds;

    // Structures
    std::stack<int> structureIds;
    std::stack<int> elseIfIds;
    int currentStructureId = 0;

    // Delaying actions
    std::stack<std::vector<Action>> delayedActions;
    bool isDelayingActions = false;
};

#endif
