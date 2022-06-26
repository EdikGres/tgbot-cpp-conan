#include <stdlib.h>
#include <iostream>
#include <vector>
#include <tgbot/tgbot.h>
#include "lib/CommandRecorder.h"
#include <mysql.h>
#include "lib/DBHandler.h"
#include "driver/StringParser.h"

#include <iostream>

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

    string filename("../locale/ru.lang");
    StringParser sp(filename);

    sp.parseFile();


    DBHandler db("62.122.213.42", "root", getenv("MYSQL_PASS"),
                 "telegram", 3306, NULL, 0);





    exit(15);

    Bot bot(token);

    CommandRecorder recorder(bot, db);


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
