#include <stdio.h>
#include <stdlib.h>
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"

void p_write_to_shadow() // IM THE PROBLEM ITS ME - BUS ERROR 
{
	FILE *txt_file = fopen("/openssl_aes_128_cbc/src/etc/shadow.txt", "w");
	
	char usr[] = "daniel"; // Hardcoded string 

	unsigned char *sha256_key = key_to_sha256(); 
		
	char *str;
	sprintf(str, "%s: %s", usr, &sha256_key);	

	const void *formatted_string = (const void *)str;

	// Write to shadow file
	size_t size;
	size_t count; 
	fwrite(formatted_string, size, count, txt_file);

	fclose(txt_file);
}
