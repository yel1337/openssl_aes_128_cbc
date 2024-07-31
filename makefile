# Variables
kdf_key = $(shell sh -c 'openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:pass -kdfopt salt:salt -kdfopt iter:2 PBKDF2') 
CC = gcc
CFLAGS = -I -L -l -o -D kdf_key = \"$(kdf_key)\"
LFAGS = -lssl -lsqcipher -lssl -lcrypto -l
db = pass_man.db
src = enc.c
EXEC = enc
sqlcipher_cmd = sqlcipher
db_sql = db_setup.sql 

all:
	$(kdf_key)
	$(CC) -o $(EXEC) $(src) -I /usr/local/include/sqlcipher/ -L /usr/local/lib/libsqlcipher.a -lssl -lsqlcipher -lssl -lcrypto -l sqlite3 

# DB Setup
db_setup:
	$(sqlcipher_cmd) $(db) < $(db_sql)	
	PRAGMA key = '$(kdf_key)';  

