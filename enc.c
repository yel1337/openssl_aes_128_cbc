#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h> 
#include <unistd.h>
#include <sys/types.h>
#define SQLITE_HAS_CODEC 1
#define TMPMAX 32
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
static int callback( void *data, int argc, char **argv, char **azColName)
{
    int i;

    for(i = 0; i < argc; i++)
    {
        printf("key: %s\n", argv[i]);
    }
}
// Function prototpes
static int key_auth_check(unsigned char *key, FILE *fp_auth);
const char *db_dump(const char *PRAGMA_TMP, FILE *fp, size_t write_pragma, int rc);

int main()
{
    char *zErrMsg = 0;

    // Key derivation using PBKDF2 from openssl
    unsigned char *key;

    scanf("openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:%s -kdfopt salt:salt -kdfopt iter:2 PBKDF2", &key);

    // Generate IV from CSPRNG
    unsigned char *buf = malloc(16 *sizeof(*buf)); // Rand buffer
    int rand_gen = RAND_bytes(buf, 16);
    unsigned char *rand_iv = (unsigned char *)malloc(16 * sizeof(rand_gen));
    unsigned char *iv = rand_iv;

    // Initialize cipher context
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    // Buffer for ciphered text
    unsigned char ciphertext[128];

    // Buffer for decryptedtext
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphered_len; 

    // Encrypt
    ciphered_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);
	
    // Decrypt
    decryptedtext_len = decrypt(ciphertext, ciphered_len, key, iv, decryptedtext);

    // Add a NULL terminator 
    decryptedtext[decryptedtext_len] = '\0';

    /*
     * SQLite3
     *
     * For every insert operation (or update, delete and select) call sqlite3_step(stmt)
     */
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("pass_man_db.db", &db); // Open db statement

    if(rc == SQLITE_ERROR )
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        return(0);
    } else if(rc == SQLITE_OK) {
        rc = sqlite3_exec(db, "ATTACH DATABASE 'pass_man_db_encrypted.db' AS pass_man_db_encrypted KEY 'key'", 0, 0, &zErrMsg); // Pragma key not yet done

        rc = sqlite3_exec(db, "SELECT sqlcipher_export('pass_man_db_encrypted');");

        rc = sqlite3_exec(db, "DETACH DATABASE pass_man_db_encrypted;");
    }

    // Prepare
    char *sql;
    sqlite3_stmt *stmt;

    // Prepare
    rc = sqlite3_prepare_v2(db, "INSERT INTO key VALUES(?);", -1, &stmt, 0);
    if(rc != SQLITE_OK)
    {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }

    // Bind text
    rc = sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);
    if(rc != SQLITE_OK)
    {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }

    // Step
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE)
    {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }

    // Free
    rc = sqlite3_finalize(stmt);

    FILE *fp;
    static int key_auth_check;
    size_t write_pragma;
    const char *PRAGMA_TMP;
    const char *db_dump;

    key_auth_check = key_auth_check(key, fp, db, rc); // Do key_auth_check

    /*
     * key_auth_check = 0 // AUTH_ERROR
     * key_auth_check = 1 // AUTH_SUCCESS
     */
    if(key_auth_check == 1)
    {
        db_dump = db_dump(PRAGMA_TMP, fp, write_pragma, rc, db);

        return 0;
    } else if(key_auth_check == 0) {
        fprintf(stderr, "PRAGMA Key AUTH_FAILED");
        exit(EXIT_FAILURE);
    }

    sqlite3_close(db);
}

/*
 * AUTH
 */
static int key_auth_check(unsigned char *key, FILE *fp_auth, sqlite3 *db, int rc )
{
        char *zErrMsg = 0;

        fp = popen("sqlcipher pass_man_db_encrypted.db", "w");

        if(fp == NULL)
        {
            perror("popen sqlcipher failed\n"); //

            exit(EXIT_FAILURE);
        }

        if(fp != NULL)
        {
            unsigned char *auth_pragma = "PRAGMA key='%s';", enc_key; // enc_key as PRAGMA key
            size_t write_auth_pragma = fwrite(auth_pragma, sizeof(char), strlen(auth_pragma), fp);

            if(write_auth_pragma != strlen(auth_pragma))
            {
                fprintf(stderr, "auth_pragma failed\n");
            }

            rc = sqlite3_exec(db, "SELECT * FROM key;", callback, 0, &zErrMsg);

            if(rc == SQLITE_ERROR) // If rc == SQLITE_ERROR -> Query will exec || enc_key is not the correct PRAGMA key
            {
                fprintf(stderr, "SQL error: AUTH_FAILED\n");

                exit(EXIT_FAILURE);

                return AUTH_ERROR;
            }
            if(rc == SQLITE_OK)
            {
                return AUTH_SUCCESS;
            }

            pclose(fp); // Close pipe
        }
}
const char *db_dump(const char *PRAGMA_TMP, FILE *fp, size_t write_pragma, int rc, sqlite3 *db;)
{
        rc = sqlite3_exec(db, "SELECT * FROM key;", callback, 0, &zErrMsg);

        if(rc == SQLITE_ERROR)
        {
             fprintf(stderr, "SQL error: ERROR DUMPING DB\n");
        }
}

