//
// Created by edik on 09.06.22.
//

#include "CommandRecorder.h"
#include "KeyboardGenerator.h"

using namespace std;
using namespace TgBot;


CommandRecorder::CommandRecorder(TgBot::Bot &bot) : mainKeyboard(new ReplyKeyboardMarkup),
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
    bot.getEvents().onCommand("start", [&bot, this](const Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id,
                                 "/start for one column keyboard\n/layout for a more complex keyboard", false, 0,
                                 mainKeyboard);
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
