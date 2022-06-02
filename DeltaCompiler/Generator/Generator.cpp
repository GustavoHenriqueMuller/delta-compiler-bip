#include "Generator.h"

#include <fstream>

std::string Generator::getCode() {
    std::string result;

    result += ".data\n";
    result += dataSection;
    result += "\n";
    result += ".text\n";
    result += textSection;
    result += "\tHLT 0\n";

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

/*
 *
    OR,
    AND,
    GREATER,
    SMALLER,
    GREATER_EQ,
    SMALLER_EQ,
    EQUAL,
    DIFFERENT,
*/

void Generator::addBinaryOperation(Operation operation) {
    std::string instructionName;

    switch (operation.type) {
        case ADDITION:
            instructionName = "ADD";
            break;
        case SUBTRACTION:
            instructionName = "SUB";
            break;
        case BIT_OR:
        case OR: // TODO: Não pode fazer!
            instructionName = "OR";
            break;
        case BIT_XOR:
            instructionName = "XOR";
            break;
        case BIT_AND:
        case AND: // TODO: Não pode fazer!
            instructionName = "AND";
            break;
        case BIT_LS:
            instructionName = "SLL";
            break;
        case BIT_RS:
            instructionName = "SRL";
            break;
        case GREATER:
        case SMALLER:
        case GREATER_EQ:
        case SMALLER_EQ:
        case EQUAL:
        case DIFFERENT:
            // TODO: Operadores relacionais
            break;
    }

    addInstruction("LD", stackTop() - 1);
    addInstruction(instructionName, stackTop());
    stackSize -= 2;

    stackSize += 1;
    addInstruction("STO", stackTop());
}

void Generator::attributeTo(const Symbol &symbol, OperationType attributionType) {
    if (attributionType != ATTRIBUTION) {
        stackSize += 1;
        addInstruction("LD", stackTop() - 1);
        addInstruction("STO", stackTop());

        addInstruction("LD", getFullIdentifier(symbol));
        addInstruction("STO", stackTop() - 1);

        addBinaryOperation(getBinaryOperationFromAttributionType(attributionType));
    }

    addInstruction("LD", stackTop());
    addInstruction("STO", getFullIdentifier(symbol));
    stackSize -= 1;
}

void Generator::attributeToArray(const Symbol &symbol, OperationType attributionType) {
    if (attributionType != ATTRIBUTION) {
        addInstruction("LD", stackTop());

        stackSize += 1;
        addInstruction("STO", stackTop());

        addInstruction("LD ", stackTop() - 2);
        addInstruction("STO", "$indr");
        addInstruction("LDV", getFullIdentifier(symbol));
        addInstruction("STO", stackTop() - 1);

        addBinaryOperation(getBinaryOperationFromAttributionType(attributionType));
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

int Generator::stackTop() {
    return baseStackPointer + stackSize - 1;
}

