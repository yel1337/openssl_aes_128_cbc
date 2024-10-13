#define SQLITE_HAS_CODEC 1 
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/src/sql/turing_sql.h"

int check_db_err(sqlite3 *db, const char *key)
{
        const char *command = "SELECT * FROM sqlite_master WHERE type = 'table';";
        sqlite3_stmt *stmt;
	const char *dbN;
	int rc; 

        rc = sqlite3_open(dbN, &db);
        if(rc != SQLITE_OK) {
                printf("%s\n", sqlite3_errmsg(db));
        }

        rc = sqlite3_key(db, key, strlen(key));
        if(rc != SQLITE_OK) {
                 printf("%s\n", sqlite3_errmsg(db));
        }

        rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
                char err = printf("%s\n", sqlite3_errmsg(db));

                sqlite3_close(db);
         }

        int i; 
        if (sqlite3_step(stmt) == SQLITE_ROW) {
                return 1;
        } else {
                return 0;
        }
}

static char tb_err(sqlite3 *db)
{	
	char tb_err_msg = printf("%s \n", sqlite3_errmsg(db));

	return tb_err_msg;
}

void *get_tb(int rc, sqlite3 *db, sqlite3_stmt *stmt, const char *tb_c, const char *tb_t)
{
	struct TABLE *tb;

        const char *command = malloc(1000 * sizeof(char));
        command = "SELECT name FROM sqlite_master WHERE type = 'table';";

        rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);

        sqlite3_finalize(stmt);

        if (rc != SQLITE_OK) {
		tb_err(db);

                sqlite3_close(db);
         }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
                tb_t = sqlite3_column_text(stmt, 0);
                
		tb_c = strdup((const char *)tb_t);
		tb->tablename = tb_c;
        } else {
                printf("get_tb: no row returned.\n");
        }

       
}

static char column_err(sqlite3 *db)
{
        char column_err_msg = printf("%s \n", sqlite3_errmsg(db));

        return column_err_msg;
}

void *get_column(int rc, sqlite3 *db, sqlite3_stmt *stmt)
{
	struct TABLE *tb; 
	struct COLUMN *clmn; 

        char column_buf[BUF_SIZE];
        sprintf(column_buf, "SELECT name FROM pragma_table_info('%s');", tb->tablename);

        rc = sqlite3_prepare_v2(db, column_buf, -1, &stmt, NULL);

        if (rc != SQLITE_OK) {	
		column_err(db);

                sqlite3_close(db);
         }

	const char *clmn_username;
	const char *clmn_email;
	const char *clmn_password;

	static char *clmn_username_c;
	static char *clmn_email_c;
	static char *clmn_password_c;
	
        if (sqlite3_step(stmt) == SQLITE_ROW) {
                clmn_username = sqlite3_column_text(stmt, 0);
                clmn_email = sqlite3_column_text(stmt, 1);
                clmn_password = sqlite3_column_text(stmt, 2);

                clmn_username_c = strdup((const char *)clmn_username);
                clmn_email_c = strdup((const char *)clmn_email);
                clmn_password_c = strdup((const char *)clmn_password);

		clmn->column_username = clmn_username_c; 
		clmn->column_email = clmn_email_c; 
		clmn->column_password = clmn_password_c; 
        } else {
                printf("get_column(): no row returned.\n");
        }

}

static char insert_err(sqlite3 *db)
{
	char insert_err_msg = printf("%s \n", sqlite3_errmsg(db));

        return insert_err_msg;

}

void insert_into(sqlite3 *db, sqlite3_stmt *stmt, struct TABLE *tb, struct COLUMN *clmn, struct DATA dt)
{
        int rc; 

	// Hardcoded data 
	strcpy(dt.username, "daniel"); 
	strcpy(dt.email, "daniel@ymail"); 
	strcpy(dt.password, "pass_test"); 

        static char format_buf[BUF_SIZE];
        sprintf(format_buf, "INSERT INTO %s (%s %s %s) VALUES (%s, %s, %s);", tb->tablename, clmn->column_username, clmn->column_email, clmn->column_password, dt.username, dt.email, dt.password);

        rc = sqlite3_prepare_v2(db, (const char *)format_buf, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
		insert_err(db); 

                sqlite3_close(db);
         }

	sqlite3_step(stmt); 
}

static char ret_err(sqlite3 *db)
{
	char ret_err_msg = printf("%s \n", sqlite3_errmsg(db));

	return ret_err_msg;
}

void *ret(sqlite3 *db, sqlite3_stmt *stmt)
{
	struct COLUMN *clmn; 
	struct TABLE *tb;

        int rc;

	static char format_buf[BUF_SIZE];
	sprintf(format_buf, "SELECT %s, %s, %s FROM %s;", clmn->column_username, clmn->column_email, clmn->column_password, tb->tablename);

	rc = sqlite3_prepare_v2(db, (const char *)format_buf, -1, &stmt, NULL);
	if(rc != SQLITE_OK) {
		ret_err(db);
		
		sqlite3_close(db);
	}

        const char *ret_username;
        const char *ret_email;
        const char *ret_password;

	static char *ret_username_c;
	static char *ret_email_c;
	static char *ret_password_c;

	while(sqlite3_step(stmt) == SQLITE_ROW) {
		ret_username = sqlite3_column_text(stmt, 0);
		ret_email = sqlite3_column_text(stmt, 1);
		ret_password = sqlite3_column_text(stmt, 2);
	}

	ret_username_c = strdup((const char *)ret_username); 
	ret_email_c  = strdup((const char *)ret_email); 
	ret_password_c = strdup((const char *)ret_password); 

	printf("%s \n", ret_username_c);
	printf("%s \n", ret_email_c);
	printf("%s \n", ret_password_c);

	sqlite3_step(stmt);
}

const char *set_key(const char *key)
{
	key = "correctkey";

	return key; 
}

void close_db(sqlite3 *db)
{
        sqlite3_close(db);
}