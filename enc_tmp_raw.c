#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h> //Sqlite header
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
	
	BIO_dump_fp(stdout, (const char *)ciphertext, ciphered_len);	

    // Decrypt
    decryptedtext_len = decrypt(ciphertext, ciphered_len, key, iv, decryptedtext);

	// Add a NULL terminator 
	decryptedtext[decryptedtext_len] = '\0';

	// Print decrypted text
	printf("%s\n", decryptedtext);

    // Create sqlite3 database
    sqlite3 *db;
    int rc;
    rc = sqlite3_open("pass_man_db_test.db", &db); // Create db name "pass_man_db_test"

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // Inserting pbkdf2 key to Sqlite3 table
    char *sql;
    char *zErrMsg = 0;

    sqlite3_stmt *stmt;
    sql = "INSERT INTO pbkdf_key VALUES(?)"; // SQL Statement

    // Prepare
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if(rc != SQLITE_OK)
    {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }

    // Bind
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
    sqlite3_close(db);
}

