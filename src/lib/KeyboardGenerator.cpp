//
// Created by edik on 09.06.22.
//

// TODO: Проверить все генераторы клавиатур

#include "KeyboardGenerator.h"
#include <iostream>


//one line(vertical)
void KeyboardGenerator::createInlineColumnKeyboard(const vector<string> &buttonStrings, InlineKeyboardMarkup::Ptr &kb) {
    //test?
    if (!kb->inlineKeyboard.empty())
        kb->inlineKeyboard.clear();
    for (const string &button: buttonStrings) {
        vector<InlineKeyboardButton::Ptr> row;
        InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
        checkButton->text = "button";
        checkButton->callbackData = "button";
        row.push_back(checkButton);
        kb->inlineKeyboard.push_back(row);
    }
}

void KeyboardGenerator::createInlineKeyboard(const vector<vector<string>> &buttons, InlineKeyboardMarkup::Ptr &kb) {
    //test?
    if (!kb->inlineKeyboard.empty())
        kb->inlineKeyboard.clear();
    for (const vector<string> &row: buttons) {
        vector<InlineKeyboardButton::Ptr> row0;
        for (const string &button: row) {
            InlineKeyboardButton::Ptr button_obj(new InlineKeyboardButton);
            button_obj->text = button;
            button_obj->callbackData = button;
            row0.push_back(button_obj);
        }
        kb->inlineKeyboard.push_back(row0);
    }
}

void KeyboardGenerator::createOneRowKeyboard(const vector<string> &buttonStrings, ReplyKeyboardMarkup::Ptr &kb) {
    if (!kb->keyboard.empty())
        kb->keyboard.clear();
    vector<KeyboardButton::Ptr> row;
    for (size_t i = 0; i < buttonStrings.size(); ++i) {
        KeyboardButton::Ptr button(new KeyboardButton);
        button->text = buttonStrings[i];
        row.push_back(button);
    }
    kb->keyboard.push_back(row);
}

void KeyboardGenerator::createKeyboard(const vector<vector<string>> &buttonLayout, ReplyKeyboardMarkup::Ptr &kb) {
    if (!kb->keyboard.empty())
        kb->keyboard.clear();
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

void
KeyboardGenerator::createInlineKeyboard(const vector<vector<string>> &buttons, const vector<vector<string>> &callback,
                                        InlineKeyboardMarkup::Ptr &kb) {
    if (buttons.size() != callback.size()) {
        cout << "callbacks != buttons" << endl;
        exit(-1);
    }
    if (!kb->inlineKeyboard.empty())
        kb->inlineKeyboard.clear();

    for (int i = 0; i < buttons.size() && i < callback.size(); ++i) {
        vector<InlineKeyboardButton::Ptr> row0;
        for (int j = 0; j < buttons[i].size() && j < callback[i].size(); ++j) {
            InlineKeyboardButton::Ptr button_obj(new InlineKeyboardButton);
            button_obj->text = buttons[i][j];
            button_obj->callbackData = callback[i][j];
            row0.push_back(button_obj);
        }
        kb->inlineKeyboard.push_back(row0);
    }
}

