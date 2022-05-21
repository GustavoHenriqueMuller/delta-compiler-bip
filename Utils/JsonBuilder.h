#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class JsonBuilder {

private:
    stringstream payload;

public:
    JsonBuilder() { }

    string build() {
        this->payload << "]";
        string temp = this->replace(this->payload.str(), ",}", "}");
        return "[" + this->replace(temp, ",]", "]");
    }

    void open() {
        this->payload << "{";
    }

    void close() {
        this->payload << "},";
    }

    template<class T>
    void set(string key, T value) {
        this->payload << this->attribute(key) << this->quotes(value) << ",";
    }

private:
    template<class T>
    string quotes(T key) {
        return "\"" + to_string(key) + "\"";
    }

    string quotes(bool key) {
        return "\"" + to_string(int(key)) + "\"";
    }

    string quotes(string key) {
        return "\"" + key + "\"";
    }

    string quotes(const char *key) {
        return "\"" + string(key) + "\"";
    }

    string quotes(char key) {
        return "\"" + string(1, key) + "\"";
    }

    string attribute(string key) {
        return this->quotes(key) + ":";
    }

    string replace(string text, const string& target, const string& newString) {
        size_t startPos = 0;
        while((startPos = text.find(target, startPos)) != string::npos) {
            text.replace(startPos, target.length(), newString);
            startPos += newString.length();
        }
        return text;
    }

};

#endif // JSON_BUILDER_H
