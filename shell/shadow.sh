# as source file 
source shell/kdf_src.sh
source shell/kdf.sh
	
# Shadow txt path	
file_txt = (/home/yel/openssl_aes_128_cbc/src/shadow.txt)

write_on_file()
{
        # $2 as $SHA256

        echo "$usr:"
	if[-z "$2"]; then 
		echo "ERR: usr struct empty"
		echo 
	
	# IF EXIST OR ERR  
	if[credential_check_in_file == 0]; then 
		echo "ERR: usr already exist"
		echo ""
	
	if[credential_check_in_file == 2]; then 
		echo "ERR: txt file"
		echo ""
	 
	# $SHA256 from "../kdf_src.sh"
	#
	#  IF credential_check_in_file == 1 
	#  then write in txt file 
	
	if[-z "$2"]; then 
		echo "ERR: SHA256 empty"
		echo 

	if(credential_check_in_file == 1); then
		echo $usr":"

		# "$2" as $SHA256  
		echo "$2" > $file_txt

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

# Exec write to shadow file function
#
# Write SHA256 on file 
write_on_file "$SHA256" 
