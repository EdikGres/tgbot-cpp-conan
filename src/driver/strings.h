//
// Created by root on 6/30/22.
//

#ifndef KOSTYA_BOT_STRINGS_H
#define KOSTYA_BOT_STRINGS_H

#include <string>

using namespace std;

extern const string start_gif;
extern const string CashFlow_menu_gif;
extern const string ru_locale;
extern const string en_locale;
extern const string CashFlow_learning_menu_gif;
extern const string CashFlow_learning_beginners_menu_gif;
extern const string CashFlow_learning_teachers_menu_gif;
extern const string CashFlow_learning_leaders_menu_gif;
extern const string CashFlow_learning_TOPleaders_menu_gif;
extern const string no_access;


enum state{
    START_ST = 0,
    CASHFLOW_MAIN_ST = 1,
    CASHFLOW_LEARNING_ST = 2,
    CASHFLOW_BEGINNERS_LEARNING = 3,
    CASHFLOW_TEACHERS_LEARNING = 4,
    CASHFLOW_LEADERS_LEARNING = 5,
    CASHFLOW_TOPLEADERS_LERNING = 6,
    CASHFLOW_MAIN_MATERIALS = 7,
    CASHFLOW_BEGINNERS_LEARNING_START = 8,



};

#endif //KOSTYA_BOT_STRINGS_H
