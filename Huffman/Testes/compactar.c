#include "compactar.h"

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8); //Desloca um byte pra esquerda e um pra direita e faz uma operacão de OU para trazer o byte mais significativo para frente
}

void escrever_metadados(FILE *arquivo, uint16_t arv_tam, uint8_t padding){
    uint16_t metadados = ((padding << 13) | arv_tam); //Desloca os bits do padding para o inicio do byte e junta com o tamanho da árvore
    metadados = swap_uint16(metadados); //Troca o byte de maior significado para frente
    fwrite(&metadados, sizeof(uint16_t), 1, arquivo); //Escreve o cabeçalho no arquivo
}

char** alocar_dicionario(int profundidade){ //Aloca mémoria para o dicionario baseado na profundidade da árvore já que é isso que diz quantos bits o novo valor vai ter
    char **dicionario; //Um array de ponteiro
    dicionario = malloc(sizeof(char*) * TAM); //Aloca mémoria com tamanho de char vezes 256 para o dicionario

    for(int i = 0; i < TAM; i++){
        dicionario[i] = calloc(profundidade, sizeof(char)); //Cria um array com o tamanho da profundidade máxima
    }

    return dicionario;
}

int tamanho_arvore(arvore *raiz){ //Retorna o tamanho da árvore
    int tamanho_esq, tamanho_dir;
    if(raiz == NULL){
        return -1;
    }
    else{
        tamanho_esq = tamanho_arvore(raiz->esq) + 1;
        tamanho_dir = tamanho_arvore(raiz->dir) + 1;
    }
    return tamanho_dir + tamanho_esq; //Retorna a soma do tamanho da direita e da esquerda
}

lista* remover_inicio(lista **head){ //remove o primeiro item da lista
    if(*head == NULL){
        return NULL; //caso não tenha item ele retorna NULL
    }

    lista *aux = *head;
    *head = (*head)->prox; //Aponta pro proximo item da lista para não perdemos o ponteiro
    aux->prox = NULL; //aponta o proximo pra NULL pra não dar problema de acesso de memoria

    return aux;
}

int altura(arvore *raiz){ //Retorna a altura da ávore
    int altura_da_esquerda, altura_da_direita;
    if(raiz == NULL){
        return -1;
    }
    else{
        altura_da_esquerda = altura(raiz->esq) + 1;
        altura_da_direita = altura(raiz->dir) + 1;
    }
    if(altura_da_direita > altura_da_esquerda){ //Decide qual é a maior altura e a retorna
        return altura_da_direita;
    }
    else{
        return altura_da_esquerda;
    }
}

void criar_dicionario(char **dicionario, arvore *raiz, char *caminho, int profundidade){ //Cria o novo dicionario a parti da árvore de Huffman
    char esquerda[profundidade], direita[profundidade];
    
    if(raiz->dir == NULL && raiz->esq == NULL){ //Caso o nó seja uma folha ele adicona ao dicionario o novo valor para o valor da folha Ex: A = 01 ao inves de A = 0100 0001
        strcpy(dicionario[*(unsigned char*)raiz->chr], caminho);
    }
    else{
        strcpy(esquerda, caminho); //Copia a string do caminho para a string da esquerda
        strcpy(direita, caminho); //Copia a string do caminho para a string da direita

        strcat(esquerda, "0"); //Caso a siga para a esquerda adiciona 0
        strcat(direita, "1"); //Caso direita adiciona 1

        criar_dicionario(dicionario, raiz->esq, esquerda, profundidade); //Chamada recursiva
        criar_dicionario(dicionario, raiz->dir, direita, profundidade);
    }
}

