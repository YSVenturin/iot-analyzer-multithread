#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"


FILE* openCSV(char *filename) {
    FILE *fptr = fopen(filename, "r");

    return fptr;
}


void closeCSV(FILE *fptr) {
    fclose(fptr);
    return;
}


int isDateInvalid(char date[MAX_CHAR]) {
    int year, month;
    char yearStr[5], monthStr[3];

    strncpy(yearStr, date, 4);
    yearStr[4] = '\0';
    strncpy(monthStr, date+5, 2);
    monthStr[2] = '\0';

    year = atoi(yearStr);
    month = atoi(monthStr);

    if (year < 2024)
        return 1;
    
    if (year == 2024 && month < 3)
        return 1;
    
    return 0;
}


Entry* createNewEntry(char *columns[CSV_NUM_COLUMNS]) {
    Entry *aux = (Entry*)malloc(sizeof(Entry));
    if (aux == NULL){
        return NULL;
    }

    strcpy(aux->device, columns[DEVICE]);
    strcpy(aux->date, columns[DATE]);
    aux->temperature = atof(columns[TEMPERATURE]);
    aux->humidity = atof(columns[HUMIDITY]);
    aux->luminosity = atof(columns[LUMINOSITY]);
    aux->noise = atof(columns[NOISE]);
    aux->eco2 = atof(columns[ECO2]);
    aux->etvoc = atof(columns[ETVOC]);

    return aux;
}


Entry** readFileByBlock(FILE *fptr, int totalBlockNumber, char delimiter[2]) {
    int indexCurrentBlock = 0;
    Entry **blocks = (Entry**)malloc(totalBlockNumber * sizeof(Entry*));
    if (blocks == NULL){
        printf("ERROR.");
        exit(0);
    }

    int invalidEntry = 0;
    while (indexCurrentBlock < totalBlockNumber){
        int i;
        char string[MAX_CHAR];
        char *lasts = NULL;
        char *token = NULL;
        char *columns[CSV_NUM_COLUMNS];

        if (fgets(string, MAX_CHAR, fptr) == NULL){
            break;
        }
        
        for (i = 0; i < CSV_NUM_COLUMNS; i++){
            columns[i] = (char*)malloc(MAX_CHAR * sizeof(char));
        }
        
        token = strtok_r(string, delimiter, &lasts);
        for (i = 0; i < CSV_NUM_COLUMNS; i++){
            if (token == NULL){
                invalidEntry = 1;
                break;
            }
            
            strcpy(columns[i], token);

            token = strtok_r(NULL, delimiter, &lasts);
        }

        invalidEntry = isDateInvalid(columns[DATE]);

        if (invalidEntry){
            invalidEntry = 0;
            
            for (i = 0; i < CSV_NUM_COLUMNS; i++){
                free(columns[i]);
                columns[i] = NULL;  
            }

            continue;
        }
        
        Entry *aux = createNewEntry(columns);
        if (aux == NULL) {
            printf("ERROR\n");
            exit(0);
        }

        blocks[indexCurrentBlock++] = aux;

        freeColumnsArray(columns);
    }

    if (indexCurrentBlock == 0){
        free(blocks);
        blocks = NULL;
    }

    return blocks;
}


void freeColumnsArray(char *columns[CSV_NUM_COLUMNS]){
    int i;

    for (i = 0; i < CSV_NUM_COLUMNS; i++) {
        free(columns[i]);
        columns[i] = NULL; 
    }
}


void freeBlock(Entry **block, int totalBlockNumber){    
    int i;

    for (i = 0; i < totalBlockNumber; i++){
        free(block[i]);
        block[i] = NULL;  
    }

    free(block);
    block = NULL;
}
