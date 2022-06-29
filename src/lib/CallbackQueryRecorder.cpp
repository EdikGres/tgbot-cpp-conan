//
// Created by root on 6/29/22.
//

#include <thread>
#include "CallbackQueryRecorder.h"
#include "my_lib.h"
#include "KeyboardGenerator.h"

CallbackQueryRecorder::CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb),
                                                                                                  testInlineKeyboard(
                                                                                                          new InlineKeyboardMarkup) {

//    bot.getEvents().onInlineQuery([](InlineQuery::Ptr query){
//        cout << query->query << endl;
//    });
//    bot.getEvents().onChosenInlineResult([](ChosenInlineResult::Ptr query){
//        cout << query->query << endl;
//    });

    //Russian
    bot.getEvents().onCallbackQuery([&bot, &db, &sb, this](CallbackQuery::Ptr query) {
        thread t1([&bot, &db, &sb, query, this]() {
            unordered_map<string, string> *text = my::get_lang(db, sb, query);
            KeyboardGenerator::createInlineKeyboard({
                                                            {text->at("UK"), text->at("RU")}
                                                    }, testInlineKeyboard);
            if (StringTools::startsWith(query->data, text->at("UK"))) {
                db.setLanguage(query->from->id, 1);
            }
            if (StringTools::startsWith(query->data, text->at("RU"))) {
                db.setLanguage(query->from->id, 0);
            }
            text = my::get_lang(db, sb, query);
            try{
                bot.getApi().editMessageText(text->at("start-successful"), query->from->id,
                                             db.getMessages(query->from->id, 1)->back(),
                                             "", "", false, testInlineKeyboard);
            }
            catch (TgException ex) {
                cerr << ex.what() << endl;
            }

        });
        t1.detach();
    });

}
