#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


int main(int argc, char* argv[]){
    int i, sum = 0;
    int BLOCK_SIZE = 1000;
    int entries;
    char numStr[3];
    int arr[99] = {0};
    FILE *fptr = openCSV("./data/input/devices.csv");

    Entry **block = readFileByMonthYear(fptr, BLOCK_SIZE, &entries, "|");
    while (block != NULL){
        for (i = 0; i < entries; i++){
            Entry *aux = block[i];
            if (aux == NULL)
                break;
            
            strncpy(numStr, aux->device + 20, 2);
            numStr[2] = '\0';
            int n = atoi(numStr);
            arr[n-1] += 1;
        }
        freeBlock(block, entries);
        block = readFileByMonthYear(fptr, BLOCK_SIZE, &entries, "|");
    }
    
    closeCSV(fptr);

    for (i=0;i<99;i++){
        sum += arr[i];
    }
    printf("%d valid entries\n", sum);
    
    
        
    return 0;
}