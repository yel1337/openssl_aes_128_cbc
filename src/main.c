#define SQLITE_HAS_CODEC 1 
#include "sqlite3.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/src/sql/turing_sql.h"

#define KEY_MACRO_1 1
#define KEY_MACRO_0 0

static int key_err(int check_db_ret_int)
{
	int key_result;

	/*
	 * CHECK_DB_MACRO_O returned by check_db_err() determines if PRAGMA key 
	 * entered is incorrect or not the right key used during DB setup 
	 *
	 * Likewise in key_err() wherein KEY_MACRO_0 and KEY_MACRO_1 works in
	 * similar ways but it is intended for key authentication
	 */

	if(check_db_ret_int == CHECK_DB_MACRO_HAS_NO_ROW) {
		key_result = KEY_MACRO_0;

		return key_result; 
	} 
	else if(check_db_ret_int == CHECK_DB_MACRO_HAS_ROW) {
		key_result = KEY_MACRO_1;
		
		return key_result; 	
	}
}

// THIS IS THE DAMN PROBLEM
static void open_db(sqlite3 *db, const char *dbN, const char *key)
{
	sqlite3_open(dbN, &db);
	
	sqlite3_key(db, key, strlen(key));

	int db_err = check_db_err(db, dbN,key);

	/*
	 * key_err(int check_db_ret_int) is some sort of a signal to SQLCipher whether the key / PRAMGA key entered 
	 * is correct or not 
	 *
	 * key_err will return 1 if key is the correct PRAGMA key used by DB otherwise key_err will return 0 as key
	 * entered or submitted is not the correct key used by DB during setup 	
	 *
 	 */

	int key_r; 
	if(db_err == CHECK_DB_MACRO_HAS_ROW) {
		key_r = key_err(db_err);
	}	
	else if(db_err == CHECK_DB_MACRO_HAS_NO_ROW) {
		key_r = key_err(db_err);

		sqlite3_close(db);
	}
}

int main() 
{ 
	struct TABLE *tb;
	struct COLUMN *clmn; 
	struct DATA dt;

	sqlite3 *db; 
	sqlite3_stmt *stmt; 
	const char *key;

	int rc;
	char *tb_c;
	const char *tb_t; 

	const char *db_key = "correctkey";
	const char *dbN = "encrypted.db"; 	

	int check_db_sig = check_db_err(db, dbN, db_key);

	char *table_name = get_tb(db, dbN, db_key, stmt);

	get_column(db, dbN, db_key, stmt, table_name);

	insert_into(db, dbN, db_key, stmt, table_name);

	ret(db, stmt, table_name);
}
