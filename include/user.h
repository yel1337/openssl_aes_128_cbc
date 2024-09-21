#ifndef USER_H
#define USER_H
#define USER_LEN_MAX 10
#define PPHRASE_LEN_MAX 20

typedef struct credentials
{
	char *user_buf[USER_LEN_MAX];
	char *pphrase_buf[PPHRASE_LEN_MAX];
} CREDENTIALS; 

#endif
