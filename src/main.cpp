#include <cstdlib>
#include <iostream>
#include <vector>
#include <tgbot/tgbot.h>
#include "lib/CommandRecorder.h"
#include <mysql.h>
#include "driver/DBHandler.h"
#include "driver/StringParser.h"
#include "driver/StringBuilder.h"
#include "lib/CallbackQueryRecorder.h"
#include "lib/MessageRecorder.h"
#include "driver/strings.h"
//TODO: СДЕЛАТЬ ОГРАНИЧИТЕЛЬ ДО 10 СООБЩЕНИЙ В СЕКУНДУ ДЛЯ 1 ЧЕЛОВЕКА!
//TODO: Добавить таймстамп к сообщению
//TODO: Сделать mysql запрос на получение последнего ид сообщения ( можно запросом WHERE max ( но не фактЮ вдруг коллизия ид наступит ))
//TODO: Сделать защиту от спамеров ( мб бан по полю isSpammer ) (уточнить)
//TODO: !!!!!!!Сделать пул соединений для mysql && сделать пул потоков для обработки сообщений!!!!!!!

using namespace std;
using namespace TgBot;

#define KOSTYA

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(2);
}

int main() {
#ifdef KOSTYA
    if (getenv("TOKEN2") == NULL || getenv("MYSQL_PASS") == NULL) {
        cerr << "ERROR! environment values don't set!" << endl;
        cerr << "TOKEN: " << getenv("TOKEN2") << endl;
        cerr << "MYSQL_PASS: " << getenv("MYSQL_PASS") << endl;
        return -1;
    }
    string token(getenv("TOKEN2"));
    printf("Token: %s\n", token.c_str());


    DBHandler db("localhost", "root", getenv("MYSQL_PASS"),
                 "telegram", 3307, NULL, 0);

#else
    if (getenv("TOKEN") == NULL || getenv("MYSQL_PASS") == NULL) {
        cerr << "ERROR! environment values don't set!" << endl;
        cerr << "TOKEN: " << getenv("TOKEN") << endl;
        cerr << "MYSQL_PASS: " << getenv("MYSQL_PASS") << endl;
        return -1;
    }
    string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());


    DBHandler db("62.122.213.42", "root", getenv("MYSQL_PASS"),
                 "telegram", 3306, NULL, 0);

#endif




    //auto* callbacks = db.getCallbacks();

    //change for deploy
    string files[] = {ru_db, en_db};
    StringBuilder sb(files, "links", db);

    Bot bot(token);

    CommandRecorder recorder(bot, db, sb);

    CallbackQueryRecorder recorder1(bot, db, sb);

    MessageRecorder recorder2(bot, db, sb);


    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception &e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
