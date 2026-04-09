/* Escreva um programa que receba como argumento do comando de linha
um CPF e imprima na tela o nome do curso para o qual o candidato se inscreveu.
Considere que já exista a funcão leRegistro, que lê uma linha do arquivo e separa os
campos em cada um dos campos da estrutura. */

/*Le e decodifica a linha do arquivo
*f e armazena no registro *r.
retorna 1 = OK ou 0 = EOF*/

// estrutura parecida com o código BuscaCEP.c

#include <stdio.h>
#include <string.h>

#define ARQUIVO "inscricoes.txt"
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
    FILE *f;
    Registro r;
    int qtd = 0;
    int c = 0;

    if(argc != 2){
        fprintf("Uso: %s [CPF]", argv[0]);
        return 1;
    }
    f = fopen(ARQUIVO, "r");
    if(f == NULL){
        fprintf("Erro ao abrir o arquivo %s", ARQUIVO);
        return 1;
    }

    qtd = fread(&r,sizeof(Registro), 1, f);
    while(qtd > 0){
        c++;
        if(strncmp(argv[1], r.cpf, 15) == 0){
            leRegistro(&r, f);
            break;
        }
        qtd = fread(&r,sizeof(Registro), 1, f);
    }    
    printf("Total de lido: %d\n", c);
    fclose(f);
    return 0;
}
