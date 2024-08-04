#Prompt
username="daniel"
password="password"
echo
# KDF command 
key=$(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$password -kdfopt salt:salt -kdfopt iter:2 PBKDF2)

# Hashed username
uname=$(echo "$username" | sha256sum | awk '{print $1}') 
kdf_key=$(echo "$key" | sha256sum | awk '{print $1}')

export deprived_key="$kdf_key"
gcc -o enc enc.c -I /usr/local/include/sqlcipher/ -L /usr/local/lib/libsqlcipher.a -lsqlcipher -lssl -lcrypto -l sqlite3 -g
./enc
