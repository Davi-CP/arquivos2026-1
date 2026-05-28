#include <stdio.h>
#include "CSVParser.h"

void f(char** cols, int ncols, void* userData) {
    int* count = (int*) userData;
    printf("-----------------------------------------------------------------\n");
    for(int i=0; i<ncols; i++) {
        printf("[%s]\n",cols[i]);
    }
    (*count)++;
}

int main() {
    int count=0;
    char* input1 = "\"A\",\"ABC,DEF,GHI\",DEF\nGHI,JKL\nMNO,PQR";
    char* input2 = "S\nTUV,WXY,\"ABC";
    char* input3 = "DEF\"\n\nGHI,JKL\n";
    CSVParser csv;
    CSVParser_init(&csv);
    printf("******************* FASE 1 *******************\n");
    CSVParser_processLines(&csv,input1,-1,f,&count);
    printf("-----------------------------------------------------------------\n");
    printf("******************* FASE 2 *******************\n");
    CSVParser_processLines(&csv,input2,-1,f,&count);
    printf("-----------------------------------------------------------------\n");
    printf("******************* FASE 3 *******************\n");
    CSVParser_processLines(&csv,input3,-1,f,&count);
    printf("-----------------------------------------------------------------\n");
    printf("Total rows: %d\n",count);
}
