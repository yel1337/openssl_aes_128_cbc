#include <stdio.h>
#include <openssl/bio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

/*
 *  Function Deprecated 
 *
 *  key_gen() - digest sha256 hashed value from openssl PBKDF shell
 */
void *key_gen() 
{
	FILE *po;

	char openssl_pbkdf2[1000];
	char pphrase[] = "password";
	char salt[] = "salt";

	sprintf(openssl_pbkdf2, "openssl kdf -keylen 16 -kdfopt digest:sha256 -kdfopt pass:%s -kdfopt salt:%s -kdfopt iter:1 PBKDF2", pphrase, salt);
	
	po = popen(openssl_pbkdf2, "r");

	pclose(po); // Close FILE* pipe 
}

void do_salt_gen(unsigned char *env_val, unsigned char *salt_bytes)
{
	salt_bytes = (unsigned char *)malloc(16 * sizeof(char));
	RAND_bytes(salt_bytes, 16);
	
	env_val = salt_bytes;  
}

/*
 * Function Deprecated
 *
 * key_to_sha256() - convert deprived key into SHA-256
 *
 * no longer recommended as "openssl_aes_128_cbc/src/main.sh" digest 
 * SHA-256 and write on file   
 */
unsigned char *key_to_sha256()
{
	EVP_MD_CTX *mdctx =  EVP_MD_CTX_new();
	const EVP_MD *md = EVP_sha256();
	unsigned char *md_value = (char *)malloc(100 * sizeof(char));
    	unsigned int *s;
    	size_t cnt;

	char *data = key_gen();

	EVP_MD_CTX_init(mdctx);
	
	EVP_DigestInit_ex(mdctx, md, NULL); 

	EVP_DigestUpdate(mdctx, &data, cnt);

    	EVP_DigestFinal_ex(mdctx, md_value, s);

    	return md_value;
}

