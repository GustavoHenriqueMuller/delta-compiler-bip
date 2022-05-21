#ifndef TYPE_H
#define TYPE_H

#include <string>

enum Primitive {
    INT,
    FLO,
    DOU,
    STR,
    CHA,
    BOO,
    VOI,
    ERR
};

class Type {
public:
    Primitive primitive;
    bool isArray = false;
    bool isConst = false;

    Type() {}
    Type(Primitive primitive): primitive (primitive) {}
    Type(Primitive primitive, bool isArray, bool isConst, bool isRef): primitive(primitive), isArray(isArray), isConst(isConst), isRef(isRef) {}

    std::string toString() {
        std::string result;

        if (isConst) {
            result += "const ";
        }

        if (isRef) {
            result += "ref ";
        }

        switch (primitive) {
            case INT:
                result += "int";
                break;
            case FLO:
                result += "float";
                break;
            case STR:
                result += "string";
                break;
            case CHA:
                result += "char";
                break;
            case BOO:
                result += "boolean";
                break;
            case VOI:
                result += "void";
                break;
        }

        if (isArray) {
            result += "[]";
        }

        return result;
    }
};

#endif
