#include "Semantico.h"

void Semantico::executeAction(int action, const Token *token) throw (SemanticError) {
    // std::cout << "Action: " << action << "\tToken: "  << token->getId() << "\tLexeme: " << token->getLexeme() << std::endl;

    TokenId tokenId = token->getId();
    std::string lexeme = token->getLexeme();

    switch (action) {
        /// PUSHING LITERALS
        case 1: // Int
            expressions.push(Expression(Type(INT)));
            break;
        case 2: // Float
            expressions.push(Expression(Type(FLO)));
            break;
        case 3: // String
            expressions.push(Expression(Type(STR)));
            break;
        case 4: // Char
            expressions.push(Expression(Type(CHA)));
            break;
        case 5: // Bool
            expressions.push(Expression(Type(BOO)));
            break;

        /// READING ID
        case 6: {
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol == nullptr) {
                throw IdentifierNotFoundError(lexeme);
            }

            if (symbol->type.isArray) {
                throw MissingArrayIndexError(lexeme);
            }

            identifierName = lexeme;
            identifierType = symbol->type;
            break;
        }

        /// READING ID (ARRAY)
        case 7: {
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol == nullptr) {
                throw IdentifierNotFoundError(lexeme);
            }

            if (!symbol->type.isArray) {
                throw ReadingNonArrayAsArrayError(lexeme);
            }

            identifierName = lexeme;
            identifierType = symbol->type;
            identifierType.isArray = false;
            break;
        }

        /// READING INDEX (ARRAY)
        case 8: {
            Expression index = expressions.top();

            if (index.type.primitive != INT) {
                throw InvalidArrayIndexError(identifierName, index.type.toString());
            }

            break;
        }

        /// PUSHING IDENTIFIER AS EXPRESSION
        case 9: {
            Symbol* symbol = getSymbolByName(identifierName);

            if (!symbol->isInitialized) {
                logger.addWarn(ReadingIdentifierWithoutInitializationWarning(symbol->name));
            }

            expressions.push(Expression(identifierType));
            symbol->isUsed = true;
            break;
        }

        /// DOING BINARY OPERATION
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
            doOperation();
            break;

        /// DOING UNARY OPERATION
        case 19:
        case 20:
            doUnaryOperation();
            break;

        /// READING BINARY OPERATOR
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
            operations.push(Operation(getOperationTypeFromTokenId(tokenId), lexeme));
            break;

        /// READING RIGHT UNARY OPERATOR
        case 39:
        case 40:
            operations.push(Operation(getRightUnaryOperationTypeFromTokenId(tokenId), lexeme));
            break;

        /// READING LEFT UNARY OPERATOR
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
            operations.push(Operation(getLeftUnaryOperationTypeFromTokenId(tokenId), lexeme));
            break;

        /// CREATING/DELETING SCOPES
        case 46: // Creating scope
            scopes.push_back(Scope(getScopeId()));
            break;
        case 47: // Deleting scope
            popScope();
            break;

        /// DECLARATION - MODIFIERS
        case 48: // Reading "const ref" modifier
            leftType = Type();
            leftType.isConst = true;
            leftType.isRef = true;
            break;
        case 49: // Reading "const" modifier
            leftType = Type();
            leftType.isConst = true;
            break;
        case 50: // Reading "ref" modifier
            leftType = Type();
            leftType.isRef = true;
            break;

        /// DECLARATION - PRIMITIVES
        case 51:
            leftType.primitive = INT;
            break;
        case 52:
            leftType.primitive = FLO;
            break;
        case 53:
            leftType.primitive = DOU;
            break;
        case 54:
            leftType.primitive = STR;
            break;
        case 55:
            leftType.primitive = CHA;
            break;
        case 56:
            leftType.primitive = BOO;
            break;
        case 57:
            leftType.primitive = VOI;
            break;

        /// DECLARATION
        case 58: // Reading array type
            leftType.isArray = true;
            break;

        case 59: { // Reading array type with size initializer
            Expression size = expressions.top();

            if (size.type.primitive != INT) {
                throw InvalidArraySizeInitializerError(leftType.toString());
            }

            expressions.pop();
            leftType.isArray = true;
            break;
        }
        case 60: { // Reading id of declaration id list
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol != nullptr) {
                throw DuplicateIdentifierError(lexeme);
            }

            leftIdentifierNames.push_back(lexeme);
            scopes.back().symbolList.push_back(Symbol(leftType, lexeme));
            break;
        }
        case 61: { // Finishing declaration
            for (const std::string &name : leftIdentifierNames) {
                Symbol* symbol = getSymbolByName(name);

                if (symbol->type.isConst && !symbol->isInitialized) {
                    throw MissingConstInitializationError(symbol->name);
                }

                symbol->isInDeclaration = false;
            }

            leftIdentifierNames.clear();
            leftType = Type();
            break;
        }

        /// ATTRIBUTION IN DECLARATION
        case 62: // Attribution
            doAttribution();
            break;

        /// ATTRIBUTION
        case 63: // Gets left type of attribution outside of declaration
            leftIdentifierNames.push_back(identifierName);
            leftType = identifierType;
            break;

        /// IF, ELSEIF, WHILE, DO WHILE, FOR BLOCK
        case 64: // Validating boolean expressions
        case 65:
        case 66:
        case 67:
        case 68: {
            std::string blockName;

            switch (action) {
                case 64:
                    blockName = "if";
                    break;
                case 65:
                    blockName = "elseif";
                    break;
                case 66:
                    blockName = "while";
                    break;
                case 67:
                    blockName = "do while";
                    break;
                case 68:
                    blockName = "for";
                    break;
            }

            if (OperationManager::checkImplicitCast(expressions.top().type, Type(BOO)) == ATT_ER) {
                throw InvalidExpressionForBlockError(expressions.top().type.toString(), blockName, Type(BOO).toString());
            }

            expressions.pop();
            break;
        }

        /// FUNCTIONS
        case 69: { // Reading function declaration identifier
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol != nullptr) {
                throw DuplicateIdentifierError(lexeme);
            }

            Symbol function = Symbol(leftType, lexeme);
            function.isFunction = true;
            scopes.back().symbolList.push_back(function);

            functionName = lexeme;
            break;
        }
        case 70: { // Reading function declaration parameter identifier
            Symbol* function = getSymbolByName(functionName);
            function->parameters.push_back(Parameter(leftType, lexeme));
            break;
        }
        case 71: { // Creating identifiers in function scope from function parameters
            Symbol* function = getSymbolByName(functionName);

            for (const Parameter &parameter : function->parameters) {
                Symbol symbol = Symbol(parameter.type, parameter.name);
                symbol.isInDeclaration = false;
                symbol.isInitialized = true;

                scopes.back().symbolList.push_back(symbol);
            }

            functionName.clear();
            break;
        }
        case 72: { // Reading function call identifier
            Symbol* function = getSymbolByName(lexeme);

            if (function == nullptr) {
                throw IdentifierNotFoundError(lexeme);
            }

            if (!function->isFunction) {
                throw IdentifierIsNotAFunctionError(lexeme);
            }

            function->isUsed = true;
            functionName = lexeme;

            expressions.push(Expression(function->type));
            break;
        }
        case 73: { // Reading function call parameter
            Symbol* function = getSymbolByName(functionName);

            if (amountFunctionParameters >= function->parameters.size()) {
                throw InvalidFunctionParameterQuantityError(function->name, function->parameters.size(), amountFunctionParameters + 1);
            }

            Type functionParameterType = function->parameters[amountFunctionParameters].type;
            Expression expression = expressions.top();

            AttributionResult result = OperationManager::checkImplicitCast(functionParameterType, expression.type);

            if (result == ATT_ER) {
                throw InvalidFunctionParameterError(function->name, amountFunctionParameters + 1, functionParameterType.toString(), expression.type.toString());
            }
            
            amountFunctionParameters++;
            expressions.pop();
            break;
        }
        case 74: { // Finish function call
            Symbol* function = getSymbolByName(functionName);

            if (amountFunctionParameters != function->parameters.size()) {
                throw InvalidFunctionParameterQuantityError(function->name, function->parameters.size(), amountFunctionParameters);
            }

            expressions.push(Expression(function->type));

            amountFunctionParameters = 0;
            functionName.clear();
            break;
        }
        case 75: { // Create scope for function
            Symbol* function = getSymbolByName(functionName);
            Scope scope = Scope(getScopeId());

            scope.returnType = function->type;
            scopes.push_back(scope);
            break;
        }

        /// RETURN STATEMENT
        case 76: { // Check for type when returning from function
            Expression expression = expressions.top();
            AttributionResult result = OperationManager::checkImplicitCast(expression.type, scopes.back().returnType);

            if (result == ATT_ER) {
                throw InvalidFunctionReturnTypeError(expression.type.toString(), scopes.back().returnType.toString());
            }

            scopes.back().hasReturned = true;
            expressions.pop();
            break;
        }

        /// WHEN STATEMENT
        case 77: { // Reading type of expression in "when" block
            whenExpressionTypes.push(expressions.top().type);
            expressions.pop();
            break;
        }
        case 78: { // Reading type of expression in "is" block
            Expression expression = expressions.top();
            AttributionResult result = OperationManager::checkImplicitCast(expression.type, whenExpressionTypes.top());

            if (result == ATT_ER) {
                throw InvalidExpressionForBlockError(expression.type.toString(), "is", whenExpressionTypes.top().toString());
            }

            expressions.pop();
            break;
        }
        case 79: // End of "when" block
            whenExpressionTypes.pop();
            break;

        /// READING ATTRIBUTION OPERATORS
        case 80:
            attributionOperation = Operation(ATTRIBUTION, lexeme);
            break;
        case 81:
            attributionOperation = Operation(INCREMENT_ATTRIBUTION, lexeme);
            break;
        case 82:
            attributionOperation = Operation(DECREMENT_ATTRIBUTION, lexeme);
            break;
    }
}

