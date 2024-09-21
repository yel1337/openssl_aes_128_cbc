# as source file 
source ./kdf_src.sh

# Shadow txt file
file_txt = (/home/yel/openssl_aes_128_cbc/src/shadow.txt)

write_on_file()
{
	# $usr from "../kdf_src.sh"    
	echo "$usr:"
	
	if($usr == ""); then 
		echo "ERR: usr empty"
		echo 
	
	# IF EXIST OR ERR  
	if(credential_check_in_file() == 0); then 
		echo "ERR: usr already exist"
		echo ""
	
	if(credential_check_in_file() == 2); then 
		echo "ERR: txt file"
		echo ""
	 
	# $SHA256 from "../kdf_src.sh"
	#
	#  IF credential_check_in_file == 1 
	#  then write in txt file 
	
	if($SHA256 == ""); then 
		echo "ERR: sha256 empty"
		echo 

	if(credential_check_in_file() == 1); then
		echo $usr":"  
		echo $SHA256 > $file_txt

	free_buf $usr 	
	free_buf $SHA256 	
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
