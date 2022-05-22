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
    void addArrayIdentifier(std::string identifier, int scopeId);
    void addBinaryOperation(OperationType operationType);

    void attributeTo(std::string identifier, int scopeId, OperationType attributionType);
    void attributeToArray(std::string identifier, int scopeId, OperationType attributionType);
    void addIdentifierDeclaration(std::string identifier, int scopeId);
    void addArrayIdentifierDeclaration(std::string identifier, int scopeId, int size);
    void addPrint();
    void addInput(std::string identifier, int scopeId);
    void addArrayInput(std::string identifier, int scopeId);

private:
    std::string getFullIdentifier(std::string identifier, int scopeId);
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
