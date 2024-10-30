#ifndef COMPACTACAO_H
#define COMPACTACAO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TAM 256

typedef struct arvore{
    void* rep;
    void* chr;
    struct arvore *esq, *dir;
}arvore;

typedef struct lista{
    struct arvore *raiz;
    struct lista *prox;
}lista;

void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list); //Compacta o arquivo
arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita); //Cria uma arvore
lista* remover_inicio(lista **head); //remove o primeiro item da lista
int altura(arvore *raiz); //Retorna o caminho da raiz até a folha mais distante
int tamanho_arvore(arvore *raiz); //Retorna o tamanho da árvore
char** alocar_dicionario(int profundidade); //Aloca mémoria para fazer o dicionario
void addlist(lista **head, unsigned char v, int r); //Adiciona item na lista
void arvore_de_huffman(lista **head); //Controi a árvore de Huffman
void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade); //Cria o dicionario
void escrever_arvore(FILE *arquivo, arvore *raiz, int *tam); //Escreve a árvore em preordem no cabeçalho
void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding); //Escreve o cabeçalho com o trash e o tamanho da arvore
uint16_t swap_uint16(uint16_t val); //Resolve o edianess fazendo ele escrever o byte mais significativo primeiro

#endif