#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "/home/yel/openssl_aes_128_cbc/include/hash/ptext_to_sha256.h"

char ptext_sha256(char *exported_value_from_sh)
{
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const void *data = exported_value_from_sh;
	const EVP_MD *type = EVP_sha256();  
	unsigned char *md; 
	unsigned int*s; 
	size_t cnt; 	

	EVP_DigestInit(mdctx, EVP_sha256());

	EVP_DigestUpdate(mdctx, data, cnt);

	EVP_DigestFinal(mdctx, md, s);

	unsigned int size = (unsigned int)sizeof(s); 

	unsigned char hashed_value; 
	unsigned char hashed_value_len = sizeof(size);
	hashed_value = (unsigned char)md[hashed_value_len];

	return hashed_value;
}




