#ifndef PARSER_H_

#define PARSER_H_
#define MAX_CHAR 400
#define CSV_NUM_COLUMNS 12

typedef struct entry {
    char device[MAX_CHAR];
    char date[30];
    float temperature;
    float humidity;
    float luminosity;
    float noise;
    float eco2;
    float etvoc;
}Entry;


enum csvcolumns {
    ID, DEVICE, COUNT, DATE, TEMPERATURE, HUMIDITY, LUMINOSITY, NOISE, ECO2, ETVOC, LATITUDE, LONGITUDE
};


FILE* openCSV(char *filename);
void closeCSV(FILE *fptr);
int isDateInvalid(char date[MAX_CHAR]);
Entry* createNewEntry(char *columns[CSV_NUM_COLUMNS]);
Entry** readFileByBlock(FILE *fptr, int totalBlockNumber, char delimiter[2]);
void freeColumnsArray(char *columns[CSV_NUM_COLUMNS]);
void freeBlock(Entry **block, int totalBlockNumber);

#endif