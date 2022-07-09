//
// Created by root on 25.06.22.
//
//TODO: сделать работу с UTF-8 строками (например для русских имён
//TODO: подумать над atoi мб поменять его на что-нибудь другое
//TODO: setCurMenu сделать проверку на возврат, есть ли юзер и установилось ли
//TODO: потестить под valgrind'ом новые функции с текстом

#include <cstdio>
#include <iostream>
#include "DBHandler.h"

DBHandler::DBHandler(const char *host, const char *user, const char *passwd, const char *db,
                     unsigned int port, const char *unixSocket, unsigned long clientflag) : host(host),
                                                                                            user(user), passwd(passwd),
                                                                                            db(db), port(port),
                                                                                            unix_socket(unixSocket),
                                                                                            clientflag(clientflag) {
    mysql_library_init;
    con = mysql_init(nullptr);
    if (con == nullptr) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(11);
    }
    if (mysql_real_connect(con, host, user, passwd,
                           db, port, unix_socket, clientflag) == nullptr) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(11);
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

int
DBHandler::add_user(int64_t tg_id, const string &name, int cur_menu, int access_level_GMP, int access_level_CashFlow,
                    int language) {
    if (name.length() > 120)
        return 1;
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT tg_id FROM users WHERE tg_id =%lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return 2;
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == nullptr) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return 3;
    }
    if (mysql_num_rows(result) == 0) {
        sprintf(buffer, "INSERT INTO users VALUES (NULL, %lld, '%s', %d, %d, %d, %d, 0, 0, 0, 0, 0, 0, 0, 0, 0)", tg_id,
                name.c_str(), cur_menu,
                access_level_GMP, access_level_CashFlow, language);
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
    sprintf(buffer, "SELECT tg_id FROM users WHERE tg_id = %lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }
    MYSQL_RES *result = mysql_store_result(con);

    //maybe delete it?
    if (result == nullptr) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }

    bool ret = mysql_num_rows(result) > 0;

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return ret;
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
    if (result == nullptr) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return false;
    }

    bool ret = mysql_num_rows(result) > 0;

    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return ret;
}

int DBHandler::getCurMenu(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT CurrentMenu FROM users WHERE tg_id = %lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
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

int DBHandler::getAccessLevelGMP(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT AccessLevelGMP FROM users WHERE tg_id = %lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
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

int DBHandler::getAccessLevelCashFlow(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT AccessLevelCashFlow FROM users WHERE tg_id = %lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
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
    sprintf(buffer, "SELECT Language FROM users WHERE tg_id = %lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
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
    sprintf(buffer, "UPDATE users SET CurrentMenu = %d WHERE tg_id = %lld", cur_menu, tg_id);
    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        m_mutex.unlock();
        return;
    }

    free(buffer);
    m_mutex.unlock();
}

void DBHandler::setAccessLevelGMP(int64_t tg_id, int cur_level) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET AccessLevelGMP = %d WHERE tg_id = %lld", cur_level, tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return;
    }
    free(buffer);
    m_mutex.unlock();
}

void DBHandler::setAccessLevelCashFlow(int64_t tg_id, int cur_level) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET AccessLevelCashFlow = %d WHERE tg_id = %lld", cur_level, tg_id);
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
    sprintf(buffer, "UPDATE users SET Language = %d WHERE tg_id = %lld", cur_language, tg_id);
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
    sprintf(buffer, "INSERT INTO messages VALUES (NULL, %d, %lld, %d, NULL)", message_id, tg_id, isBot);
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
    sprintf(buffer, "DELETE FROM messages WHERE user_id=%lld AND isBot=%d", tg_id, isBot);
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
    sprintf(buffer, "DELETE FROM messages WHERE user_id=%lld", tg_id);
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
    sprintf(buffer, "SELECT id_message FROM messages WHERE user_id =%lld AND isBot=%d", tg_id, isBot);
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
    sprintf(buffer, "SELECT id_message FROM messages WHERE user_id =%lld", tg_id);
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

int DBHandler::addMessageWithText(int64_t tg_id, int32_t message_id, int32_t isBot, string text) {
    m_mutex.lock();
    //INSERT INTO users VALUES (NULL, %ld, '%s', %d, %d, %d)
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "INSERT INTO messages VALUES (NULL, %d, %lld, %d, \"%s\")", message_id, tg_id, isBot, text.c_str());
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

string DBHandler::getText(int64_t tg_id, int32_t message_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT text FROM messages WHERE user_id =%lld AND id_message =%d", tg_id, message_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return {};
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == nullptr) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return {};
    }
    uint64_t num_fields = mysql_num_rows(result);
    if (num_fields == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return {};
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    string res{};
    if (row[0] == NULL) {
        m_mutex.unlock();
        return {};
    }
    res = row[0];
    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return res;
}

