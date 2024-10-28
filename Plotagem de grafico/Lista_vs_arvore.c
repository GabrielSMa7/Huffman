#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    int item;
    struct node *next;
} node;

typedef struct binary_tree {
    int item;
    struct binary_tree *left;
    struct binary_tree *right;
} binary_tree;

node* create_linked_list() {
    return NULL;
}

node* add_linked_list(node *tail, int item) { // adiciona um novo nó na calda
    node *new_node = (node*) malloc(sizeof(node));
    new_node->item = item;
    new_node->next = NULL;
    if (tail == NULL) {//se não houver cabeça new_node se torna a cabeça
        return new_node;
    } 
    node *current = tail; //caso haja cabeça, seu ponteiro é armazenado em uma variavel auxiliar
    while (current->next != NULL) { // e procura até achar a antiga calda
        current = current->next;
    }
    current->next = new_node; //antiga calda recebe o ponteiro da nova calda
    return tail; //retorna o endereço da cabeça
}

void print_linked_list(node *head) {
    while (head != NULL) { //imprime os dados da cabeça até a calda
        printf("%d ", head->item);
        head = head->next;
    }
    printf("\n");
}

int list_seach(node *head , int item , int i) {
    while (head != NULL) {//confere se a cabeça não é nula
        if (head->item == item) { //confere se o item do nó é igual ao item procurado
            //printf("item(%d) foi encontrado\n", item);
            return i;
        }
        head = head->next;
        i++; //adiciona mais uma comparação
    }
    //printf("item(%d) não encontrado\n", item);
    return 0;
}

binary_tree* create_empty_binary_tree() {
    return NULL;
}

binary_tree* add_binary_tree(binary_tree *root, int item) {
    if (root == NULL) { //se a raiz for nula gera uma raiz vazia, caso não gera uma folha 
        binary_tree *new_binary_tree = (binary_tree*) malloc(sizeof(binary_tree));
        new_binary_tree->item = item;
        new_binary_tree->left = NULL;
        new_binary_tree->right = NULL;
        return new_binary_tree;
    }
    if (item < root->item) { //caso o item da nova folha for menor que a do nó atual vai para esquerda
        root->left = add_binary_tree(root->left, item);
    } else {//caso o item da nova folha for maior que a do nó atual vai para direita
        root->right = add_binary_tree(root->right, item);
    }
    return root;
}

void print_binary_tree(binary_tree *root) { // Função para imprimir a árvore binária em ordem (in-order)
    if (root != NULL) {
        print_binary_tree(root->left);  // Visita os nós à esquerda
        printf("%d ", root->item);      // Visita a raiz
        print_binary_tree(root->right); // Visita os nós à direita
    }
}

int binary_tree_seach(binary_tree *root, int item , int i) {
    if (root == NULL || root->item == item) { //confere se a raiz e nula ou se o item da raiz é igual ao item procurado
        return i;
    } else if (root->item > item) { //confere se o item é menor que o item da raiz
        return binary_tree_seach(root->left, item, i + 1); //adiciona mais uma comparação
    } else { //caso o if acima seja falso logo que o item da raiz é menor 
        return binary_tree_seach(root->right, item, i + 1); //adiciona mais uma comparação
    }
}

int random_num(int* used_numbers, int size , int current) { // Função para gerar número aleatório
    int num;
    int is_unique;
    do {
        num = rand() % size; // Gera um número aleatório entre 0 e size
        is_unique = 1; // Assume que é único
        for (int i = 0; i < current; i++) {// Verifica se o número já foi gerado
            if (used_numbers[i] == num) {
                is_unique = 0;
                break;
            }
        }
    } while (!is_unique); // Repete até encontrar um número único
    return num;
}

void save_in_file(int *linked_list_comparison, int *binary_tree_comparison, int max_size , int min_size) {
    FILE *file = fopen("resultados_comparacoes.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(file, "Resultados das comparações:\n");
    fprintf(file, "Lista encadeada:\n");
    for (int i = 0; i < max_size - min_size + 1 ; i++) {
        fprintf(file, "L(%d) = %d\n", (i + 1), linked_list_comparison[i]);
    }
    fprintf(file, "\nÁrvore de busca binária:\n");
    for (int i = 0; i < max_size - min_size + 1; i++) {
        fprintf(file, "T(%d) = %d\n", (i + 1), binary_tree_comparison[i]);
    }
    fclose(file);
    printf("\nResultados salvos em 'resultados_comparacoes.txt'\n");
}

int main() { 
    int  min_size , max_size , random;
    srand(time(NULL)); // gera uma semente de geração aleatoria para a da função Rand() 
    printf("Qual o tamanho Minimo e Maximo da entrada da entrada? ");
    scanf("%d %d", &min_size , &max_size); //escaneia o tamanho maximo de dados desejado
    printf("procurar por valores Aleatorios(1) ou procurar pelo ultimo valor dos numeros gerados(0)? ");
    scanf("%d", &random);
    int linked_list_comparison[max_size - min_size + 1], binary_tree_comparison[max_size - min_size + 1];
    for(int size = min_size; size <= max_size ; size++){ //testa as estruturas com uma entrada de 1 ao número maximo definido pelo usuário 
        node* lista = create_linked_list(); //cria a lista encadeada
        binary_tree* arvore = create_empty_binary_tree(); //cria a árvore binária 
        int used_numbers[size]; // Array para armazenar números gerados
        for (int i = 0; i < size; i++) { //preenche o array, a lista e a árvore com os números aleatorios e 
            int num = random_num(used_numbers, size , i); //gera um número aleatorio
            used_numbers[i] = num; // Armazena o número gerado para evitar repetição
            lista = add_linked_list(lista, num); //adicionar o número aleatorio gerado a lista
            arvore = add_binary_tree(arvore, num); //adicionar o número aleatorio gerado a árvore
        }
        //printf("Lista encadeada: ");
        //print_linked_list(lista);// Imprimir lista 
        //printf("Arvore binaria: "); 
        //print_binary_tree(arvore);//imprime a árvore em ordem
        //printf("\n");
        int search_item;
        if(random == 1){
            search_item = used_numbers[rand() % size]; //procura por um valor aleatorio dos valores aleatorios gerados (gera um resultado da área do Big O)
        }
        else{
            search_item = used_numbers[size - 1]; //procura o ultimo item (gera um resultado da linear do Big O)
        }
        //printf("Procurando o item: %d\n", search_item);
        linked_list_comparison[size - min_size] = list_seach(lista, search_item, 0); // Buscar o número na lista e contar comparações
        binary_tree_comparison[size - min_size] = binary_tree_seach(arvore, search_item, 0); // Buscar o número na árvore e contar comparações
        //printf("Numero de comparacoes ate achar\nNa Lista encadeada: %d\nNa Arvore de busca binaria: %d\n\n", linked_list_comparison[size - min_size], binary_tree_comparison[size - min_size]);
    }
    //printf("resultados da comparacao\nNa lista encadeada: ");
    //for(int i = 0; i < (max_size - min_size + 1); i++){
        //printf("L(%d) = %d ", (i + 1), linked_list_comparison[i]);
    //}
    //printf("\n\nNa Arvore de busca binaria: ");
    //for(int i = 0; i < (max_size - min_size + 1); i++){
        //printf("T(%d) = %d ", (i + 1), binary_tree_comparison[i]);
    //}
    save_in_file(linked_list_comparison, binary_tree_comparison, max_size , min_size);
}
