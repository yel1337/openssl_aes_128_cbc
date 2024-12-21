#ifndef kms_h 
#define kms_h
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#define MAX_BYTES 64 

void *key_gen(); 
unsigned char *key_to_sha256();
unsigned char *do_salt_gen();
void key_write(char *kdf);

#endif 