void Semantico::doAttribution() {
    Symbol* symbol = getSymbolByName(leftIdentifierNames.back());
    Type expressionType = expressions.top().type;

    if (symbol == nullptr) {
        throw IdentifierNotFoundError(symbol->name);
    }

    AttributionResult resultType = OperationManager::checkAttribution(expressionType, leftType, attributionOperation);

    if (resultType == ATT_ER) {
        throw IncompatibleAttributionTypesError(symbol->name, expressionType.toString(), leftType.toString(), attributionOperation.lexeme);
    } else if (resultType == ATT_PL) {
        logger.addWarn(PrecisionLossWarning(symbol->name, leftType.toString(), expressionType.toString()));
    }

    if (leftType.isConst && !symbol->isInDeclaration) {
        throw ConstMutationError(symbol->name);
    }

    symbol->isInitialized = true;
    expressions.pop();
}

void Semantico::doOperation() {
    Expression b = expressions.top();
    expressions.pop();

    Expression a = expressions.top();
    expressions.pop();

    Operation operation = operations.top();
    operations.pop();

    Type resultType = OperationManager::checkBinaryOperation(a.type, b.type, operation);

    if (resultType.primitive == ERR) {
        throw IncompatibleOperationTypesError(operation.lexeme, a.type.toString(), b.type.toString());
    } else {
        expressions.push(Expression(resultType));
    }
}

