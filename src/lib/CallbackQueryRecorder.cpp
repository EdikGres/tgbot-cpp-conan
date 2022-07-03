//
// Created by root on 6/29/22.
//

#include <thread>
#include "CallbackQueryRecorder.h"
#include "my_lib.h"
#include "KeyboardGenerator.h"
#include "../driver/strings.h"
//TODO: Возможно перейти на стейтмашину, чтобы не париться за коллбеки.


CallbackQueryRecorder::CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb) {


    bot.getEvents().onCallbackQuery([&bot, &db, &sb, this](CallbackQuery::Ptr query) {
        thread t1([&bot, &db, &sb, query, this]() {
            unordered_map<string, string> *text = my::get_lang(db, sb, query);
            int cur_menu = db.getCurMenu(query->from->id);
            //language UK
            if (StringTools::startsWith(query->data, text->at("UK"))) {
                InlineKeyboardMarkup::Ptr languageInlineKeyboard(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("UK"), text->at("RU")},
                                                                {text->at("clear-language")}
                                                        }, languageInlineKeyboard);
                db.setLanguage(query->from->id, 1);
                text = my::get_lang(db, sb, query);
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                try {
                    bot.getApi().editMessageText(text->at("language-set-successful"), query->from->id,
                                                 msg,
                                                 "", "", false, languageInlineKeyboard);
                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                delete (l_msg);
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
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                try {
                    bot.getApi().editMessageText(text->at("language-set-successful"), query->from->id,
                                                 msg,
                                                 "", "", false, languageInlineKeyboard);
                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                delete (l_msg);
                return;
            }
            //language-hide
            if(StringTools::startsWith(query->data, text->at("clear-language"))){
                try {
                    db.deleteMessage(query->from->id, query->message->messageId);
                    bot.getApi().deleteMessage(query->from->id, query->message->messageId);
                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                return;
            }



            //GMP MENU = 1
            if (StringTools::startsWith(query->data, text->at("GMP"))) {
                InlineKeyboardMarkup::Ptr inlineGMP(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("training for beginners")},
                                                                {text->at("materials"),              text->at(
                                                                        "testing")},
                                                                {text->at("team rules"),             text->at(
                                                                        "events and training")},
                                                                {text->at("mentors and focus groups")}
                                                        }, inlineGMP);
                db.setCurMenu(query->from->id, 1);
                InputMediaPhoto::Ptr media(new InputMediaPhoto());
                media->media = GMP_menu_img;
                media->caption = text->at("text-GMP");
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                bot.getApi().editMessageMedia(media, query->from->id, msg, "", inlineGMP);
                delete (l_msg);
                return;
            }
            //training for beginners menu  MENU=2
            if (StringTools::startsWith(query->data, text->at("training for beginners")) && cur_menu == 1) {
                InlineKeyboardMarkup::Ptr beginnersStartInlineKeyboard(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("start learning"), text->at("back-menu")}
                                                        }, beginnersStartInlineKeyboard);
                db.setCurMenu(query->from->id, 2);
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                try {
                    //GMP_beginners_menu
                    //bot.getApi().editMessageCaption(query->from->id, msg, text->at("text-GMP-beginners"), "", beginnersStartInlineKeyboard);
                    InputMediaPhoto::Ptr med(new InputMediaPhoto());
                    med->media = GMP_beginners_menu;
                    med->caption = text->at("text-GMP-beginners");
                    bot.getApi().editMessageMedia(med, query->from->id, msg, "", beginnersStartInlineKeyboard);

                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                delete (l_msg);
                return;
            }
            //start learning
            if (StringTools::startsWith(query->data, text->at("start learning")) &&
                db.getCurMenu(query->from->id) == 2) {

                return;
            }
            if (StringTools::startsWith(query->data, text->at("back-menu")) && db.getCurMenu(query->from->id) == 2) {
                db.setCurMenu(query->from->id, 1);
                InlineKeyboardMarkup::Ptr inlineGMP(new InlineKeyboardMarkup());
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("training for beginners")},
                                                                {text->at("materials"),              text->at(
                                                                        "testing")},
                                                                {text->at("team rules"),             text->at(
                                                                        "events and training")},
                                                                {text->at("mentors and focus groups")}
                                                        }, inlineGMP);
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                try {

//                    bot.getApi().editMessageText(text->at("text-GMP"), query->from->id, msg, "", "", false,
//                                                 inlineGMP);
                    //string str = FileTools::read("../img/cat-start.jpeg");

                    //media->media = str;
                    //media->media = "https://jpeg.org/images/jpegxl-home.jpg";
                    InputMediaPhoto::Ptr med(new InputMediaPhoto());
                    med->media = GMP_menu_img;
                    med->caption = text->at("text-GMP");
                    bot.getApi().editMessageMedia(med, query->from->id, msg, "", inlineGMP);
                    //bot.getApi().editMessageCaption(query->from->id, msg, text->at("text-GMP"), "", inlineGMP);

                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                delete (l_msg);
                return;
            }
        });
        t1.detach();
    });

}
