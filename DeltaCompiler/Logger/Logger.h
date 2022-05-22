#ifndef LOGGER_H
#define LOGGER_H

#include "Logger.h"
#include "Warnings.h"
#include "../Gals/AnalysisError.h"

#include <vector>
#include <string>
#include <iostream>

class Logger {
public:
    Logger() {}
    void addWarn(const Warning &warning);
    void logWarns();
    void logError(const AnalysisError &logError);

private:
    std::vector<Warning> warnings;
};

#endif // LOGGER_H
