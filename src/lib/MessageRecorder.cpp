//
// Created by root on 6/29/22.
//

#include "MessageRecorder.h"
#include <iostream>
#include "KeyboardGenerator.h"
#include "my_lib.h"
#include "../driver/strings.h"

using namespace std;

MessageRecorder::MessageRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb){
    bot.getEvents().onNonCommandMessage([&bot, &db, &sb, this](const Message::Ptr &message) {
        thread t1([&bot, &db, &sb, message, this]() {
                try {
                    bot.getApi().deleteMessage(message->from->id, message->messageId);
                }
                catch (TgException& ex) {
                    cerr << ex.what()  << "\n\tline: " << __LINE__ << endl;
                }
                return;
        });
        t1.detach();
    });
}
