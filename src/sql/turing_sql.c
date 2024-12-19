#define SQLITE_HAS_CODEC 1 
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/yel/openssl_aes_128_cbc/src/sql/turing_sql.h"

/*
 * The use of sqlite3_stmt *stmt should not be complicated and avoid multiple declarations of *stmt 
 */
 
int check_db_err(sqlite3 *db, const char *dbN,const char *key)
{
        const char *command = "SELECT * FROM sqlite_master WHERE type = 'table';";
        sqlite3_stmt *stmt;
	int rc; 

        rc = sqlite3_open(dbN, &db);
        if(rc != SQLITE_OK) {
                printf("%s\n", sqlite3_errmsg(db));
        }

        rc = sqlite3_key(db, key, strlen(key));
        if(rc != SQLITE_OK) {
                 printf("%s\n", sqlite3_errmsg(db));
        }

        rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
                char err = printf("%s\n", sqlite3_errmsg(db));

                sqlite3_close(db);
         }
 
        if (sqlite3_step(stmt) == SQLITE_ROW) {
                return CHECK_DB_MACRO_HAS_ROW;
        } else {
                return CHECK_DB_MACRO_HAS_NO_ROW;
        }
}

static char tb_err(sqlite3 *db)
{	
	char tb_err_msg = printf("get_tb(): %s \n", sqlite3_errmsg(db));

	return tb_err_msg;
}

const char *get_tb(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt)
{
        static int rc;
        struct TABLE *tb; 

        sqlite3_busy_timeout(db, 2500);

        rc = sqlite3_prepare_v2(db, "SELECT name FROM sqlite_master WHERE type = 'table';", -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
		tb_err(db);
        }

        const char *tb_t; 
        if (sqlite3_step(stmt) == SQLITE_ROW) {
                tb_t = sqlite3_column_text(stmt, 0);

                return tb_t;

                sqlite3_finalize(stmt);
        } else {
                printf("get_tb: no row returned.\n");
        }
        sqlite3_close(db);
}

static char column_err(sqlite3 *db)
{
        char column_err_msg = printf("%s \n", sqlite3_errmsg(db));

        return column_err_msg;
}

// MODIFY 
const unsigned char *get_column(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt, const char *table, int index) 
{
        const char *wCol;
        const char *uCol;
        const char *pCol;
        int rc;

        sqlite3_busy_timeout(db, 2500);

        /*WEBSITE*/
        if(index == 0){
                const char *column_buf_f ="SELECT name FROM pragma_table_info(?) WHERE cid = 0;";

                rc = sqlite3_prepare_v2(db, column_buf_f, -1, &stmt, NULL);
                if(rc == SQLITE_OK) {
                        sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT);
 
                        while(sqlite3_step(stmt) == SQLITE_ROW){
                                wCol = (const char *) sqlite3_column_text(stmt, 0);

                                return wCol; 
                        }
                        sqlite3_finalize(stmt); 
                }
        }
        /*USERNAME*/
        else if(index == 1){
                const char *column_buf_l ="SELECT name FROM pragma_table_info(?) WHERE cid = 1;";

                sqlite3_busy_timeout(db, 2500);

                rc = sqlite3_prepare_v2(db, column_buf_l, -1, &stmt, NULL);
                if(rc == SQLITE_OK) {
                        sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT);
 
                        while(sqlite3_step(stmt) == SQLITE_ROW){
                                uCol = (const char *) sqlite3_column_text(stmt, 0);

                                return uCol; 
                        }
                        sqlite3_finalize(stmt);
                }
        } 
        /*PASSWORD*/
        else if(index == 2){
                const char *column_buf_l ="SELECT name FROM pragma_table_info(?) WHERE cid = 2;";

                sqlite3_busy_timeout(db, 2500);

                rc = sqlite3_prepare_v2(db, column_buf_l, -1, &stmt, NULL);
                if(rc == SQLITE_OK) {
                        sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT);
 
                        while(sqlite3_step(stmt) == SQLITE_ROW){
                                pCol = (const char *) sqlite3_column_text(stmt, 0);

                                return pCol; 
                        }
                        sqlite3_finalize(stmt);
                }
        } 
        sqlite3_close(db);
}

static char insert_err(sqlite3 *db)
{
	char insert_err_msg = printf("%s \n", sqlite3_errmsg(db));

        return insert_err_msg;

}

