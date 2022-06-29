#include <cstdlib>
#include <iostream>
#include <vector>
#include <tgbot/tgbot.h>
#include "lib/CommandRecorder.h"
#include <mysql.h>
#include "lib/DBHandler.h"
#include "driver/StringParser.h"
#include "driver/StringBuilder.h"
#include "lib/CallbackQueryRecorder.h"
#include "lib/MessageRecorder.h"
#include "driver/strings.h"
//TODO: Сделать многопоточность для mysql
//TODO: Сделать удаление сообщений в боте(чата?)
//TODO: исправить пути для языков
//TODO: обавить таймстамп к сообщению
//TODO: Сделать mysql запрос на получение последний ид сообщения ( можно запросом WHERE max)


using namespace std;
using namespace TgBot;


void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main() {
    if (getenv("TOKEN") == NULL || getenv("MYSQL_PASS") == NULL) {
        cerr << "ERROR! environment values don't set!" << endl;
        cerr << "TOKEN: " << getenv("TOKEN") << endl;
        cerr << "MYSQL_PASS: " << getenv("MYSQL_PASS") << endl;
        return -1;
    }
    string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());


    //change for deploy
    string files[] = {ru_locale, en_locale};
    StringBuilder sb(files);


    DBHandler db("62.122.213.42", "root", getenv("MYSQL_PASS"),
                 "telegram", 3306, NULL, 0);


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
