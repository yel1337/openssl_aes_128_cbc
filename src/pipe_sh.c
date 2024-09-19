#include <stdio.h>
#include <stdlib.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

void p_write_to_shadow()  
{
	FILE *txt_file = fopen("shadow.txt", "w");

	if(!txt_file)
	{
		printf("file opening failed\n");
	}

	const char *usr = "daniel"; // Hardcoded string 

	unsigned char *sha256_key = key_to_sha256(); 
	
	//char *buf = (char *)malloc(100 * sizeof(char));
	char buf[1000];
	sprintf(buf, "%s: %s", usr, &sha256_key);

	fputs(buf, txt_file);	

	fclose(txt_file);
}
