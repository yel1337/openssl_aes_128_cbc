#include <stdio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"
#include "/home/yel/openssl_aes_128_cbc/include/prompt/get_h.h"
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"

int main()
{
	USER usr_buf;  

	get_usr(); 

	p_write_to_shadow();

}
