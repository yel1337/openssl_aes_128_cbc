# Variables
kdf_key = $(shell sh -c 'echo -n "Enter password: " && read -s key && echo && openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$$key -kdfopt salt:salt -kdfopt iter:2 PBKDF2')
CFLAGS = -D kdf_key = \"$(kdf_key)\"
db = pass_man.db
sqlcipher_cmd = ./sqlcipher
db_sql = db_setup.sql 

# DB Setup
db_setup:
	$(sqlcipher_cmd) $(db) < $(db_sql)	
	PRAGMA key = '$(kdf_key)';  

