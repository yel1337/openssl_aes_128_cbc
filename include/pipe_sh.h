#ifndef pipe_sh_h
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#include <stdio.h> 
#define pipe_sh_h

typedef struct {
	char username[MAX_INPUT_LEN];
	char pphrase[MAX_INPUT_LEN];
} EXPORTED_CREDENTIALS;

extern EXPORTED_CREDENTIALS *exptd_usr_var;
extern EXPORTED_CREDENTIALS *exptd_pphrase_var;

typedef struct { 
	char tmp_username[MAX_INPUT_LEN];
} TMP_STRUCT_BUFF_CREDENTIALS;

extern TMP_STRUCT_BUFF_CREDENTIALS *tmp_stuct_buff_username; 

void  p_write_to_shadow();

#endif