void Semantico::doUnaryOperation() {
    Expression expression = expressions.top();
    expressions.pop();

    Operation operation = operations.top();
    operations.pop();

    Type resultType = OperationManager::checkUnaryOperation(expression.type, operation);

    if (resultType.primitive == ERR) {
        throw IncompatibleUnaryOperationTypeError(operation.lexeme, expression.type.toString());
    }

    OperationType type = operation.operationType;

    if (type == INCREMENT_LEFT || type == DECREMENT_LEFT || type == INCREMENT_RIGHT || type == DECREMENT_RIGHT) {
        if (expression.type.isConst) {
            throw ConstMutationError(identifierName);
        }
    }

    expressions.push(Expression(resultType));
}

Symbol* Semantico::getSymbolByName(std::string name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        for (int j = 0; j < scopes[i].symbolList.size(); j++) {
            if (scopes[i].symbolList[j].name == name) {
                return &scopes[i].symbolList[j];
            }
        }
    }

    return nullptr;
}

void Semantico::saveScope(const Scope &scope) {
    for (Symbol symbol : scope.symbolList) {
        jsonBuilder.open();

        jsonBuilder.set("scope", scope.id);
        jsonBuilder.set("name", symbol.name);
        jsonBuilder.set("type", symbol.type.toString());
        jsonBuilder.set("isArray", symbol.type.isArray);
        jsonBuilder.set("isConst", symbol.type.isConst);
        jsonBuilder.set("isRef", symbol.type.isRef);
        jsonBuilder.set("isInitialized", symbol.isInitialized);
        jsonBuilder.set("isUsed", symbol.isUsed);
        jsonBuilder.set("isInDeclaration", symbol.isInDeclaration);
        jsonBuilder.set("isFunction", symbol.isFunction);

        std::string parameters = "";
        for (Parameter parameter : symbol.parameters)
            parameters += parameter.type.toString() + " " + parameter.name + ", ";

        if (parameters.size() > 0)
            parameters.erase(parameters.begin() + parameters.size() - 2, parameters.end());

        jsonBuilder.set("parameters", parameters);
        jsonBuilder.close();
    }
}

int Semantico::getScopeId() {
    return this->scopeCounter++;
}

void Semantico::popScope() {
    if (!scopes.back().hasReturned && scopes.back().returnType.primitive != VOI) {
        throw MissingReturnStatementError(scopes.back().returnType.toString());
    }

    for (int i = 0; i < scopes.back().symbolList.size(); i++) {
        if (!scopes.back().symbolList[i].isUsed) {
            logger.addWarn(UnusedIdentifierWarning(scopes.back().symbolList[i].name));
        }
    }

    saveScope(scopes.back());
    scopes.pop_back();
}

std::string Semantico::getScopesJson() {
    return jsonBuilder.build();
}

Semantico::Semantico(Logger &logger): logger(logger) {
    scopes.push_back(Scope(getScopeId()));
}
