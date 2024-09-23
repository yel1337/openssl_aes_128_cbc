
# Source $pphrase & $salt from shell/kdf_src.sh
source ./kdf_src.sh 

# Varible $pphrase = $passphrase  
#
# Derived from prompt/get_pphrase.sh
source prompt/get_pphrase.sh
 
gen_openssl_SHA256()
{ 	
	# $1 as $pphrase
	# $2 as $SHA256
	# $3 as $salt_buf

	hashed_val = $(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$1 -kdfopt salt:$3 -kdfopt iter:2 PBKDF2)

	if[-z "$1"]; then 
		echo "ERR: pphrase empty"
		echo
	if[-z "$3"]; then
		echo "ERR: no rand bytes for salt_buf"
		echo 

	$2 = $hashed_val 
}

free_buf()
{
	local buffer = $1 

	# Free buffer 
	unset buffer 
}

# Call gen_openssl_SHA256 
gen_openssl_SHA256 "$pphrase" "$SHA256" "$salt_buf"
