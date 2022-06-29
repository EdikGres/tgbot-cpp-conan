//
// Created by root on 25.06.22.
//
//TODO: сделать работу с UTF-8 строками (например для русских имён
//TODO: подумать над atoi мб поменять его на что-нибудь другое

#include <cstdio>
#include <tgbot/tgbot.h>
#include "DBHandler.h"

DBHandler::DBHandler(const char *host, const char *user, const char *passwd, const char *db,
                     unsigned int port, const char *unixSocket, unsigned long clientflag) : host(host),
                                                                                            user(user), passwd(passwd),
                                                                                            db(db), port(port),
                                                                                            unix_socket(unixSocket),
                                                                                            clientflag(clientflag) {
    mysql_library_init;
    con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    if (mysql_real_connect(con, host, user, passwd,
                           db, port, unix_socket, clientflag) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
}

DBHandler::~DBHandler() {
    mysql_close(con);
    mysql_library_end;

}

void DBHandler::error_finish() {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void DBHandler::error_handle() {
    fprintf(stderr, "%s\n", mysql_error(con));
    //handle this
    //exit(1);
}

int DBHandler::add_user(int64_t tg_id, const string &name, int cur_menu, int access_level, int language) {
    if (name.length() > 120)
        return 1;
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT tg_id FROM users WHERE tg_id =%ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return 2;
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return 3;
    }
    if (mysql_num_rows(result) == 0) {
        sprintf(buffer, "INSERT INTO users VALUES (NULL, %ld, '%s', %d, %d, %d)", tg_id, name.c_str(), cur_menu,
                access_level, language);
        if (mysql_query(con, buffer)) {
            error_handle();
            free(buffer);
            mysql_free_result(result);
            m_mutex.unlock();
            return 4;
        }
    }
    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return 0;
}

bool DBHandler::hasUser(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT tg_id FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }
    MYSQL_RES *result = mysql_store_result(con);

    //maybe delete it?
    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return mysql_num_rows(result) > 0;
}

bool DBHandler::hasUser(const string &name) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT Name FROM users WHERE Name = '%s'", name.c_str());
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }
    MYSQL_RES *result = mysql_store_result(con);
    //maybe delete it?
    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return mysql_num_rows(result) > 0;
}

int DBHandler::getCurMenu(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT CurrentMenu FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    if (mysql_num_rows(result) == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    int res = atoi(row[0]); //CurrentMenu(mysql field)

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return res;
}

int DBHandler::getAccessLevel(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT AccessLevel FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    if (mysql_num_rows(result) == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    int res = atoi(row[0]); //CurrentMenu(mysql field)

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return res;
}

int DBHandler::getLanguage(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT Language FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    if (mysql_num_rows(result) == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    int res = atoi(row[0]); //CurrentMenu(mysql field)

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return res;

}

void DBHandler::setCurMenu(int64_t tg_id, int cur_menu) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET CurrentMenu = %d WHERE tg_id = %ld", cur_menu, tg_id);
    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        m_mutex.unlock();
        return;
    }

    free(buffer);
    m_mutex.unlock();
}

void DBHandler::setAccessLevel(int64_t tg_id, int cur_level) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET AccessLevel = %d WHERE tg_id = %ld", cur_level, tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return;
    }
    free(buffer);
    m_mutex.unlock();
}

void DBHandler::setLanguage(int64_t tg_id, int cur_language) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET Language = %d WHERE tg_id = %ld", cur_language, tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return;
    }
    free(buffer);
    m_mutex.unlock();
}

int DBHandler::addMessage(int64_t tg_id, int32_t message_id, int32_t isBot) {
    m_mutex.lock();
    //INSERT INTO users VALUES (NULL, %ld, '%s', %d, %d, %d)
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "INSERT INTO messages VALUES (NULL, %d, %ld, %d)", message_id, tg_id, isBot);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    free(buffer);
    m_mutex.unlock();
    return 0;
}

int DBHandler::deleteMessage(int64_t tg_id, int32_t message_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "DELETE FROM messages WHERE id_message=%d", message_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }

    free(buffer);
    m_mutex.unlock();
    return 0;
}

int DBHandler::deleteMessageUser(int64_t tg_id, int32_t isBot) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "DELETE FROM messages WHERE user_id=%ld AND isBot=%d", tg_id, isBot);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }

    free(buffer);
    m_mutex.unlock();
    return 0;
}

int DBHandler::deleteAllMessage(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "DELETE FROM messages WHERE user_id=%ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }

    free(buffer);
    m_mutex.unlock();
    return 0;
}

list<int32_t> *DBHandler::getMessages(int64_t tg_id, int32_t isBot) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT id_message FROM messages WHERE user_id =%ld AND isBot=%d", tg_id, isBot);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return nullptr;
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return nullptr;
    }
    uint64_t num_fields = mysql_num_rows(result);
    if (num_fields == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return nullptr;
    }
    auto *list = new std::list<int32_t>;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        list->push_back((int32_t) atoi(row[0]));
    }

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return list;
}

list<int32_t> *DBHandler::getAllMessages(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT id_message FROM messages WHERE user_id =%ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return nullptr;
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return nullptr;
    }
    uint64_t num_fields = mysql_num_rows(result);
    if (num_fields == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return nullptr;
    }
    auto *list = new std::list<int32_t>;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        list->push_back((int32_t) atoi(row[0]));
    }

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return list;
}











