//
// Created by edik on 09.06.22.
//
//TODO: оптимизировать до 1го обращения к бд запросы в команде.
//TODO: можно в onUnknownCommand() убрать проверку с контейнера, поскольку это в либе реализовано уже.

#include "CommandRecorder.h"
#include "KeyboardGenerator.h"
#include <thread>
#include <limits>
#include "my_lib.h"
#include "../driver/strings.h"

using namespace std;
using namespace TgBot;


CommandRecorder::CommandRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb
) : db(db), sb(sb) {
    //start-------------------------------------
    commands.emplace_back("start");
    bot.getEvents().onCommand("start", [&bot, &db, this, &sb](const Message::Ptr &message) {
        thread t1([&bot, &db, this, &sb, message]() {
            db.add_user(message->from->id, message->from->username, 0, 0, 0, 0);
            db.setCurMenu(message->from->id, START_ST);
            unordered_map<string, string> *text = my::get_lang(db, sb, message);
            unordered_map<string, string> *links = sb.getLinks();
            //PLUG
            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
            if(message->from->id == 524338144 || message->from->id == 307278021) {
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("Cash-Flow"), text->at("GMP")}
                                                        }, {{"cashflow", "GMP"}}, keyb);
            }
            else {
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("GMP")}
                                                        }, {{ "GMP"}}, keyb);
            }
            //PLUG
            Message::Ptr msg;
            try {
                msg = bot.getApi().sendPhoto(message->from->id, links->at("start_gif"), text->at("start-message"), 0, keyb, "HTML");
            }
            catch (TgException &ex) {
                cerr << ex.what() << endl;
                db.setIsSpammer(message->from->id, 1);
            }
            my::delete_all_messages(db, sb, message, bot);
            if (msg != nullptr)
                db.addMessage(message->from->id, msg->messageId, 1);
            try {
                bot.getApi().deleteMessage(message->from->id, message->messageId);
            }
            catch (TgException &ex) {
                cerr << ex.what() << endl;
            }
        });
        t1.detach();
    });

    //language-----------------------------------
//    commands.emplace_back("language");
//    bot.getEvents().onCommand("language", [&bot, &db, &sb, this](const Message::Ptr &message) {
//        thread t1([&bot, &db, &sb, message, this]() {
//            unordered_map<string, string> *text = my::get_lang(db, sb, message);
//            try {
//                bot.getApi().deleteMessage(message->from->id, message->messageId);
//            }
//            catch (TgException &ex) {
//                cerr << ex.what() << endl;
//            }
//            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
//            KeyboardGenerator::createInlineKeyboard({{text->at("UK"), text->at("RU")},
//                                                     {text->at("clear-language")}}, keyb);
//            Message::Ptr msg;
//            try {
//                msg = bot.getApi().sendMessage(message->from->id, text->at("select-language"), false, 0, keyb);
//            }
//            catch (TgException &ex) {
//                cerr << ex.what() << endl;
//            }
//            db.addMessageWithText(message->from->id, msg->messageId, 1, text->at("select-language"));
//        });
//        t1.detach();
//    });


    //test---------------------------------------
//    commands.emplace_back("test");
//    bot.getEvents().onCommand("test", [&bot, &db, &sb, this](const Message::Ptr &message) {
//        thread t1([&bot, &db, &sb, message, this]() {
//
//            try {
//                Message::Ptr msg;
//                msg = bot.getApi().sendVideo(message->from->id, InputFile::fromFile("files/promo-GMP.mp4", "video/mp4"), true, 0, 0, 0, "", "test");
//
//                db.addFile("promo-GMP", msg->video->fileId);
//                //cout << db.getFile("promo-GMP") << endl;
//            }
//            catch (TgException ex) {
//                cerr << ex.what() << endl;
//            }
//        });
//        t1.detach();
//    });


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
            catch (TgException &ex) {
                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
            }
            //bot.getApi().sendMessage(message->chat->id, "Неизвестная команда!");
            //cout << "Message for user: " << message->messageId << endl;
        });
        t1.detach();
    });
}
