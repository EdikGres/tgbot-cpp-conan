//
// Created by root on 6/30/22.
//
#include "strings.h"

#define DEBUG

const string start_gif = "https://mir-s3-cdn-cf.behance.net/project_modules/max_1200/5eeea355389655.59822ff824b72.gif";
const string CashFlow_menu_gif = "https://i.gifer.com/4MO.gif";
const string CashFlow_learning_menu_gif = "https://i.gifer.com/Be.gif";
const string CashFlow_learning_beginners_menu_gif = "https://i.gifer.com/AnA.gif";
const string CashFlow_learning_teachers_menu_gif = "https://i.gifer.com/ZUs.gif";
const string CashFlow_learning_leaders_menu_gif = "https://i.gifer.com/4Ez.gif";
const string CashFlow_learning_TOPleaders_menu_gif = "https://i.gifer.com/7cXx.gif";
const string no_access = "https://i.gifer.com/7HAU.gif";

#ifdef DEBUG
const string ru_locale = "../locale/ru.lang";
const string en_locale = "../locale/en.lang";
const string ru_db = "strings_ru";
const string en_db = "strings_en";

#else
const string ru_locale = "locale/ru.lang";
const string en_locale = "locale/en.lang";
#endif