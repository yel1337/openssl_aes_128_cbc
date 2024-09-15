#include <stdio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

char *key_gen() 
{
	FILE *po;
	char *kdf_buffer; 

	/*
	 * void p_open_input() from pipe_sh.h should be initialize prior key derivation operation 
	 */
	char openssl_pbkdf2[1000];
	char pphrase[] = "password";
	char salt[] = "salt";

	sprintf(openssl_pbkdf2, "openssl kdf -keylen 16 -kdfopt digest:sha256 -kdfopt pass:%s -kdfopt salt:%s -kdfopt iter:1 PBKDF2", pphrase, salt);
	
	po = popen(openssl_pbkdf2, "r");

	int count = sizeof(kdf_buffer);

	// Read output from file stream 
	fgets(kdf_buffer, count, po);

	// Close Pipe 
	pclose(po);

	return kdf_buffer;
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
    	const EVP_MD *EVP_sha256(void);
	const EVP_MD *md;
	unsigned char *md_value;
    	unsigned int *s;
    	size_t cnt;

	char *data = key_gen();

	EVP_MD_CTX_init(mdctx);
	
	EVP_DigestInit(mdctx, EVP_sha256()); 

	EVP_DigestUpdate(mdctx, &data, cnt);

    	EVP_DigestFinal(mdctx, md_value, s);

    	return md_value;
}

