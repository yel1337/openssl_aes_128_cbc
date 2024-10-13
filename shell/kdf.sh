
set -x 

hashed_val=""

genSHA256() {
	hashed_val=$(openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:"$passphrase" -kdfopt salt:test -kdfopt iter:2 PBKDF2)
}


