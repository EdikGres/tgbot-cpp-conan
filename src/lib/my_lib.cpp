//
// Created by root on 6/29/22.
//
#include "my_lib.h"

using namespace TgBot;
namespace my {
    unordered_map<string, string> *get_lang(DBHandler &db, StringBuilder &sb, const Message::Ptr &message) {
        const int lang = db.getLanguage(message->from->id);
        unordered_map<string, string> *text;
        switch (lang) {
            case 0:
                text = sb.getRu();
                break;
            case 1:
                text = sb.getEn();
                break;
            default:
                text = sb.getRu();
                break;
        }
        return text;
    }

    unordered_map<string, string> *get_lang(DBHandler &db, StringBuilder &sb, const TgBot::CallbackQuery::Ptr query) {
        const int lang = db.getLanguage(query->from->id);
        unordered_map<string, string> *text;
        switch (lang) {
            case 0:
                text = sb.getRu();
                break;
            case 1:
                text = sb.getEn();
                break;
            default:
                text = sb.getRu();
                break;
        }
        return text;
    }

    void delete_all_messages(DBHandler &db, StringBuilder &sb, const Message::Ptr &message, TgBot::Bot &bot){
        list<int32_t> *messages = db.getAllMessages(message->from->id);
        db.deleteAllMessage(message->from->id);
        if (messages != nullptr && !messages->empty()) {
            try {
                for (int32_t ms: *messages) {
                    bot.getApi().deleteMessage(message->from->id, ms);
                }
            }
            catch (TgException ex) {
                cerr << ex.what() << endl;
            }
        }
        delete (messages);
    }
}