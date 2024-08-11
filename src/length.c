#include <stdio.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include "/home/yel/openssl_aes_128_cbc/include/pipe_sh.h"

char len_err(char formatted_string, int signal)
{
	switch(signal) {
		case 0:
			printf("entered %s too short\n", formatted_string);
			break;
		case 2:
			printf("maximum characters entered exceeded\n");
		break;
	}
}
int len_re_enter(char formatted_string)
{
	FILE *po; // Pipe open
	
	switch(formatted_string) {
		case "username":
			p_open_input(po);

			len_username(exptd_usr_var);

			if(len_username == 0)
			{
				return len_err(formatted_string, 0);	
			} 
			if(len_username == 1) 
			{
				return SUCCESS;
			} 
			if(len_username == 2) 
			{
				return len_err(formatted_string, 2); 
			}
			
			pclose(fp);
			break;
		case "password":
			p_open_input(po);

			length_passphrase(exptd_pphrase_var);

			if(len_passphrase == 0)
			{
				return LENGTH_SHRT;
			} 
			if(len_passphrase == 1) 
			{
				return SUCCESS;
			} 
			if(len_passphrase == 2) 
			{
				return MAX_LENGTH_EXCEEDED; 
			}
			
			p_close_input(po); 
			break;
	}
}

/* 
 * -- Function Macros -- 
 * LENGTH_SHRT = 0 
 * LENGTH_OK = 1
 * MAX_LENGTH_EXCEEDED = 2 
 */
int len_username(EXPORTED_CREDENTIALS *exptd_usr_var;)
{
	if(strlen(exptd_usr_var) > MAX_INPUT_LENGTH)
	{
		return MAX_LENGTH_EXCEEDED;
	} else if(strlen(exptd_usr_var) < 3) {
		return LENGTH_SHRT; 
	} else {
		return LENGTH_OK;
	}
}

int len_passphrase(EXPORTED_CREDENTIALS *exptd_pphrase_var;)
{
	if(strlen(exptd_pphrase_var) > MAX_INPUT_LENGTH)
        {
                return MAX_LENGTH_EXCEEDED;
        } else if(strlen(exptd_pphrase_var) < 3){
                return LENGTH_SHRT;
        } else {
		return LENGTH_OK;
	}
}
