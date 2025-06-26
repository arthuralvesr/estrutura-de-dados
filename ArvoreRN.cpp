#include <stdio.h>
#include <stdlib.h>

typedef enum { VERMELHO, PRETO } Cor;

typedef struct No {
    int chave;
    Cor cor;
    struct No *esquerda, *direita, *pai;
};

typedef struct Arvore{
    No *raiz;
    No *NIL;  
};

void inicializa(Arvore *arvore){
    arvore->NIL = (No*)malloc(sizeof(No));

    arvore->NIL->cor = PRETO;
    arvore->NIL->direita = arvore->NIL;
    arvore->NIL->esquerda = arvore->NIL;
    arvore->NIL->pai = arvore->NIL;
    arvore->raiz = arvore->NIL;
}


No* criarNo(Arvore *ar, int num){
    No *novo = (No*)malloc(sizeof(No));

    novo->chave = num;
    novo->cor = VERMELHO;
    novo->direita = ar->NIL;
    novo->esquerda = ar->NIL;
    novo->pai = ar->NIL;

    return novo;
}

void inserir(Arvore *ar, int num){
    No *aux = ar->raiz;
    No *paiAux = ar->NIL;

    while (aux != ar->NIL) {
        paiAux = aux;

        if(num < aux->chave){
            aux = aux->direita;
        } else if (num > aux->chave){
            aux = aux->esquerda;
        }
    }
}

No* rotDireita(){

}

No* rotEsquerda(){

}

int main(){
    Arvore arvore;
    int op, num;

    inicializa(&arvore);

    do {
        printf("1 - Inserir elemento\n");
        printf("2 - Remover elemento\n");
        printf("3 - Ver árvore\n");
        printf("4 - Sair\n");

        scanf("%d", &op);

        switch(op) {
            case 1:
                printf("Elemento a inserir:\n");
                scanf("%d", &num);
                inserir(arvore, num);
                break;

            // case 2:
            //     printf("Elemento a remover:\n");
            //     scanf("%d", &rn);
            //     removeNo(&arvore, rn);
            //     break;

            // case 3:
            //     inOrder(arvore);
            //     break;

            default:
                printf("Opção inválida.\n");
        }

        printf("==========\n");
    } while (op != 4);
}