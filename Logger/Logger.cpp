#include "Logger.h"

void Logger::addWarn(const Warning &warning) {
    warnings.push_back(warning);
}

void Logger::logWarns() {
    for (int i = 0; i < warnings.size(); i++) {
        std::cout << "[WARNING]: " << warnings[i].getMessage() << std::endl;
    }
}

void Logger::error(const AnalysisError &error) {
    std::cout << "[ERROR]: " << error.getMessage() << std::endl;
}
