# Variables
CC = gcc
INPUT_SHELL = ./get_input.sh
db = pass_man.db
sqlcipher_cmd = sqlcipher
db_sql = db_setup.sql
PROMPT_COMPILE = gcc -o prompt /home/yel/openssl_aes_128_cbc/src/prompt.c  
COMPILE = gcc -o enc enc.c -I /usr/local/include/sqlcipher/ -L /usr/local/lib/libsqlcipher.a -lsqlcipher -lssl -lcrypto -l sqlite3 -g  

all:
	 gcc -o enc enc.c -I /usr/local/include/sqlcipher/ -L /usr/local/lib/libsqlcipher.a -lsqlcipher -lssl -lcrypto -l sqlite3 -g

prompt:
	

# DB setup 
db_setup:
	$(sqlcipher_cmd) $(db) < $(db_sql)	
clean:
	rm -rf $(EXEC)


