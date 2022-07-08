//
// Created by root on 7/8/22.
//

#ifndef KOSTYA_BOT_STRINGBUILDERDB_H
#define KOSTYA_BOT_STRINGBUILDERDB_H

#include "string"
#include "unordered_map"
#include "DBHandler.h"

using namespace std;

class StringBuilderDB {
public:
    StringBuilderDB(string files[], DBHandler &db);

    virtual ~StringBuilderDB();

    unordered_map<string, string> *getRu() const;

    unordered_map<string, string> *getEn() const;

private:
    unordered_map<string,string>* ru;
    unordered_map<string,string>* en;

};


#endif //KOSTYA_BOT_STRINGBUILDERDB_H
