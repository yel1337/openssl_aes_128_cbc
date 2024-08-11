#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>i
#include "include/dec.h"

int decrypt(unsigned char *ciphertext, int ciphered_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{	        
	EVP_CIPHER_CTX *ctx;

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

    	EVP_CIPHER_CTX_free(ctx);

        return plaintext_len;
}
