
# KDF  
hashed_sha256=$(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:password -kdfopt salt:salt -kdfopt iter:2 PBKDF2)

echo $hashed_sha256 > shadow.txt

