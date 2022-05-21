#include <fstream>
#include "Generator.h"

std::string Generator::getCode() {
    std::string result;

    result += ".data\n";
    result += this->dataSection;
    result += "\n";
    result += ".text\n";
    result += this->textSection;

    return result;
}

void Generator::addIdentifierDeclaration(std::string identifier, int scopeId, std::string value = "0") {
    this->addToDataSection(identifier + ": " + value);
}

void Generator::addArrayIdentifierDeclaration(std::string identifier, int scopeId, int size) {
    std::string value;

    for (int i = 0; i < size; i++) {
        value += "0";

        if (i < size - 1) {
            value += ", ";
        }
    }

    this->addToDataSection(identifier + ": " + value);
}

std::string Generator::getFullIdentifier(std::string identifier, int scopeId) {
    return identifier + "_" + std::to_string(scopeId);
}

void Generator::addToDataSection(std::string string) {
    this->dataSection += "\t" + string + "\n";
}

void Generator::addToTextSection(std::string string) {
    this->textSection += "\t" + string + "\n";
}

