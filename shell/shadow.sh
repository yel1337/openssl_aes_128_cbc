# as source file 
source /home/yel/openssl_aes_128_cbc/shell/kdf_src.sh
source /home/yel/openssl_aes_128_cbc/shell/exec.sh
source /home/yel/openssl_aes_128_cbc/shell/get_pphrase.sh
	
# Shadow txt path	
file_txt = (/home/yel/openssl_aes_128_cbc/src/shadow.txt)

write_on_file() {
	echo "$SHA256" > /home/yel/openssl_aes_128_cbc/src/shadow.txt
}

credential_check_in_file()
{
	usr_check=$(grep "$username" $file_txt) 	

# 0 = Text found in file 
# 1 = Text was not found 
# 2 error (file did not exist or syntax error) 
	
	if usr_check = 0; then
		return 0
  
	if usr_check = 1; then
		return 1
 
	if usr_check = 2; then
		return 2
}

