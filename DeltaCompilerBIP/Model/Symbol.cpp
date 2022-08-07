#include "Symbol.h"

#include <algorithm>

std::string Symbol::getMangledName() const {
    return name + "_" + std::to_string(scopeId);
}
