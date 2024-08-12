#include <stdio.h>
#include <stdlib.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include "/home/yel/openssl_aes_128_cbc/include/hash/ptext_to_sha256.h"

void p_close_input(FILE *open_pipe);
 
# Create user prompt - pipe
void p_open_create(FILE *cp)
{
	cp = popen("./create_user.sh", "w");

	char *exp_usr_cp = getenv("cp_username");	
	
	p_close_input(cp);

	// Convert deprived exported variable from shell into hashed 256sum	
	char sha256_usr = ptext_sha256(exp_usr_cp);

	FILE *fp; 
	fp = fopen("/home/yel/openssl_aes_128_cbc/usr/usr.txt", "w");

	//Write down sha256 digest to txt file 
	fwrite(sha256_usr, sizeof(char), strlen(sha256_usr), fp);	

	fclose(fp);
}

# Normal prompt - pipe  
void p_open_input(FILE *ip)
{
	ip = popen("./input.sh", "w");
	
	char *exptd_usr = getenv("username");
	exprtd_usr_var.username = exptd_usr;

	char *exptd_pphrase = getenv("passphrase");
	exprtd_pphrase_var.pphrase = exptd_usr;
}

/*
 * A function to close opened pipe
 */
void p_close_input(FILE *open_pipe)
{
	pclose(opened_input);
}
