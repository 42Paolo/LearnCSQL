#include "utils.h"
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>


int insert_user(sqlite3 *db, char *username, char *password)
{
	int rc;
	sqlite3_stmt *stmt; 
	char *sql = "INSERT INTO user_login (username, password) VALUES ( ? , ? );";

	if((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR PREPARING QUERY: %s\n", sqlite3_errmsg(db));
		return (rc);
	}

	sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);

	if((rc = sqlite3_step(stmt)) != SQLITE_DONE)
	{
		fprintf(stderr, "ERROR FILLING QUERY: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return rc;
	}
	sqlite3_finalize(stmt);
	return SQLITE_OK;
}

int main(void)
{
	sqlite3 *handle_db;
	int rc;
	char *sql;
	int flags;
	char username[100];
	char psw[100];

	flags = SQLITE_OPEN_CREATE|
			SQLITE_OPEN_READWRITE;
	if((rc = sqlite3_open_v2("login.db", &handle_db, flags, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR OPEN DB FILE: %s\n", sqlite3_errmsg(handle_db));
		sqlite3_close(handle_db);
		return rc;
	}
	sql =	"CREATE TABLE IF NOT EXISTS user_login ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"username TEXT NOT NULL,"
			"password TEXT NOT NULL);";
	if((rc = sqlite3_exec(handle_db, sql, NULL, NULL, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR CREATE TABLE: %s\n", sqlite3_errmsg(handle_db));
		sqlite3_close(handle_db);
		return rc;
	}
	printf("Insert your username: ");
	fgets(username, sizeof(username), stdin);
	username[strcspn(username, "\n")] = 0;

	printf("Insert your password: ");
	fgets(psw, sizeof(psw), stdin);
	psw[strcspn(psw, "\n")] = 0;
	if((rc = insert_user(handle_db, username, psw)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR INSERTING DATAS: %s\n", sqlite3_errmsg(handle_db));
		sqlite3_close(handle_db);
		return rc;
	}
	sqlite3_close(handle_db);
	return 0;
}
