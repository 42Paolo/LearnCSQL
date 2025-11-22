#include "utils.h"
#include <_stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int show_users_psw(sqlite3 *db)
{
	int rc;
	char *sql;
	sqlite3_stmt *stmt;
	sql = "SELECT * FROM login_user;";
	
	if((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR PREPARING QUERY: %s", sqlite3_errmsg(db));
		return rc;
	}
	while((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		int id = sqlite3_column_int(stmt, 0);
		const unsigned char *username = sqlite3_column_text(stmt, 1);
		const unsigned char *password = sqlite3_column_text(stmt, 2);
		printf("ID: %d | USER: %s | PASS: %s\n", id, username, password);
	}
	sqlite3_finalize(stmt);
	return rc;
}

int create_table(sqlite3 *db)
{
	char *sql;
	int rc;

	sql = 	"CREATE TABLE IF NOT EXISTS login_user ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"username TEXT NOT NULL,"
			"password TEXT NOT NULL );";
	if((rc = sqlite3_exec(db, sql, NULL, NULL, NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR CREAING TABLE: %s\n", sqlite3_errmsg(db));
		return rc;
	}
	return SQLITE_OK;
}

int insert_user(sqlite3 *db, char* username, char* password)
{
	char *sql;
	int rc;
	
	sqlite3_stmt *stmt;

	sql = "INSERT INTO login_user ( username , password ) VALUES ( ? , ? );";
	if((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK)
	{		
		fprintf(stderr, "ERROR PREPARING QUERY: %s\n", sqlite3_errmsg(db));
		return rc;
	}
	sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);
	if((rc = sqlite3_step(stmt)) != SQLITE_DONE)
	{
		fprintf(stderr, "ERROR EXECUTING QUERY: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return rc;
	}
	sqlite3_finalize(stmt);
	return SQLITE_OK;
}



int main()
{
	int rc;
	int flags;
	char username[25];
	char password[25];

	sqlite3 *handle_db;

	flags = SQLITE_OPEN_READWRITE
			| SQLITE_OPEN_CREATE;
	

	if((rc = sqlite3_open_v2("login.db", &handle_db, flags , NULL)) != SQLITE_OK)
	{
		fprintf(stderr, "ERROR CREATING/OPENING DATABASE: %s\n", sqlite3_errmsg(handle_db));
		sqlite3_close(handle_db);
		return rc;
	}
	if((rc = create_table(handle_db)) != SQLITE_OK)
	{
		sqlite3_close(handle_db);
		return rc;
	}
	while(is_empty_advanced(username) || is_empty_advanced(password))
	{
		printf("Insert username: ");
		fgets(username, sizeof(username), stdin);
		username[strcspn(username, "\n")] = 0;
		printf("Insert password: ");
		fgets(password, sizeof(password), stdin);
		password[strcspn(password, "\n")] = 0;
		if(is_empty_advanced(username) || is_empty_advanced(password))
		{
			printf("ERROR: username OR password EMPTY\n");
			printf("ENTER A KEY TO CONTINUE");
			getchar();
			system("clear");
		}
	}
	if((rc = insert_user(handle_db, username, password)) != SQLITE_OK)
	{
		sqlite3_close(handle_db);
		return rc;
	}
	show_users_psw(handle_db);
	return 0;
}
