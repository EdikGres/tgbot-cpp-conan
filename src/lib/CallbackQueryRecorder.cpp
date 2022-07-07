//
// Created by root on 6/29/22.
//

#include <thread>
#include "CallbackQueryRecorder.h"
#include "my_lib.h"
#include "KeyboardGenerator.h"
#include "../driver/strings.h"

//TODO: Переделать if(), сначала int, затем string. Так должно быть быстрее.


CallbackQueryRecorder::CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb) {


    bot.getEvents().onCallbackQuery([&bot, &db, &sb, this](CallbackQuery::Ptr query) {
        thread t1([&bot, &db, &sb, query, this]() {
            unordered_map<string, string> *text = my::get_lang(db, sb, query);
            int cur_menu = db.getCurMenu(query->from->id);
            int cur_access = db.getAccessLevel(query->from->id);
//----------------------------------------------------------------------------------------------------------------------
//                                            LANGUAGE_MODUL
//----------------------------------------------------------------------------------------------------------------------
            //language UK
            if (StringTools::startsWith(query->data, text->at("UK"))) {
                InlineKeyboardMarkup::Ptr languageInlineKeyboard(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("UK"), text->at("RU")},
                                                                {text->at("clear-language")}
                                                        }, languageInlineKeyboard);
                db.setLanguage(query->from->id, 1);
                text = my::get_lang(db, sb, query);
                try {
                    bot.getApi().editMessageText(text->at("language-set-successful"), query->from->id,
                                                 query->message->messageId,
                                                 "", "", false, languageInlineKeyboard);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }
                return;
            }
            //language RU
            if (StringTools::startsWith(query->data, text->at("RU"))) {
                InlineKeyboardMarkup::Ptr languageInlineKeyboard(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("UK"), text->at("RU")},
                                                                {text->at("clear-language")}
                                                        }, languageInlineKeyboard);
                db.setLanguage(query->from->id, 0);
                text = my::get_lang(db, sb, query);
                try {
                    bot.getApi().editMessageText(text->at("language-set-successful"), query->from->id,
                                                 query->message->messageId,
                                                 "", "", false, languageInlineKeyboard);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }
                return;
            }
            //language-hide
            if (StringTools::startsWith(query->data, text->at("clear-language"))) {
                try {
                    db.deleteMessage(query->from->id, query->message->messageId);
                    bot.getApi().deleteMessage(query->from->id, query->message->messageId);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }
                return;
            }

//----------------------------------------------------------------------------------------------------------------------
//                                            CASHFLOW_MODULE
//----------------------------------------------------------------------------------------------------------------------
            //CashFlow MENU=1
            if ((StringTools::startsWith(query->data, text->at("Cash-Flow")) ||
                 StringTools::startsWith(query->data, text->at("back-cashflow"))) &&
                (cur_menu == START_ST || cur_menu == CASHFLOW_LEARNING_ST)) {
                db.setCurMenu(query->from->id, CASHFLOW_MAIN_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("CashFlow-learning")},
                                                         {text->at("materials"),  text->at("testing")},
                                                         {text->at("team rules"), text->at("events and training")},
                                                         {text->at("mentors and focus groups")}
                                                        }, keyb);

                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = CashFlow_menu_gif;
                    media->caption = text->at("text-CashFlow");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                  keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }

                return;
            }

            //CashFlow-Learning MENU=2
            if ((StringTools::startsWith(query->data, text->at("CashFlow-learning")) ||
                 StringTools::startsWith(query->data, text->at("back-menu"))) &&
                (cur_menu == CASHFLOW_MAIN_ST ||
                 cur_menu >= CASHFLOW_BEGINNERS_LEARNING && cur_menu <= CASHFLOW_TOPLEADERS_LERNING)) {
                db.setCurMenu(query->from->id, CASHFLOW_LEARNING_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("training for beginners")},
                                                         {text->at("training for teachers")},
                                                         {text->at("training for leaders")},
                                                         {text->at("training for TOP-leaders")},
                                                         {text->at("back-cashflow")}
                                                        }, keyb);

                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = CashFlow_learning_menu_gif;
                    media->caption = text->at("text-CashFlow-learning");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                  keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }
                return;
            }
            //CashFlow-Learning-forBeginners MENU=3
            if (StringTools::startsWith(query->data, text->at("training for beginners")) &&
                cur_menu == CASHFLOW_LEARNING_ST) {
                db.setCurMenu(query->from->id, CASHFLOW_BEGINNERS_LEARNING);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                         {text->at("materials")}
                                                        }, keyb);
                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = CashFlow_learning_beginners_menu_gif;
                    media->caption = text->at("text-menu-beginners");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }
                return;
            }
            //CashFlow-Learning-forTeachers MENU=3
            if (StringTools::startsWith(query->data, text->at("training for teachers")) &&
                cur_menu == CASHFLOW_LEARNING_ST) {
                db.setCurMenu(query->from->id, CASHFLOW_TEACHERS_LEARNING);
                if (cur_access < 1) {
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                            }, keyb);
                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = no_access;
                        media->caption = text->at("partition-unavailable");
                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException &ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }
                    return;
                }
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                         {text->at("checklist for mentors")},
                                                         {text->at("materials")}
                                                        }, keyb);
                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = CashFlow_learning_teachers_menu_gif;
                    media->caption = text->at("text-menu-teachers");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }

                return;
            }
            //CashFlow-Learning-forLeaders MENU=3
            if (StringTools::startsWith(query->data, text->at("training for leaders")) &&
                cur_menu == CASHFLOW_LEARNING_ST) {
                db.setCurMenu(query->from->id, CASHFLOW_LEADERS_LEARNING);
                if (cur_access < 2) {
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                            }, keyb);
                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = no_access;
                        media->caption = text->at("partition-unavailable");
                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException &ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }
                    return;
                }
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                         {text->at("materials")}
                                                        }, keyb);
                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = CashFlow_learning_leaders_menu_gif;
                    media->caption = text->at("text-menu-leaders");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }

                return;
            }
            //CashFlow-Learning-forTOPLeaders MENU=3
            if (StringTools::startsWith(query->data, text->at("training for TOP-leaders")) &&
                cur_menu == CASHFLOW_LEARNING_ST) {
                db.setCurMenu(query->from->id, CASHFLOW_TOPLEADERS_LERNING);
                if (cur_access < 3) {
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                            }, keyb);
                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = no_access;
                        media->caption = text->at("partition-unavailable");
                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException &ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }
                    return;
                }
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                         {text->at("materials")}
                                                        }, keyb);
                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = CashFlow_learning_TOPleaders_menu_gif;
                    media->caption = text->at("text-menu-top-leaders");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }

                return;
            }

            //CashFlow-main-Materials
            if(cur_menu == CASHFLOW_MAIN_ST && StringTools::startsWith(query->data, text->at("materials"))){
                

                return;
            }

            //CashFlow-learning-forBeginners MENU=8
            if(cur_menu == CASHFLOW_BEGINNERS_LEARNING && StringTools::startsWith(query->data, text->at("start learning"))){

                return;
            }

        });
        t1.detach();
    });

}
