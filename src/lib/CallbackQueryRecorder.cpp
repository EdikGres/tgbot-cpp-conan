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
                //PLUG
                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                if (query->from->id == 524338144 || query->from->id == 307278021) {
                    KeyboardGenerator::createInlineKeyboard({
                                                                    {text->at("Cash-Flow"), text->at("GMP")}
                                                            }, {{"cashflow", "GMP"}}, keyb);
                } else {
                    KeyboardGenerator::createInlineKeyboard({
                                                                    {text->at("GMP")}
                                                            }, {{"GMP"}}, keyb);
                }
                //PLUG
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
                //PLUG
                if (query->from->id != 307278021 && query->from->id != 524338144)
                    return;
                //PLUG


                //PLUG
//                try {
//                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
//                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
//                                                            {{"return-to-projects"}}, keyb);
//                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
//                    anim->media = links->at("in_develop_CashFlow");
//                    anim->caption = text->at("in-develop");
//                    anim->parseMode = "HTML";
//                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "", keyb);
//                }
//                catch (TgException ex) {
//                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
//                }
//
//                return;
                //PLUG

                //CashFlow-materials
                if (StringTools::startsWith(query->data, "cashflow-materials")) {
                    goto cashflow_materials;
                    return;
                }

                //CashFlow-Learning MENU=2
                if (StringTools::startsWith(query->data, "cashflow-learning")) {

                    //CashFlow-Learning-forBeginners MENU=3
                    if (StringTools::startsWith(query->data, "cashflow-learning-beginners")) {

                        //CashFlow-Learning-Beginners-materials
                        if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials")) {
                            cashflow_materials:

                            //GMP-Learning-Beginners-materials-promo-clip
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-pc")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"cashflow-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop_CashFlow");
                                    anim->type = InputMedia::TYPE::PHOTO;
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //CashFlow-Learning-Beginners-materials-negotiation
                            if (StringTools::startsWith(query->data,
                                                        "cashflow-learning-beginners-materials-negotiation")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                vector<InlineKeyboardButton::Ptr> row0;
                                vector<InlineKeyboardButton::Ptr> row1;
                                vector<InlineKeyboardButton::Ptr> row2;
                                vector<InlineKeyboardButton::Ptr> row3;
                                InlineKeyboardButton::Ptr checkButton0(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton1(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton2(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton3(new InlineKeyboardButton);
                                checkButton0->url = links->at("CashFlow-learning-beginners-materials-negotiation-l-1");
                                checkButton0->text = text->at("CashFlow-learning-beginners-materials-negotiation-l-1");
                                checkButton1->url = links->at("CashFlow-learning-beginners-materials-negotiation-l-2");
                                checkButton1->text = text->at("CashFlow-learning-beginners-materials-negotiation-l-2");
//                                checkButton2->url = links->at("CashFlow-learning-beginners-materials-negotiation-l-3");
//                                checkButton2->text = text->at("CashFlow-learning-beginners-materials-negotiation-l-3");
                                checkButton3->text = text->at("back");
                                checkButton3->callbackData = "cashflow-learning-beginners-materials-back";
                                row0.push_back(checkButton0);
//                                row1.push_back(checkButton1);
//                                row2.push_back(checkButton2);
                                row3.push_back(checkButton3);
                                keyb->inlineKeyboard.push_back(row0);
                                //keyb->inlineKeyboard.push_back(row1);
                                //keyb->inlineKeyboard.push_back(row2);
                                keyb->inlineKeyboard.push_back(row3);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().editMessageCaption(query->from->id, query->message->messageId,
                                                                          text->at(
                                                                                  "GMP-learning-beginners-materials-negotiation-text"),
                                                                          "", keyb);

                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //CashFlow-Learning-Beginners-materials-objections
                            if (StringTools::startsWith(query->data,
                                                        "cashflow-learning-beginners-materials-objections")) {
//                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
//                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
//                                                                        {{"clear-message"}}, keyb);
//                                try {
//                                    Message::Ptr msg;
//
//                                    msg = bot.getApi().sendDocument(query->from->id,
//                                                                    db.getFile("objections"),
//                                                                    "", text->at(
//                                                    "GMP-learning-beginners-materials-objections-text"), 0, keyb,
//                                                                    "HTML");
//
//                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
//                                                          "GMP-beginners-materials-objections");
//                                }
//                                catch (TgException ex) {
//                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
//                                }

                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"cashflow-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop_CashFlow");
                                    anim->type = InputMedia::TYPE::PHOTO;
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }


                                return;
                            }


                            //CashFlow-Learning-Beginners-materials-scripts
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-s")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("scripts-1")},
                                                                         {text->at("scripts-2")},
                                                                         {text->at("back")}},
                                                                        {{"cashflow-learning-beginners-materials-s-1"},
                                                                         {"cashflow-learning-beginners-materials-s-2"},
                                                                         {"cashflow-learning-beginners-materials-back"}},
                                                                        keyb);

                                //CashFlow-Learning-Beginners-materials-scripts-1
                                if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-s-1")) {
                                    try {
                                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                        media->media = links->at("CashFlow-learning-beginners-materials-s-1");
                                        media->type = InputMedia::TYPE::PHOTO;
                                        media->caption = text->at("cashflow-learning-beginners-materials-s-1-text");
                                        media->parseMode = "HTML";
                                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId,
                                                                      "", keyb);
                                    }
                                    catch (TgException ex) {
                                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                    }
                                    return;
                                }

                                //CashFlow-Learning-Beginners-materials-scripts-2
                                if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-s-2")) {
                                    try {
                                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                        media->media = links->at("CashFlow-learning-beginners-materials-s-2");
                                        media->type = InputMedia::TYPE::PHOTO;
                                        media->caption = text->at("cashflow-learning-beginners-materials-s-2-text");
                                        media->parseMode = "HTML";
                                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId,
                                                                      "", keyb);
                                    }
                                    catch (TgException ex) {
                                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                    }
                                    return;
                                }

                                try {
                                    bot.getApi().editMessageCaption(query->from->id, query->message->messageId,
                                                                    text->at(
                                                                            "GMP-learning-beginners-materials-s-text"),
                                                                    "", keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }


                            //CashFlow-Learning-Beginners-materials-useful-links
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-ul")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                vector<InlineKeyboardButton::Ptr> row0;
                                vector<InlineKeyboardButton::Ptr> row1;
                                vector<InlineKeyboardButton::Ptr> row2;
                                vector<InlineKeyboardButton::Ptr> row3;
                                InlineKeyboardButton::Ptr checkButton0(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton1(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton2(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton3(new InlineKeyboardButton);
                                checkButton0->url = links->at("cashflow-learning-beginners-materials-ul-1");
                                checkButton0->text = text->at("cashflow-learning-beginners-materials-ul-1");
                                checkButton1->url = links->at("cashflow-learning-beginners-materials-ul-2");
                                checkButton1->text = text->at("cashflow-learning-beginners-materials-ul-2");
                                checkButton2->url = links->at("cashflow-learning-beginners-materials-ul-3");
                                checkButton2->text = text->at("cashflow-learning-beginners-materials-ul-3");
                                checkButton3->text = text->at("back");
                                checkButton3->callbackData = "cashflow-learning-beginners-materials-back";
                                row0.push_back(checkButton0);
                                row1.push_back(checkButton1);
                                row2.push_back(checkButton2);
                                row3.push_back(checkButton3);
                                keyb->inlineKeyboard.push_back(row0);
                                keyb->inlineKeyboard.push_back(row1);
                                keyb->inlineKeyboard.push_back(row2);
                                keyb->inlineKeyboard.push_back(row3);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().editMessageCaption(query->from->id, query->message->messageId,
                                                                          text->at(
                                                                                  "GMP-learning-beginners-materials-ul-text"),
                                                                          "", keyb);

                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //CashFlow-learning-beginners-materials-binance
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-binance")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    db.getFile("binance"),
                                                                    "", text->at(
                                                    "CashFlow-learning-beginners-materials-binance-text"), 0, keyb,
                                                                    "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "cashflow-beginners-materials-binance");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //CashFlow-Learning-Beginners-materials-basic-education
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-be")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"cashflow-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop_CashFlow");
                                    anim->type = InputMedia::TYPE::PHOTO;
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //CashFlow-Learning-Beginners-materials-practical-training
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-pt")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"cashflow-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop_CashFlow");
                                    anim->type = InputMedia::TYPE::PHOTO;
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //CashFlow-Learning-Beginners-materials-answers-on-questions
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-aoq")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"cashflow-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop_CashFlow");
                                    anim->type = InputMedia::TYPE::PHOTO;
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //CashFlow-Learning-Beginners-materials-presentation
                            if (StringTools::startsWith(query->data,
                                                        "cashflow-learning-beginners-materials-presentation")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    db.getFile("cashflow-presentation"),
                                                                    "", text->at(
                                                    "cashflow-learning-beginners-materials-presentation-text"), 0, keyb,
                                                                    "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-presentation");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }


                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-materials-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("presentation"), text->at("promo-clip")},
                                         {text->at("negotiation"),  text->at("objections")},
                                         {text->at("scripts")},
                                         {text->at("useful-links")},
                                         {text->at("registration-binance")},
                                         {text->at("basic-education")},
                                         {text->at("practical-training")},
                                         {text->at("answers-on-questions")},
                                         {text->at("clear-message")}},
                                        {{"cashflow-learning-beginners-materials-presentation",
                                                 "cashflow-learning-beginners-materials-pc"},
                                         {"cashflow-learning-beginners-materials-negotiation",
                                                 "cashflow-learning-beginners-materials-objections"},
                                         {"cashflow-learning-beginners-materials-s"},
                                         {"cashflow-learning-beginners-materials-ul"},
                                         {"cashflow-learning-beginners-materials-binance"},
                                         {"cashflow-learning-beginners-materials-be"},
                                         {"cashflow-learning-beginners-materials-pt"},
                                         {"cashflow-learning-beginners-materials-aoq"},
                                         {"clear-message"}},
                                        keyb);
                                try {
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("CashFlow_learning_beginners_materials_img");
                                    anim->type = InputMedia::TYPE::PHOTO;
                                    anim->caption = text->at("CashFlow-learning-beginners-materials-text");
                                    anim->parseMode = "HTML";

                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }


                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("presentation"), text->at("promo-clip")},
                                                                     {text->at("negotiation"),  text->at("objections")},
                                                                     {text->at("scripts")},
                                                                     {text->at("useful-links")},
                                                                     {text->at("registration-binance")},
                                                                     {text->at("basic-education")},
                                                                     {text->at("practical-training")},
                                                                     {text->at("answers-on-questions")},
                                                                     {text->at("clear-message")}},
                                                                    {{"cashflow-learning-beginners-materials-presentation",
                                                                             "cashflow-learning-beginners-materials-pc"},
                                                                     {"cashflow-learning-beginners-materials-negotiation",
                                                                             "cashflow-learning-beginners-materials-objections"},
                                                                     {"cashflow-learning-beginners-materials-s"},
                                                                     {"cashflow-learning-beginners-materials-ul"},
                                                                     {"cashflow-learning-beginners-materials-binance"},
                                                                     {"cashflow-learning-beginners-materials-be"},
                                                                     {"cashflow-learning-beginners-materials-pt"},
                                                                     {"cashflow-learning-beginners-materials-aoq"},
                                                                     {"clear-message"}},
                                                                    keyb);
                            try {
                                Message::Ptr msg;
                                msg = bot.getApi().sendPhoto(query->from->id,
                                                             links->at("CashFlow_learning_beginners_materials_img"),
                                                             text->at("CashFlow-learning-beginners-materials-text"), 0,
                                                             keyb, "HTML");
                                db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                      "CashFlow-beginners-materials");
                            }
                            catch (TgException ex) {
                                cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                            }
                            return;
                        }

                        //CashFlow-Learning-beginners-start
                        if (StringTools::startsWith(query->data, "cashflow-learning-beginners-start")) {

                            //CashFlow-Learning-Beginners-start-next
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-start-next")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextCashFlow(query->from->id, 0);
                                ++cur_text;
                                //!!!MAX THEMES!!!
                                if (cur_text >= sb.getCashFlowBeginnersThemes()->size()) {
                                    db.setCurTextCashFlow(query->from->id, 0, 0);
                                    KeyboardGenerator::createInlineKeyboard({{text->at("end-learning")}},
                                                                            {{"cashflow"}},
                                                                            keyb);
                                    media->media = links->at("CashFlow_learning_beginners_end");
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    media->caption = text->at("CashFlow-learning-beginners-end");
                                } else {
                                    db.setCurTextCashFlow(query->from->id, 0, cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"cashflow-learning-beginners-start-back"},
                                             {"cashflow-learning-beginners-start-next"},
                                             {"cashflow-learning-beginners-materials"},
                                             {"cashflow"}}, keyb);
                                    string str1 = "CashFlow-beginners-themes-" + to_string(cur_text);
                                    media->media = sb.getCashFlowBeginnersThemes()->at(str1);
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    const string str = "CashFlow-learning-beginners-start-t-" + to_string(cur_text);
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

                            //CashFlow-Learning-Beginners-back
                            if (StringTools::startsWith(query->data, "cashflow-learning-beginners-start-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextCashFlow(query->from->id, 0);
                                if (cur_text <= 1) {
                                    db.setCurTextCashFlow(query->from->id, 0, 0);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"cashflow-learning-beginners-start-next"},
                                             {"cashflow-learning-beginners-materials"},
                                             {"cashflow"}}, keyb);
                                    media->media = sb.getCashFlowBeginnersThemes()->at("CashFlow-beginners-themes-0");
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    media->caption = text->at("CashFlow-learning-beginners-start-t-0");
                                } else {
                                    db.setCurTextCashFlow(query->from->id, 0, --cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"cashflow-learning-beginners-start-back"},
                                             {"cashflow-learning-beginners-start-next"},
                                             {"cashflow-learning-beginners-materials"},
                                             {"cashflow"}}, keyb);
                                    string str1 = "CashFlow-beginners-themes-" + to_string(cur_text);
                                    media->media = sb.getCashFlowBeginnersThemes()->at(str1);
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    const string str = "CashFlow-learning-beginners-start-t-" + to_string(cur_text);
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

                            int cur_text = db.getCurTextCashFlow(query->from->id, 0);
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            if (cur_text <= 0) {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"cashflow-learning-beginners-start-next"},
                                         {"cashflow-learning-beginners-materials"},
                                         {"cashflow"}}, keyb);
                                media->media = sb.getCashFlowBeginnersThemes()->at("CashFlow-beginners-themes-0");
                                media->type = InputMedia::TYPE::PHOTO;
                                media->parseMode = "HTML";
                                media->caption = text->at("CashFlow-learning-beginners-start-t-0");
                            } else if (cur_text >= sb.getCashFlowBeginnersThemes()->size()) {
                                db.setCurTextCashFlow(query->from->id, 0, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"cashflow-learning-beginners-start-next"},
                                         {"cashflow-learning-beginners-materials"},
                                         {"cashflow"}}, keyb);
                                media->media = sb.getCashFlowBeginnersThemes()->at("CashFlow-beginners-themes-0");
                                media->type = InputMedia::TYPE::PHOTO;
                                media->parseMode = "HTML";
                                media->caption = text->at("CashFlow-learning-beginners-start-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"cashflow-learning-beginners-start-back"},
                                         {"cashflow-learning-beginners-start-next"},
                                         {"cashflow-learning-beginners-materials"},
                                         {"cashflow"}}, keyb);
                                string str1 = "CashFlow-beginners-themes-" + to_string(cur_text);
                                media->media = sb.getCashFlowBeginnersThemes()->at(str1);
                                media->type = InputMedia::TYPE::PHOTO;
                                media->parseMode = "HTML";
                                const string str = "CashFlow-learning-beginners-start-t-" + to_string(cur_text);
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
                        int cur_text = db.getCurTextCashFlow(query->from->id, 0);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning")},
                                     {text->at("back-menu")}},
                                    {{"cashflow-learning-beginners-start"},
                                     {"cashflow-learning"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning")},
                                     {text->at("back-menu")}},
                                    {{"cashflow-learning-beginners-start"},
                                     {"cashflow-learning"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("CashFlow_learning_beginners_menu_img");
                            media->type = InputMedia::TYPE::PHOTO;
                            media->parseMode = "HTML";
                            media->caption = text->at("CashFlow-menu-beginners-text");
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
                                media->media = links->at("cashflow_no_access_gif");
                                media->type = InputMedia::TYPE::PHOTO;
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
                                media->media = links->at("cashflow_no_access_gif");
                                media->type = InputMedia::TYPE::PHOTO;
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
                                media->media = links->at("cashflow_no_access_gif");
                                media->type = InputMedia::TYPE::PHOTO;
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
                        media->media = links->at("CashFlow_learning_menu_img");
                        media->type = InputMedia::TYPE::PHOTO;
                        media->parseMode = "HTML";
                        media->caption = text->at("CashFlow-learning-text");
                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException &ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }
                    return;
                }

                if (StringTools::startsWith(query->data, "cashflow-testing")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"cashflow"}},
                                                                keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop_CashFlow");
                        anim->type = InputMedia::TYPE::PHOTO;
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }

                    return;
                }

                if (StringTools::startsWith(query->data, "cashflow-team-rules")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"cashflow"}},
                                                                keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop_CashFlow");
                        anim->type = InputMedia::TYPE::PHOTO;
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }

                    return;
                }

                if (StringTools::startsWith(query->data, "cashflow-events-and-training")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"cashflow"}},
                                                                keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop_CashFlow");
                        anim->type = InputMedia::TYPE::PHOTO;
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }

                    return;
                }

                if (StringTools::startsWith(query->data, "cashflow-mentors-and-focus-groups")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"cashflow"}},
                                                                keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop_CashFlow");
                        anim->type = InputMedia::TYPE::PHOTO;
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                      keyb);
                    }
                    catch (TgException ex) {
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
                    media->media = links->at("CashFlow_menu_img");
                    media->type = InputMedia::TYPE::PHOTO;
                    media->parseMode = "HTML";
                    media->caption = text->at("CashFlow-text");
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

                //GMP-materials
                if (StringTools::startsWith(query->data, "GMP-materials")) {
                    goto gmp_materials;
                    return;
                }

                //GMP-learning-menu
                if (StringTools::startsWith(query->data, "GMP-learning")) {

                    //GMP-Learning-forBeginners MENU=3
                    if (StringTools::startsWith(query->data, "GMP-learning-beginners")) {

                        //GMP-Learning-Beginners-materials
                        if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials")) {
                            gmp_materials:

                            //GMP-Learning-Beginners-materials-promo-clip
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-pc")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;
                                    msg = bot.getApi().sendVideo(query->from->id, db.getFile("promo-GMP"), true, 0,
                                                                 1920,
                                                                 1080, "",
                                                                 text->at("GMP-learning-beginners-materials-pc-text"),
                                                                 0, keyb, "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-promo-clip");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }


                                return;
                            }

                            //GMP-Learning-Beginners-materials-negotiation
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-negotiation")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                vector<InlineKeyboardButton::Ptr> row0;
                                vector<InlineKeyboardButton::Ptr> row1;
                                vector<InlineKeyboardButton::Ptr> row2;
                                vector<InlineKeyboardButton::Ptr> row3;
                                InlineKeyboardButton::Ptr checkButton0(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton1(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton2(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton3(new InlineKeyboardButton);
                                checkButton0->url = links->at("GMP-learning-beginners-materials-negotiation-l-1");
                                checkButton0->text = text->at("GMP-learning-beginners-materials-negotiation-l-1");
                                checkButton1->url = links->at("GMP-learning-beginners-materials-negotiation-l-2");
                                checkButton1->text = text->at("GMP-learning-beginners-materials-negotiation-l-2");
                                checkButton2->url = links->at("GMP-learning-beginners-materials-negotiation-l-3");
                                checkButton2->text = text->at("GMP-learning-beginners-materials-negotiation-l-3");
                                checkButton3->text = text->at("back");
                                checkButton3->callbackData = "GMP-learning-beginners-materials-back";
                                row0.push_back(checkButton0);
                                row1.push_back(checkButton1);
                                row2.push_back(checkButton2);
                                row3.push_back(checkButton3);
                                keyb->inlineKeyboard.push_back(row0);
                                keyb->inlineKeyboard.push_back(row1);
                                keyb->inlineKeyboard.push_back(row2);
                                keyb->inlineKeyboard.push_back(row3);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().editMessageCaption(query->from->id, query->message->messageId,
                                                                          text->at(
                                                                                  "GMP-learning-beginners-materials-negotiation-text"),
                                                                          "", keyb);

                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //GMP-Learning-Beginners-materials-objections
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-objections")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    db.getFile("GMP-objections"),
                                                                    "", text->at(
                                                    "GMP-learning-beginners-materials-objections-text"), 0, keyb,
                                                                    "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-objections");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }


                            //GMP-Learning-Beginners-materials-registration-promo-clip
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-promoreg")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;
                                    msg = bot.getApi().sendVideo(query->from->id, db.getFile("GMP-registration"), true, 0,
                                                                 720,
                                                                 1280, "",
                                                                 text->at("GMP-learning-beginners-materials-promoreg-text"),
                                                                 0, keyb, "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-promoreg");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }


                                return;
                            }

                            //GMP-Learning-Beginners-materials-scripts
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-s")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("scripts-1")},
                                                                         {text->at("scripts-2")},
                                                                         {text->at("back")}},
                                                                        {{"GMP-learning-beginners-materials-s-1"},
                                                                         {"GMP-learning-beginners-materials-s-2"},
                                                                         {"GMP-learning-beginners-materials-back"}},
                                                                        keyb);

                                //GMP-Learning-Beginners-materials-scripts-1
                                if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-s-1")) {
                                    try {
                                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                        media->media = links->at("GMP-learning-beginners-materials-s-1");
                                        media->caption = text->at("scripts-1-text");
                                        media->parseMode = "HTML";
                                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId,
                                                                      "", keyb);
                                    }
                                    catch (TgException ex) {
                                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                    }
                                    return;
                                }

                                //GMP-Learning-Beginners-materials-scripts-2
                                if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-s-2")) {
                                    try {
                                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                        media->media = links->at("GMP-learning-beginners-materials-s-2");
                                        media->caption = text->at("scripts-2-text");
                                        media->parseMode = "HTML";
                                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId,
                                                                      "", keyb);
                                    }
                                    catch (TgException ex) {
                                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                    }
                                    return;
                                }

                                try {
                                    bot.getApi().editMessageCaption(query->from->id, query->message->messageId,
                                                                    text->at(
                                                                            "GMP-learning-beginners-materials-s-text"),
                                                                    "", keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //GMP-Learning-Beginners-materials-useful
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-us")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("GMP-useful-1")},
                                                                         {text->at("GMP-useful-2")},
                                                                         {text->at("back")}},
                                                                        {{"GMP-learning-beginners-materials-us-1"},
                                                                         {"GMP-learning-beginners-materials-us-2"},
                                                                         {"GMP-learning-beginners-materials-back"}},
                                                                        keyb);

                                //GMP-Learning-Beginners-materials-useful-1
                                if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-us-1")) {
                                    try {
                                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                        media->media = links->at("GMP-learning-beginners-materials-us-1");
                                        media->caption = text->at("GMP-learning-beginners-materials-us-1-text");
                                        media->parseMode = "HTML";
                                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId,
                                                                      "", keyb);
                                    }
                                    catch (TgException ex) {
                                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                    }
                                    return;
                                }

                                //GMP-Learning-Beginners-materials-useful-2
                                if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-us-2")) {
                                    try {
                                        InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                        media->media = links->at("GMP-learning-beginners-materials-us-2");
                                        media->caption = text->at("GMP-learning-beginners-materials-us-2-text");
                                        media->parseMode = "HTML";
                                        bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId,
                                                                      "", keyb);
                                    }
                                    catch (TgException ex) {
                                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                    }
                                    return;
                                }

                                try {
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->parseMode = "HTML";
                                    anim->caption = text->at("GMP-learning-beginners-materials-us-text");
                                    anim->media = links->at("GMP-learning-beginners-materials-us");
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }


                            //GMP-Learning-Beginners-materials-useful-links
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-ul")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                vector<InlineKeyboardButton::Ptr> row0;
                                vector<InlineKeyboardButton::Ptr> row1;
                                vector<InlineKeyboardButton::Ptr> row2;
                                vector<InlineKeyboardButton::Ptr> row3;
                                InlineKeyboardButton::Ptr checkButton0(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton1(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton2(new InlineKeyboardButton);
                                InlineKeyboardButton::Ptr checkButton3(new InlineKeyboardButton);
                                checkButton0->url = links->at("GMP-learning-beginners-materials-ul-1");
                                checkButton0->text = text->at("GMP-learning-beginners-materials-ul-1");
                                checkButton1->url = links->at("GMP-learning-beginners-materials-ul-2");
                                checkButton1->text = text->at("GMP-learning-beginners-materials-ul-2");
//                                checkButton2->url = links->at("GMP-learning-beginners-materials-negotiation-l-3");
//                                checkButton2->text = text->at("GMP-learning-beginners-materials-negotiation-l-3");
                                checkButton3->text = text->at("back");
                                checkButton3->callbackData = "GMP-learning-beginners-materials-back";
                                row0.push_back(checkButton0);
                                row1.push_back(checkButton1);
//                                row2.push_back(checkButton2);
                                row3.push_back(checkButton3);
                                keyb->inlineKeyboard.push_back(row0);
                                keyb->inlineKeyboard.push_back(row1);
                                //keyb->inlineKeyboard.push_back(row2);
                                keyb->inlineKeyboard.push_back(row3);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().editMessageCaption(query->from->id, query->message->messageId,
                                                                          text->at(
                                                                                  "GMP-learning-beginners-materials-ul-text"),
                                                                          "", keyb);

                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }

                                return;
                            }

                            //GMP-learning-beginners-materials-money
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-money")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    db.getFile("GMP-money"),
                                                                    "", text->at(
                                                    "GMP-learning-beginners-materials-money-text"), 0, keyb,
                                                                    "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-money");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }


                            //GMP-learning-beginners-materials-binance
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-binance")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    db.getFile("binance"),
                                                                    "", text->at(
                                                    "GMP-learning-beginners-materials-binance-text"), 0, keyb,
                                                                    "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-binance");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //GMP-Learning-Beginners-materials-basic-education
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-be")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"GMP-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop");
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //GMP-Learning-Beginners-materials-practical-training
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-pt")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"GMP-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop");
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //GMP-Learning-Beginners-materials-answers-on-questions
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-aoq")) {
                                try {
                                    InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                            {{"GMP-learning-beginners-materials-back"}},
                                                                            keyb);
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("in_develop");
                                    anim->caption = text->at("in-develop");
                                    anim->parseMode = "HTML";
                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }

                            //GMP-Learning-Beginners-materials-presentation
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-presentation")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard({{text->at("clear-message")}},
                                                                        {{"clear-message"}}, keyb);
                                try {
                                    Message::Ptr msg;

                                    msg = bot.getApi().sendDocument(query->from->id,
                                                                    db.getFile("GMP-PARD-presentation"),
                                                                    "", text->at(
                                                    "GMP-learning-beginners-materials-presentation-text"), 0, keyb,
                                                                    "HTML");

                                    db.addMessageWithText(query->from->id, msg->messageId, 1,
                                                          "GMP-beginners-materials-presentation");
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }


                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-materials-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("presentation"), text->at("promo-clip")},
                                         {text->at("negotiation"),  text->at("objections")},
                                         {text->at("promo-registration")},
                                         {text->at("scripts")},
                                         {text->at("GMP-useful")},
                                         {text->at("useful-links")},
                                         {text->at("GMP-money")},
                                         {text->at("registration-binance")},
                                         {text->at("basic-education")},
                                         {text->at("practical-training")},
                                         {text->at("answers-on-questions")},
                                         {text->at("clear-message")}},
                                        {{"GMP-learning-beginners-materials-presentation",
                                                 "GMP-learning-beginners-materials-pc"},
                                         {"GMP-learning-beginners-materials-negotiation",
                                                 "GMP-learning-beginners-materials-objections"},
                                         {"GMP-learning-beginners-materials-promoreg"},
                                         {"GMP-learning-beginners-materials-s"},
                                         {"GMP-learning-beginners-materials-us"},
                                         {"GMP-learning-beginners-materials-ul"},
                                         {"GMP-learning-beginners-materials-money"},
                                         {"GMP-learning-beginners-materials-binance"},
                                         {"GMP-learning-beginners-materials-be"},
                                         {"GMP-learning-beginners-materials-pt"},
                                         {"GMP-learning-beginners-materials-aoq"},
                                         {"clear-message"}},
                                        keyb);
                                try {
                                    InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                                    anim->media = links->at("GMP_learning_beginners_materials_gif");
                                    anim->caption = text->at("GMP-learning-beginners-materials-text");
                                    anim->parseMode = "HTML";

                                    bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "",
                                                                  keyb);
                                }
                                catch (TgException ex) {
                                    cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                                }
                                return;
                            }


                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            KeyboardGenerator::createInlineKeyboard({{text->at("presentation"), text->at("promo-clip")},
                                                                     {text->at("negotiation"),  text->at("objections")},
                                                                     {text->at("promo-registration")},
                                                                     {text->at("scripts")},
                                                                     {text->at("GMP-useful")},
                                                                     {text->at("useful-links")},
                                                                     {text->at("GMP-money")},
                                                                     {text->at("registration-binance")},
                                                                     {text->at("basic-education")},
                                                                     {text->at("practical-training")},
                                                                     {text->at("answers-on-questions")},
                                                                     {text->at("clear-message")}},
                                                                    {{"GMP-learning-beginners-materials-presentation",
                                                                             "GMP-learning-beginners-materials-pc"},
                                                                     {"GMP-learning-beginners-materials-negotiation",
                                                                             "GMP-learning-beginners-materials-objections"},
                                                                     {"GMP-learning-beginners-materials-promoreg"},
                                                                     {"GMP-learning-beginners-materials-s"},
                                                                     {"GMP-learning-beginners-materials-us"},
                                                                     {"GMP-learning-beginners-materials-ul"},
                                                                     {"GMP-learning-beginners-materials-money"},
                                                                     {"GMP-learning-beginners-materials-binance"},
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
                                             {text->at("GMP-beginners-registration")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-start-back"},
                                             {"GMP-learning-beginners-start-next"},
                                             {"GMP-learning-beginners-registration"},
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
                                             {text->at("GMP-beginners-registration")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-start-next"},
                                             {"GMP-learning-beginners-registration"},
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
                                             {text->at("GMP-beginners-registration")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-start-back"},
                                             {"GMP-learning-beginners-start-next"},
                                             {"GMP-learning-beginners-registration"},
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
                                         {text->at("GMP-beginners-registration")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-start-next"},
                                         {"GMP-learning-beginners-registration"},
                                         {"GMP-learning-beginners-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGMP_beginners_themes()->at("GMP-beginners-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-beginners-start-t-0");
                            } else if (cur_text >= sb.getGMP_beginners_themes()->size()) {
                                db.setCurTextGMP(query->from->id, 0, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("GMP-beginners-registration")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-start-next"},
                                         {"GMP-learning-beginners-registration"},
                                         {"GMP-learning-beginners-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGMP_beginners_themes()->at("GMP-beginners-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-beginners-start-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("GMP-beginners-registration")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-start-back"},
                                         {"GMP-learning-beginners-start-next"},
                                         {"GMP-learning-beginners-registration"},
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


                        //GMP-Learning-beginners-registration
                        if (StringTools::startsWith(query->data, "GMP-learning-beginners-registration")) {

                            //GMP-Learning-Beginners-registration-next
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-registration-next")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurGMPRegistration(query->from->id);
                                ++cur_text;
                                //!!!MAX THEMES!!!
                                if (cur_text >= sb.getGmpBeginnersRegistrations()->size()-1) {
                                    KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}}, {{"GMP-learning-beginners-start"}},
                                                                            keyb);
                                    string str1 = "GMP-beginners-registration-" + to_string(cur_text);
                                    media->media = sb.getGmpBeginnersRegistrations()->at(str1);
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    const string str = "GMP-beginners-registration-t-" + to_string(cur_text);
                                    media->caption = text->at(str);
                                    db.setCurGMPRegistration(query->from->id, 0);
                                } else {
                                    db.setCurGMPRegistration(query->from->id, cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-registration-back"},
                                             {"GMP-learning-beginners-registration-next"},
                                             {"GMP-learning-beginners-start"}}, keyb);
                                    string str1 = "GMP-beginners-registration-" + to_string(cur_text);
                                    media->media = sb.getGmpBeginnersRegistrations()->at(str1);
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    const string str = "GMP-beginners-registration-t-" + to_string(cur_text);
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

                            //GMP-Learning-Beginners-registration-next
                            if (StringTools::startsWith(query->data, "GMP-learning-beginners-registration-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurGMPRegistration(query->from->id);
                                if (cur_text <= 1) {
                                    db.setCurGMPRegistration(query->from->id,0);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("next")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-registration-next"},
                                             {"GMP-learning-beginners-start"}}, keyb);
                                    media->media = sb.getGmpBeginnersRegistrations()->at("GMP-beginners-registration-0");
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-beginners-registration-t-0");
                                } else {
                                    db.setCurGMPRegistration(query->from->id,--cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-beginners-registration-back"},
                                             {"GMP-learning-beginners-registration-next"},
                                             {"GMP-learning-beginners-start"}}, keyb);
                                    string str1 = "GMP-beginners-registration-" + to_string(cur_text);
                                    media->media = sb.getGmpBeginnersRegistrations()->at(str1);
                                    media->type = InputMedia::TYPE::PHOTO;
                                    media->parseMode = "HTML";
                                    const string str = "GMP-beginners-registration-t-" + to_string(cur_text);
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

                            int cur_text = db.getCurGMPRegistration(query->from->id);
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            if (cur_text <= 0) {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-registration-next"},
                                         {"GMP-learning-beginners-start"}}, keyb);
                                media->media = sb.getGmpBeginnersRegistrations()->at("GMP-beginners-registration-0");
                                media->type = InputMedia::TYPE::PHOTO;
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-beginners-registration-t-0");
                            } else if (cur_text >= sb.getGmpBeginnersRegistrations()->size()) {
                                db.setCurGMPRegistration(query->from->id, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-registration-next"},
                                         {"GMP-learning-beginners-start"}}, keyb);
                                media->media = sb.getGmpBeginnersRegistrations()->at("GMP-beginners-registration-0");
                                media->type = InputMedia::TYPE::PHOTO;
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-beginners-registration-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-beginners-registration-back"},
                                         {"GMP-learning-beginners-registration-next"},
                                         {"GMP-learning-beginners-start"}}, keyb);
                                string str1 = "GMP-beginners-registration-" + to_string(cur_text);
                                media->media = sb.getGmpBeginnersRegistrations()->at(str1);
                                media->type = InputMedia::TYPE::PHOTO;
                                media->parseMode = "HTML";
                                const string str = "GMP-beginners-registration-t-" + to_string(cur_text);
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
                                    {{text->at("start learning")},
                                     {text->at("back-menu")}},
                                    {{"GMP-learning-beginners-start"},
                                     {"GMP-learning"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning")},
                                     {text->at("back-menu")}},
                                    {{"GMP-learning-beginners-start"},
                                     {"GMP-learning"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("GMP_learning_beginners_menu_gif");
                            media->type = InputMedia::TYPE::PHOTO;
                            media->parseMode = "HTML";
                            media->caption = text->at("text-GMP-menu-beginners");
                            bot.getApi().editMessageMedia(media, query->from->id, query->message->messageId, "", keyb);
                        }
                        catch (TgException &ex) {
                            cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                        }
                        return;
                    }





                    //GMP-Learning-forTeachers MENU=3
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

                        //GMP-learning-teachers-materials
                        if (StringTools::startsWith(query->data, "GMP-learning-teachers-materials")) {
                            return;
                        }

                        //GMP-Learning-teachers-start
                        if (StringTools::startsWith(query->data, "GMP-learning-teachers-start")) {

                            //GMP-Learning-teachers-start-next
                            if (StringTools::startsWith(query->data, "GMP-learning-teachers-start-next")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 1);
                                ++cur_text;
                                //!!!MAX THEMES!!!
                                if (cur_text >= sb.getGmpTeachersThemes()->size()) {
                                    db.setCurTextGMP(query->from->id, 1, 0);
                                    KeyboardGenerator::createInlineKeyboard({{text->at("end-learning")}}, {{"GMP"}},
                                                                            keyb);
                                    media->media = links->at("GMP_learning_teachers_end");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-teachers-end");
                                } else {
                                    db.setCurTextGMP(query->from->id, 1, cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-teachers-start-back"},
                                             {"GMP-learning-teachers-start-next"},
                                             {"GMP-learning-teachers-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-teachers-themes-" + to_string(cur_text);
                                    media->media = sb.getGmpTeachersThemes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-teachers-start-t-" + to_string(cur_text);
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

                            //GMP-Learning-teachers-back
                            if (StringTools::startsWith(query->data, "GMP-learning-teachers-start-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 1);
                                if (cur_text <= 1) {
                                    db.setCurTextGMP(query->from->id, 1, 0);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-teachers-start-next"},
                                             {"GMP-learning-teachers-materials"},
                                             {"GMP"}}, keyb);
                                    media->media = sb.getGmpTeachersThemes()->at("GMP-teachers-themes-0");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-teachers-start-t-0");
                                } else {
                                    db.setCurTextGMP(query->from->id, 1, --cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-teachers-start-back"},
                                             {"GMP-learning-teachers-start-next"},
                                             {"GMP-learning-teachers-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-teachers-themes-" + to_string(cur_text);
                                    media->media = sb.getGmpTeachersThemes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-teachers-start-t-" + to_string(cur_text);
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


                            int cur_text = db.getCurTextGMP(query->from->id, 1);
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            if (cur_text <= 0) {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-teachers-start-next"},
                                         {"GMP-learning-teachers-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGmpTeachersThemes()->at("GMP-teachers-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-teachers-start-t-0");
                            } else if (cur_text >= sb.getGmpTeachersThemes()->size()) {
                                db.setCurTextGMP(query->from->id, 1, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-teachers-start-next"},
                                         {"GMP-learning-teachers-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGmpTeachersThemes()->at("GMP-teachers-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-teachers-start-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-teachers-start-back"},
                                         {"GMP-learning-teachers-start-next"},
                                         {"GMP-learning-teachers-materials"},
                                         {"GMP"}}, keyb);
                                string str1 = "GMP-teachers-themes-" + to_string(cur_text);
                                media->media = sb.getGmpTeachersThemes()->at(str1);
                                media->parseMode = "HTML";
                                const string str = "GMP-learning-teachers-start-t-" + to_string(cur_text);
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
                        //-----------
                        int cur_text = db.getCurTextGMP(query->from->id, 1);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning")},
                                     {text->at("back-menu")},
                                     {text->at("checklist for mentors")}
                                    },
                                    {{"GMP-learning-teachers-start"},
                                     {"GMP-learning"},
                                     {"GMP-learning-teachers-checklist-mentors"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning")},
                                     {text->at("back-menu")},
                                     {text->at("checklist for mentors")}
                                    },
                                    {{"GMP-learning-teachers-start"},
                                     {"GMP-learning"},
                                     {"GMP-learning-teachers-checklist-mentors"}
                                    }, keyb);
                        }
                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("GMP_learning_teachers_menu_gif");
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

                        //GMP-learning-leaders-start
                        if (StringTools::startsWith(query->data, "GMP-learning-leaders-start")) {

                            //GMP-Learning-leaders-start-next
                            if (StringTools::startsWith(query->data, "GMP-learning-leaders-start-next")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 2);
                                ++cur_text;
                                //!!!MAX THEMES!!!
                                if (cur_text >= sb.getGmpLeadersThemes()->size()) {
                                    db.setCurTextGMP(query->from->id, 2, 0);
                                    KeyboardGenerator::createInlineKeyboard({{text->at("end-learning")}}, {{"GMP"}},
                                                                            keyb);
                                    media->media = links->at("GMP_learning_leaders_end");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-leaders-end");
                                } else {
                                    db.setCurTextGMP(query->from->id, 2, cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-leaders-start-back"},
                                             {"GMP-learning-leaders-start-next"},
                                             {"GMP-learning-leaders-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-leaders-themes-" + to_string(cur_text);
                                    media->media = sb.getGmpLeadersThemes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-leaders-start-t-" + to_string(cur_text);
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

                            //GMP-Learning-leaders-back
                            if (StringTools::startsWith(query->data, "GMP-learning-leaders-start-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 2);
                                if (cur_text <= 1) {
                                    db.setCurTextGMP(query->from->id, 2, 0);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-leaders-start-next"},
                                             {"GMP-learning-leaders-materials"},
                                             {"GMP"}}, keyb);
                                    media->media = sb.getGmpLeadersThemes()->at("GMP-leaders-themes-0");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-leaders-start-t-0");
                                } else {
                                    db.setCurTextGMP(query->from->id, 2, --cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-leaders-start-back"},
                                             {"GMP-learning-leaders-start-next"},
                                             {"GMP-learning-leaders-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-leaders-themes-" + to_string(cur_text);
                                    media->media = sb.getGmpLeadersThemes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-leaders-start-t-" + to_string(cur_text);
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


                            int cur_text = db.getCurTextGMP(query->from->id, 2);
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            if (cur_text <= 0) {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-leaders-start-next"},
                                         {"GMP-learning-leaders-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGmpLeadersThemes()->at("GMP-leaders-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-leaders-start-t-0");
                            } else if (cur_text >= sb.getGmpLeadersThemes()->size()) {
                                db.setCurTextGMP(query->from->id, 2, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-leaders-start-next"},
                                         {"GMP-learning-leaders-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGmpLeadersThemes()->at("GMP-leaders-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-leaders-start-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-leaders-start-back"},
                                         {"GMP-learning-leaders-start-next"},
                                         {"GMP-learning-leaders-materials"},
                                         {"GMP"}}, keyb);
                                string str1 = "GMP-leaders-themes-" + to_string(cur_text);
                                media->media = sb.getGmpLeadersThemes()->at(str1);
                                media->parseMode = "HTML";
                                const string str = "GMP-learning-leaders-start-t-" + to_string(cur_text);
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

                        int cur_text = db.getCurTextGMP(query->from->id, 2);
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        if (cur_text == 0) {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("start learning")},
                                     {text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-leaders-start"},
                                     {"GMP-learning"},
                                     {"GMP-learning-leaders-materials"}
                                    }, keyb);
                        } else {
                            KeyboardGenerator::createInlineKeyboard(
                                    {{text->at("continue-learning")},
                                     {text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-leaders-start"},
                                     {"GMP-learning"},
                                     {"GMP-learning-leaders-materials"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("GMP_learning_leaders_menu_gif");
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

                        //GMP-learning-TOP-leaders-start
                        if (StringTools::startsWith(query->data, "GMP-learning-TOP-leaders-start")) {

                            //GMP-Learning-teachers-start-next
                            if (StringTools::startsWith(query->data, "GMP-learning-TOP-leaders-start-next")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 3);
                                ++cur_text;
                                //!!!MAX THEMES!!!
                                if (cur_text >= sb.getGmpTopLeadersThemes()->size()) {
                                    db.setCurTextGMP(query->from->id, 3, 0);
                                    KeyboardGenerator::createInlineKeyboard({{text->at("end-learning")}}, {{"GMP"}},
                                                                            keyb);
                                    media->media = links->at("GMP_learning_TOP_leaders_end");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-TOP-leaders-end");
                                } else {
                                    db.setCurTextGMP(query->from->id, 3, cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-TOP-leaders-start-back"},
                                             {"GMP-learning-TOP-leaders-start-next"},
                                             {"GMP-learning-TOP-leaders-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-TOP-leaders-themes-" + to_string(cur_text);
                                    media->media = sb.getGmpTopLeadersThemes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-TOP-leaders-start-t-" + to_string(cur_text);
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

                            //GMP-Learning-TOP-leaders-back
                            if (StringTools::startsWith(query->data, "GMP-learning-TOP-leaders-start-back")) {
                                InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                                InputMediaAnimation::Ptr media(new InputMediaAnimation());
                                int cur_text = db.getCurTextGMP(query->from->id, 3);
                                if (cur_text <= 1) {
                                    db.setCurTextGMP(query->from->id, 3, 0);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-TOP-leaders-start-next"},
                                             {"GMP-learning-TOP-leaders-materials"},
                                             {"GMP"}}, keyb);
                                    media->media = sb.getGmpTopLeadersThemes()->at("GMP-TOP-leaders-themes-0");
                                    media->parseMode = "HTML";
                                    media->caption = text->at("GMP-learning-TOP-leaders-start-t-0");
                                } else {
                                    db.setCurTextGMP(query->from->id, 3, --cur_text);
                                    KeyboardGenerator::createInlineKeyboard(
                                            {{text->at("back")},
                                             {text->at("next")},
                                             {text->at("materials")},
                                             {text->at("back-menu")}},
                                            {{"GMP-learning-TOP-leaders-start-back"},
                                             {"GMP-learning-TOP-leaders-start-next"},
                                             {"GMP-learning-TOP-leaders-materials"},
                                             {"GMP"}}, keyb);
                                    string str1 = "GMP-TOP-leaders-themes-" + to_string(cur_text);
                                    media->media = sb.getGmpTopLeadersThemes()->at(str1);
                                    media->parseMode = "HTML";
                                    const string str = "GMP-learning-TOP-leaders-start-t-" + to_string(cur_text);
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


                            int cur_text = db.getCurTextGMP(query->from->id, 3);
                            InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            if (cur_text <= 0) {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-TOP-leaders-start-next"},
                                         {"GMP-learning-TOP-leaders-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGmpTopLeadersThemes()->at("GMP-TOP-leaders-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-TOP-leaders-start-t-0");
                            } else if (cur_text >= sb.getGmpTopLeadersThemes()->size()) {
                                db.setCurTextGMP(query->from->id, 3, 0);
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-TOP-leaders-start-next"},
                                         {"GMP-learning-TOP-leaders-materials"},
                                         {"GMP"}}, keyb);
                                media->media = sb.getGmpTopLeadersThemes()->at("GMP-TOP-leaders-themes-0");
                                media->parseMode = "HTML";
                                media->caption = text->at("GMP-learning-TOP-leaders-start-t-0");
                            } else {
                                KeyboardGenerator::createInlineKeyboard(
                                        {{text->at("back")},
                                         {text->at("next")},
                                         {text->at("materials")},
                                         {text->at("back-menu")}},
                                        {{"GMP-learning-TOP-leaders-start-back"},
                                         {"GMP-learning-TOP-leaders-start-next"},
                                         {"GMP-learning-TOP-leaders-materials"},
                                         {"GMP"}}, keyb);
                                string str1 = "GMP-TOP-leaders-themes-" + to_string(cur_text);
                                media->media = sb.getGmpTopLeadersThemes()->at(str1);
                                media->parseMode = "HTML";
                                const string str = "GMP-learning-TOP-leaders-start-t-" + to_string(cur_text);
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
                                    {{text->at("continue-learning")},
                                     {text->at("back-menu")},
                                     {text->at("materials")}
                                    },
                                    {{"GMP-learning-TOP-leaders-start"},
                                     {"GMP-learning"},
                                     {"GMP-learning-TOP-leaders-materials"}
                                    }, keyb);
                        }

                        try {
                            InputMediaAnimation::Ptr media(new InputMediaAnimation());
                            media->media = links->at("GMP_learning_TOPleaders_menu_gif");
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
                        media->media = links->at("GMP_learning_menu_gif");
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

                if (StringTools::startsWith(query->data, "GMP-testing")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"GMP"}}, keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop");
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }

                    return;
                }

                if (StringTools::startsWith(query->data, "GMP-team-rules")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"GMP"}}, keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop");
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }

                    return;
                }

                if (StringTools::startsWith(query->data, "GMP-events-and-training")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"GMP"}}, keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop");
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException ex) {
                        cerr << ex.what() << "\n\tline: " << __LINE__ << endl;
                    }

                    return;
                }

                if (StringTools::startsWith(query->data, "GMP-mentors-and-focus-groups")) {
                    try {
                        InlineKeyboardMarkup::Ptr keyb(new InlineKeyboardMarkup());
                        KeyboardGenerator::createInlineKeyboard({{text->at("back-menu")}},
                                                                {{"GMP"}}, keyb);
                        InputMediaAnimation::Ptr anim(new InputMediaAnimation());
                        anim->media = links->at("in_develop");
                        anim->caption = text->at("in-develop");
                        anim->parseMode = "HTML";
                        bot.getApi().editMessageMedia(anim, query->from->id, query->message->messageId, "", keyb);
                    }
                    catch (TgException ex) {
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
                    media->type = InputMedia::TYPE::PHOTO;
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
