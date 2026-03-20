#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _Endereco {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

typedef struct _indiceEndereco {
    char cep[8];
    unsigned int posicao;
} indiceEndereco;

// Comparador para qsort
int compara(const void *indice1, const void *indice2)
{
    return strncmp(((const indiceEndereco *)indice1)->cep, ((const indiceEndereco *)indice2)->cep, 8);
    
    /* const indiceEndereco *i1 = (const indiceEndereco *)a;
    const indiceEndereco *i2 = (const indiceEndereco *)b;
    return strncmp(i1->cep, i2->cep, 8); */
}

int main(int argc, char **argv)
{
    FILE *f;
    Endereco e;

    f = fopen("cep.dat", "rb");
    if (!f) {
        printf("Erro ao abrir cep.dat\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long posicao = ftell(f);
    int qtd = posicao / sizeof(Endereco);
    rewind(f);

    printf("Total de registros: %d\n", qtd);

    indiceEndereco *arrayIndice = malloc(qtd * sizeof(indiceEndereco));
    if (!arrayIndice) {
        printf("Erro ao alocar memória\n");
        fclose(f);
        return 1;
    }

    for (int i = 0; i < qtd; i++) {
        fread(&e, sizeof(Endereco), 1, f);

        memcpy(arrayIndice[i].cep, e.cep, 8);
        arrayIndice[i].posicao = i;
    }

    // Ordena o índice
    qsort(arrayIndice, qtd, sizeof(indiceEndereco), compara);

    // Grava o índice ordenado
    FILE *fi = fopen("indice.dat", "wb");
    if (!fi) {
        printf("Erro ao criar indice.dat\n");
        free(arrayIndice);
        fclose(f);
        return 1;
    }

    fwrite(arrayIndice, sizeof(indiceEndereco), qtd, fi);

    fclose(fi);
    fclose(f);
    free(arrayIndice);

    printf("Índice criado com sucesso!\n");
    return 0;
}