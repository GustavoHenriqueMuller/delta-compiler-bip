#include "Generator.h"

#include <fstream>

std::string Generator::getCode() {
    std::string result;

    result += ".data\n";
    result += dataSection;
    result += "\n";
    result += ".text\n";
    result += textSection;
    result += "\tHLT 0";

    return result;
}

void Generator::addImmediate(int immediate) {
    stackSize += 1;

    addInstruction("LDI", immediate);
    addInstruction("STO", stackTop());
}

void Generator::addIdentifier(const Symbol &symbol) {
    stackSize += 1;

    addInstruction("LD", getFullIdentifier(symbol));
    addInstruction("STO", stackTop());
}

void Generator::addArrayIdentifier(const Symbol &symbol) {
    addInstruction("LD", stackTop());
    addInstruction("STO", "$indr");
    stackSize -= 1;

    stackSize += 1;
    addInstruction("LDV", getFullIdentifier(symbol));
    addInstruction("STO", stackTop());
}

void Generator::addBinaryOperation(const Operation &operation) {
    switch (operation.getOperationCategory()) {
        case CATEGORY_ARIT_HIGH:
        case CATEGORY_ARIT_LOW:
        case CATEGORY_BIT: {
            std::string instructionName = getInstructionNameFromOperation(operation);

            addInstruction("LD", stackTop() - 1);
            addInstruction(instructionName, stackTop());
            stackSize -= 2;

            stackSize += 1;
            addInstruction("STO", stackTop());
            break;
        }

        case CATEGORY_RELATIONAL: {
            if (operation.type == OR || operation.type == AND) {
                std::string instructionName = getInstructionNameFromOperation(operation);
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

                stackSize -= 3;
                addInstruction("STO", stackTop());
            } else {
                addInstruction("LD", stackTop() - 1);
                addInstruction("SUB", stackTop());
                addInstruction("STO", stackTop());

                switch (operation.type) {
                    case GREATER: // $n == 0 && $z == 0
                        pushIsNegative(stackTop());
                        pushIsZero(stackTop() - 1);

                        addInstruction("LD", stackTop() - 1);
                        addInstruction("XORI", 1);
                        addInstruction("STO", stackTop() - 1);

                        addInstruction("LD", stackTop());
                        addInstruction("XORI", 1);
                        addInstruction("AND", stackTop() - 1);

                        stackSize -= 3;
                        addInstruction("STO", stackTop());
                        break;
                    case SMALLER: // $n == 1
                        stackSize -= 2;
                        pushIsNegative(stackTop() + 2);
                        break;
                    case GREATER_EQ: // $n == 0 || $z == 1
                        pushIsNegative(stackTop());
                        pushIsZero(stackTop() - 1);

                        addInstruction("LD", stackTop() - 1);
                        addInstruction("XORI", 1);
                        addInstruction("STO", stackTop() - 1);

                        addInstruction("LD", stackTop());
                        addInstruction("OR", stackTop() - 1);

                        stackSize -= 3;
                        addInstruction("STO", stackTop());
                        break;
                    case SMALLER_EQ: // $n == 1 || $z == 1
                        pushIsNegative(stackTop());
                        pushIsZero(stackTop() - 1);

                        addInstruction("LD", stackTop() - 1);
                        addInstruction("OR", stackTop());

                        stackSize -= 3;
                        addInstruction("STO", stackTop());
                        break;
                    case EQUAL: // $z == 1
                        stackSize -= 2;
                        pushIsZero(stackTop() + 2);
                        break;
                    case DIFFERENT: // $z == 0
                        stackSize -= 2;
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

void Generator::addUnaryOperation(const Operation &operation) {
    // TODO
}

void Generator::addLabel(const std::string &label) {
    textSection += label + ":\n";
}

void Generator::addJump(const std::string &label) {
    addInstruction("JMP", label);
}

void Generator::addBranchIfFalse(const std::string &label) {
    addInstruction("LD", stackTop());
    addInstruction("BEQ", label);

    stackSize -= 1;
}

void Generator::addBranchIfTrue(const std::string &label) {
    addInstruction("LD", stackTop());
    addInstruction("BNE", label);

    stackSize -= 1;
}

void Generator::assignTo(const Symbol &symbol, OperationType assignmentOperation) {
    if (assignmentOperation != ASSIGNMENT) {
        stackSize += 1;
        addInstruction("LD", stackTop() - 1);
        addInstruction("STO", stackTop());

        addInstruction("LD", getFullIdentifier(symbol));
        addInstruction("STO", stackTop() - 1);

        addBinaryOperation(getBinaryOperationFromAssignmentType(assignmentOperation));
    }

    addInstruction("LD", stackTop());
    addInstruction("STO", getFullIdentifier(symbol));
    stackSize -= 1;
}

void Generator::assignToArray(const Symbol &symbol, OperationType assignmentOperation) {
    if (assignmentOperation != ASSIGNMENT) {
        addInstruction("LD", stackTop());

        stackSize += 1;
        addInstruction("STO", stackTop());

        addInstruction("LD ", stackTop() - 2);
        addInstruction("STO", "$indr");
        addInstruction("LDV", getFullIdentifier(symbol));
        addInstruction("STO", stackTop() - 1);

        addBinaryOperation(getBinaryOperationFromAssignmentType(assignmentOperation));
    }

    addInstruction("LD", stackTop() - 1);
    addInstruction("STO", "$indr");

    addInstruction("LD", stackTop());
    addInstruction("STOV", getFullIdentifier(symbol));
    stackSize -= 2;
}

void Generator::addIdentifierDeclaration(const Symbol &symbol) {
    addToDataSection(getFullIdentifier(symbol) + ": 0");
}

void Generator::addArrayIdentifierDeclaration(const Symbol &symbol) {
    std::string value;

    for (int i = 0; i < symbol.type.arraySize; i++) {
        value += "0";

        if (i < symbol.type.arraySize - 1) {
            value += ", ";
        }
    }

    addToDataSection(getFullIdentifier(symbol) + ": " + value);
}

void Generator::addPrint() {
    addInstruction("LD", stackTop());
    addInstruction("STO", "$out_port");

    stackSize -= 1;
}

void Generator::addInput() {
    stackSize += 1;

    addInstruction("LD", "$in_port");
    addInstruction("STO", stackTop());
}

void Generator::pushIsNegative(int address) {
    addInstruction("LD", address);
    addInstruction("SRL", 10);
    addInstruction("ANDI", 1);

    stackSize += 1;
    addInstruction("STO", stackTop());
}

void Generator::pushIsZero(int address) {
    addInstruction("LD", address);
    addInstruction("SRL", 10);
    addInstruction("ANDI", 1);

    stackSize += 1;
    addInstruction("STO", stackTop());

    addInstruction("LD", address);
    addInstruction("NOT");
    addInstruction("ADDI", 1);
    addInstruction("SRL", 10);
    addInstruction("ANDI", 1);

    addInstruction("OR", stackTop());
    stackSize -= 1;

    addInstruction("XORI", 1);

    stackSize += 1;
    addInstruction("STO", stackTop());
}

std::string Generator::getInstructionNameFromOperation(const Operation &operation) {
    switch (operation.type) {
        case ADDITION:
            return "ADD";
        case SUBTRACTION:
            return "SUB";
        case OR:
        case BIT_OR:
            return "OR";
        case BIT_XOR:
            return "XOR";
        case AND:
        case BIT_AND:
            return "AND";
        case BIT_LS:
            return "SLL";
        case BIT_RS:
            return "SRL";
    }
}

std::string Generator::getFullIdentifier(const Symbol &symbol) {
    return symbol.name + "_" + std::to_string(symbol.scopeId);
}

void Generator::addToDataSection(std::string string) {
    dataSection += "\t" + string + "\n";
}

void Generator::addInstruction(std::string instruction, std::string parameter) {
    textSection += "\t" + instruction + " " + parameter + "\n";
}

void Generator::addInstruction(std::string instruction, int parameter) {
    textSection += "\t" + instruction + " " + std::to_string(parameter) + "\n";
}

void Generator::addInstruction(std::string instruction) {
    textSection += "\t" + instruction + " 0\n";
}

int Generator::stackTop() {
    return baseStackPointer + stackSize - 1;
}

