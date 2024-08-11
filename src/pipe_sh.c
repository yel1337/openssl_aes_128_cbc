#include <stdio.h>
#include <stdlib.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#define 

void p_open_create(FILE *cp)
{
	cp = popen("./create_user.sh", "w");

}

void p_open_input(FILE *ip)
{
	FILE *ip; 

	ip = popen("./input.sh", "w");
	
	char *exptd_usr = getenv("username");
	exprtd_usr_var.username = exptd_usr;

	char *exptd_pphrase = getenv("passphrase");
	exprtd_pphrase_var.password = exptd_pphrase; 
}

/*
 * A function to close opened pipe
 */
void p_close_input(FILE *opened_pipe)
{
	pclose(opened_input);
}
