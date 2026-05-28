
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

int main()
{
    
        
    FILE *fa1, *fout;
    ArvoreB *a2;
    Endereco e;
    long pos;
    long lidos = 0, gravados = 0;

    fa1 = fopen("a2.dat", "rb");
    a2 = ArvoreB_Abre("arvore.dat");
    fout = fopen("saida.dat", "wb");

    while (fread(&e, sizeof(Endereco), 1, fa1) == 1)
    {
        lidos++;

        pos = ArvoreB_Busca(a2, e.cep);

        if (pos != -1)
        {
            fwrite(&e, sizeof(Endereco), 1, fout);
            gravados++;
        }
    }

    fclose(fa1);
    fclose(fout);
    ArvoreB_Fecha(a2);

    printf("Total lido em a1: %ld\n", lidos);
    printf("Total em comum gravado na saida: %ld\n", gravados);

  return 0;

}