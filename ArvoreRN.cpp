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
    No *paiAux = ar->NIL; //Inicializa para se a arvore estiver vazia, o pai da primeira raiz aponta para NIL
    No *novo;

    while (aux != ar->NIL) {
        paiAux = aux;

        if(num < aux->chave){
            aux = aux->direita;
        } else if (num > aux->chave){
            aux = aux->esquerda;
        } else {
            printf("Número já existente!");
            return;
        }
    }

    novo = criarNo(ar, num);

    if(paiAux == ar->NIL){ //Arvore vazia
        ar->raiz = novo;
    } else if(num < paiAux->chave){
        paiAux->esquerda = novo;
    } else {
        paiAux->direita = novo;
    }

    ajustarInsercao(ar, novo);
}

void ajustarInsercao(Arvore *ar, No *nv){

    while(nv->pai->cor == VERMELHO){
        No *tio = nv->pai->pai->esquerda;

        if(tio->cor == VERMELHO){
            nv->pai->cor = PRETO;
            tio->cor = PRETO;
            nv->pai->pai->cor = VERMELHO;
            
            nv = nv->pai->pai; // Passa o problema pro vo
        } else {

            if(nv = nv->pai->direita){
                rotEsquerda(ar, nv);
            } 

            rotDireita(ar, nv);
        }
    }
    




}

No* rotDireita(Arvore *ar, No *x){
    No *y = x->esquerda;
    x->esquerda = y->direita;

    if()
}

No* rotEsquerda(Arvore *ar, No *x){
    No *y = x->direita;
    x->direita = y->esquerda;

    if(y->esquerda != ar->NIL){
        y->esquerda->pai = x;
    }

    y->pai = x->pai;

    
    if(x->pai == ar->NIL){ // raiz geral
        ar->raiz = y;
    } else if(x == x->pai->esquerda){ // filho esquerda
        x->pai->esquerda = y;
    } else { // filho direita
        x->pai->direita = y;
    }

    y->esquerda = x; 
    x->pai = y;

    return x;
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