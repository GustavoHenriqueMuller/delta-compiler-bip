#ifndef GENERATOR_H
#define GENERATOR_H

#include "../Model/Operation.h"

#include <string>

class Generator {
public:
    Generator() {}
    std::string getCode();

    void addImmediate(int immediate);
    void addIdentifier(std::string identifier, int scopeId);
    void addPlus();
    void addMinus();

    void attributeTo(std::string identifier, int scopeId);
    void addIdentifierDeclaration(std::string identifier, int scopeId, std::string value);
    void addArrayIdentifierDeclaration(std::string identifier, int scopeId, int size);

private:
    std::string getFullIdentifier(std::string identifier, int scopeId);
    void addBinaryOperation(std::string instructionName);
    void addToDataSection(std::string string);
    void addToTextSection(std::string string);
    int stackTop();

    std::string dataSection;
    std::string textSection;

    // Stack for temporary data
    const int baseStackPointer = 1000;
    int stackSize = 0;
};

#endif
