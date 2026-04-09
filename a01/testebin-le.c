#include <stdio.h>
#include <string.h>

typedef struct _Pessoa Pessoa;

struct _Pessoa {
    char nome[40];
    int idade;
    float altura;
};

int main() {
    FILE *f = fopen("pessoas.dat","rb");
    Pessoa p[1000];
    int qt = fread(p,sizeof(Pessoa),1000,f);
    printf("%d\n",qt);
    printf("%s,%d,%f\n",p[0].nome,p[0].idade,p[0].altura);
    fclose(f);
    return 0;
}

