#include "descompactar.h"

arvore* ler_arvore(unsigned char *buffer, int tamanho){
    static int posicao = 0;
    
    if (posicao >= tamanho){
        return NULL;
    }

    arvore *no = criar_arvore('\0', NULL, NULL); //Cria uma árvore sem char

    no->rep = malloc(sizeof(int));
    *((int*)no->rep) = 1;

    if (buffer[posicao] == '*'){
        posicao++;
        no->chr = malloc(sizeof(unsigned char)); //Aloca tamanho de char
        *(unsigned char*)no->chr = '*'; //Muda o char da árvore para *
        no->esq = ler_arvore(buffer, tamanho);
        no->dir = ler_arvore(buffer, tamanho);
    } 
    else{
        if (buffer[posicao] == '\\'){
            posicao++;
            if (posicao >= tamanho){
                free(no);
                return NULL;
            }
        }
        no->chr = malloc(sizeof(unsigned char));
        *(unsigned char*)no->chr = buffer[posicao]; //Muda o char da árvore para o char do buffer
        posicao++;
    }

    return no;
}

void escrever_arquivo(int tamanho, int filesize, int trash, char *buffer, arvore* huff, FILE* new_file){
    arvore* aux = huff;
    unsigned char byte;

    for(long i = tamanho + 2; i < filesize; i++){
        byte = buffer[i];
        if(i < filesize - 1){ //Verifica se não é o ultimo byte do arquivo
            for (int i = 7; i >= 0; i--){
                if ((byte & (1 << i)) != 0){ //Se o bite for 1 ele vai pra direita
                    aux = aux->dir;
                } 
                else { //Se o bite for 1 ele vai pra direita
                    aux = aux->esq;
                }
                if (aux->esq == NULL && aux->dir == NULL){ //Verifica se é folha
                    fwrite(((unsigned char*)aux->chr), 1, 1, new_file);
                    aux = huff; //Volta pra raiz
                }
            }
        }
        else{
            for (int i = 7; i >= trash; i--){ //Caso seja o ultimo byte ele só ler até o trash
                if((byte & (1 << i)) != 0){ //Se o bite for 1 ele vai pra direita
                    aux = aux->dir;
                } 
                else{ //Se o bite for 1 ele vai pra direita
                    aux = aux->esq;
                }
                if (aux->esq == NULL && aux->dir == NULL){ //Verifica se é folha
                    fwrite(((unsigned char*)aux->chr), 1, 1, new_file);
                    aux = huff; //Volta para raiz
                }
            }
        }
    }
}

void descompactar(const char *nomedoarquivo, const char *novoarquivo){ //Ainda vamos mudar isso
    FILE *file;
    FILE *new_file;
    arvore *aux;
    arvore *huff = NULL;
    size_t current;
    uint8_t trash;
    uint16_t tamanho;

    //Leitura do arquivo

    file = fopen(nomedoarquivo, "rb"); //Abre o arquivo que vai ser compactado
    if(file == NULL){ //Conferir se foi aberto corretamente
        perror("Erro ao ler o arquivo: ");
        return;
    }

    fseek(file, 0, SEEK_END); //Salta o ponteiro para o final do arquivo
    long filesize = ftell(file); //Quarda a posição atual do arquivo que no caso é o final logo quarda o tamanho do arquivo
    rewind(file); //Retorna para o começo do arquivo

    unsigned char *buffer = (unsigned char*) malloc(filesize); //Aloca mémoria do buffer
    if(buffer == NULL){ //Tratamento de erro
        perror("Erro ao alocar mémoria: ");
        free(buffer);
        fclose(file);
        return;
    }

    current = fread(buffer, 1, filesize, file); //Salva os dados do arquivo no buffer
    if(current != filesize){ //Tratamento de erro
        perror("Erro ao ler o arquivo: ");
        free(buffer);
        fclose(file);
        return;
    }

    uint16_t metadados = (uint16_t) (buffer[0] << 8) | buffer[1];
    trash = (buffer[0] >> 5);
    tamanho = metadados & 0x1FFF;

    huff = ler_arvore(buffer + 2, tamanho);

    //Escrever o arquivo
    
    new_file = fopen(novoarquivo, "wb"); //Abre o novo arquivo em modo de escrida binaria
    if (new_file == NULL) { //tratamento de erro
        perror("Error opening output file");
        fclose(new_file);
        return;
    }

    escrever_arquivo(tamanho, filesize, trash, buffer, huff, new_file); //escreve o arquivo no novo arquivo

    fclose(new_file);
    free(buffer);
    fclose(file);
}