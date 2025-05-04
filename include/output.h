#ifndef OUTPUT_H
#define OUTPUT_H

#include "analysis.h"

void writeMonthlyDeviceAnalysis(DeviceAnalyse* device, char* filename);
void writeHeader(char* filename);

#endif