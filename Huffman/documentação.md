# Huffman

Este projeto implementa um sistema de compressão de dados baseado na árvore de Huffman. A árvore de Huffman é construída com base nas frequências dos caracteres em um dado arquivo, permitindo reduzir o tamanho do arquivo ao associar a cada caractere um código binário proporcional à sua frequência.

## Estrutura do Projeto

### 1. Estrutura de Dados

**Árvore:** Representa a estrutura binária da árvore de Huffman, onde cada nó contém um caractere (`chr`), uma frequência (`rep`) e ponteiros para os nós filhos (`esq`, `dir`).

```c
typedef struct arvore {
    void* rep;
    void* chr;
    struct arvore *esq, *dir;
} arvore;
```
**Lista:** Estrutura de lista duplamente encadeada onde cada nó armazena uma árvore de Huffman, usada para organizar as árvores intermediárias durante a construção da árvore de Huffman.
```c
typedef struct lista {
    struct arvore *raiz;
    struct lista *prox, *ant;
} lista;
```
# 2. Funções Principais
## Árvore de Huffman
criar_arvore: Constrói um nó da árvore.

arvore_de_huffman: Constrói a árvore de Huffman ao iterar sobre a lista encadeada, combinando os dois nós de menor frequência em um nó pai, e os inserindo novamente na lista de forma ordenada.
## Manipulação da Lista
**addlist:** Insere novos nós na lista encadeada com base na frequência de seus caracteres.

**remover_inicio:** Remove o primeiro item da lista.
## Dicionário de Huffman
**alocar_dicionario:** Aloca memória para o dicionário de compressão.

**criar_dicionario:** Constrói o dicionário com códigos binários a partir da árvore de Huffman.
## Compressão e Descompressão
**compactar:** Implementa o processo de compressão, criando a árvore e o dicionário de Huffman, e gravando o conteúdo compactado em um novo arquivo.

**descompactar:** Reconstrói a árvore de Huffman a partir do arquivo compactado, interpretando os dados e restaurando o conteúdo original.
## Funções Auxiliares
**print_arvore e print_lista:** Imprimem as árvores e listas para facilitar a visualização e o debug.

**swap_uint16:** Troca a ordem dos bytes em valores de 16 bits para compatibilidade entre sistemas.

**analisar_dicionario:** Avalia o comprimento dos códigos no dicionário, apresentando estatísticas.
# 3. Métodos de Entrada e Saída
**escrever_metadados e ler_metadados:** Manipulam metadados no cabeçalho dos arquivos compactados, armazenando ou recuperando o tamanho da árvore de Huffman e o preenchimento necessário para completar o último byte do arquivo compactado.

**escrever_arvore e ler_arvore:** Salvam e restauram a árvore de Huffman, representada no cabeçalho do arquivo, para garantir a reprodutibilidade dos dados na descompactação.
