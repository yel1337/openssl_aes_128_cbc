#ifndef length_h
#define length_h
#define MAX_LENGTH_EXCEEDED 2
#define LEN_SHRT 0 
#define MAX_INPUT_LEN 16
#define LEN_OK 1 
#define SUCCESS 1

typedef struct{
	char username[MAX_INPUT_LEN];
	char password[MAX_INPUT_LEN];
} credentials;

int len_re_enter(char formatted_string);
int len_username(char *export_usn); 
int len_passphrase(char *export_pphrase);

#endif 
