# Variables
password := $(shell bash -c 'read -s -p "password:" pwd; echo $$pwd')
kdf_key = $(shell bash -c 'openssl kdf -keylen 32 -kdfopt digest:SHA256 -kdfopt pass:$(password) -kdfopt salt:salt -kdfopt iter:2 PBKDF2')
CC = gcc
CFLAGS = -I -L -l -o
LFAGS = -lssl -lsqcipher -lssl -lcrypto -l
db = pass_man.db
src = enc.c
EXEC = enc
sqlcipher_cmd = sqlcipher
db_sql = db_setup.sql

all:
	echo $(password)
	$(kdf_key)
	$(CC) -o $(EXEC) $(src) -I /usr/local/include/sqlcipher/ -L /usr/local/lib/libsqlcipher.a -lssl -lsqlcipher -lssl -lcrypto -l sqlite3 -Dkdf_key=$(kdf_key)

# DB Setup
db_setup:
	$(sqlcipher_cmd) $(db) < $(db_sql)	
	PRAGMA key = '$(kdf_key)';  
clean:
	rm -rf $(EXEC)


