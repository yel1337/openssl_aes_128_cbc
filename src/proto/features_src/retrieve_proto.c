#include <stdio.h>
#include <sqlite3.h>
#include </home/yel/openssl_aes_128_cbc/src/proto/features_include/retrieve_proto_header.h>

void open_db(const char *db_file, sqlite3 **db)
{
    int sql_open = sqlite3_open(db_file, db);

    if(sql_open == 0){
        
        printf("sqlite3_open: OK\n");
    } else if(sql_open == 1) {
        
        printf("sqlite3_open: ERROR\n");
    }
}

void close_db(sqlite3 *db)
{
    sqlite3_close(db);
}

void ret_cred()
{
    const char *db_file = "cred_db";
    const char *firstCol = "SELECT website, username, pass FROM credentials LIMIT 1;";

    sqlite3_stmt *stmt;
    sqlite3 *db;

    open_db(db_file, &db);

    int sqlite_prepare = sqlite3_prepare_v2(db, firstCol, -1, &stmt, NULL);
    if(sqlite_prepare != SQLITE_OK){
        printf("%s\n", sqlite3_errmsg(db));
    }

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        /*  sqlite3_column_text are use to get string values from results
            by using iCol as column's index  
            
            specific rows can be fetched by using OFFSETS   
            
            columns and rows are two diffrent thing     */
        const unsigned char *f_index = sqlite3_column_text(stmt, 0); /* first column starts at 0 */
        const unsigned char *s_index = sqlite3_column_text(stmt, 1); 
        const unsigned char *t_index = sqlite3_column_text(stmt, 2);

        /*  Example outputs: 

            each variables are named according to their ascending indexes */
        printf("%s ", f_index);
        printf("%s ", s_index);
        printf("%s\n", t_index);
    }

    sqlite3_finalize(stmt);

    const char *followR = "SELECT website, username, pass FROM credentials LIMIT 1 OFFSET ?;";

    for(int index_val = 1; index_val <= 2; index_val++){
        sqlite3_prepare_v2(db, followR, -1, &stmt, NULL);

        sqlite3_bind_int(stmt, 1, index_val);

        while(sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *follow_var_f = sqlite3_column_text(stmt, 0);
            const unsigned char *follow_var_s = sqlite3_column_text(stmt, 1);
            const unsigned char *follow_var_t = sqlite3_column_text(stmt, 2);

            printf("%s ", follow_var_f);
            printf("%s ", follow_var_s);
            printf("%s\n", follow_var_t);
        }
    }

    close_db(db);  
}