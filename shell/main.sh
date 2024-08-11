
# KDF  
key=$(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$password -kdfopt salt:salt -kdfopt iter:2 PBKDF2)

# Hashed username
kdf_key=$(echo "$username" | sha256sum | awk '{print $1}')

# Hashed password 
derived_key=$(echo "$key" | sha256sum | awk '{print $1}')



