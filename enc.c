#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h> 
#include <unistd.h>
#include <sys/types.h>
#define SQLITE_HAS_CODEC 1
#define AUTH_ERROR 0
#define AUTH_SUCCESS 1

int encrypt(unsigned char *plaintext, int plaintext_len,  unsigned char *key, const unsigned char *iv, unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;

	// Init context
	if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    }

	int len; 

	int ciphertext_len; 

        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL , key, iv))
        {
                fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        }
        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        {
                fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        }
	ciphertext_len = len; 

        if (1 != EVP_EncryptFinal(ctx, ciphertext + len, &len))
        {
                fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        }
	ciphertext_len += len; 

	// Context clean up 
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len; 	
}
int decrypt(unsigned char *ciphertext, int ciphered_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;

    // Init context
	if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
    }
        int len;

        int plaintext_len;

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL , key, iv))
        {
                fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        }

        if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphered_len))
        {
                fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        }
	plaintext_len = len;

        if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        {
                fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        }
	plaintext_len += len;

	// Context clean up
    EVP_CIPHER_CTX_free(ctx);

	return plaintext_len; 
}
static int credentials_callback(void *data, int argc, char **argv, char **azColName)
{
    int i;

    for(i = 0; i < argc; i++)
    {
        printf("%s\n", azColName[i]);
    }
}
const int key_auth_check(unsigned char *key, FILE *fp_auth);
void insert_data_to_db(sqlite3 *db, int rc, unsigned char *kdf_key, char *username)
{
        sqlite3_stmt *stmt;

        // Prepare
        rc = sqlite3_prepare_v2(db, "INSERT INTO credentials(id, username, password), VALUES(?, '?', '?');", -1, &stmt, 0);

        // Bind text
        rc = sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

        // Step
        rc = sqlite3_step(stmt);

        // Finalize
        rc = sqlite3_finalize(stmt);
}

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

    key_auth = key_auth_check(kdf_key, fp); // Do key_auth_check

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
    } else if(key_auth_check == 0) {
        fprintf(stderr, "PRAGMA Key AUTH_FAILED");
        exit(EXIT_FAILURE);
    }

    char *username = "Daniel";

    insert_data_to_db(db, rc, kdf_key, username);

    sqlite3_close(db);
}

/*
 * KEY_AUTH
 */
int key_auth_check(kdf_key, FILE *fp)
{
        fp = popen("sqlcipher test.db", "w");

        char *sprintf_buffer = malloc(128 *sizeof(char));

        /*
         * kdf_key defined from makefile preprocessor
         */
        int pragma_key_auth = sprintf(sprintf_buffer, "PRAGMA key = '%s'", kdf_key);

        if(pragma_key_auth != sizeof(sprintf_buffer))
        {
                fprintf(stderr, "pragma_key_auth: ERROR\n");

                return AUTH_ERROR;

                exit(EXIT_FAILURE);
        } else if(pragma_key_auth == sizeof(sprintf_buffer)) {
                return AUTH_SUCCESS;
        }

        pclose(fp); // Close pipe
}