void escrever_arvore(FILE *arquivo, arvore *raiz, int *tam){
    if(raiz->dir == NULL && raiz->esq == NULL){ //Olha se o nó atual é folha ou não
        if((*((unsigned char*)raiz->chr) == '*' || *((unsigned char*)raiz->chr) == '\\')){ //Caso seja folha e o char for * ou \ ele escreve um \ antes como escape
            unsigned char barra = '\\';
            fwrite(&barra, sizeof(unsigned char), 1, arquivo);
            (*tam)++;
            fwrite((unsigned char*)raiz->chr, sizeof(unsigned char), 1, arquivo);
        }
        else{
            fwrite(((unsigned char*)raiz->chr), sizeof(unsigned char), 1, arquivo); //Escreve o char da raiz no cabeçalho
        }
    }
    else{ //
        fwrite(((unsigned char*)raiz->chr), sizeof(unsigned char), 1, arquivo); //Escreve o char da raiz no cabeçalho

        escrever_arvore(arquivo, raiz->esq, tam); //Chama a função recursivamente. Chama o filho da esquerda primeiro para ser escrido em preordem
        escrever_arvore(arquivo, raiz->dir, tam);
    }
}

arvore* criar_arvore(unsigned char v, arvore *esquerda, arvore *direita){ //Cria uma arvore
    arvore *new = (arvore*) malloc(sizeof(arvore));

    new->dir = direita;
    new->esq = esquerda;

    new->chr = malloc(sizeof(unsigned char));

    *(unsigned char*)new->chr = v;

    return new;
}

void addlist(lista **head, unsigned char v, int r){ //Adiciona item na lista
    lista *aux, *new = (lista*) malloc(sizeof(lista));
    if(new == NULL){
        perror("Erro ao alocar memoria para lista");
        return;
    }

    new->raiz = criar_arvore(v, NULL, NULL);

    new->raiz->rep = malloc(sizeof(int));

    *((int*)new->raiz->rep) = r;
    
    if(*head == NULL){ //Se nao tem item na lista ele cria um
        new->prox = NULL;
        *head = new;
        return;
    }
    else if(*((int*)(*head)->raiz->rep) > *((int*)new->raiz->rep)){
        new->prox = *head;
        *head = new;
        return;
    }
    else{
        aux = *head;
        while(aux->prox != NULL && *((int*)aux->prox->raiz->rep) <= *((int*)new->raiz->rep)){
            aux = aux->prox;
        }
        new->prox = aux->prox;
        aux->prox = new;
        return;
    }
}

void arvore_de_huffman(lista **head){ //Pega os dois primeiros itens da lista e cria uma uma lista binaria com eles
    lista *primeiro, *segundo, *aux = *head;
    

    while((*head)->prox != NULL){
        primeiro = remover_inicio(head); //Pega o primeiro item da lista
        //Como o primeiro item foi removido da lista o segundo vira o primeiro
        segundo = remover_inicio(head); //Pega o primeiro item da lista que era o segundo

        lista *new = (lista*) malloc(sizeof(lista)); //Aloca mémoria para o novo item da lista

        new->raiz = criar_arvore('*', primeiro->raiz, segundo->raiz); //Cria uma arvore que tem o primeiro e segundo item como folhas

        new->raiz->rep = malloc(sizeof(int));
        *((int*)new->raiz->rep) = *((int*)primeiro->raiz->rep) + *((int*)segundo->raiz->rep); //O novo valor de rep é a soma dos rep do primeiro intem e do segundo

        if(*head == NULL || *((int*)(*head)->raiz->rep) > *((int*)new->raiz->rep)){
            new->prox = *head;
            *head = new;
        }
        else{
            aux = *head;
            while(aux->prox != NULL && *((int*)aux->prox->raiz->rep) <= *((int*)new->raiz->rep)){
                aux = aux->prox;
            }
            new->prox = aux->prox;
            aux->prox = new;
        }

        free(primeiro->raiz->rep);
        free(segundo->raiz->rep);
        free(primeiro->raiz);
        free(segundo->raiz);
        free(primeiro); //Free para evitar memory leak
        free(segundo);
    }
}

