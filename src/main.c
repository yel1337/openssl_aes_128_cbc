#define SQLITE_HAS_CODEC 1 
#include "sqlite3.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/src/sql/turing_sql.h"

static int key_err(int check_db_ret_int)
{
	int key_result;

	if(check_db_ret_int == 0) {
		key_result = 0;

		return key_result; 
	} 
	else if(check_db_ret_int == 1) {
		key_result = 1;
		
		return key_result; 	
	}
}

static int open_db(sqlite3 *db, const char *key)
{
	sqlite3_open("proto_enc.db", &db);
	
	sqlite3_key(db, key, strlen(key));

	int db_err = check_db_err(db, key);

	/*
	 * key_err(int check_db_ret_int) is some sort of a signal to SQLCipher whether the key / PRAMGA key entered 
	 * is correct or not 
	 *
	 * key_err will return 1 if key is the correct PRAGMA key used by DB otherwise key_err will return 0 as key
	 * entered or submitted is not the correct key used by DB during setup 	
	 *
 	 */
	int key_r; 
	if(db_err == 1) {
		key_r = key_err(db_err);
	}	
	else if(db_err == 0) {
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

	const char *db_key = set_key(key);

	open_db(db, db_key);

	get_tb(rc, db, stmt, tb_c, tb_t);

	get_column(rc, db, stmt);

	insert_into(db, stmt, tb, clmn, dt);

	ret(db, stmt);
}
