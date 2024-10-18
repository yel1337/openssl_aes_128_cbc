#define SQLITE_HAS_CODEC 1 
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/src/sql/turing_sql.h"

int check_db_err(sqlite3 *db, const char *dbN,const char *key)
{
        const char *command = "SELECT * FROM sqlite_master WHERE type = 'table';";
        sqlite3_stmt *stmt;
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
 
        if (sqlite3_step(stmt) == SQLITE_ROW) {
                return CHECK_DB_MACRO_HAS_ROW;
        } else {
                return CHECK_DB_MACRO_HAS_NO_ROW;
        }
}

static char tb_err(sqlite3 *db)
{	
	char tb_err_msg = printf("%s \n", sqlite3_errmsg(db));

	return tb_err_msg;
}

char *get_tb(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt)
{
        int rc;
        struct TABLE *tb; 

        sqlite3_open(dbN, &db);

        sqlite3_key(db, key, strlen(key));

        rc = sqlite3_prepare_v2(db, "SELECT name FROM sqlite_master WHERE type = 'table';", -1, &stmt, NULL);
        if(rc != SQLITE_OK) {
                printf("%s \n", sqlite3_errmsg(db));
        }

        if (rc != SQLITE_OK) {
		tb_err(db);

                sqlite3_close(db);
         }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
               const char *tb_t = sqlite3_column_text(stmt, 0);
                
		tb->tablename = strdup((const char *)tb_t);
        } else {
                printf("get_tb: no row returned.\n");
        }

        char *tb_c = tb->tablename;

        return tb_c; 

        sqlite3_close(db);
}

static char column_err(sqlite3 *db)
{
        char column_err_msg = printf("%s \n", sqlite3_errmsg(db));

        return column_err_msg;
}

void *get_column(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt, char *table) {
	struct TABLE *tb; 
	struct COLUMN *clmn; 

        sqlite3_open(dbN, &db);

        sqlite3_key(db, key, strlen(key));

        // char column_buf; 
        // sprintf(column_buf, "SELECT name FROM pragma_table_info('%s');", tb->tablename);

        sqlite3_prepare_v2(db, "", -1, &stmt, NULL);

        //

	const char *clmn_firstname;
	const char *clmn_lastname;

        if (sqlite3_step(stmt) == SQLITE_ROW) {
                clmn_firstname = sqlite3_column_text(stmt, 0);
                clmn_lastname = sqlite3_column_text(stmt, 1);

		clmn->column_firstname = strdup((const char *)clmn_firstname); 
		clmn->column_lastname = strdup((const char *)clmn_lastname); 
        } else {
                printf("get_column(): no row returned.\n");
        }

}

static char insert_err(sqlite3 *db)
{
	char insert_err_msg = printf("%s \n", sqlite3_errmsg(db));

        return insert_err_msg;

}

void insert_into(sqlite3 *db, const char *dbN, const char *key,sqlite3_stmt *stmt, char *tablename)
{
        struct COLUMN *clmn; 
        struct TABLE *tb; 
        struct DATA dt; 
        int rc; 

	// Hardcoded data 
	strcpy(dt.first_name, "ruby"); 
	strcpy(dt.last_name, "ann"); 

        sqlite3_open(dbN, &db);

        sqlite3_key(db, key, strlen(key));

        static char format_buf[BUF_SIZE];
        sprintf(format_buf, "INSERT INTO %s (%s %s) VALUES (%s, %s);", tablename, clmn->column_firstname, clmn->column_lastname, dt.first_name, dt.last_name);

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

void *ret(sqlite3 *db, sqlite3_stmt *stmt, char *tablename)
{
	struct COLUMN *clmn; 
	struct TABLE *tb;

        int rc;

	static char format_buf[BUF_SIZE];
	sprintf(format_buf, "SELECT %s, %s FROM %s;", clmn->column_firstname, clmn->column_lastname, tablename);

	rc = sqlite3_prepare_v2(db, (const char *)format_buf, -1, &stmt, NULL);
	if(rc != SQLITE_OK) {
		ret_err(db);
		
		sqlite3_close(db);
	}

        const char *ret_firstname;
        const char *ret_lastname;

	static char *ret_firstname_c;
	static char *ret_lastname_c;

	if (sqlite3_step(stmt) == SQLITE_ROW) {
	        ret_firstname = sqlite3_column_text(stmt, 0);
		ret_lastname = sqlite3_column_text(stmt, 1);
	}

	ret_firstname_c = strdup((const char *)ret_firstname); 
	ret_lastname_c = strdup((const char *)ret_lastname); 

	printf("%s \n", ret_firstname_c);
	printf("%s \n", ret_lastname_c);

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