#ifndef SQL_H
#define SQL_H
#include "sqlite3.h"
#include <pthread.h>

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

#define CHECK_DB_MACRO_HAS_ROW 1
#define CHECK_DB_MACRO_HAS_NO_ROW 0

int check_db_err(sqlite3 *db, const char *dbN,const char *key); 

#define BUF_SIZE 100 
#define STRUCT_SIZE 100

/*
 * get_tb() retrieves sqlite3_column_text() string 
 */
const char *get_tb(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt); 

/*
 * get_column() retrieves sqlite3_column_text() string
 */
const unsigned char *get_column(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt1, sqlite3_stmt *stmt2, sqlite3_stmt *stmt3, const char *table, int index);

#define WEB_LEN 100
#define USER_LEN 30
#define PASS_LEN 40

struct DATA {
	char website[WEB_LEN]; 
	char username[USER_LEN]; 
	char pass[PASS_LEN]; 
};

/*
 * INSERT into DB 
 */

void *insert_into(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt, const char *tablename, const unsigned char *wCol, const unsigned char *uCol, const unsigned char *pCol); 

/*
 * RETRIEVE FROM DB 
 *
 * ret() retrieves sqlite3_column_text() and sqlite3_column_int() for data retrieval from DB
 */
void *ret(sqlite3 *db, sqlite3_stmt *stmt, const char *table, const unsigned char *wCol, const unsigned char *uCol, const unsigned char *pCol); 

const char *set_key(const char *key);

#endif