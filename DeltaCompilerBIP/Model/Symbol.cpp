#include "Symbol.h"

#include <algorithm>

std::string Symbol::getMangledName() const {
    if (isFunction) {
        std::string parameterNames;

        for (int i = 0; i < parameters.size(); i++) {
            std::string typeString = parameters[i].type.toString();
            std::replace(typeString.begin(), typeString.end(), ' ', '_');

            parameterNames += typeString;

            if (i < parameters.size() - 1) {
                parameterNames += "_";
            }
        }

        return type.toString() + "_" + name + (parameters.size() > 0 ? "_" : "") + parameterNames;
    } else {
        return name + "_" + std::to_string(scopeId);
    }
}
