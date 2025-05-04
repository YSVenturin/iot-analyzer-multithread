#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include "analysis.h"

#define MAX_DEVICES 30


/**
 * @brief Struct representing a Thread parameter.
 */
typedef struct ThreadParam {
    int id_thread;           //Thread identifier.
    int *id_thread_global;   //Global thread identifier.
    int qtd_entry;           //Number of entries read.
    struct entry** block;    //Pointer to a list of struct 'entry'.
}ThreadParam;


typedef struct threadInitialArgs {
    int nprocs;
    int blockSize;
    int maxNumDevices;
    FILE *fptr;
}ThreadArgs;


void* readFileThread(void* arg);
void* deviceAnaliseThread(void* arg);


#endif