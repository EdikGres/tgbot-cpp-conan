//
// Created by edik on 09.06.22.
//

#include "CommandRecorder.h"
#include "KeyboardGenerator.h"


using namespace std;
using namespace TgBot;


CommandRecorder::CommandRecorder(TgBot::Bot &bot, DBHandler &db) : db(db), mainKeyboard(new ReplyKeyboardMarkup),
                                                                   testKeyboard(new ReplyKeyboardMarkup),
                                                                   testInlineKeyboard(new InlineKeyboardMarkup) {
    //start-------------------------------------
    commands.emplace_back("start");
    mainKeyboard->resizeKeyboard = true;
    mainKeyboard->oneTimeKeyboard = true;
    KeyboardGenerator::createKeyboard({
                                              {"Открыть стол", "О проекте"},
                                              {"Презентация",  "Мой статус"},
                                              {"Реф ссылка",   "Тех. поддержка"},
                                      }, mainKeyboard);
    bot.getEvents().onCommand("start", [&bot, &db, this](const Message::Ptr &message) {


        bot.getApi().sendMessage(message->chat->id,
                                 "/start for one column keyboard\n/layout for a more complex keyboard", false, 0,
                                 mainKeyboard);

        db.add_user(message->from->id, message->from->username, 0, 0);
    });

    //test---------------------------------------
    commands.emplace_back("test");
    KeyboardGenerator::createInlineKeyboard({
                                                    {"1", "2", "3"},
                                                    {"4", "5", "6"},
                                                    {"7", "8", "9"},
                                            }, testInlineKeyboard);

    bot.getEvents().onCommand("test", [&bot, this](const Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id,
                                 "/start for one column keyboard\n/layout for a more complex keyboard", false, 0,
                                 testInlineKeyboard);
    });

    //my_id
    commands.emplace_back("my_id");
    bot.getEvents().onCommand("my_id", [&bot, this](const Message::Ptr &message) {
        string text = "ID:";
        text += std::to_string(message->from->id);
        text += "\nchat_id:" + to_string(message->chat->id);
        bot.getApi().sendMessage(message->chat->id, text);
    });

    //any command----------------------------------
    bot.getEvents().onUnknownCommand([&bot, this](const Message::Ptr &message) {
        //printf("User wrote %s\n", message->text.c_str());
        for (const auto &command: this->commands) {
            if ("/" + command == message->text)
                return;
        }
        bot.getApi().sendMessage(message->chat->id, "Неизвестная команда!");
    });
}
