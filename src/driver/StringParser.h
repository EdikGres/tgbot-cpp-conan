//
// Created by root on 26.06.22.
//

#ifndef KOSTYA_BOT_STRINGPARSER_H
#define KOSTYA_BOT_STRINGPARSER_H

#include <unordered_map>
#include <fstream>

using namespace std;

class StringParser {
public:
    StringParser(string &filename);


    ~StringParser();

    int changeFile(string &file);

    unordered_map<string, string> *getTmp() const;

    unordered_map<string, string> *parseFile();

private:
    unordered_map<string, string> *tmp;
    string &filename;
    std::ifstream in;
};


#endif //KOSTYA_BOT_STRINGPARSER_H
