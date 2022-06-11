//
// Created by edik on 09.06.22.
//

#include "KeyboardGenerator.h"
#include <iostream>

//one line(vertical)
void KeyboardGenerator::createInlineColumnKeyboard(const vector<string> &buttonStrings, InlineKeyboardMarkup::Ptr &kb) {
    for (const string &button: buttonStrings) {
        vector<InlineKeyboardButton::Ptr> row;
        InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
        checkButton->text = "button";
        checkButton->callbackData = "button";
        row.push_back(checkButton);
        kb->inlineKeyboard.push_back(row);
    }
}

void KeyboardGenerator::createOneRowKeyboard(const vector<string> &buttonStrings, ReplyKeyboardMarkup::Ptr &kb) {
    vector<KeyboardButton::Ptr> row;
    for (size_t i = 0; i < buttonStrings.size(); ++i) {
        KeyboardButton::Ptr button(new KeyboardButton);
        button->text = buttonStrings[i];
        row.push_back(button);
    }
    kb->keyboard.push_back(row);
}

void KeyboardGenerator::createKeyboard(const vector<vector<string>> &buttonLayout, ReplyKeyboardMarkup::Ptr &kb) {
    for (size_t i = 0; i < buttonLayout.size(); ++i) {
        vector<KeyboardButton::Ptr> row;
        for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
            KeyboardButton::Ptr button(new KeyboardButton);
            button->text = buttonLayout[i][j];
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
}
