#ifndef auth_h
#define auth_h 
#define AUTH_CMP_CHECK 1 
#define AUTH_CMP_FALSE 0 
#define EXIST 1 
#define N_EXIST 0

int usncmp(const char *lhs, const char *rhs);
int username_if_exist(char *username);
int do_len_check(int len_sig);

#endif 

