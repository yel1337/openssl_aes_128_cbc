#ifndef kms_h 
#define kms_h
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#define MAX_BYTES 64 

typedef struct {
	unsigned char *generated_salt;
} SALT;  

typedef struct {
	uint32_t key_array[MAX_BYTES];
	int len_key_array; 
	uint32_t array_key_index[MAX_BYTES]; 
} INDEX;  

char *key_gen(); 
unsigned char *key_to_sha256();
void free_salt(unsigned char *salt_buffer); 
unsigned char *do_salt_gen();
void key_write(char *kdf);

#endif 
