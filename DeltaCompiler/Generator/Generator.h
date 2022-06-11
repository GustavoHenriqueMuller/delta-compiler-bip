#ifndef GENERATOR_H
#define GENERATOR_H

#include "../Model/Operation.h"
#include "../Model/Symbol.h"

#include <string>

class Generator {
public:
    Generator() {}
    std::string getCode();

    void addImmediate(int immediate);
    void addIdentifier(const Symbol &symbol);
    void addArrayIdentifier(const Symbol &symbol);
    void popStack();

    void addBinaryOperation(const Operation &operation);
    void addUnaryOperation(const Operation &operation);

    void addLabel(const std::string &label);
    void addJump(const std::string &label);
    void addBranchIfFalse(const std::string &label);
    void addBranchIfTrue(const std::string &label);

    void assignTo(const Symbol &symbol, OperationType assignmentType);
    void assignToArray(const Symbol &symbol, OperationType assignmentType);
    void addIdentifierDeclaration(const Symbol &symbol);
    void addArrayIdentifierDeclaration(const Symbol &symbol);
    void addPrint();
    void addInput();

private:
    void pushIsNegative(int address);
    void pushIsZero(int address);
    std::string getInstructionNameFromOperation(const Operation &operation);
    std::string getFullIdentifier(const Symbol &symbol);
    void addToDataSection(std::string string);
    void addInstruction(std::string instruction, std::string parameter);
    void addInstruction(std::string instruction, int parameter);
    void addInstruction(std::string instruction);
    int stackTop();

    std::string dataSection;
    std::string textSection;

    // Stack for temporary data
    const int baseStackPointer = 1000;
    int stackSize = 0;
};

#endif
