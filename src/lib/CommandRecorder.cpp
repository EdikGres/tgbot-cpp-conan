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
            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
            KeyboardGenerator::createInlineKeyboard({
                                                            {text->at("Cash-Flow"), text->at("GMP")}
                                                    }, {{"cashflow", "GMP"}}, keyb);
            Message::Ptr msg;
            try {
                msg = bot.getApi().sendAnimation(message->from->id, links->at("start_gif"), 0, 0, 0, "",
                                                 text->at("start-message"),
                                                 0, keyb, "HTML");
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
    commands.emplace_back("test");
    bot.getEvents().onCommand("test", [&bot, &db, &sb, this](const Message::Ptr &message) {
        thread t1([&bot, &db, &sb, message, this]() {
            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
            KeyboardGenerator::createInlineKeyboard({{"1", "2", "3"},
                                                     {"4", "5", "6"},
                                                     {"7", "8", "9"}}, {{"1-cal", "2-cal", "3-cal"},
                                                                        {"4-cal", "5-cal", "6-cal"},
                                                                        {"7-cal", "8-cal", "9-cal"}}, keyb);
            InputMediaAnimation::Ptr anim(new InputMediaAnimation());
            try {
                bot.getApi().sendAnimation(message->from->id, "https://i.ibb.co/7gNYd7j/1.gif", 0, 600, 150, "https://i.ibb.co/7gNYd7j/1.gif",
                                           "<b>Что необходимо понять каждому?</b>\n"
                                           "<i>Если ты читаешь это сообщение, то ты на правильном пути. Здесь есть возможность быть простым пользователем маркетплейса GMP PARD и выгодно закрывать свои базовые потребности. А также есть возможность пользоваться инвестиционной площадкой GMP PARD, покупать долю рекламного трафика на реализацию того или иного товара и ежемесячно зарабатывать, в среднем 8-10%. Согласись, что очень классно, когда есть реальный бизнес, который работает, и в то же время даёт возможность не только выгодно совершать покупки, но и получать ежемесячную прибыль, как на пассиве, так и активно строя команду. </i>", 0, NULL, "HTML");
            }
            catch (TgException ex) {
                cerr << ex.what() << endl;
            }
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
            catch (TgException &ex) {
                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
            }
            //bot.getApi().sendMessage(message->chat->id, "Неизвестная команда!");
            //cout << "Message for user: " << message->messageId << endl;
        });
        t1.detach();
    });
}
