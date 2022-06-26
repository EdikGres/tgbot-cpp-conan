//
// Created by root on 26.06.22.
//
//TODO: переделать парсер на регулярки

#include "StringParser.h"

#include <iostream>

StringParser::StringParser(string &filename) : filename(filename) {
    tmp = new unordered_map<string, string>;
    in.open(filename);

    if (in.is_open())
        cout << "Success open " << filename << endl;


}

StringParser::~StringParser() {
    delete (tmp);
    in.close();
}

int StringParser::changeFile(string &file) {
    if (in.is_open())
        in.close();
    filename = file;
    in.open(filename);
    if (in.is_open()) {
        cout << "Success open " << filename << endl;
        return 0;
    }
    return 1;
}

bool isMySymbols(const char a) {
    return a == ' ' || a == '-' || a == '#' || a == ',' || a == '(' || a == ')';
}

unordered_map<string, string> *StringParser::parseFile() {
    if (!in.is_open())
        return nullptr;
    string s;
    for (int i = 0; !in.eof(); ++i) {
        getline(in, s);
        if (s.rfind('\"', 0) == 0) {
            //cout << s << endl;
            string key, value;

            for (int j = 1; j < s.length() && (isMySymbols(s[j]) | isalpha(s[j]) | isdigit(s[j])); ++j) {
                key.insert(key.end(), s[j]);
            }
            cout << "key:" << key << endl;

            for (unsigned long j = s.find(": \"", 0)+3; j < s.length()-2; ++j) {
                value.insert(value.end(), s[j]);
            }
            cout << "value:" << value << endl;
        }
    }


    return nullptr;
}


