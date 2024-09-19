#include <stdio.h>
#include <openssl/bio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

void *key_gen() 
{
	FILE *po;

	/*
	 * void p_open_input() from pipe_sh.h should be initialize prior key derivation operation 
	 */
	char openssl_pbkdf2[1000];
	char pphrase[] = "password";
	char salt[] = "salt";

	sprintf(openssl_pbkdf2, "openssl kdf -keylen 16 -kdfopt digest:sha256 -kdfopt pass:%s -kdfopt salt:%s -kdfopt iter:1 PBKDF2", pphrase, salt);
	
	po = popen(openssl_pbkdf2, "r");

	// Close Pipe 
	pclose(po);
}

void key_write(char *kdf)
{
	FILE *fp;  

	fp = fopen("key.txt.", "w");

	fwrite(kdf, sizeof(char), strlen(kdf), fp); 

	fclose(fp);
}

unsigned char *do_salt_gen()
{
	SALT get_salt; 
	unsigned char *salt_buf = malloc(16 * sizeof(char)); 

	RAND_bytes(salt_buf, 16);

	return salt_buf;
}

void free_salt(unsigned char *salt_buffer)
{
	free(salt_buffer);
}

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

