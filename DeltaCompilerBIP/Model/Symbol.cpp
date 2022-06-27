#include "Symbol.h"

std::string Symbol::getMangledName() const {
    if (isFunction) {
        std::string parameterNames;

        for (int i = 0; i < parameters.size(); i++) {
            parameterNames += parameters[i].type.toString();

            if (i < parameters.size() - 1) {
                parameterNames += "_";
            }
        }

        return type.toString() + "_" + name + (parameters.size() > 0 ? "_" : "") + parameterNames;
    } else {
        return name + "_" + std::to_string(scopeId);
    }
}
