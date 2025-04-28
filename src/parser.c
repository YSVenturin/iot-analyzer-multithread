#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


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
Entry* createNewEntry(char **columns) {
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
 * @brief Formats the date in the given columns array.
 * 
 * This function extracts the first 7 characters from the date stored in the 
 * `columns[DATE]` field and stores them back into the same field, ensuring 
 * that only the relevant portion of the date is retained.
 *
 * @param columns An array of strings representing the CSV columns, where
 *                `columns[DATE]` holds the date to be formatted.
 */
void formatDate(char **columns) {
    int i;
    char strAux[8];

    for (i = 0; i < 7 && columns[DATE][i] != '\0'; i++) {
        strAux[i] = columns[DATE][i];
    }

    strAux[i] = '\0';
    strcpy(columns[DATE], strAux);
}


/**
 * @brief Reads a line from the CSV file and returns the columns as an array of strings.
 * 
 * This function reads a line from the provided file pointer `fptr`, splits it 
 * into columns using the specified delimiter, and returns the resulting array 
 * of column values. The function ensures that the date in the columns is valid 
 * and properly formatted. If any issues are found (e.g., invalid date), it skips 
 * that line and attempts to read the next one.
 *
 * @param fptr A pointer to the file from which to read the line.
 * @param delimiter A two-character array representing the delimiter used to split the line into columns.
 * 
 * @return A pointer to an array of strings containing the column values, or `NULL` 
 *         if the end of the file is reached or an error occurs while reading.
 */
char **readLineReturnColumns(FILE *fptr, char delimiter[2]) {
    char **columns;
    char string[MAX_CHAR];

    while(1){
        if (fgets(string, MAX_CHAR, fptr) == NULL)
            return NULL;

        columns = getColumns(string, delimiter);
        if (columns == NULL){
            continue;
        }

        if (isDateInvalid(columns[DATE])){
            freeColumnsArray(columns);
            continue;
        }

        break;
    }
    
    formatDate(columns);
    return columns;
}


/**
 * @brief Reads and groups entries by month and year from the CSV file.
 * 
 * This function reads lines from the CSV file, grouping the entries by 
 * the month and year extracted from the `DATE` field in each row. The function 
 * dynamically allocates memory for blocks of `Entry` structures, expanding 
 * the memory as needed. It stops reading when it encounters a different month-year 
 * combination than the first one in the block or when the end of the file is reached.
 *
 * @param fptr A pointer to the file from which to read the entries.
 * @param totalBlockNumber The initial number of blocks allocated for storing entries.
 * @param totalEntries A pointer to an integer where the total number of entries read 
 *                     will be stored.
 * @param delimiter A two-character array representing the delimiter used to split the line into columns.
 * 
 * @return A pointer to an array of `Entry*` containing the grouped entries, or `NULL` 
 *         if the file could not be read or an error occurred.
 */
Entry** readFileByMonthYear(FILE *fptr, int totalBlockNumber, int *totalEntries, char delimiter[2]) {
    int indexCurrentBlock = 0;
    Entry **blocks = (Entry**)malloc(totalBlockNumber * sizeof(Entry*));
    if (blocks == NULL){
        fprintf(stderr, "Error in memory allocation.\n");
        exit(0);
    }

    char **columns = readLineReturnColumns(fptr, delimiter);
    if (columns == NULL){
        return NULL;
    }
    
    char yearMonthOfBlock[MAX_CHAR];
    strcpy(yearMonthOfBlock, columns[DATE]);

    long lastPosition;
    while (1) { 
        if (strcmp(yearMonthOfBlock, columns[DATE]) != 0) {
            fseek(fptr, lastPosition, SEEK_SET);
            break;
        }

        lastPosition = ftell(fptr);

        if (indexCurrentBlock >= totalBlockNumber) {
            totalBlockNumber *= 2;
            Entry **blocksReallocated = realloc(blocks, sizeof(Entry*) * totalBlockNumber);
            if (blocksReallocated == NULL) {
                fprintf(stderr, "Error in memory allocation.\n");
                free(blocks);
                exit(0);
            }
            blocks = blocksReallocated;
        }

        Entry *aux = createNewEntry(columns);
        
        blocks[indexCurrentBlock++] = aux;
        freeColumnsArray(columns);

        columns = readLineReturnColumns(fptr, delimiter);
        if (columns == NULL){
            break;
        }
    }

    freeColumnsArray(columns);

    *totalEntries = indexCurrentBlock;

    return blocks;
}


/**
 * @brief Splits a CSV line into columns based on the specified delimiter.
 * 
 * This function takes a CSV line as a string and splits it into an array of 
 * column values using the specified delimiter. It allocates memory for the 
 * columns and copies each tokenized value into the respective column. If there 
 * are not enough tokens to fill all the columns, the function returns `NULL`.
 *
 * @param string A string representing a line from the CSV file to be split into columns.
 * @param delimiter A two-character array representing the delimiter used to split the line into columns.
 * 
 * @return A pointer to an array of strings (columns) if successful, or `NULL` 
 *         if memory allocation fails or if there are not enough tokens in the line.
 */
char** getColumns(char string[MAX_CHAR], char delimiter[2]){
    int i;
    char **columns = (char**)malloc(CSV_NUM_COLUMNS * sizeof(char*));
    if (columns == NULL){
        fprintf(stderr, "Error in memory allocation.\n");
        exit(0);
    }

    for (i = 0; i < CSV_NUM_COLUMNS; i++){
        columns[i] = (char*)malloc(MAX_CHAR * sizeof(char));
        if (columns[i] == NULL) {
            fprintf(stderr, "Error in memory allocation.\n");
            exit(0);
        }
    }

    char *lasts = NULL;
    char *token = strtok_r(string, delimiter, &lasts);

    for (i = 0; i < CSV_NUM_COLUMNS; i++){
        if (token == NULL){
            freeColumnsArray(columns);
            return NULL;
        }

        strcpy(columns[i], token);

        token = strtok_r(NULL, delimiter, &lasts);
    }

    return columns;
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
        char **columns;

        if (fgets(string, MAX_CHAR, fptr) == NULL){
            break;
        }

        columns = getColumns(string, delimiter);

        if (columns == NULL){
            continue;
        }
        
        invalidEntry = isDateInvalid(columns[DATE]);

        if (invalidEntry){
            invalidEntry = 0;
            freeColumnsArray(columns);
            continue;
        }
        
        formatDate(columns);
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
void freeColumnsArray(char **columns){
    int i;

    if (columns == NULL) {
        return;
    }

    for (i = 0; i < CSV_NUM_COLUMNS; i++) {
        free(columns[i]);
    }

    free(columns);
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
