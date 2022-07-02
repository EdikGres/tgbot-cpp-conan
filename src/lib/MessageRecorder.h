//
// Created by root on 6/29/22.
//

#ifndef KOSTYA_BOT_MESSAGERECORDER_H
#define KOSTYA_BOT_MESSAGERECORDER_H


#include <tgbot/Bot.h>
#include "DBHandler.h"
#include "../driver/StringBuilder.h"

using namespace std;
using namespace TgBot;

class MessageRecorder {
public:
    MessageRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb);

private:
    vector<string> messages;
    DBHandler &db;
    StringBuilder &sb;

    const string GMP_photoPath = "../img/GMP.jpg";
    const string GMP_photoMimeType = "image/jpeg";
};


#endif //KOSTYA_BOT_MESSAGERECORDER_H
