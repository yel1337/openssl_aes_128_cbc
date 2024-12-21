#ifndef kdf_proto
#define kdf_proto 
#include <sqlite3.h>

char *key_gen(unsigned char *salt);

unsigned char *salt_gen(); 

void set_db_key(sqlite3 *db, sqlite3_stmt *stmt, char *generated_key);

#endif
