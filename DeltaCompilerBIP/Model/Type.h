#ifndef TYPE_H
#define TYPE_H

#include <string>

enum Primitive {
    PRIMITIVE_INT,
    PRIMITIVE_CHAR,
    PRIMITIVE_BOOLEAN,
    PRIMITIVE_VOID,
    PRIMITIVE_ERR
};

struct Type {
    Type() {}
    Type(Primitive primitive): primitive (primitive) {}
    Type(Primitive primitive, bool isArray, bool isConst): primitive(primitive), isArray(isArray), isConst(isConst) {}

    std::string toString() const {
        std::string result;

        if (isConst) {
            result += "const ";
        }

        switch (primitive) {
            case PRIMITIVE_INT:
                result += "int";
                break;
            case PRIMITIVE_CHAR:
                result += "char";
                break;
            case PRIMITIVE_BOOLEAN:
                result += "boolean";
                break;
            case PRIMITIVE_VOID:
                result += "void";
                break;
        }

        if (isArray) {
            result += "[" + std::to_string(arraySize) + "]";
        }

        return result;
    }

    Primitive primitive;
    bool isArray = false;
    bool isConst = false;

    int arraySize = 0;
};

#endif
