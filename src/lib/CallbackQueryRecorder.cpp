//
// Created by root on 6/29/22.
//

#include <thread>
#include "CallbackQueryRecorder.h"
#include "my_lib.h"
#include "KeyboardGenerator.h"
#include "../driver/strings.h"
//TODO: Возможно перейти на стейтмашину, чтобы не париться за коллбеки.


CallbackQueryRecorder::CallbackQueryRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb),
                                                                                                  languageInlineKeyboard(
                                                                                                          new InlineKeyboardMarkup),
                                                                                                  beginnersStartInlineKeyboard(
                                                                                                          new InlineKeyboardMarkup),
                                                                                                  NextBackInlineKeyboard(
                                                                                                          new InlineKeyboardMarkup),
                                                                                                  NextMenuInlineKeyboard(
                                                                                                          new InlineKeyboardMarkup),
                                                                                                  inlineGMP(
                                                                                                          new InlineKeyboardMarkup), media(new InputMediaPhoto) {


    bot.getEvents().onCallbackQuery([&bot, &db, &sb, this](CallbackQuery::Ptr query) {
        thread t1([&bot, &db, &sb, query, this]() {
            unordered_map<string, string> *text = my::get_lang(db, sb, query);
            int cur_menu = db.getCurMenu(query->from->id);
            //language
            if (StringTools::startsWith(query->data, text->at("UK"))) {
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("UK"), text->at("RU")}
                                                        }, languageInlineKeyboard);
                db.setLanguage(query->from->id, 1);
                text = my::get_lang(db, sb, query);
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                try {
                    bot.getApi().editMessageText(text->at("start-successful"), query->from->id,
                                                 msg,
                                                 "", "", false, languageInlineKeyboard);
                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                delete (l_msg);
            }
            //language
            if (StringTools::startsWith(query->data, text->at("RU"))) {
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("UK"), text->at("RU")}
                                                        }, languageInlineKeyboard);
                db.setLanguage(query->from->id, 0);
                text = my::get_lang(db, sb, query);
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                try {
                    bot.getApi().editMessageText(text->at("start-successful"), query->from->id,
                                                 msg,
                                                 "", "", false, languageInlineKeyboard);
                }
                catch (TgException ex) {
                    cerr << ex.what() << endl;
                }
                delete (l_msg);
            }
            //GMP MENU = 1
            if (StringTools::startsWith(query->data, text->at("GMP"))) {
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("training for beginners"), text->at(
                                                                        "training for teachers")},
                                                                {text->at("training for leaders"),   text->at(
                                                                        "training for TOP-leaders")},
                                                                {text->at("materials"),              text->at(
                                                                        "testing")},
                                                                {text->at("team rules"),             text->at(
                                                                        "events and training")},
                                                                {text->at("mentors and focus groups")}
                                                        }, inlineGMP);
                db.setCurMenu(query->from->id, 1);
                media->media = GMP_menu_img;
                media->caption = text->at("text-GMP");
                auto *l_msg = db.getMessages(query->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t msg = l_msg->back();
                bot.getApi().editMessageMedia(media, query->from->id, msg, "", inlineGMP);
                delete(l_msg);
            }

            //training for beginners menu  MENU=2
            if (StringTools::startsWith(query->data, text->at("training for beginners")) && cur_menu == 1) {
                //string postfix = "beginners";
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
            }
            //start learning
            if (StringTools::startsWith(query->data, text->at("start learning")) &&
                db.getCurMenu(query->from->id) == 2) {
            }
            if (StringTools::startsWith(query->data, text->at("back-menu")) && db.getCurMenu(query->from->id) == 2) {
                db.setCurMenu(query->from->id, 1);
                KeyboardGenerator::createInlineKeyboard({
                                                                {text->at("training for beginners"), text->at(
                                                                        "training for teachers")},
                                                                {text->at("training for leaders"),   text->at(
                                                                        "training for TOP-leaders")},
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
            }
        });
        t1.detach();
    });

}
