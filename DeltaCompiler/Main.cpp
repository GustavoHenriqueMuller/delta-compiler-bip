#include "Gals/AnalysisError.h"
#include "Gals/Constants.h"
#include "Gals/Lexico.h"
#include "Gals/Semantico.h"
#include "Gals/Sintatico.h"
#include "Utils/FileManager.h"
#include "Logger/Logger.h"
#include "Generator/Generator.h"

#include <iostream>

void printBar() {
    std::cout << std::string(40, '_') << std::endl << std::endl;
}

int main(int argc, char **argv) {
    bool watch = false;

    if (argc == 1) {
        std::cout << "Syntax: delta [options] [filepath]." << std::endl << std::endl;
        std::cout << "[options]:" << std::endl;
        std::cout << "-watch: Necessary when compiling from IDE;" << std::endl;
        std::cout << std::endl << "[filepath]: Relative or absolute path of .delta file to be compiled." << std::endl << std::endl;
        return 0;
    }

    for (int i = 1; i < argc - 1; i++) {
        std::string argument(argv[i]);

        if (argument == "-watch") {
            watch = true;
        } else {
            std::cout << "Error: Invalid argument '" + argument + "'." << std::endl;
            return 0;
        }
    }

    std::string filePath = argv[argc - 1];
    std::string extension = filePath.substr(filePath.find_last_of('.') + 1, filePath.size());

    if (extension != "delta") {
        std::cout << "Error: File extension must be 'delta', got '" + extension + "'." << std::endl;
        return 0;
    }

    std::string sourceCode = FileManager::getFileContent(filePath);
    std::istringstream stream = std::istringstream(sourceCode);

    Logger logger;
    Generator generator;
	
	try {
        Lexico* lexico = new Lexico(stream);
        Sintatico* sintatico = new Sintatico();
        Semantico* semantico = new Semantico(logger, generator);

        sintatico->parse(lexico, semantico);
        semantico->popScope();

        printBar();
        std::cout << sourceCode << std::endl;
        printBar();
        std::cout << generator.getCode() << std::endl;
        printBar();

        std::string asmFilePath = FileManager::getAsmFilePath(filePath);

        if (watch) {
            std::cout << "[SCOPES]: " << semantico->getScopesJson() << std::endl;
            std::cout << "[ASM_PATH]: " << asmFilePath << std::endl;
            std::cout << std::endl;
        }

        logger.logWarns();
        FileManager::saveToFile(generator.getCode(), asmFilePath);

        std::cout << std::endl;
        std::cout << "File '" << FileManager::getSimplifiedName(filePath) << "' compiled successfully." << std::endl;
        std::cout << "File '" << FileManager::getSimplifiedName(asmFilePath) << "' created." << std::endl << std::endl;
    } catch (AnalysisError e) {
        std::cout << std::endl;
        logger.logWarns();

        std::cout << std::endl;
        logger.error(e);
    }

    return 0;
}
