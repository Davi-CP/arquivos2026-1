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

// Comparador para busca no índice
int compara(const void *indice1, const void *indice2)
{
    return strncmp(((const indiceEndereco *)indice1)->cep, ((const indiceEndereco *)indice2)->cep, 8);
}

int main(int argc, char **argv)
{
    FILE *fi;
    FILE *fd;
    Endereco e;
    int qtd;

    if (argc != 2) {
        fprintf(stderr, "USO: %s [CEP]\n", argv[0]);
        return 1;
    }

    // Abre o arquivo de índice para leitura
    fi = fopen("indice.dat", "rb");
    if (!fi) {
        printf("Erro ao abrir indice.dat\n");
        return 1;
    }

    // Determina a quantidade de registros no índice
    fseek(fi, 0, SEEK_END);
    long tamanhoBytes = ftell(fi);
    qtd = tamanhoBytes / sizeof(indiceEndereco);
    rewind(fi);

    printf("Total de registros no índice: %d\n", qtd);

    // Aloca memória para o índice
    indiceEndereco *arrayIndice = malloc(qtd * sizeof(indiceEndereco));
    if (!arrayIndice) {
        printf("Erro ao alocar memória\n");
        fclose(fi);
        return 1;
    }

    // Lê todo o índice na memória
    size_t lidos = fread(arrayIndice, sizeof(indiceEndereco), qtd, fi);
    if ((int)lidos != qtd) {
        printf("Erro ao ler índice\n");
        free(arrayIndice);
        fclose(fi);
        return 1;
    }
    fclose(fi);

    // Realiza busca binária no índice
    indiceEndereco chave;
    memcpy(chave.cep, argv[1], 8);

    indiceEndereco *resultado = (indiceEndereco *)bsearch(
        &chave,
        arrayIndice,
        qtd,
        sizeof(indiceEndereco),
        compara
    );

    if (resultado == NULL) {
        printf("CEP não encontrado no índice\n");
        free(arrayIndice);
        return 1;
    }

    printf("CEP encontrado! Posição no arquivo original: %u\n", resultado->posicao);

    // Abre o arquivo de dados original
    fd = fopen("cep.dat", "rb");
    if (!fd) {
        printf("Erro ao abrir cep.dat\n");
        free(arrayIndice);
        return 1;
    }

    // Busca o registro na posição encontrada
    fseek(fd, resultado->posicao * sizeof(Endereco), SEEK_SET);
    int ok = fread(&e, sizeof(Endereco), 1, fd);
    fclose(fd);

    if (ok != 1) {
        printf("Erro ao ler registro no arquivo original\n");
        free(arrayIndice);
        return 1;
    }

    // Exibe o registro encontrado
    printf("\n=== Registro Encontrado ===\n");
    printf("%.72s\n", e.logradouro);
    printf("%.72s\n", e.bairro);
    printf("%.72s\n", e.cidade);
    printf("%.72s\n", e.uf);
    printf("%.2s\n", e.sigla);
    printf("%.8s\n", e.cep);

    free(arrayIndice);
    return 0;
}
