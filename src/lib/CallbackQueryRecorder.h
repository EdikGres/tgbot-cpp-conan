//
// Created by root on 6/29/22.
//

#ifndef KOSTYA_BOT_CALLBACKQUERYRECORDER_H
#define KOSTYA_BOT_CALLBACKQUERYRECORDER_H


#include <tgbot/Bot.h>
#include "../driver/StringBuilder.h"
#include "DBHandler.h"

using namespace std;
using namespace TgBot;

class CallbackQueryRecorder {
public:
    CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb);

private:
    DBHandler &db;
    StringBuilder &sb;
    TgBot::InlineKeyboardMarkup::Ptr languageInlineKeyboard;
    TgBot::InlineKeyboardMarkup::Ptr beginnersStartInlineKeyboard;
    TgBot::InlineKeyboardMarkup::Ptr inlineGMP;
    TgBot::InlineKeyboardMarkup::Ptr NextBackInlineKeyboard;
    TgBot::InlineKeyboardMarkup::Ptr NextMenuInlineKeyboard;
    InputMediaPhoto::Ptr media;
};


#endif //KOSTYA_BOT_CALLBACKQUERYRECORDER_H
