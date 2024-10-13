# Passphrase prompt 
read -s -p "Enter passphrase: " passphrase
echo 

# openssl rand bytes gen 
salt_rand=$(openssl rand -hex 16)

# SHA256 gen
hashed_val=$(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:"$passphrase" -kdfopt salt:"$salt_rand" -kdfopt iter:2 PBKDF2)

export hashed_val

# write on shadow file
echo "daniel:$hashed_val" > /home/yel/openssl_aes_128_cbc/src/shadow.txt	
