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
