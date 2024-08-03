read -s -p "Enter password: " input 
echo 

key=$(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$input -kdfopt salt:salt -kdfopt iter:2 PBKDF2)
echo "$key" | openssl enc -aes-256-cbc -salt -out keyfile.enc -pass pass:password


