//
// Created by edik on 09.06.22.
//
// TODO: Переделать хранение клавиатур в боте, с Commandrecorder'a, на какое-нибудь другое. Хотя впрочем оно хранится в хипе. Мб указатели куда вынести,
//  ебучие shared_ptr

#ifndef TGBOT_TEST_COMMANDRECORDER_H
#define TGBOT_TEST_COMMANDRECORDER_H

#include <tgbot/tgbot.h>
#include <vector>
#include <string>

using namespace std;

class CommandRecorder {
public:
    explicit CommandRecorder(TgBot::Bot &bot);

private:
    vector<string> commands;
    TgBot::ReplyKeyboardMarkup::Ptr mainKeyboard;
    TgBot::ReplyKeyboardMarkup::Ptr testKeyboard;
    TgBot::InlineKeyboardMarkup::Ptr testInlineKeyboard;
};


#endif //TGBOT_TEST_COMMANDRECORDER_H
