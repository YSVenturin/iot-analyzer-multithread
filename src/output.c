#include <stdio.h>

#include "analysis.h"

void writeMonthlyDeviceAnalysis(DeviceAnalyse* device, char* filename) {
    FILE *file;
    if (fopen(filename, "r") == NULL) {
        file = fopen(filename, "w");
        fprintf(file,"device;ano-mes;sensor;valor_maximo;valor_medio;valor_minimo\n");
    }
    else {
        file = fopen(filename, "a");
    }

    if(file==NULL){
        printf("Error, file couldn't be opened");
        return;
    }

    fprintf(
        file,"%s;%s;temperatura;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->max_temp,
            (device->total_temp)/(device->analysis_count),
            device->min_temp
    );
    fprintf(
        file,"%s;%s;umidade;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->max_humi,
            (device->total_humi)/(device->analysis_count),
            device->min_humi
    );
    fprintf(
        file,"%s;%s;luminosidade;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->max_lumi,
            (device->total_lumi)/(device->analysis_count),
            device->min_lumi
    );
    fprintf(
        file,"%s;%s;ruido;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->max_noise,
            (device->total_noise)/(device->analysis_count),
            device->min_noise
    );
    fprintf(
        file,"%s;%s;eco2;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->max_eco2,
            (device->total_eco2)/(device->analysis_count),
            device->min_eco2
    );
    fprintf(
        file,"%s;%s;etvoc;%0.2f;%0.2f;%0.2f\n",
            device->device_name,
            device->date,
            device->max_etvoc,
            (device->total_etvoc)/(device->analysis_count),
            device->min_etvoc
    );

    fclose(file);
}