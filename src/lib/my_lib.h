//
// Created by root on 6/29/22.
//

#ifndef KOSTYA_BOT_MY_LIB_H
#define KOSTYA_BOT_MY_LIB_H

#include <tgbot/tgbot.h>
#include <thread>
#include <limits>
#include "../driver/StringBuilder.h"
#include "DBHandler.h"

namespace my{
    unordered_map<string, string>* get_lang(DBHandler &db, StringBuilder &sb, const TgBot::Message::Ptr &message);
    unordered_map<string, string>* get_lang(DBHandler &db, StringBuilder &sb, const TgBot::CallbackQuery::Ptr query);
    void delete_all_messages(DBHandler &db, StringBuilder &sb, const TgBot::Message::Ptr &message, TgBot::Bot &bot);

}

#endif //KOSTYA_BOT_MY_LIB_H
