#include <stdio.h>
#include "/home/yel/openssl_aes_128_cbc/include/kms_h/kms.h"
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"

int main()
{
	FILE *fp_sh;

	fp_sh = popen("./main.sh", "w");

	pclose(fp_sh); 
}
