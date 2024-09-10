#include <stdio.h>
#include <stdlib.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"
#include "/home/yel/openssl_aes_128_cbc/include/prompt/get_h.h"

void p_write_to_shadow()
{
	FILE *txt_file = fopen("/home/yel/openssl_aes_128_cbc/src/etc/shadow.txt", "w");
	
	const char *format_str = "%s: %s";
	
	USER usr_buf;

	unsigned char *sha256_key = key_to_sha256(); 
	
	char *str;
	sprintf(str, format_str, usr_buf.usr, sha256_key);	

	// Write to shadow file 
	fwrite(format_str, sizeof(char), strlen(str), txt_file);

	if(strlen(format_str) < strlen(str))
	{
		printf("fwrite error occured\n");
	}

	if(strlen(format_str) == strlen(str))
	{
		printf("fwrite success\n");
	}

	fclose(txt_file);
}
