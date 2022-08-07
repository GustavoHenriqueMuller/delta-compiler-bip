#include "Generator.h"
#include "../Utils/Utils.h"

#include <fstream>

std::string Generator::getCode() {
    std::string result;

    result += ".data\n";
    result += dataSection;
    result += "\n";
    result += ".text\n";
    result += "\tCALL void_main\n";
    result += "\tHLT 0\n\n";
    result += textSection;

    return result;
}

void Generator::addImmediate(const int& immediate) {
    pushStack();
    addInstruction("LDI", immediate);
    addInstruction("STO", stackTop());
}

void Generator::addIdentifier(const Symbol& symbol) {
    pushStack();
    addInstruction("LD", symbol.getMangledName());
    addInstruction("STO", stackTop());
}

void Generator::addArrayIdentifier(const Symbol& symbol) {
    addInstruction("LD", stackTop());
    addInstruction("STO", "$indr");
    addInstruction("LDV", symbol.getMangledName());
    addInstruction("STO", stackTop());
}

void Generator::duplicateStackTop() {
    addInstruction("LD", stackTop());
    pushStack();
    addInstruction("STO", stackTop());
}

void Generator::pushStack(int amount) {
    stackSize += amount;
}

void Generator::popStack(int amount) {
    stackSize -= amount;
}

void Generator::addBinaryOperation(const OperationType& operationType) {
    switch (getOperationCategory(operationType)) {
        case CATEGORY_ARIT:
        case CATEGORY_BIT: {
            std::string instructionName = getInstructionNameFromOperation(operationType);

            addInstruction("LD", stackTop() - 1);
            addInstruction(instructionName, stackTop());

            popStack(2);
            pushStack();

            addInstruction("STO", stackTop());
            break;
        }

        case CATEGORY_RELATIONAL: {
            if (operationType == OP_OR || operationType == OP_AND) {
                std::string instructionName = getInstructionNameFromOperation(operationType);
                pushIsZero(stackTop() - 1);
                pushIsZero(stackTop() - 1);

                addInstruction("LD", stackTop() - 1);
                addInstruction("XORI", 1);
                addInstruction("STO", stackTop() - 1);

                addInstruction("LD", stackTop());
                addInstruction("XORI", 1);
                addInstruction("STO", stackTop());

                addInstruction("LD", stackTop() - 1);
                addInstruction(instructionName, stackTop());

                popStack(3);
                addInstruction("STO", stackTop());
            } else {
                addInstruction("LD", stackTop() - 1);
                addInstruction("SUB", stackTop());
                addInstruction("STO", stackTop());

                switch (operationType) {
                    case OP_GREATER: // $n == 0 && $z == 0
                        pushIsNegative(stackTop());
                        pushIsZero(stackTop() - 1);

                        addInstruction("LD", stackTop() - 1);
                        addInstruction("XORI", 1);
                        addInstruction("STO", stackTop() - 1);

                        addInstruction("LD", stackTop());
                        addInstruction("XORI", 1);
                        addInstruction("AND", stackTop() - 1);

                        popStack(3);
                        addInstruction("STO", stackTop());
                        break;
                    case OP_SMALLER: // $n == 1
                        popStack(2);
                        pushIsNegative(stackTop() + 2);
                        break;
                    case OP_GREATER_EQ: // $n == 0 || $z == 1
                        pushIsNegative(stackTop());
                        pushIsZero(stackTop() - 1);

                        addInstruction("LD", stackTop() - 1);
                        addInstruction("XORI", 1);
                        addInstruction("STO", stackTop() - 1);

                        addInstruction("LD", stackTop());
                        addInstruction("OR", stackTop() - 1);

                        popStack(3);
                        addInstruction("STO", stackTop());
                        break;
                    case OP_SMALLER_EQ: // $n == 1 || $z == 1
                        pushIsNegative(stackTop());
                        pushIsZero(stackTop() - 1);

                        addInstruction("LD", stackTop() - 1);
                        addInstruction("OR", stackTop());

                        popStack(3);
                        addInstruction("STO", stackTop());
                        break;
                    case OP_EQUAL: // $z == 1
                        popStack(2);
                        pushIsZero(stackTop() + 2);
                        break;
                    case OP_DIFFERENT: // $z == 0
                        popStack(2);
                        pushIsZero(stackTop() + 2);

                        addInstruction("LD", stackTop());
                        addInstruction("XORI", 1);
                        addInstruction("STO", stackTop());
                        break;
                    }
            }
            break;
        }
    }
}