void *insert_into(sqlite3 *db, const char *dbN, const char *key, sqlite3_stmt *stmt, const char *tablename, const unsigned char *wCol, const unsigned char *uCol, const unsigned char *pCol)
{

        struct DATA dt; 

        int rc; 

        char buf[1000];
        sprintf(buf, "INSERT INTO %s (%s, %s, %s) VALUES (?, ?, ?);", tablename, wCol, uCol, pCol);        
        printf("%s\n", buf);

        sqlite3_busy_timeout(db, 2500);

        rc = sqlite3_prepare_v2(db, buf, -1, &stmt, NULL);
        if (rc == SQLITE_OK){
                printf("insert_into PREPARE: OK\n");

                // Website VALUE bind
                int w_bind = sqlite3_bind_text(stmt, 1, dt.website, -1, SQLITE_TRANSIENT);

                if(w_bind != SQLITE_OK){
                        printf("f_bind: NOT OK\n");
                } 
                else if(w_bind == SQLITE_OK){
                        printf("w_bind: OK\n");
                }

                // Username VALUE bind
                int u_bind = sqlite3_bind_text(stmt, 2, dt.username, -1, SQLITE_TRANSIENT);

                if(u_bind != SQLITE_OK){
                        printf("u_bind: NOT OK\n");
                } 
                else if(u_bind == SQLITE_OK){
                        printf("u_bind: OK\n");
                }

                // Password VALUE bind
                int p_bind = sqlite3_bind_text(stmt, 3, dt.pass, -1, SQLITE_TRANSIENT);

                if(p_bind != SQLITE_OK){
                        printf("p_bind: NOT OK\n");
                } 
                else if(p_bind == SQLITE_OK){
                        printf("p_bind: OK\n");
                }


                // STEP
                int insert_step = sqlite3_step(stmt);

                if(insert_step == SQLITE_DONE){
                        printf("insert_step: OK\n");
                }
                else if(insert_step != SQLITE_DONE){
                        printf("%s \n", sqlite3_errmsg(db));
                        printf("insert_step: NOT OK\n");

                        sqlite3_finalize(stmt);
                }
        }
        else if(rc != SQLITE_OK){
                insert_err(db);
        }
        sqlite3_close(db);
}

static char ret_err(sqlite3 *db)
{
	char ret_err_msg = printf("%s \n", sqlite3_errmsg(db));

	return ret_err_msg;
}

void *ret(sqlite3 *db, sqlite3_stmt *stmt, const char *table, const unsigned char *wCol, const unsigned char *uCol, const unsigned char *pCol, int num_of_rows)
{
        int rc;

	static char first_index_buf[BUF_SIZE];

        static const unsigned char *result_web; 
        static const unsigned char *result_user; 
        static const unsigned char *result_pass; 

        sprintf(first_index_buf, "SELECT * FROM %s LIMIT 1;", table); /* Query for first row of results */

        rc = sqlite3_prepare_v2(db, first_index_buf, -1, &stmt, NULL);

        while(sqlite3_step(stmt) == SQLITE_ROW){

                /* 
                 * const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
                 * iCol is the index of a result from sqlite3_prepare_v2
                 * 
                 * indexes normally starts at 0 as for first index of the result
                 *
                 * if iCol = 0 then it means iCol is looking for the first column of the result and so forth... 
                 */
                        result_web = sqlite3_column_text(stmt, 0);
                        result_user = sqlite3_column_text(stmt, 1);
                        result_pass = sqlite3_column_text(stmt, 2);
                                
                        printf("%s ", result_web);
                        printf("%s ", result_user);
                        printf("%s\n", result_pass);
        }

        sqlite3_finalize(stmt);

        static char following_indexes_buf[1000];

        static const unsigned char *wColN;        
        static const unsigned char *uColN;        
        static const unsigned char *pColN;        

        for(int x = 1; x <= num_of_rows; x++){
                sprintf(following_indexes_buf, "SELECT %s, %s, %s FROM %s LIMIT 1 OFFSET %d;", wCol, uCol, pCol, table, x); /* Following results queries */

                sqlite3_prepare_v2(db, following_indexes_buf, -1, &stmt, NULL);

                while(sqlite3_step(stmt) == SQLITE_ROW) {
                        wColN = sqlite3_column_text(stmt, 0);
                        uColN = sqlite3_column_text(stmt, 1);
                        pColN = sqlite3_column_text(stmt, 2);

                        printf("%s ", wColN);
                        printf("%s ", uColN);
                        printf("%s\n", pColN);
                }

                sqlite3_finalize(stmt);
        }

        if(rc != SQLITE_OK) {
		ret_err(db);
		
		sqlite3_close(db);
	}
        
        sqlite3_close(db);
}

const char *set_key(const char *key)
{
	key = "correctkey";

	return key; 
}

int count_rows(sqlite3 *db, sqlite3_stmt *stmt, const char *table, const unsigned char *column)
{
        static char count_row_buf[1000];
        static int row_count_result;

        sprintf(count_row_buf, "SELECT COUNT(%s) FROM %s;", column, table);

        sqlite3_prepare_v2(db, count_row_buf, -1, &stmt, NULL);

        while(sqlite3_step(stmt) == SQLITE_ROW) {
                row_count_result = sqlite3_column_int(stmt, 0);

                return row_count_result;
        }

        sqlite3_finalize(stmt);

        sqlite3_close(db);
}

void close_db(sqlite3 *db)
{
        sqlite3_close(db);
}