#ifndef PARSER_H_
#define PARSER_H_


#define MAX_CHAR 400        // Maximum character limit for strings.
#define CSV_NUM_COLUMNS 12  // NUmber of columns in the CSV file.

/**
 * @brief Struct representing a CSV entry.
 */
typedef struct entry {
    char device[MAX_CHAR];  // Device name.
    char date[30];          // Date in "YYYY-MM-DD HH:MM:SS.ffffff"
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
Entry* createNewEntry(char *columns[CSV_NUM_COLUMNS]);
Entry** readFileByBlock(FILE *fptr, int totalBlockNumber, char delimiter[2]);
void freeColumnsArray(char *columns[CSV_NUM_COLUMNS]);
void freeBlock(Entry **block, int totalBlockNumber);


#endif
