#include "utils.h"
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>

int insert_user(sqlite3 *db, char *username, char *psw)
{
    int rc;
    sqlite3_stmt *stmt;

    const char *sql =
        "INSERT INTO user_login (username, password) VALUES (?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERROR PREPARING QUERY: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    printf("QUERY PREPARED CORRECTLY\n");

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, psw, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "ERROR STEP QUERY: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    printf("QUERY EXECUTED CORRECTLY\n");

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}

int main(void)
{
	sqlite3 *handle_db;
	char *sql_cmnd;
	int f_open_db;
	int rc;
	int err_msg;

	f_open_db = SQLITE_OPEN_CREATE
				| SQLITE_OPEN_READWRITE;
	if((rc = sqlite3_open_v2("login.db", &handle_db, f_open_db, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR OPENING DB: %s\n", sqlite3_errmsg(handle_db));
		sqlite3_close(handle_db);
		return (rc);
	}
	fprintf(stdout, "DB OPENED CORRECTLY \n");
	sql_cmnd = "CREATE TABLE IF NOT EXISTS user_login ("
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"username TEXT NOT NULL,"
				"password TEXT NOT NULL"
				");";
	if((rc = sqlite3_exec(handle_db, sql_cmnd, NULL, NULL, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR CREATE TABLE: %s\n", sqlite3_errmsg(handle_db));
		sqlite3_close(handle_db);
		return (rc);
	}
	fprintf(stdout, "TABLE CREATED CORRECTLY");
	sql_cmnd = ""
	return 0;
}