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

    addToTextSection("LDI " + std::to_string(immediate));
    addToTextSection("STO " + std::to_string(stackTop()));
}

void Generator::addIdentifier(std::string identifier, int scopeId) {
    stackSize += 1;

    addToTextSection("LD " + getFullIdentifier(identifier, scopeId));
    addToTextSection("STO " + std::to_string(stackTop()));
}

void Generator::addArrayIdentifier(std::string identifier, int scopeId) {
    addToTextSection("LD " + std::to_string(stackTop()));
    addToTextSection("STO $indr");
    stackSize -= 1;

    stackSize += 1;
    addToTextSection("LDV " + getFullIdentifier(identifier, scopeId));
    addToTextSection("STO " + std::to_string(stackTop()));
}

void Generator::addBinaryOperation(OperationType operationType) {
    std::string instructionName;

    switch (operationType) {
        case ADD:
            instructionName = "ADD";
            break;
        case SUBTRACT:
            instructionName = "SUB";
            break;
        case BIT_OR:
            instructionName = "OR";
            break;
        case BIT_XOR:
            instructionName = "XOR";
            break;
        case BIT_AND:
            instructionName = "AND";
            break;
        case BIT_LS:
            instructionName = "SLL";
            break;
        case BIT_RS:
            instructionName = "SRL";
            break;
    }

    addToTextSection("LD " + std::to_string(stackTop() - 1));
    addToTextSection(instructionName + " " + std::to_string(stackTop()));
    stackSize -= 2;

    stackSize += 1;
    addToTextSection("STO " + std::to_string(stackTop()));
}

void Generator::attributeTo(std::string identifier, int scopeId, OperationType attributionType) {
    if (attributionType != ATTRIBUTION) {
        stackSize += 1;
        addToTextSection("LD " + std::to_string(stackTop() - 1));
        addToTextSection("STO " + std::to_string(stackTop()));

        addToTextSection("LD " + getFullIdentifier(identifier, scopeId));
        addToTextSection("STO " + std::to_string(stackTop() - 1));

        switch (attributionType) {
            case INCREMENT_ATTRIBUTION: {
                addBinaryOperation(ADD);
                break;
            }
            case DECREMENT_ATTRIBUTION: {
                addBinaryOperation(SUBTRACT);
                break;
            }
        }
    }

    addToTextSection("LD " + std::to_string(stackTop()));
    addToTextSection("STO " + getFullIdentifier(identifier, scopeId));
    stackSize -= 1;
}

void Generator::attributeToArray(std::string identifier, int scopeId, OperationType attributionType) {
    if (attributionType != ATTRIBUTION) {
        addToTextSection("LD " + std::to_string(stackTop()));

        stackSize += 1;
        addToTextSection("STO " + std::to_string(stackTop()));

        addToTextSection("LD " + std::to_string(stackTop() - 2));
        addToTextSection("STO $indr");
        addToTextSection("LDV " + getFullIdentifier(identifier, scopeId));
        addToTextSection("STO " + std::to_string(stackTop() - 1));

        switch (attributionType) {
            case INCREMENT_ATTRIBUTION: {
                addBinaryOperation(ADD);
                break;
            }
            case DECREMENT_ATTRIBUTION: {
                addBinaryOperation(SUBTRACT);
                break;
            }
        }
    }

    addToTextSection("LD " + std::to_string(stackTop() - 1));
    addToTextSection("STO $indr");

    addToTextSection("LD " + std::to_string(stackTop()));
    addToTextSection("STOV " + getFullIdentifier(identifier, scopeId));
    stackSize -= 2;
}

void Generator::addIdentifierDeclaration(std::string identifier, int scopeId) {
    addToDataSection(getFullIdentifier(identifier, scopeId) + ": 0");
}

void Generator::addArrayIdentifierDeclaration(std::string identifier, int scopeId, int size) {
    std::string value;

    for (int i = 0; i < size; i++) {
        value += "0";

        if (i < size - 1) {
            value += ", ";
        }
    }

    addToDataSection(getFullIdentifier(identifier, scopeId) + ": " + value);
}

void Generator::addPrint() {
    addToTextSection("LD " + std::to_string(stackTop()));
    addToTextSection("STO $out_port");

    stackSize -= 1;
}

void Generator::addInput(std::string identifier, int scopeId) {
    addToTextSection("LD $in_port");
    addToTextSection("STO " + getFullIdentifier(identifier, scopeId));

    stackSize -= 1;
}

void Generator::addArrayInput(std::string identifier, int scopeId) {
    addToTextSection("LD $in_port");
    addToTextSection("STOV " + getFullIdentifier(identifier, scopeId));

    stackSize -= 1;
}

std::string Generator::getFullIdentifier(std::string identifier, int scopeId) {
    return identifier + "_" + std::to_string(scopeId);
}

void Generator::addToDataSection(std::string string) {
    dataSection += "\t" + string + "\n";
}

void Generator::addToTextSection(std::string string) {
    textSection += "\t" + string + "\n";
}

int Generator::stackTop() {
    return baseStackPointer + stackSize - 1;
}

