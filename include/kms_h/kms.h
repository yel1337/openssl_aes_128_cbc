#ifndef kms_h 
#define kms_h
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#define MAX_BYTES 64 

void *key_gen(); 
unsigned char *key_to_sha256();
void do_salt_gen(unsigned char *env_val, unsigned char *salt_bytes);
void key_write(char *kdf);

#endif 
