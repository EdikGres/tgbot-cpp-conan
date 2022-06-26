#include <stdlib.h>
#include <iostream>
#include <vector>
#include <tgbot/tgbot.h>
#include "lib/CommandRecorder.h"
#include <mysql.h>
#include "lib/DBHandler.h"
#include "driver/StringParser.h"
#include "driver/StringBuilder.h"
#include <iostream>
//TODO: mysql_store_result пофиксил утечку, нужно всё проверить 100500 раз
//TODO: Сделать удаление сообщений в боте(чата?)
//TODO: исправить пути для языков

using namespace std;
using namespace TgBot;


void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main() {
    string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());



    string files[] = {"../locale/ru.lang", "../locale/en.lang"};
    StringBuilder sb(files);


    DBHandler db("62.122.213.42", "root", getenv("MYSQL_PASS"),
                 "telegram", 3306, NULL, 0);




    Bot bot(token);

    CommandRecorder recorder(bot, db, sb);


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
