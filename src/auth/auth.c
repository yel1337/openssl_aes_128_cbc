#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/include/auth.h"

/*
 * Compare two strings
 */
int usncmp(const char *lhs, const char *rhs)
{
	int cmp = stcmp(lhs, rhs);

	return cmp;
}

/* Check if username string exist on file
 *
 * -- Function Macros -- 
 * If username_if_exist is equal to 1 = EXIST = / USER ALREADY EXIST 
 * If username_if_exist is equal to 0 = N_EXIST / USER NOT EXIST YET
 *
 */
int username_if_exist(char *username)
{	
	FILE *fp; 
	char sprintf_buffer[100];
	char string_to_hash_buffer[100]; 	
	char hashed_username[100]; 

	int string_to_hash = sprintf(sprintf_buffer, "echo \"%s\" | sha256sum", username);

	fp = popen(sprintf_buffer, "r");
	
	hashed_username = fgets(string_to_hash_buffer, sizeof(string_to_hash_buffer), fp);
		
	pclose(fp);	

	if(string_to_hash != strlen(string_to_hash))
	{
		printf("%s\n", sterror(errno));
	}

	char fgets_buffer[32]; 
	
	fp = fpopen("/home/yel/openssl_aes_128_cbc/usr.txt", "r"); 
	
	// Run through txt file 	
	while(fgets(fgets_buffer, sizeof(fgets_buffer), fp) != sizeof(fgets_buffer))
	{
		const *input_username = (const char *)username;
		const *stored_username = (const char *)fgets_buffer;
		
		int rs_cmp = usncmp(input_username, stored_username); 
		
		if(rs_cmp == 1)
		{
			return EXIST; 
		} else if(rs_cmp = 0) {
		 	return  N_EXIST; 
		}
	}
	
	fclose(fp);
}
int do_auth_check(int len_sig)
{
  if(len_sig == 1)
        {

                if(username_if_exist(export_usn) == 0)
                {
                        printf("Create user? ");
                        scanf("%c\n", &opt);

                        if(opt == 'y')
                        {
                                fp = popen("create_user.sh", "w");
                                pclose(fp);
                        }
                        if(opt == 'n')
                        {
                                exit(0);
                        }
                } else if(username_if_exist(export_usn) == 1) {
                        // SUCCESS -> ??? PASSWORD AUTH?
                }

        } else if(len_sig == 0) {
                char err_formatted_string = "username";

                len_re_enter(err_formatted_string);
        }
}
