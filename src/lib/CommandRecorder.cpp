//
// Created by edik on 09.06.22.
//
//TODO: оптимизировать до 1го обращения к бд запросы в команде.
//TODO: защитить /start от насилия ( возможно поставить мютексы)
//TODO: можно в onUnknownCommand() убрать проверку с контейнера, поскольку это в либе реализовано уже.

#include "CommandRecorder.h"
#include "KeyboardGenerator.h"
#include <thread>
#include <limits>
#include "my_lib.h"
#include "../driver/strings.h"

using namespace std;
using namespace TgBot;


CommandRecorder::CommandRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb) {
    //start-------------------------------------
    commands.emplace_back("start");
    bot.getEvents().onCommand("start", [&bot, &db, this, &sb](const Message::Ptr &message) {
        thread t1([&bot, &db, this, &sb, message]() {
            db.add_user(message->from->id, message->from->username, 0, 0, 0);
            db.setCurMenu(message->from->id, 0);
            unordered_map<string, string> *text = my::get_lang(db, sb, message);
            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
//            KeyboardGenerator::createKeyboard({
//                                                      {text->at("Cash-Flow"), text->at(
//                                                              "GMP")},
//                                              }, mainKeyboard);
            KeyboardGenerator::createInlineKeyboard({
                                                            {text->at("Cash-Flow"), text->at("GMP")}
                                                    }, keyb);
            //language select
//            KeyboardGenerator::createInlineKeyboard({
//                                                            {text->at("UK"), text->at("RU")}
//                                                    }, testInlineKeyboard);
//            Message::Ptr msg = bot.getApi().sendPhoto(message->from->id,
//                                                      start_img,
//                                                      "", 0, mainKeyboard);
//
//            Message::Ptr msg2 = bot.getApi().sendMessage(message->chat->id,
//                                                         text->at("start-message"), false, 0,
//                                                         testInlineKeyboard);

            Message::Ptr msg = bot.getApi().sendPhoto(message->from->id, start_img, text->at("start-message"), 0,
                                                      keyb, "HTML",
                                                      false);
            my::delete_all_messages(db, sb, message, bot);
            db.addMessage(message->from->id, msg->messageId, 1);
            //db.addMessage(message->from->id, msg2->messageId, 1);
            //db.addMessage(message->from->id, message->messageId, 0);
            bot.getApi().deleteMessage(message->from->id, message->messageId);
            //db.deleteMessage(message->from->id, message->messageId);
        });
        t1.detach();
    });


    //test---------------------------------------
    commands.emplace_back("test");
    bot.getEvents().onCommand("test", [&bot, &db, &sb, this](const Message::Ptr &message) {
        thread t1([&bot, &db, &sb, message, this]() {
            const int lang = db.getLanguage(message->from->id);
            unordered_map<string, string> *text;
            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
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
            KeyboardGenerator::createInlineKeyboard({
                                                            {text->at("UK"), text->at("RU")}
                                                    }, keyb);
            bot.getApi().sendMessage(message->chat->id,
                                     "/start for one column keyboard\n/layout for a more complex keyboard", false,
                                     0,
                                     keyb);
        });
        t1.detach();
    });

    //my_id
    commands.emplace_back("my_id");
    bot.getEvents().onCommand("my_id", [&bot, this](const Message::Ptr &message) {
        string text = "ID:";
        text += std::to_string(message->from->id);
        text += "\nchat_id:" + to_string(message->chat->id);
        bot.getApi().sendMessage(message->chat->id, text);
    });

    //clear_all
    commands.emplace_back("clear");
    bot.getEvents().onCommand("clear", [&bot, &db, this](const Message::Ptr &message) {

        thread t1([&bot, &db, message, this]() {
            db.addMessage(message->from->id, message->messageId, 0);
            Message::Ptr msg = bot.getApi().sendMessage(message->from->id, "Test sms!");
            db.addMessage(message->from->id, msg->messageId, 1);

        });
        t1.detach();
    });

    //any command----------------------------------
    bot.getEvents().onUnknownCommand([&bot, this](const Message::Ptr &message) {
        thread t1([&bot, message, this]() {
            //cout << "Message for bot: " << message->messageId << endl;
            for (const auto &command: this->commands) {
                if ("/" + command == message->text)
                    return;
            }
            try {
                bot.getApi().deleteMessage(message->from->id, message->messageId);
            }
            catch (TgException ex) {
                cerr << ex.what() << endl;
            }
            //bot.getApi().sendMessage(message->chat->id, "Неизвестная команда!");
            //cout << "Message for user: " << message->messageId << endl;
        });
        t1.detach();
    });
}
