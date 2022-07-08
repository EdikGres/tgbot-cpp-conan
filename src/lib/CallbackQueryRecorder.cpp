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
            //DEBUG
            cout << "query: " << query->data << endl;
            unordered_map<string, string> *text = my::get_lang(db, sb, query);
            int cur_menu = db.getCurMenu(query->from->id);
            //optimize in 1 !!!
            int cur_access_CashFlow = db.getAccessLevelCashFlow(query->from->id);
            int cur_access_GMP = db.getAccessLevelGMP(query->from->id);
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
            if (query->data == "cashflow") {



                //db.setCurMenu(query->from->id, CASHFLOW_MAIN_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("CashFlow-learning")},
                                                         {text->at("materials"),  text->at("testing")},
                                                         {text->at("team rules"), text->at("events and training")},
                                                         {text->at("mentors and focus groups")}
                                                        }, {{"cashflow-learning"},
                                                            {"cashflow-materials",  "cashflow-testing"},
                                                            {"cashflow-team-rules", "cashflow-events-and-training"},
                                                            {"cashflow-mentors-and-focus-groups"}
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
            if (StringTools::startsWith(query->data, "cashflow-learning")) {
                //db.setCurMenu(query->from->id, CASHFLOW_LEARNING_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("training for beginners")},
                                                         {text->at("training for teachers")},
                                                         {text->at("training for leaders")},
                                                         {text->at("training for TOP-leaders")},
                                                         {text->at("back-back")}
                                                        }, {{"cashflow-training-for-beginners"},
                                                            {"cashflow-training-for-teachers"},
                                                            {"cashflow-training-for-leaders"},
                                                            {"cashflow-training-for-TOP-leaders"},
                                                            {"cashflow"}
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
            if (StringTools::startsWith(query->data, "cashflow-training-for-beginners")) {
                //db.setCurMenu(query->from->id, CASHFLOW_BEGINNERS_LEARNING);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                         {text->at("materials")}
                                                        }, {{"cashflow-beginners-start-learning", "cashflow-learning"},
                                                            {"cashflow-beginner-materials"}
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
            if (StringTools::startsWith(query->data, "cashflow-training-for-teachers")) {
                //db.setCurMenu(query->from->id, CASHFLOW_TEACHERS_LEARNING);
                if (cur_access_CashFlow < 1) {
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                            }, {{"cashflow-learning"}}, keyb);
                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = no_access_gif;
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
                                                        }, {{"cashflow-teachers-start-learning", "cashflow-learning"},
                                                            {"cashflow-teachers-checklist-for-mentors"},
                                                            {"cashflow-teachers-materials"}
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
            if (StringTools::startsWith(query->data, "cashflow-training-for-leaders")) {
                //db.setCurMenu(query->from->id, CASHFLOW_LEADERS_LEARNING);
                if (cur_access_CashFlow < 2) {
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                            }, {{"cashflow-learning"}}, keyb);
                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = no_access_gif;
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
                                                        }, {{"cashflow-leaders-start-learning", "cashflow-learning"},
                                                            {"cashflow-leaders-materials"}
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
            if (StringTools::startsWith(query->data, "cashflow-training-for-TOP-leaders")) {
                //db.setCurMenu(query->from->id, CASHFLOW_TOPLEADERS_LERNING);
                if (cur_access_CashFlow < 3) {
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                            }, {{"cashflow-learning"}}, keyb);
                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = no_access_gif;
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
                                                        }, {{"cashflow-TOP-leaders-start-learning", "cashflow-learning"},
                                                            {"cashflow-TOP-leaders-materials"}
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
            if (StringTools::startsWith(query->data, "materials")) {


                return;
            }

            //CashFlow-learning-forBeginners MENU=8
            if (StringTools::startsWith(query->data, "start-learning")) {


                return;
            }


//----------------------------------------------------------------------------------------------------------------------
//                                            CASHFLOW_MODULE
//----------------------------------------------------------------------------------------------------------------------
            //GMP main menu
            if(query->data == "GMP"){
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("GMP-learning")},
                                                         {text->at("materials"),  text->at("testing")},
                                                         {text->at("team rules"), text->at("events and training")},
                                                         {text->at("mentors and focus groups")}
                                                        }, {{"GMP-learning"},
                                                            {"GMP-materials",  "GMP-testing"},
                                                            {"GMP-team-rules", "GMP-events-and-training"},
                                                            {"GMP-mentors-and-focus-groups"}
                                                        }, keyb);

                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = GMP_menu_gif;
                    media->caption = text->at("text-GMP");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                  keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }

                return;
            }
            //GMP-learning-menu
            if (StringTools::startsWith(query->data, "GMP-learning")) {
                //db.setCurMenu(query->from->id, CASHFLOW_LEARNING_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("training for beginners")},
                                                         {text->at("training for teachers")},
                                                         {text->at("training for leaders")},
                                                         {text->at("training for TOP-leaders")},
                                                         {text->at("back-cashflow")}
                                                        }, {{"training-for-beginners"},
                                                            {"training-for-teachers"},
                                                            {"training-for-leaders"},
                                                            {"training-for-TOP-leaders"},
                                                            {"cashflow"}
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

        });
        t1.detach();
    });

}
