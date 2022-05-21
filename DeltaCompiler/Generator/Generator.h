#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>

class Generator
{
public:
    Generator() {}
    std::string getCode();

    // Generating code
    void addIdentifierDeclaration(std::string identifier, int scopeId, std::string value);
    void addArrayIdentifierDeclaration(std::string identifier, int scopeId, int size);

private:
    std::string getFullIdentifier(std::string identifier, int scopeId);
    void addToDataSection(std::string string);
    void addToTextSection(std::string string);

    std::string dataSection;
    std::string textSection;
};

#endif
