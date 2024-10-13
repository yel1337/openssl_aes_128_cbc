#ifndef SQL_H
#define SQL_H
#include "sqlite3.h"

/*
 * check_db_err() to verify PRAGMA key if valid or not 
 *
 * As stated in SQLCipher's Documentation, PRAGMA key validation check is usually done
 * by submitting a query to encrypted DB.    
 *
 * check_db_err() will return DB_1 if DB is DB_OK otherwise 
 * the function will return DB_0 as DB_NOT_OK indicating that the key entered 
 * is not the correct key used by DB  
 *
 */

int check_db_err(sqlite3 *db, const char *key); 

#define BUF_SIZE 100 
#define STRUCT_SIZE 100

struct TABLE {
	const char *tablename[STRUCT_SIZE]; 
}; 

/*
 * get_tb() retrieves sqlite3_column_text() string 
 */
void *get_tb(int rc, sqlite3 *db, sqlite3_stmt *stmti, char *tb_c, const char *tb_t); 

struct COLUMN {
	char *column_username;
	char *column_email; 
	char *column_password;
}; 

/*
 * get_column() retrieves sqlite3_column_text() string
 */
void *get_column(int rc, sqlite3 *db, sqlite3_stmt *stmt);

#define USER_LEN 10
#define EMAIL_LEN 30
#define PASS_LEN 40

struct DATA {
	char username[USER_LEN]; 
	char email[EMAIL_LEN];
	char password[PASS_LEN]; 
};

/*
 * INSERT into DB 
 */
void insert_into(sqlite3 *db, sqlite3_stmt *stmt, struct TABLE *tb, struct COLUMN *clmn, struct DATA dt); 

/*
 * RETRIEVE FROM DB 
 *
 * ret() retrieves sqlite3_column_text() and sqlite3_column_int() for data retrieval from DB
 */
void *ret(sqlite3 *db, sqlite3_stmt *stmt); 

const char *set_key(const char *key);

#endif
