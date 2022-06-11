//
// Created by edik on 09.06.22.
//

#ifndef TGBOT_TEST_KEYBOARDGENERATOR_H
#define TGBOT_TEST_KEYBOARDGENERATOR_H

#include <tgbot/tgbot.h>
#include <vector>
#include <string>

using namespace TgBot;
using namespace std;

class KeyboardGenerator {
public:
    static void createInlineColumnKeyboard(const vector<string> &buttonStrings, InlineKeyboardMarkup::Ptr &kb);
    static void createOneRowKeyboard(const vector<string> &buttonStrings, ReplyKeyboardMarkup::Ptr &kb);
    static void createKeyboard(const vector<vector<string>> &buttonLayout, ReplyKeyboardMarkup::Ptr &kb);
    static void createInlineKeyboard(const vector<vector<string>> &buttons, InlineKeyboardMarkup::Ptr &kb);
private:


};


#endif //TGBOT_TEST_KEYBOARDGENERATOR_H
