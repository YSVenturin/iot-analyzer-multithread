#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "thread.h"
#include "parser.h"
#include "output.h"

int main(){
    int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 2) {
        fprintf(stderr, "Error: This program requires at least 2 CPU cores.\n");
        exit(0);
    }

    int BLOCK_SIZE = 1000;

    writeHeader("./analysis.csv");

    FILE *fptr = openCSV("./data/input/devices.csv");
    ThreadArgs *threadArguments = (ThreadArgs*)malloc(sizeof(ThreadArgs));
    if (threadArguments == NULL) {
        fprintf(stderr, "Error in memory allocation.");
        exit(0);
    }

    threadArguments->blockSize = BLOCK_SIZE;
    threadArguments->fptr = fptr;
    threadArguments->nprocs = nprocs;
    threadArguments->maxNumDevices = 30;
    
    pthread_t thread;
    pthread_create(&thread, NULL, readFileThread, (void*)threadArguments);
    pthread_join(thread, NULL);

    free(threadArguments);

    closeCSV(fptr);
        
    return 0;
}
