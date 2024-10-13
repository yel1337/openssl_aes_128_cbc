source /home/yel/openssl_aes_128_cbc/shell/kdf_src.sh

get_pphrase()
{
	# Passphrase prompt 
	read -s -p "Enter passphrase: " passphrase

	eval "$pphrase=\"$passphrase\""
}

