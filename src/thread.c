#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "analysis.h"
#include "parser.h"
#include "thread.h"
#include "output.h"


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void* deviceAnaliseThread(void* arg){
    char device_name[MAX_CHAR];
    int i;
    ThreadParam* param = (ThreadParam*)arg;

    while((param->id_thread)<MAX_DEVICES){

        pthread_mutex_lock(&lock);
        param->id_thread = *(param->id_thread_global);
        (*(param->id_thread_global))++;
        pthread_mutex_unlock(&lock);

        snprintf(device_name, MAX_CHAR, "sirrosteste_UCS_AMV-%02d",param->id_thread);

        DeviceAnalyse* device = (DeviceAnalyse*)calloc(1, sizeof(DeviceAnalyse));
        if (device == NULL) {
            exit(1);
        }

        for (i = 0; i < param->qtd_entry; i++) {
            if(strcmp(device_name,param->block[i]->device)==0) {
                update_device_info(device, param->block[i]);
            }
        }

        if(device->analysis_count == 0){
            free(device);
            continue;
        }

        pthread_mutex_lock(&lock);
        writeMonthlyDeviceAnalysis(device, "./analysis.csv");
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}


void* readFileThread(void* arg){
    ThreadArgs *args = (ThreadArgs*)arg;
    int nprocs = args->nprocs;
    int BLOCK_SIZE = args->blockSize;
    int entries;
    pthread_t threads[nprocs-1];
    ThreadParam* param;
    ThreadParam *param_copy[nprocs-1];
    FILE *fptr = args->fptr;

    Entry **block = readFileByMonthYear(fptr, BLOCK_SIZE, &entries, "|");
    while (block != NULL){
        int i;

        param = (ThreadParam*)malloc(sizeof(ThreadParam));
        if(param == NULL){
            exit(1);
        }
        param->qtd_entry = entries;
        param->id_thread = 0;
        param->block = block;
        param->id_thread_global = (int*)malloc(sizeof(int));
        *(param->id_thread_global) = 1;

        for (i = 0; i < nprocs-1; i++) {
            param_copy[i] = (ThreadParam*)malloc(sizeof(ThreadParam));
            *param_copy[i] = *param;
            pthread_create(&threads[i], NULL, deviceAnaliseThread, param_copy[i]);
        }

        for(i = 0; i < nprocs-1; i++){
            pthread_join(threads[i], NULL);
            free(param_copy[i]);
        }

        free(param);
        freeBlock(block, entries);
        block = readFileByMonthYear(fptr, BLOCK_SIZE, &entries, "|");
    }

    return NULL;
}
