//
// Created by root on 26.06.22.
//

#include <iostream>
#include "StringBuilder.h"

StringBuilder::StringBuilder(string files[]) {
    StringParser sp(files[0]);
    unordered_map<string,string>* tmp;
    tmp = sp.parseFile();
    if(tmp == nullptr){
        cerr << "File dont parse! ERROR EXIT!" << endl;
        exit(-1);
    }
    ru = new unordered_map<string,string>(*tmp);

    sp.changeFile(files[1]);
    tmp = sp.parseFile();
    if(tmp == nullptr){
        cerr << "File dont parse! ERROR EXIT!" << endl;
        exit(-1);
    }
    en = new unordered_map<string,string>(*sp.parseFile());


}

StringBuilder::StringBuilder(string *files, DBHandler &db) {
    if(files == nullptr)
        exit(-9);
    ru = db.getStrings(files[0]);
    if(ru == nullptr) {
        exit(-11);
        cout << "RU strings dont create" << endl;
    }
    en = db.getStrings(files[1]);
    if(en == nullptr) {
        exit(-11);
        cout << "EN strings dont create" << endl;
    }
}

unordered_map<string, string> *StringBuilder::getRu() const {
    return ru;
}

unordered_map<string, string> *StringBuilder::getEn() const {
    return en;
}

StringBuilder::~StringBuilder() {
    delete(en);
    delete(ru);
}


