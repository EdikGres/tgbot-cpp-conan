//
// Created by root on 6/29/22.
//

#include "MessageRecorder.h"
#include <iostream>
#include "KeyboardGenerator.h"
#include "my_lib.h"
#include "../driver/strings.h"

using namespace std;

MessageRecorder::MessageRecorder(TgBot::Bot &bot, DBHandler &db, StringBuilder &sb) : db(db), sb(sb), inlineGMP(
        new InlineKeyboardMarkup), media(new InputMediaPhoto), keyb(new ReplyKeyboardMarkup) {
    bot.getEvents().onNonCommandMessage([&bot, &db, &sb, this](const Message::Ptr &message) {
        thread t1([&bot, &db, &sb, message, this]() {
            //GMP MENU=1
            if (StringTools::startsWith(message->text, "GMP")) {
                //db.addMessage(message->from->id, message->messageId, 0);
                unordered_map<string, string> *text = my::get_lang(db, sb, message);
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

                db.setCurMenu(message->from->id, 1);
                //Message::Ptr msg;
                //InputMediaPhoto::Ptr media(new InputMediaPhoto());
                //media->media = "https://downloader.disk.yandex.ru/preview/61241fc476ae5969542b473d30ce83e8a274f611ab9d07bcbcbb241822568b28/62bd0299/2cZVERsdGNFHqEPJTA68j38xtNzgE25P95g6hF-QW3v6g7i8FGJ8KCLYAKmqHK-kVc6_QT6Fr1ZK35cLcDOYEg%3D%3D?uid=0&filename=GMP.jpg&disposition=inline&hash=&limit=0&content_type=image%2Fjpeg&owner_uid=0&tknv=v2&size=2048x2048";
                //media->caption = "hwyerghwerg";
                media->media = "https://i.ibb.co/2NpKYJJ/GMP.jpg";
                media->caption = "test";

                keyb->resizeKeyboard = true;
                keyb->oneTimeKeyboard = false;
                KeyboardGenerator::createKeyboard({
                                                          {"123", "1414"},
                                                  }, keyb);
                auto *l_msg = db.getMessages(message->from->id, 1);
                if (l_msg == NULL)
                    return;
                int32_t last_message = l_msg->back();
                cout << last_message << endl;
                //bot.getApi().editMessageMedia(media, message->from->id, last_message, "", keyb);

                //msg = bot.getApi().sendPhoto(message->from->id, GMP_menu_img, text->at("text-GMP"), 0, inlineGMP);
                //msg2 = bot.getApi().sendMessage(message->from->id, text->at("text-GMP"), false, 0, inlineGMP);
                //bot.getApi().deleteMessage(message->from->id, message->messageId);
                //my::delete_all_messages(db, sb, message, bot);
                //db.addMessage(message->from->id, msg->messageId, 1);
                //db.addMessage(message->from->id, msg2->messageId, 1);
            }
        });
        t1.detach();
    });
}
