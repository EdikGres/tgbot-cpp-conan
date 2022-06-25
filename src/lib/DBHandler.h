//
// Created by root on 25.06.22.
//

#ifndef KOSTYA_BOT_DBHANDLER_H
#define KOSTYA_BOT_DBHANDLER_H

#include <mysql.h>
#include <string>

using namespace std;

class DBHandler {
private:
    MYSQL *con;
    const char *host;
    const char *user;
    const char *passwd;
    const char *db;
    unsigned int port;
    const char *unix_socket;
    unsigned long clientflag;

    void error_finish();

    void error_handle();

public:
    DBHandler(const char *host, const char *user, const char *passwd, const char *db, unsigned int port,
              const char *unixSocket, unsigned long clientflag);

    int add_user(int64_t tg_id, const string &name, int cur_menu, int access_level);

    bool hasUser(int64_t tg_id);

    bool hasUser(const string &name);

    int getCurMenu(int64_t tg_id);

    int getAccessLevel(int64_t tg_id);

    void setCurMenu(int64_t tg_id, int cur_menu);

    void setAccessLevel(int64_t tg_id, int cur_level);
};


#endif //KOSTYA_BOT_DBHANDLER_H
