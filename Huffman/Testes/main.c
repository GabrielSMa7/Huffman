#include "descompactar.h"
#include "compactar.h"

int main(){
    int op; //Variavel para escolher se compacta ou descompacta
    char nomedoarquivo[TAM]; //Array com o nome do arquivo existente
    char novoarquivo[TAM]; //Array com o nome do arquivo que vai ser criado
    lista *list = NULL;
    FILE *file;

    do{ //Switch case 1 comapacta, 2 descompacta, 3 encerra
        printf("Digite 1 para compactar um arquivo, 2 para descompactar, ou 3 para encerrar o programa.\n");
        scanf("%d", &op);
        
        while (getchar() != '\n'); //Limpa o buffer para escrever o nomedoarquivo
        
        switch (op){
        case 1:
            
            //Receber o nome do arquivo que vai ser compactado
            printf("Insira o nome do arquivo (Ex: imagem.png, Aula_20_-_Huffman.pdf):");
            fgets(nomedoarquivo, TAM, stdin);
            nomedoarquivo[strcspn(nomedoarquivo, "\n")] = '\0'; //Remove a quebra de linha da string

            //Recebe o nome para o novo arquivo que vai ser criado
            printf("Insira o nome para o novo arquivo compactado (SEM A EXTENSAO):");
            fgets(novoarquivo, TAM, stdin);
            novoarquivo[strcspn(novoarquivo, "\n")] = '.'; //Troca a quebra de linha por um .
            strcat(novoarquivo, "watts"); //Contatena "watts" na string

            compactar(nomedoarquivo, novoarquivo, &list); //Chama a função de compactação

            //Verifica se o arquivo novo foi criado
            file = fopen(novoarquivo, "r"); //Abre o novo arquivo em modo de leitura

            if(file != NULL){ //Caso ele consiga ler o arquivo, então ele existe
                printf("Arquivo %s criado\n", novoarquivo);
            }
            else{ //Caso não, ele não consegue
                printf("Arquivo %s nao foi criado\n\n", novoarquivo);
            }
            break;

        case 2:

            //Recebe o nome do arquivo existente
            printf("Insira o nome do arquivo compactado (Com a extensao):");
            fgets(nomedoarquivo, TAM, stdin);
            nomedoarquivo[strcspn(nomedoarquivo, "\n")] = '\0';

            //Recebe o nome do arquivo que vai ser criado
            printf("Insira o nome para o novo arquivo com extensao (Ex: imagem.png, Aula_20_-_Huffman.pdf):");
            fgets(novoarquivo, TAM, stdin);
            novoarquivo[strcspn(novoarquivo, "\n")] = '\0';

            descompactar(nomedoarquivo, novoarquivo); //Chama a função de descompactar
            
            //Verifica se o arquivo novo foi criado
            file = fopen(novoarquivo, "r"); //Abre o novo arquivo em modo de leitura

            if(file != NULL){ //Caso ele consiga ler o arquivo, então ele existe
                printf("Arquivo %s criado\n", novoarquivo);
            }
            else{ //Caso não, ele não consegue
                printf("Arquivo %s nao foi criado\n\n", novoarquivo);
            }
            break;

        case 3:
            //Encerra o programa
            break;

        default:
            printf("Entrada invalida. Por favor, digite 1 ou 2 ou 3.\n");
            break;
        }

    }while(op != 3);

    return 0;
}