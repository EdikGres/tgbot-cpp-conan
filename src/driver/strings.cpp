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

const string no_access_gif = "https://i.gifer.com/7HAU.gif";

const string GMP_menu_gif = "https://i.gifer.com/XUkW.gif";
const string GMP_learning_menu_gif = "https://i.gifer.com/3ww9.gif";
const string GMP_learning_beginners_menu_gif = "https://i.gifer.com/TBX1.gif";
const string GMP_learning_teachers_menu_gif = "https://i.gifer.com/iJb.gif";
const string GMP_learning_leaders_menu_gif = "https://i.gifer.com/20Jm.gif";
const string GMP_learning_TOPleaders_menu_gif = "https://i.gifer.com/22n.gif";

const unordered_map<int, string> GMP_Beginners_gif({{0, "https://i.gifer.com/Cwgf.gif"},
                                                    {1, "https://i.gifer.com/BLK2.gif"},
                                                    {2, "https://i.gifer.com/RrVB.gif"},
                                                    {3, "https://i.gifer.com/U0fP.gif"},
                                                    {4, "https://i.gifer.com/NTf0.gif"},
                                                    {5, "https://i.gifer.com/LynA.gif"},
                                                    {6, "https://i.gifer.com/72Y3.gif"},
                                                    {7, "https://i.gifer.com/Utkc.gif"},
                                                    {8, "https://i.gifer.com/4uE7.gif"},
                                                    {9, "https://i.gifer.com/DRFl.gif"}});

const string GMP_learning_beginners_end = "https://i.gifer.com/3A8q.gif";

#ifdef DEBUG
const string ru_locale = "../locale/ru.lang";
const string en_locale = "../locale/en.lang";
const string ru_db = "strings_ru";
const string en_db = "strings_en";

#else
const string ru_locale = "locale/ru.lang";
const string en_locale = "locale/en.lang";
#endif