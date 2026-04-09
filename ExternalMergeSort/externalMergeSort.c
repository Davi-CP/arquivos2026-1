#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_SIZE 50000

typedef struct _Endereco {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

// Comparador para qsort
int compara(const void *e1, const void *e2)
{
    return strncmp(((const Endereco *)e1)->cep, ((const Endereco *)e2)->cep, 8);
}

static int merge_duas_partes(const char *arquivoA, const char *arquivoB, const char *arquivoSaida)
{
    FILE *a = fopen(arquivoA, "rb");
    FILE *b = fopen(arquivoB, "rb");
    FILE *out = fopen(arquivoSaida, "wb");
    Endereco ea;
    Endereco eb;
    int okA;
    int okB;

    if (!a || !b || !out) {
        if (a) fclose(a);
        if (b) fclose(b);
        if (out) fclose(out);
        return 0;
    }

    okA = fread(&ea, sizeof(Endereco), 1, a) == 1;
    okB = fread(&eb, sizeof(Endereco), 1, b) == 1;

    while (okA && okB) {
        if (compara(&ea, &eb) <= 0) {
            fwrite(&ea, sizeof(Endereco), 1, out);
            okA = fread(&ea, sizeof(Endereco), 1, a) == 1;
        } else {
            fwrite(&eb, sizeof(Endereco), 1, out);
            okB = fread(&eb, sizeof(Endereco), 1, b) == 1;
        }
    }

    while (okA) {
        fwrite(&ea, sizeof(Endereco), 1, out);
        okA = fread(&ea, sizeof(Endereco), 1, a) == 1;
    }

    while (okB) {
        fwrite(&eb, sizeof(Endereco), 1, out);
        okB = fread(&eb, sizeof(Endereco), 1, b) == 1;
    }

    fclose(a);
    fclose(b);
    fclose(out);
    return 1;
}

static int copia_arquivo(const char *origem, const char *destino)
{
    FILE *in = fopen(origem, "rb");
    FILE *out = fopen(destino, "wb");
    char buffer[8192];
    size_t lidos;

    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }

    while ((lidos = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        fwrite(buffer, 1, lidos, out);
    }

    fclose(in);
    fclose(out);
    return 1;
}

static int intercala_runs(char nomes[][80], int qtdRuns)
{
    char novosNomes[2048][80];
    int qtdAtual = qtdRuns;
    int rodada = 0;

    if (qtdAtual <= 0) {
        return 0;
    }

    while (qtdAtual > 1) {
        int novaQtd = 0;
        int i;

        for (i = 0; i < qtdAtual; i += 2) {
            if (i + 1 < qtdAtual) {
                sprintf(novosNomes[novaQtd], "run_m%03d_%04d.dat", rodada, novaQtd);
                if (!merge_duas_partes(nomes[i], nomes[i + 1], novosNomes[novaQtd])) {
                    return 0;
                }
            } else {
                sprintf(novosNomes[novaQtd], "run_m%03d_%04d.dat", rodada, novaQtd);
                if (!copia_arquivo(nomes[i], novosNomes[novaQtd])) {
                    return 0;
                }
            }
            novaQtd++;
        }

        for (i = 0; i < qtdAtual; i++) {
            remove(nomes[i]);
        }

        for (i = 0; i < novaQtd; i++) {
            strcpy(nomes[i], novosNomes[i]);
        }

        qtdAtual = novaQtd;
        rodada++;
    }

    remove("cep_ordenado.dat");
    if (rename(nomes[0], "cep_ordenado.dat") != 0) {
        return 0;
    }

    return 1;
}

int main(int argc, char **argv)
{
    FILE *f;
    Endereco *buffer;
    size_t lidos;
    int qtdRuns = 0;
    char nomesRuns[2048][80];
    (void)argc;
    (void)argv;

    f = fopen("cep.dat", "rb");
    if (!f) {
        printf("Erro ao abrir cep.dat\n");
        return 1;
    }

    buffer = (Endereco *)malloc(RUN_SIZE * sizeof(Endereco));
    if (!buffer) {
        fclose(f);
        printf("Erro de memoria\n");
        return 1;
    }

    while ((lidos = fread(buffer, sizeof(Endereco), RUN_SIZE, f)) > 0) {
        FILE *saida;

        qsort(buffer, lidos, sizeof(Endereco), compara);
        sprintf(nomesRuns[qtdRuns], "run_%04d.dat", qtdRuns);

        saida = fopen(nomesRuns[qtdRuns], "wb");
        if (!saida) {
            free(buffer);
            fclose(f);
            printf("Erro ao criar arquivo temporario\n");
            return 1;
        }

        fwrite(buffer, sizeof(Endereco), lidos, saida);
        fclose(saida);
        qtdRuns++;

        if (qtdRuns >= 2048) {
            free(buffer);
            fclose(f);
            printf("Quantidade de runs excede o limite suportado\n");
            return 1;
        }
    }

    free(buffer);
    fclose(f);

    if (qtdRuns == 0) {
        FILE *saida = fopen("cep_ordenado.dat", "wb");
        if (saida) {
            fclose(saida);
            return 0;
        }
        printf("Erro ao criar cep_ordenado.dat\n");
        return 1;
    }

    if (!intercala_runs(nomesRuns, qtdRuns)) {
        printf("Erro durante a intercalacao dos blocos\n");
        return 1;
    }

    printf("Ordenacao concluida. Arquivo gerado: cep_ordenado.dat\n");
    return 0;
}