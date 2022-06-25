//
// Created by root on 25.06.22.
//
//TODO: сделать работу с UTF-8 строками (например для русских имён
//TODO: подумать над atoi мб поменять его на что-нибудь другое

#include <cstdio>
#include "DBHandler.h"

DBHandler::DBHandler(const char *host, const char *user, const char *passwd, const char *db,
                     unsigned int port, const char *unixSocket, unsigned long clientflag) : host(host),
                                                                                            user(user), passwd(passwd),
                                                                                            db(db), port(port),
                                                                                            unix_socket(unixSocket),
                                                                                            clientflag(clientflag) {
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

void DBHandler::error_finish() {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void DBHandler::error_handle() {
    fprintf(stderr, "%s\n", mysql_error(con));
    //handle this
    exit(1);
}

int DBHandler::add_user(int64_t tg_id, const string &name, int cur_menu, int access_level, int language) {
    if (name.length() > 120)
        return 1;
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT tg_id FROM users WHERE tg_id =%ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return 2;
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        error_handle();
        free(buffer);
        return 3;
    }
    if (mysql_num_rows(result) == 0) {
        sprintf(buffer, "INSERT INTO users VALUES (NULL, %ld, '%s', %d, %d, %d)", tg_id, name.c_str(), cur_menu,
                access_level, language);
        if (mysql_query(con, buffer)) {
            error_handle();
            free(buffer);
            return 4;
        }
    }

    free(buffer);
    return 0;
}

bool DBHandler::hasUser(int64_t tg_id) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT tg_id FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return false;
    }
    MYSQL_RES *result = mysql_store_result(con);

    //maybe delete it?
    if (result == NULL) {
        error_handle();
        free(buffer);
        return false;
    }

    free(buffer);
    return mysql_num_rows(result) > 0;
}

bool DBHandler::hasUser(const string &name) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT Name FROM users WHERE Name = '%s'", name.c_str());
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return false;
    }
    MYSQL_RES *result = mysql_store_result(con);
    //maybe delete it?
    if (result == NULL) {
        error_handle();
        free(buffer);
        return false;
    }

    free(buffer);
    return mysql_num_rows(result) > 0;
}

int DBHandler::getCurMenu(int64_t tg_id) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT CurrentMenu FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        error_handle();
        free(buffer);
        return -1;
    }
    if (mysql_num_rows(result) == 0) {
        free(buffer);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    int res = atoi(row[0]); //CurrentMenu(mysql field)

    mysql_free_result(result);
    free(buffer);
    return res;
}

int DBHandler::getAccessLevel(int64_t tg_id) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT AccessLevel FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        error_handle();
        free(buffer);
        return -1;
    }
    if (mysql_num_rows(result) == 0) {
        free(buffer);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    int res = atoi(row[0]); //CurrentMenu(mysql field)

    mysql_free_result(result);
    free(buffer);
    return res;
}

int DBHandler::getLanguage(int64_t tg_id) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "SELECT Language FROM users WHERE tg_id = %ld", tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return -1;
    }
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        error_handle();
        free(buffer);
        return -1;
    }
    if (mysql_num_rows(result) == 0) {
        free(buffer);
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    int res = atoi(row[0]); //CurrentMenu(mysql field)

    mysql_free_result(result);
    free(buffer);
    return res;

}

void DBHandler::setCurMenu(int64_t tg_id, int cur_menu) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET CurrentMenu = %d WHERE tg_id = %ld", cur_menu, tg_id);
    if (mysql_query(con, buffer)) {
        free(buffer);
        error_handle();
        return;
    }

    free(buffer);
}

void DBHandler::setAccessLevel(int64_t tg_id, int cur_level) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET AccessLevel = %d WHERE tg_id = %ld", cur_level, tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return;
    }
    free(buffer);
}

void DBHandler::setLanguage(int64_t tg_id, int cur_language) {
    char *buffer = (char *) malloc(1024);
    sprintf(buffer, "UPDATE users SET Language = %d WHERE tg_id = %ld", cur_language, tg_id);
    if (mysql_query(con, buffer)) {
        error_handle();
        free(buffer);
        return;
    }
    free(buffer);
}









