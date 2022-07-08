//
// Created by root on 6/29/22.
//

#ifndef KOSTYA_BOT_CALLBACKQUERYRECORDER_H
#define KOSTYA_BOT_CALLBACKQUERYRECORDER_H


#include <tgbot/Bot.h>
#include "../driver/StringBuilder.h"
#include "../driver/DBHandler.h"

using namespace std;
using namespace TgBot;

class CallbackQueryRecorder {
public:
    CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb);

private:
    DBHandler &db;
    StringBuilder &sb;
};


#endif //KOSTYA_BOT_CALLBACKQUERYRECORDER_H
