
# Source $pphrase & $salt from shell/kdf_src.sh
source ./kdf_src.sh 

# Varible $pphrase = $passphrase  
#
# Derived from prompt/get_pphrase.sh
source prompt/get_pphrase.sh
 
gen_openssl_SHA256()
{ 	
	hashed_val = $(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$pphrase -kdfopt salt:$salt_buf -kdfopt iter:2 PBKDF2)

	if(!$pphrase); then 
		echo "ERR: pphrase empty"
		echo
	if(!$salt_buf); then
		echo "ERR: no rand bytes for salt_buf"
		echo 

	$SHA256 = $hashed_val 
}

free_buf()
{
	local buffer = $1 

	# Free buffer 
	unset buffer 
}

# Call gen_openssl_SHA256 
gen_openssl_SHA256
