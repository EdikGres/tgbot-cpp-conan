//
// Created by root on 6/29/22.
//

#include <thread>
#include "CallbackQueryRecorder.h"
#include "my_lib.h"
#include "KeyboardGenerator.h"
#include "../driver/strings.h"

//TODO: Сделать ORM модель для парсера. 1 объект - 1 коллбек, но при этом они наследуются, чтобы можно было сгенерировать XML дерево
//TODO: Сделать колбек на материалы, чтобы клавиатура не зависала на пк.


CallbackQueryRecorder::CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb) {


    bot.getEvents().onCallbackQuery([&bot, &db, &sb, this](CallbackQuery::Ptr query) {
        thread t1([&bot, &db, &sb, query, this]() {
            //DEBUG
            cout << "query: " << query->data << endl;
            unordered_map<string, string> *text = my::get_lang(db, sb, query);
            unordered_map<string, string> *links = sb.getLinks();
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
//                                            COMMON_CALLBACKS
//----------------------------------------------------------------------------------------------------------------------
            //return to select projects
            if (StringTools::startsWith(query->data, "return-to-projects")) {
                //db.setCurMenu(query->from->id, START_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("Cash-Flow"), text->at("GMP")}
                                                        }, {{"cashflow", "GMP"}}, keyb);
                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->caption = text->at("start-message");
                    media->parseMode = "HTML";
                    media->media = links->at("start_gif");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                }
                catch (TgException ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << " FILE: " << __FILE__ << endl;
                }
                return;
            }
            //clear-message
            if (StringTools::startsWith(query->data, "clear-message")) {
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
            //CashFlow MENU=1cashflow
            if (StringTools::startsWith(query->data, "cashflow")) {

                //CashFlow-Learning MENU=2
                if (StringTools::startsWith(query->data, "cashflow-learning")) {

                    //CashFlow-Learning-forBeginners MENU=3
                    if (StringTools::startsWith(query->data, "cashflow-learning-beginners")) {
                        //db.setCurMenu(query->from->id, CASHFLOW_BEGINNERS_LEARNING);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                                 {text->at("materials")}
                                                                },
                                                                {{"cashflow-learning-beginners-start", "cashflow-learning"},
                                                                 {"cashflow-learning-beginners-materials"}
                                                                }, keyb);
                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_beginners_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-CashFlow-beginners");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }
                        return;
                    }

                    //CashFlow-Learning-forTeachers MENU=3
                    if (StringTools::startsWith(query->data, "cashflow-learning-teachers")) {
                        //db.setCurMenu(query->from->id, CASHFLOW_TEACHERS_LEARNING);
                        if (cur_access_CashFlow < 1) {
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                                    }, {{"cashflow-learning"}}, keyb);
                            try {
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                media->media = links->at("no_access_gif");
                                media->parseMode = "HTML";
                                media->caption = text->at("partition-unavailable");
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
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
                                                                },
                                                                {{"cashflow-learning-teachers-start", "cashflow-learning"},
                                                                 {"cashflow-learning-teachers-checklist-mentors"},
                                                                 {"cashflow-learning-teachers-materials"}
                                                                }, keyb);
                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_teachers_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-CashFlow-menu-teachers");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }

                        return;
                    }

                    //CashFlow-Learning-forLeaders MENU=3
                    if (StringTools::startsWith(query->data, "cashflow-learning-leaders")) {
                        //db.setCurMenu(query->from->id, CASHFLOW_LEADERS_LEARNING);
                        if (cur_access_CashFlow < 2) {
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                                    }, {{"cashflow-learning"}}, keyb);
                            try {
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                media->media = links->at("no_access_gif");
                                media->parseMode = "HTML";
                                media->caption = text->at("partition-unavailable");
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
                            }
                            catch (TgException &ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                                 {text->at("materials")}
                                                                },
                                                                {{"cashflow-learning-leaders-start", "cashflow-learning"},
                                                                 {"cashflow-learning-leaders-materials"}
                                                                }, keyb);
                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_leaders_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-CashFlow-menu-teachers");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }

                        return;
                    }

                    //CashFlow-Learning-forTOPLeaders MENU=3
                    if (StringTools::startsWith(query->data, "cashflow-learning-TOP-leaders")) {
                        //db.setCurMenu(query->from->id, CASHFLOW_TOPLEADERS_LERNING);
                        if (cur_access_CashFlow < 3) {
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                                    }, {{"cashflow-learning"}}, keyb);
                            try {
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                media->media = links->at("no_access_gif");
                                media->parseMode = "HTML";
                                media->caption = text->at("partition-unavailable");
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
                            }
                            catch (TgException &ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("start learning"), text->at("back-menu")},
                                                                 {text->at("materials")}
                                                                },
                                                                {{"cashflow-learning-TOP-leaders-start", "cashflow-learning"},
                                                                 {"cashflow-learning-TOP-leaders-materials"}
                                                                }, keyb);
                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_TOPleaders_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-CashFlow-menu-top-leaders");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }

                        return;
                    }

                    //db.setCurMenu(query->from->id, CASHFLOW_LEARNING_ST);
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("training for beginners")},
                                                             {text->at("training for teachers")},
                                                             {text->at("training for leaders")},
                                                             {text->at("training for TOP-leaders")},
                                                             {text->at("back-back")}
                                                            }, {{"cashflow-learning-beginners"},
                                                                {"cashflow-learning-teachers"},
                                                                {"cashflow-learning-leaders"},
                                                                {"cashflow-learning-TOP-leaders"},
                                                                {"cashflow"}
                                                            }, keyb);

                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = links->at("CashFlow_learning_menu_gif");
                        media->parseMode = "HTML";
                        media->caption = text->at("text-CashFlow-learning");
                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException &ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }
                    return;
                }


                //db.setCurMenu(query->from->id, CASHFLOW_MAIN_ST);
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("CashFlow-learning")},
                                                         {text->at("materials"), text->at("testing")},
                                                         {text->at("team rules")},
                                                         {text->at("events and training")},
                                                         {text->at("mentors and focus groups")},
                                                         {text->at("return-to-projects")}
                                                        }, {{"cashflow-learning"},
                                                            {"cashflow-materials", "cashflow-testing"},
                                                            {"cashflow-team-rules"},
                                                            {"cashflow-events-and-training"},
                                                            {"cashflow-mentors-and-focus-groups"},
                                                            {"return-to-projects"}
                                                        }, keyb);

                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = links->at("CashFlow_menu_gif");
                    media->parseMode = "HTML";
                    media->caption = text->at("text-CashFlow");
                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                  keyb);
                }
                catch (TgException &ex) {
                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                }

                return;
            }



