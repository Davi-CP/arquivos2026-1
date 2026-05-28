
#include <stdio.h>
#include <stdlib.h>
#include "ArvoreB.h"

typedef struct _Endereco Endereco;
struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

int main(int argc, char **argv){
    
    FILE *f;
    ArvoreB *a;
    Endereco e;
    long posicao = 0;

    //USO: %s [arquivo_dados] [arquivo_arvore]\n"
    f = fopen("a1.dat", "rb");
    a = ArvoreB_Abre("arvore.dat");
    
    while (fread(&e, sizeof(Endereco), 1, f) == 1)
    {
        ArvoreB_Insere(a,e.cep, posicao);
        posicao++;
    }

    
    fclose(f);
    ArvoreB_Fecha(a);

    printf("Indexacao concluida. Total de registros indexados: %ld\n", posicao);

    return 0;

    
}