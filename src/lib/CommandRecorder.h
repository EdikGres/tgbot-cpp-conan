//
// Created by edik on 09.06.22.
//

#ifndef TGBOT_TEST_COMMANDRECORDER_H
#define TGBOT_TEST_COMMANDRECORDER_H

#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include "DBHandler.h"
#include "../driver/StringBuilder.h"

using namespace std;

class CommandRecorder {
public:
    explicit CommandRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb);

private:
    vector<string> commands;
    DBHandler &db;
    StringBuilder &sb;

    const string start_photoPath = "../img/cat-start.jpeg";
    const string start_photoMimeType = "image/jpeg";
};


#endif //TGBOT_TEST_COMMANDRECORDER_H
