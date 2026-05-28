
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


int main()
{
    FILE *fEntrada, *fSaida;
    Endereco e;
    int lidos = 0, gravados = 0;

    srand(time(NULL));
    fEntrada = fopen("cep.dat", "rb");

    if (fEntrada == NULL)
    {
        fprintf(stderr, "Erro a abrir arquivo de entrada. \n");
        return 1;
    }

    fSaida = fopen("cep_80.dat", "wb");
    if (fSaida == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo de saida. \n");
        fclose(fEntrada);
        return 1;
    }

    while (fread(&e, sizeof(Endereco), 1, fEntrada) == 1)
    {
        //RAND_MAX eh uma constante do C que retorna o valor maximo da funcao rand
        double random_num = (double)rand()/RAND_MAX;
        lidos++;

        if(random_num < 0.08)
        {
            fwrite(&e, sizeof(Endereco), 1, fSaida);
            gravados++;
        }
    }

    //fechamento dos arquivos
    fclose(fEntrada);
    fclose(fSaida);

    printf("Total lido: %d\n", lidos);
    printf("Total gravado: %d\n", gravados);

    return 0;



}