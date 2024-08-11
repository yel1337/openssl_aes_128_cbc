#ifndef pip_sh_h
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#define pipe_sh_h

typedef struct {
        char username[MAX_INPUT_LEN];
} EXPORTED_CREDENTIALS;

EXPORTED_CREDENTIALS *exptd_usr_var;

typedef struct { 
	char tmp_username[MAX_INPUT_LEN];
} TMP_STRUCT_BUFF_CREDENTIALS;

TMP_STRUCT_BUFF_CREDENTIALS *tmp_stuct_buff_username; 


void p_open_create(); 
void p_open_input();
void p_close_input(FILE *opened_pipe);

#endif
