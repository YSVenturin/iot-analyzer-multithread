#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"


/**
 * @brief Open a CSV file in read mode.
 * 
 * @param filename A pointer to a string representing the file name.
 * 
 * @return FILE* A pointer to FILE object if the file is successfully opened;
 *               NULL if the file could not be opened.
 */
FILE* openCSV(char *filename) {
    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL) {
        perror("Error opening CSV file\n");
    }

    return fptr;
}


/**
 * @brief Closes the CSV file.
 *
 * @param fptr A pointer to the file to be closed.
 * 
 * @return None.
 */
void closeCSV(FILE *fptr) {
    fclose(fptr);
    return;
}


/**
 * @brief Checks if the given date is invalid based on year and month.
 * 
 * If the date is before March 2024, it returns returns 1 (invalid). Otherwise, it returns 0 (valid).
 *
 * @param date A string representing the date, having the first 7 characters as "YYYY-MM".
 * 
 * @return 1 if the date is invalid, 0 if valid.
 */
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


/**
 * @brief Creates a new Entry from the provided columns.
 * 
 * This function allocates memory for a new `Entry` and populates its fields 
 * using the values in the `columns` array.
 *
 * @param columns An array of strings representing the CSV columns.
 * 
 * @return A pointer to the newly created `Entry`, or `NULL` if memory allocation fails.
 */
Entry* createNewEntry(char *columns[CSV_NUM_COLUMNS]) {
    Entry *aux = (Entry*)malloc(sizeof(Entry));
    if (aux == NULL){
        fprintf(stderr, "Error in memory allocation.\n");
        exit(1);
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


/**
 * @brief Reads a CSV file and stores entries in blocks.
 * 
 * This function reads `totalBlockNumber` blocks from the CSV file, processes 
 * each line, and creates an `Entry` for valid data. It returns a pointer to 
 * an array of `Entry` pointers.
 *
 * @param fptr File pointer to the opened CSV file.
 * @param totalBlockNumber Number of lines to read.
 * @param delimiter The delimiter used to separate CSV columns.
 * 
 * @return A pointer to an array of `Entry` pointers.
 */
Entry** readFileByBlock(FILE *fptr, int totalBlockNumber, char delimiter[2]) {
    int indexCurrentBlock = 0;
    Entry **blocks = (Entry**)malloc(totalBlockNumber * sizeof(Entry*));
    if (blocks == NULL){
        fprintf(stderr, "Error in memory allocation.\n");
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

        blocks[indexCurrentBlock++] = aux;

        freeColumnsArray(columns);
    }

    if (indexCurrentBlock == 0){
        free(blocks);
        blocks = NULL;
    }

    return blocks;
}


/**
 * @brief Frees memory allocated for an array of column strings.
 *
 * @param columns Array of strings representing CSV columns.
 * 
 * @return None.
 */
void freeColumnsArray(char *columns[CSV_NUM_COLUMNS]){
    int i;

    for (i = 0; i < CSV_NUM_COLUMNS; i++) {
        free(columns[i]);
        columns[i] = NULL; 
    }
}


/**
 * @brief Frees memory allocated for an array of Entry structures.
 * 
 *
 * @param block Pointer to the array of `Entry` pointers.
 * @param totalBlockNumber Number of elements in the `block` array.
 * 
 * @return None.
 */
void freeBlock(Entry **block, int totalBlockNumber){    
    int i;

    for (i = 0; i < totalBlockNumber; i++){
        free(block[i]);
        block[i] = NULL;  
    }

    free(block);
    block = NULL;
}
