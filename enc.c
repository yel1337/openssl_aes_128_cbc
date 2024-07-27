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

int main()
{
    unsigned char *plaintext = (unsigned char *)"passwordpassword";
    FILE *fp;

    // Key derivation using PBKDF2 from openssl
    fp = popen("openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:passwordpassword -kdfopt salt:salt -kdfopt iter:2 PBKDF2", "r");
    char tmp[TMPMAX];
    unsigned char *key = fgets(tmp, TMPMAX, fp);

    // Generate IV from CSPRNG
    unsigned char *buf; // Rand buffer
    int rand_gen = RAND_bytes(buf, 16);
    unsigned char *rand_iv = (unsigned char *)malloc(sizeof(rand_gen));
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
    rc = sqlite3_open("pass_man_db_test.db", &db); // Open db statement
    if(rc == SQLITE_ERROR ) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        return(0);
    }

    // Prepare
    char *sql;
    char *zErrMsg = 0;
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

    // Insert PRAGMA key
    pid_t pid_1, pid_2;
    pid_1 = fork(); // Create new process

    if(pid_1 < 0) // Err handling
    {
        perror("fork() failed");

        exit(EXIT_FAILURE);
    }
    if(pid_1 == 0) // Child process
    {
        fp = popen("PRAGMA key = ", "r");
        if(fp == NULL)
        {
            perror("popen error\n");
        }

        pid_2 = fork(); // Create new process
        if(pid_2 > 0)
        {
            const char *sql = "'SELECT + \"key\" + FROM + \"key\" + WHERE rowid=1;'";
            rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
            if( rc == SQLITE_ERROR)
            {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }

        }
        else if(pid_2 == 0)
        {
            fp = popen(".q", "r");
        }
    }
    if(pid_1 > 0) // Parent process
    {
        fp = popen("sqlcipher pass_man_db_test.db", "r");

        if(fp == NULL )
        {
            perror("popen error\n");

            exit(EXIT_FAILURE);
        }
    }

    sqlite3_close(db);
}

