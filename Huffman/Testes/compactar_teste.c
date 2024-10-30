#include "cunit.h"
#include "compactar.h"

CUNIT_GLOBALS

// Test swap_uint16 function
CUNIT_TEST(HuffmanUtils, SwapUint16) {
    uint16_t original = 0x1234;
    uint16_t expected = 0x3412;
    uint16_t result = swap_uint16(original);
    
    CUNIT_ASSERT_EQ(result, expected);
    
    // Test with different values
    original = 0xFF00;
    expected = 0x00FF;
    result = swap_uint16(original);
    
    CUNIT_ASSERT_EQ(result, expected);
}

// Test escrever_metadados function
CUNIT_TEST(HuffmanUtils, EscreverMetadados) {
    const char* test_file = "test_metadata.bin";
    FILE* file = fopen(test_file, "wb");
    CUNIT_ASSERT_TRUE(file != NULL);
    
    uint16_t arv_tam = 123;
    uint8_t padding = 5;
    
    escrever_metadados(file, arv_tam, padding);
    fclose(file);
    
    // Read and verify the written metadata
    file = fopen(test_file, "rb");
    CUNIT_ASSERT_TRUE(file != NULL);
    
    uint16_t written_data;
    fread(&written_data, sizeof(uint16_t), 1, file);
    fclose(file);
    remove(test_file);
    
    uint16_t expected = ((padding << 13) | arv_tam);
    expected = swap_uint16(expected);
    
    CUNIT_ASSERT_EQ(written_data, expected);
}

// Test tamanho_arvore function
CUNIT_TEST(HuffmanTree, TamanhoArvore) {
    // Create a simple tree for testing
    arvore* root = criar_arvore('*', NULL, NULL);
    CUNIT_ASSERT_EQ(tamanho_arvore(root), 0);
    
    arvore* left = criar_arvore('a', NULL, NULL);
    arvore* right = criar_arvore('b', NULL, NULL);
    root->esq = left;
    root->dir = right;
    
    int size = tamanho_arvore(root);
    CUNIT_ASSERT_EQ(size, 2);  // Size should be 1 for a tree with two leaves
    
    // Cleanup
    free(left->chr);
    free(right->chr);
    free(root->chr);
    free(left);
    free(right);
    free(root);
}

// Test altura function
CUNIT_TEST(HuffmanTree, Altura) {
    // Test empty tree
    CUNIT_ASSERT_EQ(altura(NULL), -1);
    
    // Create a simple tree for testing
    arvore* root = criar_arvore('*', NULL, NULL);
    CUNIT_ASSERT_EQ(altura(root), 0);
    
    // Add one level
    root->esq = criar_arvore('a', NULL, NULL);
    CUNIT_ASSERT_EQ(altura(root), 1);
    
    // Add another level
    root->dir = criar_arvore('b', NULL, NULL);
    root->esq->esq = criar_arvore('c', NULL, NULL);
    CUNIT_ASSERT_EQ(altura(root), 2);
    
    // Cleanup
    free(root->esq->esq->chr);
    free(root->esq->chr);
    free(root->dir->chr);
    free(root->chr);
    free(root->esq->esq);
    free(root->esq);
    free(root->dir);
    free(root);
}

// Test criar_arvore function
CUNIT_TEST(HuffmanTree, CriarArvore) {
    unsigned char value = 'A';
    arvore* tree = criar_arvore(value, NULL, NULL);
    
    CUNIT_ASSERT_TRUE(tree != NULL);
    CUNIT_ASSERT_TRUE(tree->chr != NULL);
    CUNIT_ASSERT_EQ(*(unsigned char*)tree->chr, value);
    CUNIT_ASSERT_TRUE(tree->esq == NULL);
    CUNIT_ASSERT_TRUE(tree->dir == NULL);
    
    // Cleanup
    free(tree->chr);
    free(tree);
}

// Test lista functions
CUNIT_TEST(HuffmanList, AddListAndRemoveInicio) {
    lista* head = NULL;
    
    // Test adding items
    addlist(&head, 'A', 5);
    CUNIT_ASSERT_TRUE(head != NULL);
    CUNIT_ASSERT_EQ(*(unsigned char*)head->raiz->chr, 'A');
    CUNIT_ASSERT_EQ(*(int*)head->raiz->rep, 5);
    
    addlist(&head, 'B', 3);  // Should be added at start since 3 < 5
    CUNIT_ASSERT_EQ(*(unsigned char*)head->raiz->chr, 'B');
    
    // Test removing from start
    lista* removed = remover_inicio(&head);
    CUNIT_ASSERT_TRUE(removed != NULL);
    CUNIT_ASSERT_EQ(*(unsigned char*)removed->raiz->chr, 'B');
    
    // Cleanup
    free(removed->raiz->chr);
    free(removed->raiz->rep);
    free(removed->raiz);
    free(removed);
    
    free(head->raiz->chr);
    free(head->raiz->rep);
    free(head->raiz);
    free(head);
}

CUNIT_MAIN()