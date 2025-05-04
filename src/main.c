#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "thread.h"
#include "parser.h"

#include <time.h>
int main(int argc, char* argv[]){
    int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    int BLOCK_SIZE = 100;
    //int nprocs = 1;

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
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    pthread_t thread;
    pthread_create(&thread, NULL, readFileThread, (void*)threadArguments);
    pthread_join(thread, NULL);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);

    free(threadArguments);

    
    closeCSV(fptr);
        
    return 0;
}
