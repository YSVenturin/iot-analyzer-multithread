#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


int main(int argc, char* argv[]){
    int i;
    int BLOCK_SIZE = atoi(argv[1]);
    char numStr[3];
    int arr[99] = {0};
    FILE *fptr = openCSV("./data/input/devices.csv");

    Entry **block = readFileByBlock(fptr, BLOCK_SIZE, "|");

    while (block != NULL){
        for (i=0; i<BLOCK_SIZE; i++){
            Entry *aux = block[i];
            if (aux == NULL)
                break;
            
            //printf("%s %s %.2f %.2f %.2f %.2f %.2f %.2f\n", aux->device, aux->date, aux->temperature, aux->humidity, aux->luminosity, aux->noise, aux->eco2, aux->etvoc);
            strncpy(numStr, aux->device + 20, 2);
            numStr[2] = '\0';
            int n = atoi(numStr);
            arr[n-1] += 1;

        }

        freeBlock(block, BLOCK_SIZE);
        block = readFileByBlock(fptr, BLOCK_SIZE, "|");
    }
    closeCSV(fptr);

    for (i=0;i<99;i++)
        printf("%d ", arr[i]);
        
    return 0;
}