void Generator::addUnaryOperation(const OperationType& operationType) {
    switch (operationType) {
        case OP_MINUS_INVERSION:
            addInstruction("LD", stackTop());
            addInstruction("NOT");
            addInstruction("ADDI", 1);
            addInstruction("STO", stackTop());
            break;
        case OP_BIT_NOT:
            addInstruction("LD", stackTop());
            addInstruction("NOT");
            addInstruction("STO", stackTop());
            break;
        case OP_NOT:
            pushIsZero(stackTop());
            popStack();
            addInstruction("STO", stackTop());
            break;
    }
}

void Generator::addMutableUnaryOperation(const OperationType& operationType, const Symbol& symbol) {
    addInstruction("LD", symbol.getMangledName());

    switch (operationType) {
        case OP_INCREMENT_RIGHT:
            pushStack();
            addInstruction("STO", stackTop());
            addInstruction("ADDI", 1);
            addInstruction("STO", symbol.getMangledName());
            break;
        case OP_DECREMENT_RIGHT:
            pushStack();
            addInstruction("STO", stackTop());
            addInstruction("SUBI", 1);
            addInstruction("STO", symbol.getMangledName());
            break;
        case OP_INCREMENT_LEFT:
            addInstruction("ADDI", 1);

            pushStack();
            addInstruction("STO", stackTop());
            addInstruction("STO", symbol.getMangledName());
            break;
        case OP_DECREMENT_LEFT:
            addInstruction("SUBI", 1);

            pushStack();
            addInstruction("STO", stackTop());
            addInstruction("STO", symbol.getMangledName());
            break;
    }
}

void Generator::addMutableUnaryOperationOnArray(const OperationType& operationType, const Symbol& symbol) {
    addInstruction("LD", stackTop());
    addInstruction("STO", "$indr");

    switch (operationType) {
        case OP_INCREMENT_RIGHT:
            addInstruction("LDV", symbol.getMangledName());
            addInstruction("STO", stackTop());
            addInstruction("ADDI", 1);
            addInstruction("STOV", symbol.getMangledName());
            break;
        case OP_DECREMENT_RIGHT:
            addInstruction("LDV", symbol.getMangledName());
            addInstruction("STO", stackTop());
            addInstruction("SUBI", 1);
            addInstruction("STOV", symbol.getMangledName());
            break;
        case OP_INCREMENT_LEFT:
            addInstruction("LDV", symbol.getMangledName());
            addInstruction("ADDI", 1);
            addInstruction("STO", stackTop());
            addInstruction("STOV", symbol.getMangledName());
            break;
        case OP_DECREMENT_LEFT:
            addInstruction("LDV", symbol.getMangledName());
            addInstruction("SUBI", 1);
            addInstruction("STO", stackTop());
            addInstruction("STOV", symbol.getMangledName());
            break;
    }
}

void Generator::addLabel(const std::string& label) {
    textSection += label + ":\n";
}

void Generator::addJump(const std::string& label) {
    addInstruction("JMP", label);
}

void Generator::addBranchIfFalse(const std::string& label) {
    addInstruction("LD", stackTop());
    addInstruction("BEQ", label);

    popStack();
}

void Generator::addBranchIfTrue(const std::string& label) {
    addInstruction("LD", stackTop());
    addInstruction("BNE", label);

    popStack();
}

