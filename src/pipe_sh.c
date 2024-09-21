#include <stdio.h>
#include <stdlib.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

/*
 * Function Deprecated
 *
 * p_write_to_shadow - allows FILE stream to write on file
 *
 */
void p_write_to_shadow() 
{
	FILE *txt_file = fopen("shadow.txt", "w");

	unsigned char *sha256_key = key_to_sha256(); 
	
	char buf[1000];
	sprintf(buf, "%s: %s", usr, &sha256_key);

	fputs(buf, txt_file);	

	fclose(txt_file);
}
