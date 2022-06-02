#ifndef TYPE_H
#define TYPE_H

#include <string>

enum Primitive {
    INT,
    FLOAT,
    DOUBLE,
    STRING,
    CHAR,
    BOOLEAN,
    VOID,
    ERR
};

class Type {
public:
    Primitive primitive;
    bool isArray = false;
    bool isConst = false;

    int arraySize = 0;

    Type() {}
    Type(Primitive primitive): primitive (primitive) {}
    Type(Primitive primitive, bool isArray, bool isConst): primitive(primitive), isArray(isArray), isConst(isConst) {}

    std::string toString() const {
        std::string result;

        if (isConst) {
            result += "const ";
        }

        switch (primitive) {
            case INT:
                result += "int";
                break;
            case FLOAT:
                result += "float";
                break;
            case STRING:
                result += "string";
                break;
            case CHAR:
                result += "char";
                break;
            case BOOLEAN:
                result += "boolean";
                break;
            case VOID:
                result += "void";
                break;
        }

        if (isArray) {
            result += "[" + std::to_string(arraySize) + "]";
        }

        return result;
    }
};

#endif
