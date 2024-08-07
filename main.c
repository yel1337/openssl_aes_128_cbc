#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h> 
#include <unistd.h>
#include <sys/types.h>
#define SQLITE_HAS_CODEC 1
#define AUTH_ERROR 0
#define AUTH_SUCCESS 1

int kdf_auth_check(char *kdf_key); 
int main()
{

    /*
     * For every insert operation (or update, delete and select) call sqlite3_step(stmt)
     */
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;
    FILE *fp;
    int key_auth;

    char *kdf_key = getenv("DERIVED_KEY");

    key_auth = kdf_auth_check(kdf_key); // Do key_auth_check

    /*
     * key_auth_check = 0 // AUTH_ERROR
     * key_auth_check = 1 // AUTH_SUCCESS
     */
    if(key_auth == 1)
    {
        fp = popen("sqlcipher pass_man.db", "w");

        rc = sqlite3_exec(db, "SELECT * FROM credentials", 0, 0, &zErrMsg);

        if(rc == SQLITE_ERROR)
        {
            fprintf(stderr, "SQL ERROR: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }

        pclose(fp);
    } else if(kdf_auth_check == 0) {
        fprintf(stderr, "PRAGMA Key AUTH_FAILED");
        exit(EXIT_FAILURE);
    }

    sqlite3_close(db);
}

/*
 * KEY_AUTH
 */
int kdf_auth_check(char *kdf_key)
{
	FILE *fp;
        fp = popen("sqlcipher test.db", "w");
	char *zErrMsg = 0;
        char sprintf_buffer[100];

        /*
         * kdf_key defined from makefile preprocessor
         */
        int pragma_key_auth = sprintf(sprintf_buffer, "PRAGMA key='%s'", kdf_key);

        if(pragma_key_auth != strlen(sprintf_buffer))
        {
                fprintf(stderr, "\npragma_key_auth: %s\n", zErrMsg);

		return AUTH_ERROR;
        } else if(pragma_key_auth == strlen(sprintf_buffer)) {
                return AUTH_SUCCESS;
        }

        pclose(fp); // Close pipe
}


