#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sqlite3.h>
#include <unistd.h>
#include <openssl/rand.h>
#include </home/yel/openssl_aes_128_cbc/src/proto/features_include/kdf_proto.h>

unsigned char *salt_gen()
{
	unsigned char *salt = malloc(16 * sizeof(char));;
	
	int salt_ret_val = RAND_bytes(salt, sizeof(salt));
	
    if(salt_ret_val == 1){
        printf("salt_gen: ok\n");
    } else if(salt_ret_val == -1){
        printf("salt_gen: err\n");
    }

	return salt;  
}

char *key_gen(unsigned char *salt) 
{
	FILE *po;

	char openssl_pbkdf2[1000];
	char pphrase[] = "password"; /* test pphrase or as DEFAULT */
    char buffer[1000];
    static char gen_key[1000];

	sprintf(openssl_pbkdf2, "openssl kdf -keylen 16 -kdfopt digest:sha256 -kdfopt pass:%s -kdfopt salt:salt -kdfopt iter:1 PBKDF2", pphrase);
	
	po = popen(openssl_pbkdf2, "r");
    
    while(fgets(buffer, sizeof(buffer), po) != NULL) {
        for(int i = 0; i <= strlen(buffer); i++) {
            gen_key[i] = buffer[i];

            return gen_key; 
        }
    }

	pclose(po); /* Close FILE* pipe */
}

void set_db_key(sqlite3 *db, sqlite3_stmt *stmt, char *generated_key)
{
    int sql_exec, sql_prep;
    char *err_msg = 0;

    int sql_open = sqlite3_open("/home/yel/openssl_aes_128_cbc/src/proto/features_src/cred_db", &db);

    if(sql_open != SQLITE_OK){
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
    } else if(sql_open == SQLITE_OK) {
        printf("sqlite3_open: OK\n");
    }

    // 2025
    /* ATTACH DB*/
    char *attach_key_buf = malloc(sizeof(char *) * 1000); 

    sprintf(attach_key_buf, "ATTACH DATABASE '/home/yel/openssl_aes_128_cbc/src/proto/features_src/cred_db' AS cred_enc KEY '%s';", generated_key);

    int attach_prep = sqlite3_prepare_v2(db, attach_key_buf, -1, &stmt, NULL);

    if(attach_prep == SQLITE_OK) {
        printf("ATTACH_PREP: ok\n");
    } else if(attach_prep != SQLITE_OK) {
        printf("ATTACH_PREP ERR: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    // Export the data to the plaintext database
    const char *export_sql = "SELECT sqlcipher_export('cred_enc');";
    sql_exec = sqlite3_exec(db, export_sql, 0, 0, &err_msg);
    if(sql_exec != SQLITE_OK) {
        printf("EXPORT DB: %s\n", sqlite3_errmsg(db));
    }

    // Detach the plaintext database
    const char *detach_sql = "DETACH DATABASE cred_enc;";
    sql_exec = sqlite3_exec(db, detach_sql, 0, 0, &err_msg);
    if(sql_exec == SQLITE_OK) {
        printf("DETACH DB: ok\n");
    }

    sqlite3_close(db);
}