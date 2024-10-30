#ifndef DESCOMPACTACAO_H
#define DESCOMPACTACAO_H

#include "compactar.h"

void descompactar(const char *nomedoarquivo, const char *novoarquivo); //Descompacta o arquivo
void escrever_arquivo(int tamanho, int filesize, int trash, char *buffer, arvore* huff, FILE* new_file); //Escreve o arquivo novo
arvore* ler_arvore(unsigned char *buffer, int tamanho);

#endif