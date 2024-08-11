#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "include/inc.h"

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
