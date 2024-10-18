#define SQLITE_HAS_CODEC 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"


char *ret_tb(sqlite3 *db, sqlite3_stmt *stmt)
{	
	int rc; 
	const char *command = "SELECT name FROM sqlite_master WHERE type = 'table';"; 
	const char *key = "correctkey";

	sqlite3_open("test_encrypted.db", &db);

	sqlite3_key(db, key, strlen(key)); 

	rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
	if(rc != SQLITE_OK) {
  		printf("PREP err - %s\n", sqlite3_errmsg(db));
        }
	char *my_copy;

	if (sqlite3_step(stmt) == SQLITE_ROW) {
    		const unsigned char *text = sqlite3_column_text(stmt, 0);

			my_copy = strdup((const char *)text);	
	} else {
    		printf("No row returned.\n");
	}

	return my_copy;	
}
char *ret_col(int rc, sqlite3 *db, sqlite3_stmt *stmt, const char *column, const char *command, char *my_copy)
{
	char column_buf[100]; 
        sprintf(column_buf, "SELECT name FROM pragma_table_info('%s');", my_copy);

        rc = sqlite3_prepare_v2(db, (const char *)column_buf, -1, &stmt, NULL);

        if (rc != SQLITE_OK) {
                char err = printf("COLUMN err - %s\n", sqlite3_errmsg(db));

                sqlite3_close(db);
         }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
                column = sqlite3_column_text(stmt, 0);

                my_copy = strdup((const char *)column);
        } else {
                printf("get_column: no row returned.\n");
        }

        return my_copy;
}


int main() 
{
	const char *dbN = "test";
	sqlite3 *db;
	int rc;
	sqlite3_stmt *stmt; 	
	const char *command; 
	char *my_copy; 

	char *tb = ret_tb(db, stmt);

	printf("%s\n", tb);

}
