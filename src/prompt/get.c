#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"
#include "/home/yel/openssl_aes_128_cbc/include/prompt/get_h.h"

int get_usr()
{
	USER usr_buf;

	printf("username: ");
	scanf("%s\n", usr_buf.usr);
	
	return 1; 
}


