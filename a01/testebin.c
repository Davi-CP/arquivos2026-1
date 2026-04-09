#include <stdio.h>
#include <string.h>

typedef struct _Pessoa Pessoa;

struct _Pessoa {
    char nome[40];
    int idade;
    float altura;
};

int main() {
    FILE *f = fopen("saida.dat","wb");
    Pessoa p[1000];
    memset(p,0,1000*sizeof(Pessoa));
    // p.nome = "Renato Mauro";
    strcpy(p[0].nome, "Renato Mauro");
    p[0].idade = 51;
    p[0].altura = 1.84;
    fwrite(p,sizeof(Pessoa),1000,f);
    fclose(f);
    return 0;
}

