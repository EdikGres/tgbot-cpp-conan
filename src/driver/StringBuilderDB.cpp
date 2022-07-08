//
// Created by root on 7/8/22.
//

#include "StringBuilderDB.h"
#include <iostream>

StringBuilderDB::StringBuilderDB(string files[], DBHandler &db) {
    if(files == nullptr)
        exit(-9);
    ru = db.getStrings(files[0]);
    if(ru == nullptr) {
        exit(-11);
        cout << "RU strings dont create" << endl;
    }
    en = db.getStrings(files[0]);
    if(en == nullptr) {
        exit(-11);
        cout << "EN strings dont create" << endl;
    }

}

StringBuilderDB::~StringBuilderDB() {
    delete(ru);
    delete(en);
}

unordered_map<string, string> *StringBuilderDB::getRu() const {
    return ru;
}

unordered_map<string, string> *StringBuilderDB::getEn() const {
    return en;
}
