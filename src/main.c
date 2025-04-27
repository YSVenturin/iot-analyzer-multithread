#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"


int main(){
    int i, max_len = 0;
    char numStr[3];
    int arr[99] = {0};
    int cont = 0;
    FILE *fptr = openCSV("../data/input/devices.csv");

    Entry **block = readFileByBlock(fptr, 100, "|");

    while (block != NULL){
        for (i=0; i<100; i++){
            Entry *aux = block[i];
            if (aux == NULL)
                break;
            
            strncpy(numStr, aux->device + 20, 2);
            numStr[2] = '\0';
            int n = atoi(numStr);
            arr[n-1] += 1;

        }

        freeBlock(block, 100);
        block = readFileByBlock(fptr, 100, "|");
    }
    closeCSV(fptr);

    for (i=0;i<99;i++)
        printf("%d ", arr[i]);
        
    return 0;
}