//
// Created by root on 25.06.22.
//
//

#ifndef KOSTYA_BOT_DBHANDLER_H
#define KOSTYA_BOT_DBHANDLER_H

#include <mysql.h>
#include <string>
#include <mutex>
#include <list>

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
    mutex m_mutex;

    void error_finish();

    void error_handle();

public:
    DBHandler(const char *host, const char *user, const char *passwd, const char *db, unsigned int port,
              const char *unixSocket, unsigned long clientflag);

    virtual ~DBHandler();

    int add_user(int64_t tg_id, const string &name, int cur_menu, int access_level, int language);

    bool hasUser(int64_t tg_id);

    bool hasUser(const string &name);

    int getCurMenu(int64_t tg_id);

    int getAccessLevel(int64_t tg_id);

    int getLanguage(int64_t tg_id);

    void setCurMenu(int64_t tg_id, int cur_menu);

    void setAccessLevel(int64_t tg_id, int cur_level);

    void setLanguage(int64_t tg_id, int cur_language);

    int addMessage(int64_t tg_id, int32_t message_id, int32_t isBot);

    int deleteMessage(int64_t tg_id, int32_t message_id);

    int deleteMessageUser(int64_t tg_id, int32_t isBot);

    int deleteAllMessage(int64_t tg_id);

    list<int32_t> *getMessages(int64_t tg_id, int32_t isBot);

    list<int32_t> *getAllMessages(int64_t tg_id);

};


#endif //KOSTYA_BOT_DBHANDLER_H
