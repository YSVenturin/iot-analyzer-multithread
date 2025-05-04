#include <stdio.h>

#include "analysis.h"

void writeMonthlyDeviceAnalysis(DeviceAnalyse* device, char* filename) {
    FILE *file;
    if (fopen(filename, "r") == NULL) {
        file = fopen(filename, "w");
        fprintf(file,"device;ano-mes;min_temp;max_temp;total_temp;min_humi;max_humi;total_humi;min_lumi;max_lumi;total_lumi;min_noise;max_noise;total_noise;min_eco2;max_eco2;total_eco2;min_etvoc;max_etvoc;total_etvoc\n");
    }
    else {
        file = fopen(filename, "a");
    }

    if(file==NULL){
        printf("Error, file couldn't be opened");
        return;
    }

    fprintf(
        file,"%s;%s;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->min_temp,
            device->max_temp,
            (device->total_temp)/(device->analysis_count),
            device->min_humi,
            device->max_humi,
            (device->total_humi)/(device->analysis_count),
            device->min_lumi,
            device->max_lumi,
            (device->total_lumi)/(device->analysis_count),
            device->min_noise,
            device->max_noise,
            (device->total_noise)/(device->analysis_count),
            device->min_eco2,
            device->max_eco2,
            (device->total_eco2)/(device->analysis_count),
            device->min_etvoc,
            device->max_etvoc,
            (device->total_etvoc)/(device->analysis_count)
    );

    fclose(file);
}