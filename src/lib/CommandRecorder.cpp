//
// Created by edik on 09.06.22.
//

#include "CommandRecorder.h"
#include "KeyboardGenerator.h"
#include <thread>


using namespace std;
using namespace TgBot;


CommandRecorder::CommandRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb), mainKeyboard(new ReplyKeyboardMarkup),
                                                                   testKeyboard(new ReplyKeyboardMarkup),
                                                                   testInlineKeyboard(new InlineKeyboardMarkup) {
    //start-------------------------------------
    commands.emplace_back("start");
    mainKeyboard->resizeKeyboard = true;
    mainKeyboard->oneTimeKeyboard = true;
    bot.getEvents().onCommand("start", [&bot, &db, this, &sb](const Message::Ptr &message) {

        thread t1([&bot, &db, this, &sb, message](){

            db.add_user(message->from->id, message->from->username, 0, 0, 0);
            db.setCurMenu(message->from->id, 0);
            const int lang = db.getLanguage(message->from->id);
            unordered_map<string,string>* text;
            switch (lang) {
                case 0:
                    text = sb.getRu();
                    break;
                case 1:
                    text = sb.getEn();
                    break;
                default:
                    text = sb.getRu();
                    break;
            }
            KeyboardGenerator::createKeyboard({
                                                      {text->at("training for beginners"), text->at("training for teachers")},
                                                      {text->at("training for leaders"),  text->at("training for TOP-leaders")},
                                                      {text->at("team rules"),   text->at("events and training")},
                                                      {text->at("mentors and focus groups"),   text->at("testing")},
                                                      {text->at("materials")},
                                              }, mainKeyboard);
            bot.getApi().sendMessage(message->chat->id,
                                     text->at("start-message"), false, 0,
                                     mainKeyboard);
        });
        t1.detach();
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
