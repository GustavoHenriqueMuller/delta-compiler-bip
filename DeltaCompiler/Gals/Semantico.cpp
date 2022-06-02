#include "Semantico.h"

void Semantico::executeAction(int action, const Token *token) throw (SemanticError) {
    TokenId tokenId = token->getId();
    std::string lexeme = token->getLexeme();

    if (consoleParser.getDebug()) {
        std::cout << "Action: " << action << "\tToken: "  << tokenId << "\tLexeme: " << lexeme << std::endl;
    }

    switch (action) {
        /// PUSHING LITERALS
        case 1: // Int
            expressions.push(Expression(Type(INT)));
            generator.addImmediate(Utils::lexemeToInt(lexeme));
            break;
        case 2: // Float
            expressions.push(Expression(Type(FLOAT)));
            break;
        case 3: // String
            expressions.push(Expression(Type(STRING)));
            break;
        case 4: // Char
            expressions.push(Expression(Type(CHAR)));
            break;
        case 5: // Bool
            expressions.push(Expression(Type(BOOLEAN)));
            generator.addImmediate(Utils::lexemeToBoolean(lexeme));
            break;

        /// READING ID
        case 6: {
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol == nullptr) {
                throw IdentifierNotFoundError(lexeme);
            }

            identifierNames.push(lexeme);
            identifierTypes.push(symbol->type);
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

            identifierNames.push(lexeme);
            identifierTypes.push(symbol->type);
            break;
        }

        /// READING INDEX (ARRAY)
        case 8: {
            Expression index = expressions.top();

            if (index.type.primitive != INT) {
                throw InvalidArrayIndexError(identifierNames.top(), index.type);
            }

            identifierTypes.top().isArray = false;
            identifierTypes.top().arraySize = 0;
            break;
        }

        /// PUSHING LEFT-VALUE AS EXPRESSION
        case 9: {
            Symbol* symbol = getSymbolByName(identifierNames.top());

            if (symbol->type.isArray) {
                generator.addArrayIdentifier(*symbol);
            } else {
                generator.addIdentifier(*symbol);
            }

            if (!symbol->isInitialized) {
                logger.addWarn(ReadingIdentifierWithoutInitializationWarning(symbol->name));
            }

            expressions.push(Expression(identifierTypes.top()));
            identifierNames.pop();
            identifierTypes.pop();

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
            generator.addBinaryOperation(operations.top());
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

        /// READING ATTRIBUTION OPERATORS
        case 46:
            attributionOperation = Operation(ATTRIBUTION, lexeme);
            break;
        case 47:
            attributionOperation = Operation(INCREMENT_ATTRIBUTION, lexeme);
            break;
        case 48:
            attributionOperation = Operation(DECREMENT_ATTRIBUTION, lexeme);
            break;

        /// CREATING/DELETING SCOPES
        case 49: // Creating scope
            scopes.push_back(Scope(getScopeId()));
            break;
        case 50: // Deleting scope
            popScope();
            break;

        /// DECLARATION - MODIFIERS
        case 51: // Cleaning type
            leftType = Type();
            break;

        case 52: // Reading "const" modifier
            leftType.isConst = true;
            break;

        /// DECLARATION - PRIMITIVES
        case 53:
            leftType.primitive = INT;
            break;
        case 54:
            leftType.primitive = FLOAT;
            break;
        case 55:
            leftType.primitive = DOUBLE;
            break;
        case 56:
            leftType.primitive = STRING;
            break;
        case 57:
            leftType.primitive = CHAR;
            break;
        case 58:
            leftType.primitive = BOOLEAN;
            break;
        case 59:
            leftType.primitive = VOID;
            break;

        /// DECLARATION
        case 60: // Reading array type with size initializer
            leftType.isArray = true;
            leftType.arraySize = Utils::lexemeToInt(lexeme);
            break;

        case 61: { // Reading id of declaration id list
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol != nullptr) {
                throw DuplicateIdentifierError(lexeme);
            }

            leftIdentifierNames.push_back(lexeme);
            scopes.back().symbolList.push_back(Symbol(leftType, lexeme, scopes.back().id));
            break;
        }
        case 62: { // Finishing declaration
            for (const std::string &name : leftIdentifierNames) {
                Symbol* symbol = getSymbolByName(name);

                if (symbol->type.isConst && !symbol->isInitialized) {
                    throw MissingConstInitializationError(symbol->name);
                }

                symbol->isInDeclaration = false;

                if (symbol->type.isArray) {
                    generator.addArrayIdentifierDeclaration(*symbol);
                } else {
                    generator.addIdentifierDeclaration(*symbol);
                }
            }

            leftIdentifierNames.clear();
            leftType = Type();
            break;
        }

        /// ATTRIBUTION
        case 63: { // Attribution
            Symbol* symbol = getSymbolByName(leftIdentifierNames.back());

            if (symbol->type.isArray) {
                generator.attributeToArray(*symbol, attributionOperation.type);
            } else {
                generator.attributeTo(*symbol, attributionOperation.type);
            }

            doAttribution();
            break;
        }

        case 64: // Gets left type of attribution outside of declaration
            leftIdentifierNames.push_back(identifierNames.top());
            leftType = identifierTypes.top();

            identifierNames.pop();
            identifierTypes.pop();
            break;

        case 65: // Clears identifier names after attribution outside of declaration
            leftIdentifierNames.pop_back();
            break;

        /// IF, ELSEIF, WHILE, DO WHILE, FOR BLOCK
        case 66: // Validating boolean expressions
        case 67:
        case 68:
        case 69:
        case 70: {
            std::string blockName;

            switch (action) {
                case 66:
                    blockName = "if";
                    break;
                case 67:
                    blockName = "elseif";
                    break;
                case 68:
                    blockName = "while";
                    break;
                case 69:
                    blockName = "do while";
                    break;
                case 70:
                    blockName = "for";
                    break;
            }

            if (OperationManager::checkImplicitCast(expressions.top().type, Type(BOOLEAN)) == ATT_ER) {
                throw InvalidExpressionForBlockError(expressions.top().type, blockName, Type(BOOLEAN));
            }

            expressions.pop();
            break;
        }

        /// WHEN STATEMENT
        case 71: { // Reading type of expression in "when" block
            whenExpressionTypes.push(expressions.top().type);
            expressions.pop();
            break;
        }
        case 72: { // Reading type of expression in "is" block
            Expression expression = expressions.top();
            AttributionResult result = OperationManager::checkImplicitCast(expression.type, whenExpressionTypes.top());

            if (result == ATT_ER) {
                throw InvalidExpressionForBlockError(expression.type, "is", whenExpressionTypes.top());
            }

            expressions.pop();
            break;
        }
        case 73: // End of "when" block
            whenExpressionTypes.pop();
            break;

        /// FUNCTIONS
        case 74: { // Reading function declaration identifier
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol != nullptr) {
                throw DuplicateIdentifierError(lexeme);
            }

            Symbol function = Symbol(leftType, lexeme, scopes.back().id);
            function.isFunction = true;
            scopes.back().symbolList.push_back(function);

            functionName = lexeme;
            break;
        }
        case 75: { // Reading function declaration parameter identifier
            Symbol* function = getSymbolByName(functionName);
            function->parameters.push_back(Parameter(leftType, lexeme));

            leftType = Type();
            break;
        }
        case 76: { // Creating identifiers in function scope from function parameters
            Symbol* function = getSymbolByName(functionName);

            for (const Parameter &parameter : function->parameters) {
                Symbol symbol = Symbol(parameter.type, parameter.name, scopes.back().id);
                symbol.isInDeclaration = false;
                symbol.isInitialized = true;

                scopes.back().symbolList.push_back(symbol);
            }

            function->isInDeclaration = false;
            functionName.clear();
            break;
        }
        case 77: { // Reading function call identifier
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
        case 78: { // Reading function call parameter
            Symbol* function = getSymbolByName(functionName);

            if (amountFunctionParameters >= function->parameters.size()) {
                throw InvalidFunctionParameterQuantityError(function->name, function->parameters.size(), amountFunctionParameters + 1);
            }

            Type functionParameterType = function->parameters[amountFunctionParameters].type;
            Expression expression = expressions.top();

            AttributionResult result = OperationManager::checkImplicitCast(functionParameterType, expression.type);

            if (result == ATT_ER) {
                throw InvalidFunctionParameterError(function->name, amountFunctionParameters + 1, functionParameterType, expression.type);
            }

            amountFunctionParameters++;
            expressions.pop();
            break;
        }
        case 79: { // Finish function call
            Symbol* function = getSymbolByName(functionName);

            if (amountFunctionParameters != function->parameters.size()) {
                throw InvalidFunctionParameterQuantityError(function->name, function->parameters.size(), amountFunctionParameters);
            }

            expressions.push(Expression(function->type));

            amountFunctionParameters = 0;
            functionName.clear();
            break;
        }
        case 80: { // Create scope for function
            Symbol* function = getSymbolByName(functionName);
            Scope scope = Scope(getScopeId());

            scope.returnType = function->type;
            scopes.push_back(scope);
            break;
        }

        /// RETURN STATEMENT
        case 81: { // Check for type when returning from function
            Expression expression = expressions.top();
            AttributionResult result = OperationManager::checkImplicitCast(expression.type, scopes.back().returnType);

            if (result == ATT_ER) {
                throw InvalidFunctionReturnTypeError(expression.type, scopes.back().returnType);
            }

            scopes.back().hasReturned = true;
            expressions.pop();
            break;
        }

        /// INPUT AND OUTPUT
        case 82: { // Print
            if (expressions.top().type.isArray) {
                throw InvalidFunctionParameterError("print", 0, Type(VOID), expressions.top().type);
            }

            generator.addPrint();
            expressions.pop();
            break;
        }

        case 83: { // Input
            expressions.push(Expression(INT));
            generator.addInput();
            break;
        }
    }
}