void DBHandler::setIsSpammer(int64_t tg_id, int isSpammer) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET isSpammer = %d WHERE tg_id = %lld", isSpammer, tg_id);
    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        m_mutex.unlock();
        return;
    }

    free(buffer);
    m_mutex.unlock();
}

int DBHandler::getIsSpammer(int64_t tg_id) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT isSpammer FROM users WHERE tg_id =%lld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == nullptr) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    uint64_t num_fields = mysql_num_rows(result);
    if (num_fields == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (row[0] == NULL) {
        m_mutex.unlock();
        return -1;
    }

    int ret = atoi(row[0]);


    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return ret;
}

int DBHandler::getCurTextGMP(int64_t tg_id, int section) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    switch (section) {
        case 0:
            sprintf(buffer, "SELECT TextGMPBeginners FROM users WHERE tg_id = %lld", tg_id);
            break;
        case 1:
            sprintf(buffer, "SELECT TextGMPTeachers FROM users WHERE tg_id = %lld", tg_id);
            break;
        case 2:
            sprintf(buffer, "SELECT TextGMPLeaders FROM users WHERE tg_id = %lld", tg_id);
            break;
        case 3:
            sprintf(buffer, "SELECT TextGMPTOPLeaders FROM users WHERE tg_id = %lld", tg_id);
            break;
        default:
            return -1;
    }

    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
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

int DBHandler::getCurTextCashFlow(int64_t tg_id, int section) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    switch (section) {
        case 0:
            sprintf(buffer, "SELECT TextCashFlowBeginners FROM users WHERE tg_id = %lld", tg_id);
            break;
        case 1:
            sprintf(buffer, "SELECT TextCashFlowTeachers FROM users WHERE tg_id = %lld", tg_id);
            break;
        case 2:
            sprintf(buffer, "SELECT TextCashFlowLeaders FROM users WHERE tg_id = %lld", tg_id);
            break;
        case 3:
            sprintf(buffer, "SELECT TextCashFlowTOPLeaders FROM users WHERE tg_id = %lld", tg_id);
            break;
        default:
            return -1;
    }

    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
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

void DBHandler::setCurTextGMP(int64_t tg_id, int section, int cur_text) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    switch (section) {
        case 0:
            sprintf(buffer, "UPDATE users SET TextGMPBeginners = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        case 1:
            sprintf(buffer, "UPDATE users SET TextGMPTeachers = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        case 2:
            sprintf(buffer, "UPDATE users SET TextGMPLeaders = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        case 3:
            sprintf(buffer, "UPDATE users SET TextGMPTOPLeaders = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        default:
            return;
    }

    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        m_mutex.unlock();
        return;
    }

    free(buffer);
    m_mutex.unlock();
}

void DBHandler::setCurTextCashFlow(int64_t tg_id, int section, int cur_text) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    switch (section) {
        case 0:
            sprintf(buffer, "UPDATE users SET TextCashFlowBeginners = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        case 1:
            sprintf(buffer, "UPDATE users SET TextCashFlowTeachers = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        case 2:
            sprintf(buffer, "UPDATE users SET TextCashFlowLeaders = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        case 3:
            sprintf(buffer, "UPDATE users SET TextCashFlowTOPLeaders = %d WHERE tg_id = %lld", cur_text, tg_id);
            break;
        default:
            return;
    }

    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        m_mutex.unlock();
        return;
    }

    free(buffer);
    m_mutex.unlock();
}

unordered_map<string, string> *DBHandler::getStrings(string table_name) {
    m_mutex.lock();
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT * FROM %s", table_name.c_str());
    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        m_mutex.unlock();
        return nullptr;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
        error_handle();
        free(buffer);
        m_mutex.unlock();
        return nullptr;
    }
    uint64_t rows = mysql_num_rows(result);
    if (rows == 0) {
        free(buffer);
        mysql_free_result(result);
        m_mutex.unlock();
        return nullptr;
    }
    auto *ret = new unordered_map<string, string>;
    MYSQL_ROW row;
    string key, value;
    for (int i = 0; i < rows; ++i) {
        row = mysql_fetch_row(result);
        key = row[0];
        if (row[1] == nullptr) {
            value = "NULL";
        } else {
            value = row[1];
        }
        ret->insert({key, value});
    }
    mysql_free_result(result);
    free(buffer);
    m_mutex.unlock();
    return ret;
}











