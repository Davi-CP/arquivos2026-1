#include <stdio.h>
#include <stdlib.h>
#include "CSVParser.h"

// emcc Teste2.c CSVParser.c -lnoderawfs.js -lnodefs.js

#define READ_BUF_SIZE 8192

void callback(char** cols, int ncols, void* userData) {
    int* count = (int*) userData;
 //   if(*count > 315012) {
 //       printf("-----------------------------------------------------------------\n");
 //       for(int i=0; i<ncols; i++) {
 //           printf("[%s]\n",cols[i]);
 //       }
 //   }
    (*count)++;
}

int main() {    
    int qt, count=0;
    unsigned int total = 0;
    char* buf = (char*) malloc(READ_BUF_SIZE);
    CSVParser csv;
    CSVParser_init(&csv);
    FILE *f = fopen("./sinasc.csv","rb");
    if(f) {
        qt = fread(buf,1,READ_BUF_SIZE,f);
        while(qt > 0) {
            total += qt;
            CSVParser_processLines(&csv,buf,qt,callback,&count);
            qt = fread(buf,1,READ_BUF_SIZE,f);
        }
        fclose(f);
        CSVParser_processLines(&csv,"\n",1,callback,&count);
        printf("Total rows: %d\n", count);
    }
    free(buf);
    return 0;
}
