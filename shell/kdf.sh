
# Source $pphrase & $salt from shell/kdf_src.sh
source ./kdf_src.sh 

# Salt buf 
#
# salt_buf as argv[1]
#
salt_buf = $1 
 
gen_openssl_SHA256()
{ 	
	hashed_val = $(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$passphrase -kdfopt salt:$salt_buf -kdfopt iter:2 PBKDF2)

	hashed_val = $SHA256
}

free_buf()
{
	local buffer = $1 

	# Free buffer 
	unset buffer 
}

exec_salt_gen() 
{
	
}
