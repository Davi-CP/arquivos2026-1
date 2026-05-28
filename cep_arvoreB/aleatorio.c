#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int entra = 0;
    for(int i=0; i<10000; i++) {
        float random_num = 1.0*rand()/RAND_MAX;
        if(random_num < 0.8) {
            entra++;
        }
    }
    printf("%d\n",entra);
    return 0;
}
