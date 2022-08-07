#include "Semantico.h"

void Semantico::executeAction(int action, const Token* token) throw (SemanticError) {
    TokenId tokenId = token->getId();
    std::string lexeme = token->getLexeme();

    if (action == 501) {
        isDelayingActions = false;
    }

    if (isDelayingActions) {
        delayedActions.top().push_back(Action(action, *token));
        return;
    }

    if (consoleParser.isDebug()) {
        std::cout << "Action: " << action << "\tToken: "  << tokenId << "\tLexeme: " << lexeme << std::endl;
    }

    switch (action) {
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Expressions

        /// PUSHING LITERALS
        case 101: // Int
            expressions.push(Expression(Type(PRIMITIVE_INT)));
            generator.addImmediate(Utils::lexemeToInt(lexeme));
            break;
        case 102: // Float
            expressions.push(Expression(Type(PRIMITIVE_FLOAT)));
            break;
        case 103: // String
            expressions.push(Expression(Type(PRIMITIVE_STRING)));
            break;
        case 104: // Char
            expressions.push(Expression(Type(PRIMITIVE_CHAR)));
            generator.addImmediate(Utils::lexemeToChar(lexeme));
            break;
        case 105: // Bool
            expressions.push(Expression(Type(PRIMITIVE_BOOLEAN)));
            generator.addImmediate(Utils::lexemeToBoolean(lexeme));
            break;

        /// READING ID
        case 106: {
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol == nullptr) {
                throw IdentifierNotFoundError(lexeme);
            }

            if (symbol->type.isArray) {
                throw MissingArrayIndexError(lexeme);
            }

            identifierNames.push(lexeme);
            identifierTypes.push(symbol->type);
            break;
        }

        /// READING ID (ARRAY)
        case 107: {
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
        case 108: {
            Expression index = expressions.top();

            if (index.type.primitive != PRIMITIVE_INT) {
                throw InvalidArrayIndexError(identifierNames.top(), index.type);
            }

            identifierTypes.top().isArray = false;
            identifierTypes.top().arraySize = 0;

            expressions.pop();
            break;
        }

        /// PUSHING LEFT-VALUE AS EXPRESSION
        case 109: {
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
            symbol->isUsed = true;

            identifierNames.pop();
            identifierTypes.pop();
            break;
        }

        /// DOING BINARY OPERATION
        case 110:
        case 111:
        case 112:
        case 113:
        case 114:
        case 115:
        case 116:
        case 117:
        case 118:
            generator.addBinaryOperation(operations.top().type);
            doOperation();
            break;

        /// DOING UNARY OPERATION
        case 119: // Applying unary operation on right-value
            generator.addUnaryOperation(operations.top().type);
            doUnaryOperation();
            break;

        case 120: { // Applying unary operation on left-value
            Symbol* symbol = getSymbolByName(identifierNames.top());

            if (!symbol->isInitialized) {
                logger.addWarning(ReadingIdentifierWithoutInitializationWarning(symbol->name));
            }

            expressions.push(Expression(identifierTypes.top()));
            symbol->isUsed = true;

            OperationCategory category = getOperationCategory(operations.top().type);

            if (category == CATEGORY_UNARY_LEFT_MUTABLE || category == CATEGORY_UNARY_RIGHT_MUTABLE) {
                if (symbol->type.isArray) {
                    generator.addMutableUnaryOperationOnArray(operations.top().type, *symbol);
                } else {
                    generator.addMutableUnaryOperation(operations.top().type, *symbol);
                }
            } else {
                if (symbol->type.isArray) {
                    generator.addArrayIdentifier(*symbol);
                } else {
                    generator.addIdentifier(*symbol);
                }

                generator.addUnaryOperation(operations.top().type);
            }

            doUnaryOperation();

            identifierNames.pop();
            identifierTypes.pop();
            break;
        }

        /// READING BINARY OPERATOR
        case 121:
        case 122:
        case 123:
        case 124:
        case 125:
        case 126:
        case 127:
        case 128:
        case 129:
        case 130:
        case 131:
        case 132:
        case 133:
        case 134:
        case 135:
        case 136:
        case 137:
        case 138:
            operations.push(Operation(getOperationTypeFromTokenId(tokenId), lexeme));
            break;

        /// READING RIGHT UNARY OPERATOR
        case 139:
        case 140:
            operations.push(Operation(getRightUnaryOperationTypeFromTokenId(tokenId), lexeme));
            break;

        /// READING LEFT UNARY OPERATOR
        case 141:
        case 142:
        case 143:
        case 144:
        case 145:
            operations.push(Operation(getLeftUnaryOperationTypeFromTokenId(tokenId), lexeme));
            break;

        /// READING ASSIGNMENT OPERATORS
        case 146:
        case 147:
        case 148:
        case 149:
        case 150:
        case 151:
        case 152:
        case 153:
        case 154:
        case 155:
        case 156:
            assignmentOperation = Operation(getAssignmentOperationTypeFromTokenId(tokenId), lexeme);
            break;

        /// CREATING/DELETING SCOPES
        case 157:
            scopes.push_back(Scope(getScopeId()));
            break;
        case 158:
            popScope();
            break;

        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Declaration and assignment

        /// DECLARATION - MODIFIERS
        case 200: // Reading "const" modifier
            leftType.isConst = true;
            break;

        /// DECLARATION - PRIMITIVES
        case 201:
            leftType.primitive = PRIMITIVE_INT;
            break;
        case 202:
            leftType.primitive = PRIMITIVE_FLOAT;
            break;
        case 203:
            leftType.primitive = PRIMITIVE_DOUBLE;
            break;
        case 204:
            leftType.primitive = PRIMITIVE_STRING;
            break;
        case 205:
            leftType.primitive = PRIMITIVE_CHAR;
            break;
        case 206:
            leftType.primitive = PRIMITIVE_BOOLEAN;
            break;
        case 207:
            leftType.primitive = PRIMITIVE_VOID;
            break;

        /// DECLARATION
        case 208: // Reading array type with size initializer
            leftType.isArray = true;
            leftType.arraySize = Utils::lexemeToInt(lexeme);
            break;

        case 209: { // Reading id of declaration id list
            Symbol* symbol = getSymbolByName(lexeme);

            if (symbol != nullptr) {
                throw IdentifierAlreadyExistsError(lexeme);
            }

            leftIdentifierNames.push_back(lexeme);
            scopes.back().symbolList.push_back(Symbol(leftType, lexeme, scopes.back().id));
            break;
        }

        case 210: { // Finishing declaration
            for (const std::string& name : leftIdentifierNames) {
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
        case 211: { // Assignment
            Symbol* symbol = getSymbolByName(leftIdentifierNames.back());

            if (symbol->type.isArray) {
                generator.assignToArray(*symbol, assignmentOperation.type);
            } else {
                generator.assignTo(*symbol, assignmentOperation.type);
            }

            doAssignment();
            break;
        }

        case 212: // Gets left type of assignment outside of declaration
            leftIdentifierNames.push_back(identifierNames.top());
            leftType = identifierTypes.top();

            identifierNames.pop();
            identifierTypes.pop();
            break;

        case 213: // Clears identifier names after assignment outside of declaration
            leftIdentifierNames.pop_back();
            break;

        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Statements and structure

        case 300: // Validating boolean expressions
        case 301:
        case 302:
        case 303:
        case 304: {
            std::string blockName;

            switch (action) {
                case 300:
                    blockName = "if";
                    break;
                case 301:
                    blockName = "elseif";
                    break;
                case 302:
                    blockName = "while";
                    break;
                case 303:
                    blockName = "do while";
                    break;
                case 304:
                    blockName = "for";
                    break;
            }

            if (OperationManager::checkImplicitCast(expressions.top().type, Type(PRIMITIVE_BOOLEAN)) == ASSIGNMENT_ER) {
                throw InvalidExpressionForBlockError(expressions.top().type, blockName, Type(PRIMITIVE_BOOLEAN));
            }

            expressions.pop();
            break;
        }

        /// WHEN STATEMENT
        case 305: { // Reading type of expression in "when" block
            whenExpressionTypes.push(expressions.top().type);
            break;
        }

        case 306: { // Reading type of expression in "is" block
            Expression expression = expressions.top();
            AssignmentResult result = OperationManager::checkImplicitCast(expression.type, whenExpressionTypes.top());

            if (result == ASSIGNMENT_ER) {
                throw InvalidExpressionForBlockError(expression.type, "is", whenExpressionTypes.top());
            }

            expressions.pop();
            break;
        }

        /// CODE GENERATION OF "WHILE"
        case 307: { // Creating start label of "while"
            generator.addLabel("while_start_" + std::to_string(currentStructureId));

            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 308: { // Checking expression of "while"
            generator.addBranchIfFalse("while_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 309: { // Creating end label of "while"
            generator.addJump("while_start_" + std::to_string(structureIds.top()));
            generator.addLabel("while_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            break;
        }

        /// CODE GENERATION OF "DO WHILE"
        case 310: { // Creating start label of "do while"
            generator.addLabel("do_while_start_" + std::to_string(currentStructureId));

            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 311: { // Checking expression of "do while"
            generator.addBranchIfTrue("do_while_start_" + std::to_string(structureIds.top()));
            structureIds.pop();
            break;
        }

        /// CODE GENERATION OF FOR
        case 312: { // Creating start label of "for"
            generator.addLabel("for_start_" + std::to_string(currentStructureId));

            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 313: { // Checking expression of "for"
            generator.addBranchIfFalse("for_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 314: { // Creating end label of "for"
            generator.addJump("for_start_" + std::to_string(structureIds.top()));
            generator.addLabel("for_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            break;
        }

        /// CODE GENERATION OF IF / ELSE IF / ELSE
        case 315: { // Checking expression for "if"
            generator.addBranchIfFalse("if_end_" + std::to_string(currentStructureId));

            elseIfIds.push(0);
            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 316: { // Adds label at end of "if"
            generator.addJump("if_stmt_end_" + std::to_string(structureIds.top()));
            generator.addLabel("if_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 317: { // Checking expression for "else if"
            generator.addBranchIfFalse("else_if_" + std::to_string(elseIfIds.top()) + "_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 318: { // Adds label at end of "else if"
            generator.addJump("if_stmt_end_" + std::to_string(structureIds.top()));
            generator.addLabel("else_if_" + std::to_string(elseIfIds.top()) + "_end_" + std::to_string(structureIds.top()));

            elseIfIds.top() += 1;
            break;
        }

        case 319: { // Adds label for end of entire "if" statement
            generator.addLabel("if_stmt_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            elseIfIds.pop();
            break;
        }

        /// CODE GENERATION OF "WHEN IS"
        case 320: { // // Checking expression for "is"
            generator.addBinaryOperation(OP_EQUAL);
            generator.addBranchIfFalse("when_is_" + std::to_string(whenIsIds.top()) + "_end_" + std::to_string(structureIds.top()));
            break;
        }

        case 321: { // Adds label at end of "when"
            generator.addLabel("when_end_" + std::to_string(structureIds.top()));

            structureIds.pop();
            whenIsIds.pop();
            break;
        }

        case 322: { // Adds jump from end of "is" to end of "when" statement
            generator.addJump("when_end_" + std::to_string(structureIds.top()));
            generator.addLabel("when_is_" + std::to_string(whenIsIds.top()) + "_end_" + std::to_string(structureIds.top()));

            whenIsIds.top() += 1;
            break;
        }

        case 323: { // Starts "when" statement
            whenIsIds.push(0);
            structureIds.push(currentStructureId);
            currentStructureId++;
            break;
        }

        case 324: { // Duplicates top of stack (expression in "when") for comparison in "is" statement
            expressions.push(expressions.top());
            generator.duplicateStackTop();
            break;
        }

        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Scopes and functions

        case 400: { // Reading function declaration identifier
            functionDeclaration = Symbol(leftType, lexeme, scopes.back().id);
            functionDeclaration.isFunction = true;
            leftType = Type();
            break;
        }

        case 401: { // Reading function declaration parameter identifier
            functionDeclaration.parameters.push_back(Symbol(leftType, lexeme, scopeCounter));
            leftType = Type();
            break;
        }

        case 402: { // Create scope for function
            std::string mangledName = functionDeclaration.getMangledName();

            if (functionExists(mangledName)) {
                throw FunctionIdentifierAlreadyExistsError(functionDeclaration);
            }

            scopes.back().symbolList.push_back(functionDeclaration);
            scopes.push_back(Scope(getScopeId(), functionDeclaration.type));

            generator.addLabel(mangledName);
            break;
        }

        case 403: { // Creating identifiers in function scope from function parameters
            for (Symbol& symbol : functionDeclaration.parameters) {
                symbol.isInDeclaration = false;
                symbol.isInitialized = true;

                scopes.back().symbolList.push_back(symbol);

                if (symbol.type.isArray) {
                    generator.addArrayIdentifierDeclaration(symbol);
                } else {
                    generator.addIdentifierDeclaration(symbol);
                }
            }

            break;
        }

        case 404: { // Reading function call identifier
            functionCallNames.push(lexeme);
            functionCallParameterTypes.push({});
            break;
        }

        case 405: { // Reading function call parameter
            functionCallParameterTypes.top().push_back(expressions.top().type);
            break;
        }

        case 406: { // Finish function call
            Symbol* function = findAppropriateFunctionCall();

            if (function == nullptr) {
                throw FunctionIdentifierNotFoundError(functionCallNames.top(), functionCallParameterTypes.top());
            }

            for (int i = function->parameters.size() - 1; i >= 0; i--) {
                Type typeToAssign = function->parameters[i].type;
                Type typeExpression = functionCallParameterTypes.top()[i];

                AssignmentResult result = OperationManager::checkImplicitCast(typeExpression, typeToAssign);

                if (result == ASSIGNMENT_PL) {
                    logger.addWarning(PrecisionLossWarning(function->parameters[i].name, typeExpression, typeToAssign));
                }

                generator.assignTo(function->parameters[i], OP_ASSIGNMENT);
                expressions.pop();
            }

            Symbol functionCopy = *function;

            generator.addCall(functionCopy);
            generator.pushStack();

            function->isUsed = true;
            expressions.push(Expression(function->type));

            functionCallNames.pop();
            functionCallParameterTypes.pop();
            break;
        }

        /// RETURN STATEMENT
        case 407: { // Return statement with expression
            Expression expression = expressions.top();
            AssignmentResult result = OperationManager::checkImplicitCast(expression.type, scopes.back().returnType);

            if (result == ASSIGNMENT_ER) {
                throw InvalidFunctionReturnTypeError(expression.type, scopes.back().returnType);
            }

            scopes.back().hasReturned = true;
            generator.addReturn();
            break;
        }

        case 408: { // Empty return statement
            AssignmentResult result = OperationManager::checkImplicitCast(Type(PRIMITIVE_VOID), scopes.back().returnType);

            if (result == ASSIGNMENT_ER) {
                throw InvalidFunctionReturnTypeError(Type(PRIMITIVE_VOID), scopes.back().returnType);
            }

            scopes.back().hasReturned = true;
            generator.addReturn();
            break;
        }

        /// INPUT AND OUTPUT
        case 409: { // Print
            if (expressions.top().type.isArray) {
                throw InvalidFunctionParameterError("print", 0, Type(PRIMITIVE_VOID), expressions.top().type);
            }

            generator.addPrint();

            expressions.pop();
            expressions.push(Expression(PRIMITIVE_VOID));
            break;
        }

        case 410: { // Input
            generator.addInput();
            expressions.push(Expression(PRIMITIVE_INT));
            break;
        }

        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Utilities

        /// DELAYING ACTIONS
        case 500: { // Starts delaying actions
            isDelayingActions = true;
            delayedActions.push({});
            break;
        }

        case 501: { // Stops delaying actions (implemented at start of executeAction)
            break;
        }

        case 502: { // Executes delayed actions
            for (const Action& action : delayedActions.top()) {
                executeAction(action.action, &action.token);
            }

            delayedActions.pop();
            break;
        }

        /// POPPING EXPRESSIONS
        case 503: {
            if (expressions.top().type.primitive != PRIMITIVE_VOID) {
                generator.popStack();
            }

            expressions.pop();
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

    if (resultType == ASSIGNMENT_ER) {
        throw IncompatibleAssignmentTypesError(symbol->name, expressionType, typeToAssign, assignmentOperation);
    } else if (resultType == ASSIGNMENT_PL) {
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

    if (resultType.primitive == PRIMITIVE_ERR) {
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

    if (resultType.primitive == PRIMITIVE_ERR) {
        throw IncompatibleUnaryOperationTypeError(operation.lexeme, expression.type);
    }

    OperationCategory category = getOperationCategory(operation.type);

    if ((category == CATEGORY_UNARY_LEFT_MUTABLE || category == CATEGORY_UNARY_RIGHT_MUTABLE) && expression.type.isConst) {
        throw ConstMutationError(identifierNames.top());
    }

    expressions.push(Expression(resultType));
}

Symbol* Semantico::getSymbolByName(const std::string& name) {
    for (Scope& scope : scopes) {
        for (Symbol& symbol : scope.symbolList) {
            if (symbol.name == name) {
                return &symbol;
            }
        }
    }

    return nullptr;
}

Symbol* Semantico::findAppropriateFunctionCall() {
    for (Symbol& symbol : scopes.front().symbolList) {
        if (isSymbolAppropriateForFunctionCall(symbol)) {
            return &symbol;
        }
    }

    return nullptr;
}

bool Semantico::isSymbolAppropriateForFunctionCall(const Symbol& symbol) {
    if (!symbol.isFunction || symbol.name != functionCallNames.top() || symbol.parameters.size() != functionCallParameterTypes.top().size()) {
        return false;
    }

    for (int i = 0; i < symbol.parameters.size(); i++) {
        if (OperationManager::checkImplicitCast(functionCallParameterTypes.top()[i], symbol.parameters[i].type) == ASSIGNMENT_ER) {
            return false;
        }
    }

    return true;
}

bool Semantico::functionExists(const std::string &functionName) {
    for (const Symbol& symbol : scopes.front().symbolList) {
        if (symbol.getMangledName() == functionName) {
            return true;
        }
    }

    return false;
}

void Semantico::saveScope(const Scope& scope) {
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

        for (const Symbol& parameter : symbol.parameters)
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
    if (scopes.size() > 1 && !scopes.back().hasReturned && scopes.back().returnType.primitive != PRIMITIVE_VOID) {
        throw MissingReturnStatementError(scopes.back().returnType);
    }

    for (const Symbol& symbol : scopes.back().symbolList) {
        if (!symbol.isUsed && symbol.getMangledName() != "void_main") {
            logger.addWarning(UnusedIdentifierWarning(symbol.name));
        }
    }

    if (scopes.size() == 1 && !functionExists("void_main")) {
        throw MissingEntryPointError();
    }

    saveScope(scopes.back());
    scopes.pop_back();
}

std::string Semantico::getScopesJson() {
    return jsonBuilder.build();
}

Semantico::Semantico(Logger& logger, Generator& generator, ConsoleParser& consoleParser):
    logger(logger), generator(generator), consoleParser(consoleParser)
{
    scopes.push_back(Scope(getScopeId()));
}
