//
// Created by root on 26.06.22.
//

#include <iostream>
#include <utility>
#include "StringBuilder.h"

//deprecated
StringBuilder::StringBuilder(string files[]) {
    StringParser sp(files[0]);
    unordered_map<string, string> *tmp;
    tmp = sp.parseFile();
    if (tmp == nullptr) {
        cerr << "File dont parse! ERROR EXIT!" << endl;
        exit(-1);
    }
    ru = new unordered_map<string, string>(*tmp);

    sp.changeFile(files[1]);
    tmp = sp.parseFile();
    if (tmp == nullptr) {
        cerr << "File dont parse! ERROR EXIT!" << endl;
        exit(-1);
    }
    en = new unordered_map<string, string>(*sp.parseFile());

}

StringBuilder::StringBuilder(string *files, string links, DBHandler &db) {
    if (files == nullptr)
        exit(-9);
    ru = db.getStrings(files[0]);
    if (ru == nullptr) {
        cout << "RU strings dont create" << endl;
        exit(-11);
    }
    en = db.getStrings(files[1]);
    if (en == nullptr) {
        cout << "EN strings dont create" << endl;
        exit(-11);
    }
    this->links = db.getLinks(std::move(links));
    if (this->links == nullptr) {
        cout << "links strings dont create" << endl;
        exit(-11);
    }
    GMP_beginners_themes = db.getRequest("SELECT * FROM links WHERE key_str LIKE 'GMP-beginners-themes-%'");
    GMP_teachers_themes = db.getRequest("SELECT * FROM links WHERE key_str LIKE 'GMP-teachers-themes-%'");
    GMP_leaders_themes = db.getRequest("SELECT * FROM links WHERE key_str LIKE 'GMP-leaders-themes-%'");
    GMP_TOP_leaders_themes = db.getRequest("SELECT * FROM links WHERE key_str LIKE 'GMP-TOP-leaders-themes-%'");
//    cout << GMP_beginners_themes->size() << endl;
//    for (auto& el : *GMP_beginners_themes) {
//        cout << el.first << " : " << el.second << endl;
//    }

}

unordered_map<string, string> *StringBuilder::getRu() const {
    return ru;
}

unordered_map<string, string> *StringBuilder::getEn() const {
    return en;
}

StringBuilder::~StringBuilder() {
    delete (en);
    delete (ru);
    delete (links);
    delete (GMP_beginners_themes);
}

unordered_map<string, string> *StringBuilder::getLinks() const {
    return this->links;
}

unordered_map<string, string> *StringBuilder::getGMP_beginners_themes() const {
    return GMP_beginners_themes;
}

unordered_map<string, string> *StringBuilder::getGmpTeachersThemes() const {
    return GMP_teachers_themes;
}

unordered_map<string, string> *StringBuilder::getGmpLeadersThemes() const {
    return GMP_leaders_themes;
}

unordered_map<string, string> *StringBuilder::getGmpTopLeadersThemes() const {
    return GMP_TOP_leaders_themes;
}


