#include <stdio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

char key_gen(EXPORTED_CREDENTIALS *exptd_pphrase_var) 
{
	FILE *po;
	SALT *get_salt; 
	char kdf_buffer[16]; 

	do_salt_gen();
	unsigned char salted = get_salt->generated_salt; 
	
	/*
	 * void p_open_input() from pipe_sh.h should be initialize prior key derivation operation 
	 */
	char openssl_pbkdf2;
	char pphrase_var = exptd_pphrase_var.pphrase;

	sprintf(openssl_pbkdf2, "openssl kdf -keylen 16 -kdfopt digest:sha256 -kdfopt pass:%s -kdfopt salt:%s-kdfopt iter:1 PBKDF2", pphrase_var, salted);
	po = popen(openssl_pbkdf2, "r");

	// Read output from file stream 
	kdf_buffer = fgets(kdf_buffer, sizeof(kdf_buffer), po);

	// Close Pipe 
	pclose(po);

	return kdf_buffer; 
}

void key_write(char kdf)
{
	FILE fp; 
	char *deprived_key = (char *) kdf; 

	fp = fopen("key.txt.", "w");

	fwrite(deprived_key, sizeof(char), strlen(deprived_key), fp); 

	fclose(fp);
}

void do_salt_gen()
{
	SALT *get_salt; 
	unsigned char *salt_buf = malloc(16 * sizeof(char)); 

	RAND_bytes(salt_buf, 16);

	get_salt->generated_salt = salt_buf;
}

void free_salt(unsigned char *salt_buffer)
{
	free(salt_buffer);
}

char key_to_sha256();
{
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *type = EVP_sha256();
    unsigned char *md;
    unsigned int*s;
    size_t cnt;

	char get_key = ket_gen();
	const void *data = get_key;

	EVP_DigestInit(mdctx, EVP_sha256()); 

	EVP_DigestUpdate(mdctx, data, cnt);

    EVP_DigestFinal(mdctx, md, s);

	unsigned char hashed_value;
    unsigned char hashed_value_len = sizeof(size);
    hashed_value = (unsigned char)md[hashed_value_len];

    return hashed_value;
}
