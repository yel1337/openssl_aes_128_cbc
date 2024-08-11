#ifndef pip_sh_h
#include "/home/yel/openssl_aes_128_cbc/include/length.h"
#define pip_sh_sh

typedef struct {
        char username[MAX_INPUT_LEN];
        char password[MAX_INPUT_LEN];
} EXPORTED_CREDENTIALS;

EXPORTED_CREDENTIALS *exptd_usr_var;
EXPORTED_CREDENTIALS *exptd_pphrase_var;

void p_open_create(); 
void p_open_input();
void p_close_input(FILE *opened_pipe);
#endif
