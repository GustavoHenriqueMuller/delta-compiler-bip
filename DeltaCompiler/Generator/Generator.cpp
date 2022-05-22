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

    addToTextSection("LD" + getFullIdentifier(identifier, scopeId));
    addToTextSection("STO " + std::to_string(stackTop()));
}

void Generator::addPlus() {
    addBinaryOperation("ADD");
}

void Generator::addMinus() {
    addBinaryOperation("SUB");
}

void Generator::attributeTo(std::string identifier, int scopeId) {
    stackSize -= 1;

    addToTextSection("LD " + std::to_string(stackTop() + 1));
    addToTextSection("STO " + getFullIdentifier(identifier, scopeId));
}

void Generator::addIdentifierDeclaration(std::string identifier, int scopeId, std::string value = "0") {
    addToDataSection(getFullIdentifier(identifier, scopeId) + ": " + value);
}

void Generator::addArrayIdentifierDeclaration(std::string identifier, int scopeId, int size) {
    stackSize -= 1;

    std::string value;

    for (int i = 0; i < size; i++) {
        value += "0";

        if (i < size - 1) {
            value += ", ";
        }
    }

    addToDataSection(getFullIdentifier(identifier, scopeId) + ": " + value);
}

std::string Generator::getFullIdentifier(std::string identifier, int scopeId) {
    return identifier + "_" + std::to_string(scopeId);
}

void Generator::addBinaryOperation(std::string instructionName) {
    stackSize -= 2;

    addToTextSection("LD " + std::to_string(stackTop() + 1));
    addToTextSection(instructionName + " " + std::to_string(stackTop() + 2));

    stackSize += 1;
    addToTextSection("STO " + std::to_string(stackTop()));
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

