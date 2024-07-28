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
    char tmp[TMPMAX];
    unsigned char *deprived_key;
    fp = popen("openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:passwordpassword -kdfopt salt:salt -kdfopt iter:2 PBKDF2", "r");
    if(fp != NULL){
         deprived_key = fgets(tmp, TMPMAX, fp);

         pclose(fp);
    }

    unsigned char *key = deprived_key;

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
    fp = popen("sqlcipher pass_man_db_test.db", "w");

    if(fp)
    {
        perror("popen sqlcipher failed\n");

        exit(EXIT_FAILURE);
    }
    if(fp != NULL)
    {
        const char *PRAGMA_TMP = "PRAGMA key = 'SELECT + \"key\" + FROM + \"key\" + WHERE rowid=1;'";
        size_t write_pragma;
        write_pragma = fwrite(PRAGMA_TMP, sizeof(char), strlen(PRAGMA_TMP), fp);

<<<<<<< HEAD
        if(write_pragma != strlen(PRAGMA_TMP)) // writing err handling
||||||| a291755
        pid_2 = fork();
        if(pid_2 > 0)
=======
        pid_2 = fork(); // Create new process
        if(pid_2 > 0)
>>>>>>> 0b071dee7a46deee9de596d9461c07c6db86d072
        {
            fprintf(stderr, "insert_pragma failed:");

            exit(EXIT_FAILURE);
        }

        pclose(fp);
    }

    sqlite3_close(db);
}