//----------------------------------------------------------------------------------------------------------------------
//                                            GMP_MODULE
//----------------------------------------------------------------------------------------------------------------------
            //GMP main menu
            if (StringTools::startsWith(query->data, "GMP")) {

                //GMP-learning-menu
                if (StringTools::startsWith(query->data, "GMP-learning")) {

                    //GMP-Learning-forBeginners MENU=3
                    if (StringTools::startsWith(query->data, "GMP-learning-beginners")) {

                        //GMP-Learning-Beginners-materials
                        if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials")) {

                            //GMP-Learning-Beginners-materials-presentation
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-presentation")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;
                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    "https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf",
                                                                    "", "pdf", 0, keyb, "HTML");
                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-presentation");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("presentation"), text->at("promo-clip")},
                                                                     {text->at("negotiation"),  text->at("objections")},
                                                                     {text->at("scripts-1"),    text->at("scripts-2")},
                                                                     {text->at("useful-links")},
                                                                     {text->at("basic-education")},
                                                                     {text->at("practical-training")},
                                                                     {text->at("answers-on-questions")},
                                                                     {text->at("clear-message")}},
                                                                    {{"GMP-learning-beginners-materials-presentation",
                                                                             "GMP-learning-beginners-materials-pc"},
                                                                     {"GMP-learning-beginners-materials-negotiation",
                                                                             "GMP-learning-beginners-materials-objections"},
                                                                     {"GMP-learning-beginners-materials-s-1",
                                                                             "GMP-learning-beginners-materials-s-2"},
                                                                     {"GMP-learning-beginners-materials-ul"},
                                                                     {"GMP-learning-beginners-materials-be"},
                                                                     {"GMP-learning-beginners-materials-pt"},
                                                                     {"GMP-learning-beginners-materials-aoq"},
                                                                     {"clear-message"}},
                                                                    keyb);
                            try {
                                Message::Ptr msg;
                                msg = bot.getApi().sendAnimation(query->from->id,
                                                                 links->at("GMP_learning_beginners_materials_gif"),
                                                                 0, 0,
                                                                 0, "",
                                                                 text->at("GMP-learning-beginners-materials-text"),
                                                                 0, keyb, "HTML");
                                db.addMessageWithText(query->from->id, msg->messageId, 1, "GMP-beginners-materials");
                            }
                            catch (TgException ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }

                        //GMP-Learning-beginners-start
                        if (StringTools::startsWith(query->data, "GMP-learning-beginners-start")) {

                            //GMP-Learning-Beginners-start-next
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-start-next")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 0);
                                ++cur_text;
                                //!!!MAX THEMES!!!
                                if (cur_text >= sb.getGMP_beginners_themes()->size()) {
                                    db.setCurTextGMP(query->from->id, 0, 0);
                                    KeyboardGenerator::createInlineKeyboard({{text->at("end-learning")}}, {{"GMP"}},
                                                                            keyb);
                                    media->media = links->at("GMP_learning_beginners_end");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-beginners-end");
                                } else {
                                    db.setCurTextGMP(query->from->id, 0, cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-start-back"},
                                             {"GMP-learning-beginners-start-next"},
                                             {"GMP-learning-beginners-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-beginners-themes-" + to_string(cur_text);
                                    media->media = sb.getGMP_beginners_themes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-beginners-start-t-" + to_string(cur_text);
                                    media->caption = text->at(str);
                                }
                                try {
                                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //GMP-Learning-Beginners-back
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-start-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 0);
                                if (cur_text <= 1) {
                                    db.setCurTextGMP(query->from->id, 0, 0);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-start-next"},
                                             {"GMP-learning-beginners-materials"},
                                             {"GMP"}}, keyb);
                                    media->media = sb.getGMP_beginners_themes()->at("GMP-beginners-themes-0");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-beginners-start-t-0");
                                } else {
                                    db.setCurTextGMP(query->from->id, 0, --cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-start-back"},
                                             {"GMP-learning-beginners-start-next"},
                                             {"GMP-learning-beginners-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-beginners-themes-" + to_string(cur_text);
                                    media->media = sb.getGMP_beginners_themes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-beginners-start-t-" + to_string(cur_text);
                                    media->caption = text->at(str);
                                }
                                try {
                                    bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            int cur_text = db.getCurTextGMP(query->from->id, 0);
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            if (cur_text <= 0) {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-start-next"},
                                         {"GMP-learning-beginners-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGMP_beginners_themes()->at("GMP-beginners-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-beginners-start-t-0");
                            } else if (cur_text >= sb.getGMP_beginners_themes()->size()) {
                                db.setCurTextGMP(query->from->id, 0, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-start-next"},
                                         {"GMP-learning-beginners-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGMP_beginners_themes()->at("GMP-beginners-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-beginners-start-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-start-back"},
                                         {"GMP-learning-beginners-start-next"},
                                         {"GMP-learning-beginners-materials"},
                                         {"GMP"}}, keyb);
                                string str1 = "GMP-beginners-themes-" + to_string(cur_text);
                                media->media = sb.getGMP_beginners_themes()->at(str1);
                                media->parseMode = "HTML";
                                const string str = "GMP-learning-beginners-start-t-" + to_string(cur_text);
                                media->caption = text->at(str);
                            }
                            try {
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
                            }
                            catch (TgException ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }
                        int cur_text = db.getCurTextGMP(query->from->id, 0);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning"), text->at("back-menu")}},
                                    {{"GMP-learning-beginners-start", "GMP-learning"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning"), text->at("back-menu")}},
                                    {{"GMP-learning-beginners-start", "GMP-learning"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_beginners_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-GMP-menu-beginners");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }
                        return;
                    }

                    //CashFlow-Learning-forTeachers MENU=3
                    if (StringTools::startsWith(query->data, "GMP-learning-teachers")) {
                        if (cur_access_GMP < 1) {
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                                    }, {{"GMP-learning"}}, keyb);
                            try {
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                media->media = links->at("no_access_gif");
                                media->parseMode = "HTML";
                                media->caption = text->at("partition-unavailable");
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
                            }
                            catch (TgException &ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }
                        int cur_text = db.getCurTextGMP(query->from->id, 1);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning"), text->at("back-menu")},
                                     {text->at("checklist for mentors")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-teachers-start", "GMP-learning"},
                                     {"GMP-learning-teachers-checklist-mentors"},
                                     {"GMP-learning-teachers-materials"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning"), text->at("back-menu")},
                                     {text->at("checklist for mentors")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-teachers-start", "GMP-learning"},
                                     {"GMP-learning-teachers-checklist-mentors"},
                                     {"GMP-learning-teachers-materials"}
                                    }, keyb);
                        }
                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_teachers_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-GMP-menu-teachers");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }

                        return;
                    }

                    //CashFlow-Learning-forLeaders MENU=3
                    if (StringTools::startsWith(query->data, "GMP-learning-leaders")) {
                        //db.setCurMenu(query->from->id, CASHFLOW_LEADERS_LEARNING);
                        if (cur_access_GMP < 2) {
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                                    }, {{"GMP-learning"}}, keyb);
                            try {
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                media->media = links->at("no_access_gif");
                                media->parseMode = "HTML";
                                media->caption = text->at("partition-unavailable");
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
                            }
                            catch (TgException &ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }
                        int cur_text = db.getCurTextGMP(query->from->id, 2);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning"), text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-leaders-start", "GMP-learning"},
                                     {"GMP-learning-leaders-materials"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning"), text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-leaders-start", "GMP-learning"},
                                     {"GMP-learning-leaders-materials"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_leaders_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-GMP-menu-leaders");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }

                        return;
                    }

                    //CashFlow-Learning-forTOPLeaders MENU=3
                    if (StringTools::startsWith(query->data, "GMP-learning-TOP-leaders")) {
                        //db.setCurMenu(query->from->id, CASHFLOW_TOPLEADERS_LERNING);
                        if (cur_access_GMP < 3) {
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}
                                                                    }, {{"GMP-learning"}}, keyb);
                            try {
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                media->media = links->at("no_access_gif");
                                media->parseMode = "HTML";
                                media->caption = text->at("partition-unavailable");
                                bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                              keyb);
                            }
                            catch (TgException &ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }
                        int cur_text = db.getCurTextGMP(query->from->id, 3);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning"), text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-TOP-leaders-start", "GMP-learning"},
                                     {"GMP-learning-TOP-leaders-materials"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning"), text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-TOP-leaders-start", "GMP-learning"},
                                     {"GMP-learning-TOP-leaders-materials"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_TOPleaders_menu_gif");
                            media->parseMode = "HTML";
                            media->caption = text->at("text-GMP-menu-TOP-leaders");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }

                        return;
                    }

                    //db.setCurMenu(query->from->id, CASHFLOW_LEARNING_ST);
                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                    KeyboardGenerator::createInlineKeyboard({{text->at("training for beginners")},
                                                             {text->at("training for teachers")},
                                                             {text->at("training for leaders")},
                                                             {text->at("training for TOP-leaders")},
                                                             {text->at("back-back")}
                                                            }, {{"GMP-learning-beginners"},
                                                                {"GMP-learning-teachers"},
                                                                {"GMP-learning-leaders"},
                                                                {"GMP-learning-TOP-leaders"},
                                                                {"GMP"}
                                                            }, keyb);

                    try {
                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                        media->media = links->at("CashFlow_learning_menu_gif");
                        media->parseMode = "HTML";
                        media->caption = text->at("text-GMP-learning");
                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException &ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }
                    return;
                }

                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({{text->at("GMP-learning")},
                                                         {text->at("materials"), text->at("testing")},
                                                         {text->at("team rules")},
                                                         {text->at("events and training")},
                                                         {text->at("mentors and focus groups")},
                                                         {text->at("return-to-projects")}
                                                        }, {{"GMP-learning"},
                                                            {"GMP-materials", "GMP-testing"},
                                                            {"GMP-team-rules"},
                                                            {"GMP-events-and-training"},
                                                            {"GMP-mentors-and-focus-groups"},
                                                            {"return-to-projects"}
                                                        }, keyb);

                try {
                    InputMediaAnimation::Ptr media(new InputMediaAnimation());
                    media->media = links->at("GMP_menu_gif");
                    media->parseMode = "HTML";
                    media->caption = text->at("text-GMP");
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
