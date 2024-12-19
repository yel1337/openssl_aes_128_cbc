#ifndef ret_proto
#define ret_proto
#include <sqlite3.h>

void open_db(const char *db_file, sqlite3 **db);
void close_db(sqlite3 *db);
void ret_cred(); 

#endif
