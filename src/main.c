#include <stdio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"

int main(int argc, char *argv[])
{

	/*
 	 *  SHA256 digest 
         */
	do_salt_gen(argv[1]);

	// Call exec.sh to also call 3 functions to digest SHA256 
	FILE *pipe_exec = popen("shell/exec.sh", "w");
	
	// Free do_salt_gen() buffer after use 	
	free_salt(argv[1]);	
	
	pclose(pipe_exec);
}
