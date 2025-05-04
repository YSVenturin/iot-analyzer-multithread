#include <math.h>
#include <string.h>

#include "analysis.h"
#include "parser.h"

int update_device_info(DeviceAnalyse* device, Entry* info) {
    if (device == NULL || info == NULL) {
        return 0;
    }

    if (device->analysis_count == 0) {
        strcpy(device->device_name, info->device);
        strcpy(device->date, info->date);
        device->min_temp = info->temperature;
        device->total_temp = info->temperature;
        device->max_temp = info->temperature;
        device->min_humi = info->humidity;
        device->total_humi = info->humidity;
        device->max_humi = info->humidity;
        device->min_lumi = info->luminosity;
        device->total_lumi = info->luminosity;
        device->max_lumi = info->luminosity;
        device->min_noise = info->noise;
        device->total_noise = info->noise;
        device->max_noise = info->noise;
        device->min_eco2 = info->eco2;
        device->total_eco2 = info->eco2;
        device->max_eco2 = info->eco2;
        device->min_etvoc = info->etvoc;
        device->total_etvoc = info->etvoc;
        device->max_etvoc = info->etvoc;
        device->analysis_count += 1;

        return 1;
    }

    device->total_temp += info->temperature;
    device->total_humi += info->humidity;
    device->total_lumi += info->luminosity;
    device->total_noise += info->noise;
    device->total_eco2 += info->eco2;
    device->total_etvoc += info->etvoc;
    device->analysis_count += 1;

    device->min_temp = fmin(device->min_temp, info->temperature);
    device->max_temp = fmax(device->max_temp, info->temperature);
    device->min_humi = fmin(device->min_humi, info->humidity);
    device->max_humi = fmax(device->max_humi, info->humidity);
    device->min_lumi = fmin(device->min_lumi, info->luminosity);
    device->max_lumi = fmax(device->max_lumi, info->luminosity);
    device->min_noise = fmin(device->min_noise, info->noise);
    device->max_noise = fmax(device->max_noise, info->noise);
    device->min_eco2 = fmin(device->min_eco2, info->eco2);
    device->max_eco2 = fmax(device->max_eco2, info->eco2);
    device->min_etvoc = fmin(device->min_etvoc, info->etvoc);
    device->max_etvoc = fmax(device->max_etvoc, info->etvoc);

    return 1;
}
