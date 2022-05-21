#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>

class Generator
{
public:
    Generator() {}

    // Saving code to file
    std::string getCode();
    void saveToFile(std::string filePath);

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
