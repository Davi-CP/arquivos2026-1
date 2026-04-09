/*Considere agora dois arquivos no mesmo formato: candidatosA.csv e candida-
tosB.csv, com candidatosA.csv ordenado por CPF e candidatosB.csv não ordenado.
Faça um programa que imprima na tela o email dos canditados que se candidataram
para as duas edições do concurso, ou seja, aqueles que aparecem nos dois arquivos
simultaneamente. Escreva de forma eficiente!*/

//acesso sequencial para o arquivo não ordenado e busca binária para o arquivo ordenado

#include <stdio.h>
#include <string.h>

#define naoOrdenado "inscricoes.dat"
#define ordenado "inscricoes_ordenadas.dat"
struct Registro{
    int id_inscricao;
    char curso[20];
    char cpf[15];
    char sexo;
    char email[40];
    char opcaoQuadro;
}; typedef struct Registro Registro;

int leRegistro(Registro *r, FILE *f){
    char linha[100];
    if(fgets(linha, sizeof(linha), f) == NULL){
        return 0; // EOF
    }
    sscanf(linha, "%d %s %s %c %s %c", &r->id_inscricao, r->curso, r->cpf, &r->sexo, r->email, &r->opcaoQuadro);
    return 1; // OK
}

int main(int argc, char **argv){
    FILE *fNaoOrdenado;
    FILE *fOrdenado;
    Registro rNaoOrdenado;
    Registro rOrdenado;
    int qtdNaoOrdenado = 0;
    int qtdOrdenado = 0;
    int c = 0;

    fOrdenado = fopen(ordenado, "rb");
    fNaoOrdenado = fopen(naoOrdenado, "r");
    
    if(argc != 2){
        fprintf(stderr, "Uso: %s [CPF]\n", argv[0]);
        return 1;
    }
    
    if(fOrdenado == NULL || fNaoOrdenado == NULL){
        fprintf(stderr, "Erro ao abrir os arquivos");
        return 1;
    }   
    
    fNaoOrdenado = fopen(naoOrdenado, "rb");
    qtdNaoOrdenado = fread(&rNaoOrdenado, sizeof(Registro), 1, fNaoOrdenado);
    while(qtdNaoOrdenado > 0){
        if(strncmp(argv[1], rNaoOrdenado.cpf, 15) == 0){
            //Busca binária no arquivo ordenado
            long tamanhoBytes = ftell(fOrdenado);
            long tamanhoRegistros = tamanhoBytes/sizeof(Registro);
            long inicio = 0;
            long fim = tamanhoRegistros-1;
            long meio;
            while(inicio <= fim){
                meio = (inicio+fim)/2;
                fseek(fOrdenado, meio*sizeof(Registro), SEEK_SET);
                qtdOrdenado = fread(&rOrdenado, sizeof(Registro), 1, fOrdenado);
                if(strncmp(argv[1], rOrdenado.cpf, 15) == 0){
                    leRegistro(&rOrdenado, fOrdenado);
                }
                else if(strncmp(argv[1], rOrdenado.cpf, 15) < 0){
                    fim = meio - 1; //metade antes do meio
                }
                else{
                    inicio = meio + 1; //metade depois do meio
                   }   
    
                }   
        }  
        rewind(fOrdenado); // Volta para o início do arquivo ordenado para a próxima busca
    }

    fclose(fNaoOrdenado);
    fclose(fOrdenado);
    return 0;
}