void Semantico::doAttribution() {
    Symbol* symbol = getSymbolByName(leftIdentifierNames.back());

    Type expressionType = expressions.top().type;
    Type typeToAssign = leftType;

    if (symbol == nullptr) {
        throw IdentifierNotFoundError(symbol->name);
    }

    if (leftType.isConst && !symbol->isInDeclaration) {
        throw ConstMutationError(symbol->name);
    }

    AttributionResult resultType = OperationManager::checkAttribution(expressionType, typeToAssign, attributionOperation);

    if (resultType == ATT_ER) {
        throw IncompatibleAttributionTypesError(symbol->name, expressionType, typeToAssign, attributionOperation);
    } else if (resultType == ATT_PL) {
        logger.addWarn(PrecisionLossWarning(symbol->name, expressionType, typeToAssign));
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
        throw IncompatibleOperationTypesError(operation, a.type, b.type);
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
        throw IncompatibleUnaryOperationTypeError(operation.lexeme, expression.type);
    }

    OperationCategory category = operation.getOperationCategory();

    if ((category == CATEGORY_UNARY_LEFT_MUTABLE || category == CATEGORY_UNARY_RIGHT_MUTABLE) && expression.type.isConst) {
        throw ConstMutationError(identifierNames.top());
    }

    expressions.push(Expression(resultType));
}

Symbol* Semantico::getSymbolByName(const std::string &name) {
    for (Scope &scope : scopes) {
        for (Symbol &symbol : scope.symbolList) {
            if (symbol.name == name) {
                return &symbol;
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
        jsonBuilder.set("arraySize", symbol.type.arraySize);

        jsonBuilder.set("isConst", symbol.type.isConst);
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
    if (!scopes.back().hasReturned && scopes.back().returnType.primitive != VOID) {
        throw MissingReturnStatementError(scopes.back().returnType);
    }

    for (const Symbol &symbol : scopes.back().symbolList) {
        if (!symbol.isUsed) {
            logger.addWarn(UnusedIdentifierWarning(symbol.name));
        }
    }

    saveScope(scopes.back());
    scopes.pop_back();
}

std::string Semantico::getScopesJson() {
    return jsonBuilder.build();
}

Semantico::Semantico(Logger &logger, Generator &generator, ConsoleParser &consoleParser):
    logger(logger), generator(generator), consoleParser(consoleParser)
{
    scopes.push_back(Scope(getScopeId()));
}