void compactar(const char *nomedoarquivo, const char *novoarquivo, lista **list){ //Ainda vamos mudar isso
    int profundidade;
    char **dicionario;
    char *codificado;
    long filesize;
    FILE *file, *new_file;
    size_t current;
    uint8_t trash;
    uint16_t tamanho;
    unsigned char bit_no_buffer = 0;
    int bit_count = 0;
    long total_bits = 0;

    //Leitura do arquivo

    file = fopen(nomedoarquivo, "rb"); //Abre o arquivo que vai ser compactado
    if(file == NULL){ //Conferir se foi aberto corretamente
        perror("Erro ao ler o arquivo: ");
        return;
    }

    fseek(file, 0, SEEK_END); //Salta o ponteiro para o final do arquivo
    filesize = ftell(file); //Quarda a posição atual do arquivo que no caso é o final logo quarda o tamanho do arquivo
    rewind(file); //Retorna para o começo do arquivo

    unsigned char *buffer = (unsigned char*) malloc(filesize); //Aloca mémoria do buffer
    if(buffer == NULL){ //Tratamento de erro
        perror("Erro ao alocar mémoria: ");
        free(buffer);
        fclose(file);
        return;
    }
    
    current = fread(buffer, 1, filesize, file); //Lê o conteudo do arquivo e salva no buffer
    if(current != filesize){ //tratamento de erro
        perror("Erro ao ler o arquivo: ");
        free(buffer);
        fclose(file);
        return;
    }

    unsigned int repeticoes[TAM] = {0};

    for(long i=0; i < filesize; i++){ //Salva o numero de repetições de cada caracter
        repeticoes[buffer[i]]++;
    }

    for(int i = 0; i < TAM; i++){ //Adiciona o conteudo do arquivo na lista
        if(repeticoes[i] > 0){
            addlist(list, i, repeticoes[i]);
        }
    }

    if(*list == NULL){ //tratamento de erro
        perror("Falha ao criar a lista");
        free(buffer);
        fclose(file);
        return;
    }

    //----------------------------
    //Compactação

    arvore_de_huffman(list); //Cria a arvore de huffman

    profundidade = altura((*list)->raiz);

    dicionario = alocar_dicionario(profundidade);

    criar_dicionario(dicionario, (*list)->raiz, "", profundidade);
    
    //----------------------------
    //Escrevendo o arquivo

    new_file = fopen(novoarquivo, "wb"); //Abre o novo arquivo em modo binario
    if(new_file == NULL){ //tratamento de erro
        perror("Erro ao escrever o arquivo");
        free(buffer);
        for(int i = 0; i < TAM; i++) {
            free(dicionario[i]);
        }
        free(dicionario);
        fclose(file);
        return;
    }

    //Reservar espaço para os metadados
    fseek(new_file, sizeof(uint16_t), SEEK_SET);

    int sobra = 0;
    escrever_arvore(new_file, (*list)->raiz, &sobra); //Escreve a árvore no novo arquivo
    
    for(long i = 0; i < filesize; i++){
        char *code = dicionario[buffer[i]];
        for(long j = 0; code[j]; j++){
            if(code[j] == '1') {
                bit_no_buffer |= (1 << (7 - bit_count));
            }
            bit_count++;
            total_bits++;

            if(bit_count == 8){
                fwrite(&bit_no_buffer, 1, 1, new_file);
                bit_no_buffer = 0;
                bit_count = 0;
            }
        }
    }

    //Escrever os bits restantes, se houver
    if(bit_count > 0){
        fwrite(&bit_no_buffer, 1, 1, new_file);
    }

    //Calcular e escrever os metadados
    tamanho = tamanho_arvore((*list)->raiz) + 1 + sobra;
    trash = (8 - (total_bits % 8)) % 8;

    fseek(new_file, 0, SEEK_SET);
    escrever_metadados(new_file, tamanho, trash);

    fclose(new_file);
    free(buffer);
    fclose(file);
    for(int i = 0; i < TAM; i++){
        free(dicionario[i]);
    }
    free(dicionario);
}