//
// Created by root on 6/30/22.
//
#include "strings.h"

#define DEBUG



#ifdef DEBUG
const string ru_locale = "../locale/ru.lang";
const string en_locale = "../locale/en.lang";
const string ru_db = "strings_ru";
const string en_db = "strings_en";

#else
const string ru_locale = "locale/ru.lang";
const string en_locale = "locale/en.lang";
#endif