#include "Gals/AnalysisError.h"
#include "Gals/Constants.h"
#include "Gals/Lexico.h"
#include "Gals/Semantico.h"
#include "Gals/Sintatico.h"
#include "Utils/FileManager.h"
#include "Logger/Logger.h"

#include <iostream>

// TODO: Corrigir vagabundagem do declarationType usado no assignment
// TODO: Corrigir coisas da gramática (++ em retorno ref de função deveria funcionar mas não está)

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cout << "Syntax: cp2 [filepath]." << std::endl;
        return 0;
    }

    std::string filepath = argv[1];
    std::string sourceCode = FileManager::getFileContent(filepath);
    std::istringstream stream = std::istringstream(sourceCode);

    std::cout << "'" << filepath << "'" << std::endl << std::endl;
    std::cout << sourceCode << std::endl << std::endl;

    Logger logger;
	
	try {
        Lexico* lexico = new Lexico(stream);
        Sintatico* sintatico = new Sintatico();
        Semantico* semantico = new Semantico(logger);

        sintatico->parse(lexico, semantico);
        semantico->popScope();

        std::cout << std::endl;
		std::cout << "[SCOPES] " << semantico->getScopesJson() << std::endl;
        logger.logWarns();
        std::cout << std::endl;

        std::cout << "File '" << filepath << "' compiled successfully." << std::endl << std::endl;
    } catch (AnalysisError e) {
        std::cout << std::endl;
        logger.logWarns();
        std::cout << std::endl;

        logger.error(e);
    }

    return 0;
}
