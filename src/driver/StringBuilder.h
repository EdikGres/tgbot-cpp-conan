//
// Created by root on 26.06.22.
//

#ifndef KOSTYA_BOT_STRINGBUILDER_H
#define KOSTYA_BOT_STRINGBUILDER_H
#include "unordered_map"
#include "StringParser.h"

using namespace std;

class StringBuilder {
public:
    StringBuilder(string files[]);

    virtual ~StringBuilder();

    unordered_map<string, string> *getRu() const;

    unordered_map<string, string> *getEn() const;

private:
    unordered_map<string,string>* ru;
    unordered_map<string,string>* en;
};


#endif //KOSTYA_BOT_STRINGBUILDER_H
