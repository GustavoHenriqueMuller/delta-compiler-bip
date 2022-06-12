#include "Semantico.h"

void Semantico::executeAction(int action, const Token *token) throw (SemanticError) {
    TokenId tokenId = token->getId();
    std::string lexeme = token->getLexeme();

    if (action == 106) {
        isDelayingActions = false;
    }

    if (isDelayingActions) {
        delayedActions.push_back(Action(action, *token));
        return;
    }

    if (consoleParser.isDebug()) {
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
                logger.addWarning(ReadingIdentifierWithoutInitializationWarning(symbol->name));
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
            generator.addBinaryOperation(operations.top().type);
            doOperation();
            break;

        /// DOING UNARY OPERATION
        case 19:
        case 20: {
            OperationCategory category = getOperationCategory(operations.top().type);

            if (category == CATEGORY_UNARY_LEFT_MUTABLE || category == CATEGORY_UNARY_RIGHT_MUTABLE) {
                Symbol* symbol = getSymbolByName(identifierNames.top());

                if (symbol->type.isArray) {
                    generator.addMutableUnaryOperationOnArray(operations.top().type, *symbol);
                } else {
                    generator.addMutableUnaryOperation(operations.top().type, *symbol);
                }

                expressions.push(Expression(symbol->type));
            } else {
                generator.addUnaryOperation(operations.top().type);
            }

            doUnaryOperation();

            identifierNames.pop();
            identifierTypes.pop();
            break;
        }

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

        /// READING ASSIGNMENT OPERATORS
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56: {
            assignmentOperation = Operation(getAssignmentOperationTypeFromTokenId(tokenId), lexeme);
            break;
        }

        /// CREATING/DELETING SCOPES
        case 57: // Creating scope
            scopes.push_back(Scope(getScopeId()));
            break;
        case 58: // Deleting scope
            popScope();
            break;

        /// DECLARATION - MODIFIERS
        case 59: // Cleaning type
            leftType = Type();
            break;

        case 60: // Reading "const" modifier
            leftType.isConst = true;
            break;

        /// DECLARATION - PRIMITIVES
        case 61:
            leftType.primitive = INT;
            break;
        case 62:
            leftType.primitive = FLOAT;
            break;
        case 63:
            leftType.primitive = DOUBLE;
            break;
        case 64:
            leftType.primitive = STRING;
            break;
        case 65:
            leftType.primitive = CHAR;
            break;
        case 66:
            leftType.primitive = BOOLEAN;
            break;
        case 67:
            leftType.primitive = VOID;
            break;

        /// DECLARATION
        case 68: // Reading array type with size initializer
            leftType.isArray = true;
            leftType.arraySize = Utils::lexemeToInt(lexeme);
            break;

        case 69: { // Reading id of declaration id list
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol != nullptr) {
                throw DuplicateIdentifierError(lexeme);
            }

            leftIdentifierNames.push_back(lexeme);
            scopes.back().symbolList.push_back(Symbol(leftType, lexeme, scopes.back().id));
            break;
        }
        case 70: { // Finishing declaration
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

        /// ASSIGNMENT
        case 71: { // Assignment
            Symbol* symbol = getSymbolByName(leftIdentifierNames.back());

            if (symbol->type.isArray) {
                generator.assignToArray(*symbol, assignmentOperation.type);
            } else {
                generator.assignTo(*symbol, assignmentOperation.type);
            }

            doAssignment();
            break;
        }

        case 72: // Gets left type of assignment outside of declaration
            leftIdentifierNames.push_back(identifierNames.top());
            leftType = identifierTypes.top();

            identifierNames.pop();
            identifierTypes.pop();
            break;

        case 73: // Clears identifier names after assignment outside of declaration
            leftIdentifierNames.pop_back();
            break;

        /// IF, ELSEIF, WHILE, DO WHILE, FOR BLOCK
        case 74: // Validating boolean expressions
        case 75:
        case 76:
        case 77:
        case 78: {
            std::string blockName;

            switch (action) {
                case 74:
                    blockName = "if";
                    break;
                case 75:
                    blockName = "elseif";
                    break;
                case 76:
                    blockName = "while";
                    break;
                case 77:
                    blockName = "do while";
                    break;
                case 78:
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
        case 79: { // Reading type of expression in "when" block
            whenExpressionTypes.push(expressions.top().type);
            expressions.pop();
            break;
        }

        case 80: { // Reading type of expression in "is" block
            Expression expression = expressions.top();
            AssignmentResult result = OperationManager::checkImplicitCast(expression.type, whenExpressionTypes.top());

            if (result == ATT_ER) {
                throw InvalidExpressionForBlockError(expression.type, "is", whenExpressionTypes.top());
            }

            expressions.pop();
            break;
        }

        case 81: // End of "when" block
            whenExpressionTypes.pop();
            break;

        /// FUNCTIONS
        case 82: { // Reading function declaration identifier
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

        case 83: { // Reading function declaration parameter identifier
            Symbol* function = getSymbolByName(functionName);
            function->parameters.push_back(Parameter(leftType, lexeme));

            leftType = Type();
            break;
        }

        case 84: { // Creating identifiers in function scope from function parameters
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

        case 85: { // Reading function call identifier
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

        case 86: { // Reading function call parameter
            Symbol* function = getSymbolByName(functionName);

            if (amountFunctionParameters >= function->parameters.size()) {
                throw InvalidFunctionParameterQuantityError(function->name, function->parameters.size(), amountFunctionParameters + 1);
            }

            Type functionParameterType = function->parameters[amountFunctionParameters].type;
            Expression expression = expressions.top();

            AssignmentResult result = OperationManager::checkImplicitCast(functionParameterType, expression.type);

            if (result == ATT_ER) {
                throw InvalidFunctionParameterError(function->name, amountFunctionParameters + 1, functionParameterType, expression.type);
            }

            amountFunctionParameters++;
            expressions.pop();
            break;
        }

        case 87: { // Finish function call
            Symbol* function = getSymbolByName(functionName);

            if (amountFunctionParameters != function->parameters.size()) {
                throw InvalidFunctionParameterQuantityError(function->name, function->parameters.size(), amountFunctionParameters);
            }

            expressions.push(Expression(function->type));

            amountFunctionParameters = 0;
            functionName.clear();
            break;
        }

        case 88: { // Create scope for function
            Symbol* function = getSymbolByName(functionName);
            Scope scope = Scope(getScopeId());

            scope.returnType = function->type;
            scopes.push_back(scope);
            break;
        }

        /// RETURN STATEMENT
        case 89: { // Check for type when returning from function
            Expression expression = expressions.top();
            AssignmentResult result = OperationManager::checkImplicitCast(expression.type, scopes.back().returnType);

            if (result == ATT_ER) {
                throw InvalidFunctionReturnTypeError(expression.type, scopes.back().returnType);
            }

            scopes.back().hasReturned = true;
            expressions.pop();
            break;
        }

        /// INPUT AND OUTPUT
        case 90: { // Print
            if (expressions.top().type.isArray) {
                throw InvalidFunctionParameterError("print", 0, Type(VOID), expressions.top().type);
            }

            generator.addPrint();

            expressions.pop();
            expressions.push(Expression(VOID));
            break;
        }

        case 91: { // Input
            generator.addInput();
            expressions.push(Expression(INT));
            break;
        }

        /// CODE GENERATION OF "WHILE"
        case 92: { // Creating start label of "while"
            generator.addLabel("while_start_" + std::to_string(currentStructureId));

            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 93: { // Checking expression of "while"
            generator.addBranchIfFalse("while_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 94: { // Creating end label of "while"
            generator.addJump("while_start_" + std::to_string(structureIds.top()));
            generator.addLabel("while_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            break;
        }

        /// CODE GENERATION OF "DO WHILE"
        case 95: { // Creating start label of "do while"
            generator.addLabel("do_while_start_" + std::to_string(currentStructureId));

            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 96: { // Checking expression of "do while"
            generator.addBranchIfTrue("do_while_start_" + std::to_string(structureIds.top()));
            structureIds.pop();
            break;
        }

        /// CODE GENERATION OF FOR
        case 97: { // Creating start label of "for"
            generator.addLabel("for_start_" + std::to_string(currentStructureId));

            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 98: { // Checking expression of "for"
            generator.addBranchIfFalse("for_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 99: { // Creating end label of "for"
            generator.addJump("for_start_" + std::to_string(structureIds.top()));
            generator.addLabel("for_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            break;
        }

        /// CODE GENERATION OF IF / ELSE IF / ELSE
        case 100: { // Checking expression for "if"
            generator.addBranchIfFalse("if_end_" + std::to_string(currentStructureId));

            elseIfIds.push(0);
            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 101: { // Adds label at end of "if"
            generator.addJump("if_stmt_end_" + std::to_string(structureIds.top()));
            generator.addLabel("if_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 102: { // Checking expression for "else if"
            generator.addBranchIfFalse("else_if_" + std::to_string(elseIfIds.top()) + "_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 103: { // Adds label at end of "else if"
            generator.addJump("if_stmt_end_" + std::to_string(structureIds.top()));
            generator.addLabel("else_if_" + std::to_string(elseIfIds.top()) + "_end_" + std::to_string(structureIds.top()));

            elseIfIds.top() += 1;
            break;
        }

        case 104: { // Adds label for end of entire "if" statement
            generator.addLabel("if_stmt_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            elseIfIds.pop();
            break;
        }

        /// DELAYING ACTIONS
        case 105: { // Starts delaying actions
            isDelayingActions = true;
            break;
        }

        case 106: { // Stops delaying actions (implemented at start of executeAction)
            break;
        }

        case 107: { // Executes delayed actions
            for (const Action &action : delayedActions) {
                executeAction(action.action, &action.token);
            }

            delayedActions.clear();
            break;
        }

        /// POPPING EXPRESSIONS
        case 108: {
            expressions.pop();
            generator.popStack();
            break;
        }

        /// CODE GENERATION OF "WHEN IS"
        case 109: { // // Checking expression for "is"
            generator.addBinaryOperation(EQUAL);
            generator.addBranchIfFalse("when_is_" + std::to_string(whenIsIds.top()) + "_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 110: { // Adds label at end of "when"
            generator.addLabel("when_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            whenIsIds.pop();
            break;
        }

        case 111: { // Adds jump from end of "is" to end of "when" statement
            generator.addJump("when_end_" + std::to_string(structureIds.top()));
            generator.addLabel("when_is_" + std::to_string(whenIsIds.top()) + "_end_" + std::to_string(structureIds.top()));

            whenIsIds.top() += 1;
            break;
        }

        case 112: { // Starts "is" statement
            whenIsIds.push(0);
            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }
    }
}

void Semantico::doAssignment() {
    Symbol* symbol = getSymbolByName(leftIdentifierNames.back());

    Type expressionType = expressions.top().type;
    Type typeToAssign = leftType;

    if (symbol == nullptr) {
        throw IdentifierNotFoundError(symbol->name);
    }

    if (leftType.isConst && !symbol->isInDeclaration) {
        throw ConstMutationError(symbol->name);
    }

    AssignmentResult resultType = OperationManager::checkAssignment(expressionType, typeToAssign, assignmentOperation);

    if (resultType == ATT_ER) {
        throw IncompatibleAssignmentTypesError(symbol->name, expressionType, typeToAssign, assignmentOperation);
    } else if (resultType == ATT_PL) {
        logger.addWarning(PrecisionLossWarning(symbol->name, expressionType, typeToAssign));
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

    OperationCategory category = getOperationCategory(operation.type);

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
            logger.addWarning(UnusedIdentifierWarning(symbol.name));
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
