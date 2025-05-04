#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>

#define MAX_CHAR 400        // Maximum character limit for strings.
#define CSV_NUM_COLUMNS 12  // NUmber of columns in the CSV file.

/**
 * @brief Struct representing a CSV entry.
 */
typedef struct entry {
    char device[MAX_CHAR];  // Device name.
    char date[8];          // Date in "YYYY-MM"
    float temperature;      // Temperature value.
    float humidity;         // Humidity value.
    float luminosity;       // Luminosity value.
    float noise;            // Noise value.
    float eco2;             // eCO2 value.
    float etvoc;            // eTVOC value.
}Entry;


/**
 * @brief Enum for column indexes in the CSV file.
 */
enum csvcolumns {
    ID, DEVICE, COUNT, DATE, TEMPERATURE, HUMIDITY, LUMINOSITY, NOISE, ECO2, ETVOC, LATITUDE, LONGITUDE
};


// FUNCTION PROTOTYPES
FILE* openCSV(char *filename);
void closeCSV(FILE *fptr);
int isDateInvalid(char date[MAX_CHAR]);
void formatDate(char **columns);
Entry* createNewEntry(char **columns);
void freeColumnsArray(char **columns);
void freeBlock(Entry **block, int totalBlockNumber);
Entry** readFileByMonthYear(FILE *fptr, int totalBlockNumber, int *totalEntries, char delimiter[2]);
char** getColumns(char string[MAX_CHAR], char delimiter[2]);
char **readLineReturnColumns(FILE *fptr, char delimiter[2]);


#endif
