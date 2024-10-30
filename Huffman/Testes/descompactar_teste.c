#include "descompactar.h"
#include "cunit.h"

CUNIT_GLOBALS;

arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita){ //Cria uma arvore
    arvore *new = (arvore*) malloc(sizeof(arvore));

    new->dir = direita;
    new->esq = esquerda;

    new->chr = malloc(sizeof(unsigned char));

    *(unsigned char*)new->chr = v;

    return new;
}

CUNIT_TEST(descompactar, ler_arvore) {
    //CUnitTestResult *result = _cunitResult;

    unsigned char buffer[] = "*a*bc";  //Exemplo de buffer
    int tamanho = 7;
    arvore* raiz = ler_arvore(buffer, tamanho);

    CUNIT_ASSERT_TRUE(raiz != NULL);
    CUNIT_ASSERT_TRUE(raiz->esq != NULL && raiz->dir != NULL);
    CUNIT_ASSERT_EQ(*(unsigned char*)raiz->esq->chr, 'a');
    CUNIT_ASSERT_EQ(*(unsigned char*)raiz->dir->chr, '*');
    CUNIT_ASSERT_EQ(*(unsigned char*)raiz->dir->esq->chr, 'b');

    //Libere a memória da árvore (idealmente criar uma função própria para liberar memória)
    //liberar_arvore(raiz);
}

CUNIT_TEST(descompactar, escrever_arquivo) {
    //CUnitTestResult *result = _cunitResult;

    //Setup inicial de dados para simular
    unsigned char buffer[] = {0x00}; // Dados de exemplo
    arvore *dir = criar_arvore('a', NULL, NULL); // Nó de exemplo
    arvore *esq = criar_arvore('b', NULL, NULL); // Nó de exemplo
    arvore *huff = criar_arvore('*', esq, dir); // Nó de exemplo
    FILE *new_file = fopen("output_test.bin", "wb");
    CUNIT_ASSERT_TRUE(new_file != NULL);
    escrever_arquivo(-2, 3, 0, (char *)buffer, huff, new_file);
    fclose(new_file);
    //Verificar se o arquivo foi escrito corretamente
    new_file = fopen("output_test.bin", "rb");
    CUNIT_ASSERT_TRUE(new_file != NULL);

    unsigned char result_char;
    fread(&result_char, sizeof(unsigned char), 1, new_file);
    fclose(new_file);

    CUNIT_ASSERT_EQ(result_char, 'b');
}

CUNIT_MAIN();
