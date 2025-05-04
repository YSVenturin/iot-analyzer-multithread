
# IoT Analyzer Multithread

A multithreaded C program to process CSV files containing sensor data from multiple devices. The program reads data in monthly blocks, computes statistical metrics (minimum, maximum, average) for each sensor type per device per month, and outputs the results in a structured format.


## Requirements

- At least 2 CPU cores
- POSIX system
- GCC compiler


## Installation

To compile the project, run the following command on terminal (make sure you're in the project root folder):

```bash
gcc -I./include src/*.c -pthread -lm -o program
```

## Usage

1. Place the input CSV file **(devices.csv)** in the **data/input/** directory.
Make sure the filename is **devices.csv**\
The CSV structure must contain 12 columns, separated by `|`: **id|device|count|date|temperature|humidity|luminosity|noise|eco2|etvoc|latitude|longitude**

2. After that, you can run the program using the following command:

```bash
./program
```

3. The output file **(analysis.csv)** will be generated in the root folder.


## CSV Parsing
One thread will be responsible for loading the CSV data. 

This thread will read the CSV file in blocks grouped by month, since the input file is assumed to be sorted by date.

Each block is parsed into a dynamic array of Entry structs, which represent one line of the CSV. Incomplete lines or containing nan values are skipped during parsing. 

**Entry struct**
```bash
typedef struct entry {
    char device[MAX_CHAR];  // Device name.
    char date[8];           // Date in "YYYY-MM"
    float temperature;      // Temperature value.
    float humidity;         // Humidity value.
    float luminosity;       // Luminosity value.
    float noise;            // Noise value.
    float eco2;             // eCO2 value.
    float etvoc;            // eTVOC value.
}Entry;
```
## Thread Distribution
The number of threads is automatically determined based on the number of available CPU cores. 

One thread is responsible for parsing the CSV.

Each one of the other threads processes a portion of the data corresponding to a specific device, calculating the monthly statistics (max, min, and average) for sensors such as temperature, humidity, luminosity, noise, eco2, and etvoc.

**Mutex locks** are used to ensure thread safety while writing results and managing shared resources.
## Thread Data Analysis
For each IoT device, the program computes the following monthly statistics for each sensor type:

- Maximum value: The highest recorded value for the sensor in a given month.

- Minimum value: The lowest recorded value for the sensor in a given month.

- Average value: The average of all recorded values for the sensor in a given month.

As a thread processes a line corresponding to a specific device monthly data, the program uses the `DeviceAnalyse` structure, which holds the statistics for each device per month, to update the values. The structure is defined as follows:
``` bash
typedef struct DeviceAnalyse {
    char device_name[MAX_CHAR];     // Device name
    char date[8];                   // Date in "YYYY-MM"
    float min_temp;                 // Minimun temperature value.
    float max_temp;                 // Maximun temperature value.
    float total_temp;               // Avarage temperature value.
    float min_humi;                 // Minimun humidity value.
    float max_humi;                 // Maximun humidity value.
    float total_humi;               // Avarage humidity value.
    float min_lumi;                 // Minimun luminosity value.
    float max_lumi;                 // Maximun humidity value.
    float total_lumi;               // Avarage humidity value.
    float min_noise;                // Minimun noise value.
    float max_noise;                // Maximun noise value.
    float total_noise;              // Avarage noise value.
    float min_eco2;                 // Minimun eco2 value.
    float max_eco2;                 // Maximun eco2 value.
    float total_eco2;               // Avarage eco2 value.
    float min_etvoc;                // Minimun etvoc value.
    float max_etvoc;                // Maximun etvoc value.
    float total_etvoc;              // Avarage etvoc value.
    int analysis_count;             // Number of analyses from a single device.
}DeviceAnalyse;
```

The monthly statistics for each device and sensor are saved to the output CSV file **(analysis.csv)** in the following format:
```bash
device;ano-mes;sensor;valor_maximo;valor_medio;valor_minimo
```
## Kernel Threads
The threads created are POSIX threads and are managed by the operating system.
## Possible Concurrency Issues
## Authors

- [@YSVenturin](https://github.com/YSVenturin/)
- [@Thecrystage](https://github.com/Thecrystage/)
