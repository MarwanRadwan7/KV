#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE_FILE "database.txt" 

struct keyValue
{
    int key ;
    char value[256];
};

void put(struct keyValue *db, int *dbSize, int key, const char *value){

    // Check if key is already exist
    for(int i = 0 ; i < *dbSize; i++){
        if(db[i].key == key){
            printf("Key %d is already exist\n" , key) ;
            strcpy(db[i].value , value) ;
            return ;
        }
    }

    // Store
    db[*dbSize].key = key ;
    strcpy(db[*dbSize].value , value) ;
    (*dbSize)++ ;
    printf("Key: %d with value: %s added successfully\n" , key, value) ;

}

void get(struct keyValue *db, int dbSize, int key){
    for(int i = 0 ; i < dbSize ; i++){
        if(db[i].key == key){
            printf("%d,%s\n" , key , db[i].value) ;
            return;
        }
    }
    printf("%d not found in the database.\n", key);
}

void delete(struct keyValue *db , int *dbSize , int key){
    for(int i = 0 ; i < *dbSize ; i++){
        if(db[i].key == key){
            for(int j = i ; j < *dbSize; j++){
                db[j] = db[j+1] ; // Overwrite current pair and move pairs to left
                (*dbSize)-- ;
                printf("Key %d deleted from the database successfully.\n" , key) ;
                return ;
            }
        }
    }
     printf("%d not found in the database.\n", key);
}

void clear(int *dbSize) {
    *dbSize = 0;
    printf("All key-value pairs cleared from the database.\n");
}

void printAll(const struct keyValue *db, int dbSize) {
    for (int i = 0; i < dbSize; i++) {
        printf("%d,%s\n", db[i].key, db[i].value);
    }
}


int main(int argc, char *argv[]){
    struct keyValue db[256] ; // Array of key value pairs
    int dbSize = 0 ;

    // Load database file if it exists
    FILE *file = fopen(DATABASE_FILE , "r") ;
    if(file){
        while(fscanf(file, "%d,%255[^\n]" , &db[dbSize].key ,db[dbSize].value )!=EOF){
            dbSize++ ;
        }
        fclose(file) ;
    }


    // Process command-line args
    for(int i = 1 ; i < argc ; i++){
        char *token = strtok(argv[i] , ",");
        if(token == NULL){
            printf("Bad command: %s\n" , argv[i]) ;
            continue ;
        }
        if (strcmp(token, "p") == 0) {
            int key;
            const char *value;

            token = strtok(NULL, ",");
            if (token != NULL) {
                key = atoi(token);
                token = strtok(NULL, ",");
                if (token != NULL) {
                    value = token;
                    put(db, &dbSize, key, value);
                } else {
                    printf("Bad command: %s\n", argv[i]);
                }
            } else {
                printf("Bad command: %s\n", argv[i]);
            }
        } else if (strcmp(token, "g") == 0) {
            int key;
            token = strtok(NULL, ",");
            if (token != NULL) {
                key = atoi(token);
                get(db, dbSize, key);
            } else {
                printf("Bad command: %s\n", argv[i]);
            }
        } else if (strcmp(token, "d") == 0) {
            int key;
            token = strtok(NULL, ",");
            if (token != NULL) {
                key = atoi(token);
                delete(db, &dbSize, key);
            } else {
                printf("Bad command: %s\n", argv[i]);
            }
        } else if (strcmp(token, "c") == 0) {
            clear(&dbSize);
        } else if (strcmp(token, "a") == 0) {
            printAll(db, dbSize);
        } else {
            printf("Bad command: %s\n", argv[i]);
        }
    }

    // Save data to the database file
    file = fopen(DATABASE_FILE, "w");
    if (file) {
        for (int i = 0; i < dbSize; i++) {
            fprintf(file, "%d,%s\n", db[i].key, db[i].value);
        }
        fclose(file);
    } else {
        printf("Failed to save data to the database file.\n");
    }


    return 0 ;
}