void Generator::assignTo(const Symbol& symbol, const OperationType& assignmentOperation) {
    if (assignmentOperation != OP_ASSIGNMENT) {
        pushStack();
        addInstruction("LD", stackTop() - 1);
        addInstruction("STO", stackTop());

        addInstruction("LD", symbol.getMangledName());
        addInstruction("STO", stackTop() - 1);

        addBinaryOperation(getBinaryOperationFromAssignmentType(assignmentOperation).type);
    }

    addInstruction("LD", stackTop());
    addInstruction("STO", symbol.getMangledName());
    popStack();
}

void Generator::assignToArray(const Symbol& symbol, const OperationType& assignmentOperation) {
    if (assignmentOperation != OP_ASSIGNMENT) {
        addInstruction("LD", stackTop());

        pushStack();
        addInstruction("STO", stackTop());

        addInstruction("LD ", stackTop() - 2);
        addInstruction("STO", "$indr");
        addInstruction("LDV", symbol.getMangledName());
        addInstruction("STO", stackTop() - 1);

        addBinaryOperation(getBinaryOperationFromAssignmentType(assignmentOperation).type);
    }

    addInstruction("LD", stackTop() - 1);
    addInstruction("STO", "$indr");

    addInstruction("LD", stackTop());
    addInstruction("STOV", symbol.getMangledName());
    popStack(2);
}

void Generator::addIdentifierDeclaration(const Symbol& symbol) {
    addToDataSection(symbol.getMangledName() + ": 0");
}

void Generator::addArrayIdentifierDeclaration(const Symbol& symbol) {
    std::string value;

    for (int i = 0; i < symbol.type.arraySize; i++) {
        value += "0";

        if (i < symbol.type.arraySize - 1) {
            value += ", ";
        }
    }

    addToDataSection(symbol.getMangledName() + ": " + value);
}

void Generator::addPrint() {
    addInstruction("LD", stackTop());
    addInstruction("STO", "$out_port");

    popStack();
}

void Generator::addInput() {
    pushStack();

    addInstruction("LD", "$in_port");
    addInstruction("STO", stackTop());
}

void Generator::addCall(const Symbol& function) {
    addInstruction("CALL", function.getMangledName());
}

void Generator::addReturn() {
    addInstruction("RETURN", 0);
}

void Generator::pushIsNegative(const int& address) {
    addInstruction("LD", address);
    addInstruction("SRL", 10);
    addInstruction("ANDI", 1);

    pushStack();
    addInstruction("STO", stackTop());
}

void Generator::pushIsZero(const int& address) {
    addInstruction("LD", address);
    addInstruction("SRL", 10);
    addInstruction("ANDI", 1);

    pushStack();
    addInstruction("STO", stackTop());

    addInstruction("LD", address);
    addInstruction("NOT");
    addInstruction("ADDI", 1);
    addInstruction("SRL", 10);
    addInstruction("ANDI", 1);

    addInstruction("OR", stackTop());
    popStack();

    addInstruction("XORI", 1);

    pushStack();
    addInstruction("STO", stackTop());
}

std::string Generator::getInstructionNameFromOperation(const OperationType& operationType) {
    switch (operationType) {
        case OP_ADDITION:
            return "ADD";
        case OP_SUBTRACTION:
            return "SUB";
        case OP_OR:
        case OP_BIT_OR:
            return "OR";
        case OP_BIT_XOR:
            return "XOR";
        case OP_AND:
        case OP_BIT_AND:
            return "AND";
        case OP_BIT_LS:
            return "SLL";
        case OP_BIT_RS:
            return "SRL";
    }
}

void Generator::addToDataSection(const std::string& string) {
    dataSection += "\t" + string + "\n";
}

void Generator::addInstruction(const std::string& instruction, const std::string& parameter) {
    textSection += "\t" + instruction + " " + parameter + "\n";
}

void Generator::addInstruction(const std::string& instruction, const int& parameter) {
    textSection += "\t" + instruction + " " + std::to_string(parameter) + "\n";
}

void Generator::addInstruction(const std::string& instruction) {
    textSection += "\t" + instruction + " 0\n";
}

int Generator::stackTop() {
    return baseStackPointer + stackSize - 1;
}

