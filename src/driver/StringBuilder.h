//
// Created by root on 26.06.22.
//

#ifndef KOSTYA_BOT_STRINGBUILDER_H
#define KOSTYA_BOT_STRINGBUILDER_H

#include "unordered_map"
#include "StringParser.h"
#include "DBHandler.h"

using namespace std;

class StringBuilder {
public:
    //deprecated
    StringBuilder(string files[]);

    StringBuilder(string files[], string links, DBHandler &db);

    virtual ~StringBuilder();

    unordered_map<string, string> *getRu() const;

    unordered_map<string, string> *getEn() const;

    unordered_map<string, string> *getLinks() const;

    unordered_map<string, string> *getGMP_beginners_themes() const;

private:
    unordered_map<string, string> *ru;
    unordered_map<string, string> *en;
    unordered_map<string, string> *links;

    unordered_map<string, string> *GMP_beginners_themes;
};


#endif //KOSTYA_BOT_STRINGBUILDER_H
