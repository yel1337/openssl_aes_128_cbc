#ifndef kms_h 
#define kms_h
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"

typedef struct {
	int generated_salt;
} SALT;  

char key_gen(); 
char key_to_sha256();
void free_salt(unsigned char *salt_buffer); 
void do_salt_gen();
void key_write(char kdf);

#endif 
