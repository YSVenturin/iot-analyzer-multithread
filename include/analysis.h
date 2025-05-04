#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include "parser.h"

/**
 * @brief Struct representing an analyse from a device.
 */
typedef struct DeviceAnalyse {
    char device_name[MAX_CHAR];
    char date[8];         // Date in "YYYY-MM"
    float min_temp;       //Minimun temperature value.
    float max_temp;       //Maximun temperature value.
    float total_temp;       //Avarage temperature value.
    float min_humi;       //Minimun humidity value.
    float max_humi;       //Maximun humidity value.
    float total_humi;       //Avarage humidity value.
    float min_lumi;       //Minimun luminosity value.
    float max_lumi;       //Maximun humidity value.
    float total_lumi;       //Avarage humidity value.
    float min_noise;      //Minimun noise value.
    float max_noise;      //Maximun noise value.
    float total_noise;      //Avarage noise value.
    float min_eco2;       //Minimun eco2 value.
    float max_eco2;       //Maximun eco2 value.
    float total_eco2;       //Avarage eco2 value.
    float min_etvoc;      //Minimun etvoc value.
    float max_etvoc;      //Maximun etvoc value.
    float total_etvoc;      //Avarage etvoc value.
    int analysis_count;              //Number of analyses from a single device.
}DeviceAnalyse;


/**
 * @brief Struct representing a list of references to an analyse.
 */
typedef struct ReferenceAnalyse {
    DeviceAnalyse *analyse;         //Pointer to a 'DeviceAnalyse' struct.
    struct ReferenceAnalyse* next;  //Pointer to the next Node.  
}ReferenceAnalyse;


// FUNCTION PROTOTYPES
int update_device_info(DeviceAnalyse* device, Entry* info);


#endif
