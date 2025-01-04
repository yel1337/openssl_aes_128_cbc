#include </home/yel/openssl_aes_128_cbc/src/proto/features_include/retrieve_proto_header.h>
#include </home/yel/openssl_aes_128_cbc/src/proto/features_include/kdf_proto.h>
#define SQLITE_HAS_CODEC 1
#define SQLITE_TEMP_STORE 2
#include <stdio.h>

int main()
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    // ret_cred();

    unsigned char *salt = salt_gen();

    char *key = key_gen(salt);

    set_db_key(db, stmt, key);
}