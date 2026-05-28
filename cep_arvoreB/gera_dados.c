
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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



int main(int argc, char **argv)
{
    FILE *fin, *fout;
    Endereco e;
    double r;
    long lidos = 0, gravados = 0;

    if (argc != 3)
    {
        fprintf(stderr, "USO: %s [arquivo_entrada] [arquivo_saida]\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    fin = fopen(argv[1], "rb");
    if (!fin)
    {
        fprintf(stderr, "Erro ao abrir arquivo de entrada %s\n", argv[1]);
        return 1;
    }

    fout = fopen(argv[2], "wb");
    if (!fout)
    {
        fprintf(stderr, "Erro ao criar arquivo de saida %s\n", argv[2]);
        fclose(fin);
        return 1;
    }

    while (fread(&e, sizeof(Endereco), 1, fin) == 1)
    {
        lidos++;

        r = (double) rand() / RAND_MAX;

        if (r < 0.8)
        {
            fwrite(&e, sizeof(Endereco), 1, fout);
            gravados++;
        }
    }

    fclose(fin);
    fclose(fout);

    printf("Total lido: %ld\n", lidos);
    printf("Total gravado: %ld\n", gravados);

    return 0;
}
