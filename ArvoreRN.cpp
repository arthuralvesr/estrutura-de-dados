#include <stdio.h>
#include <stdlib.h>

typedef enum { VERMELHO, PRETO } Cor;

typedef struct No {
    int chave;
    Cor cor;
    struct No *esquerda, *direita, *pai;
} No;

typedef struct Arvore {
    No *raiz;
    No *NIL;
} Arvore;

void inicializa(Arvore *arvore) {
    arvore->NIL = (No*)malloc(sizeof(No));
    arvore->NIL->cor = PRETO;
    arvore->NIL->direita = arvore->NIL;
    arvore->NIL->esquerda = arvore->NIL;
    arvore->NIL->pai = arvore->NIL;
    arvore->raiz = arvore->NIL;
}

No* criarNo(Arvore *ar, int num) {
    No *novo = (No*)malloc(sizeof(No));
    novo->chave = num;
    novo->cor = VERMELHO;
    novo->direita = ar->NIL;
    novo->esquerda = ar->NIL;
    novo->pai = ar->NIL;
    return novo;
}

No* rotDireita(Arvore *ar, No *x) {
    No *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != ar->NIL) {
        y->direita->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == ar->NIL) {
        ar->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->direita = x;
    x->pai = y;
    return x;
}

No* rotEsquerda(Arvore *ar, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != ar->NIL) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == ar->NIL) {
        ar->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
    return x;
}

void ajustarInsercao(Arvore *ar, No *nv) {
    while (nv->pai->cor == VERMELHO) {
        if (nv->pai == nv->pai->pai->esquerda) {
            No *tio = nv->pai->pai->direita;
            if (tio->cor == VERMELHO) {
                nv->pai->cor = PRETO;
                tio->cor = PRETO;
                nv->pai->pai->cor = VERMELHO;
                nv = nv->pai->pai;
            } else {
                if (nv == nv->pai->direita) {
                    nv = nv->pai;
                    rotEsquerda(ar, nv);
                }
                nv->pai->cor = PRETO;
                nv->pai->pai->cor = VERMELHO;
                rotDireita(ar, nv->pai->pai);
            }
        } else {
            No *tio = nv->pai->pai->esquerda;
            if (tio->cor == VERMELHO) {
                nv->pai->cor = PRETO;
                tio->cor = PRETO;
                nv->pai->pai->cor = VERMELHO;
                nv = nv->pai->pai;
            } else {
                if (nv == nv->pai->esquerda) {
                    nv = nv->pai;
                    rotDireita(ar, nv);
                }
                nv->pai->cor = PRETO;
                nv->pai->pai->cor = VERMELHO;
                rotEsquerda(ar, nv->pai->pai);
            }
        }
    }
    ar->raiz->cor = PRETO;
}

void inserir(Arvore *ar, int num) {
    No *aux = ar->raiz;
    No *paiAux = ar->NIL;
    No *novo;

    while (aux != ar->NIL) {
        paiAux = aux;
        if (num < aux->chave) {
            aux = aux->esquerda;
        } else if (num > aux->chave) {
            aux = aux->direita;
        } else {
            printf("Número já existente!\n");
            return;
        }
    }

    novo = criarNo(ar, num);
    novo->pai = paiAux;

    if (paiAux == ar->NIL) {
        ar->raiz = novo;
    } else if (num < paiAux->chave) {
        paiAux->esquerda = novo;
    } else {
        paiAux->direita = novo;
    }

    ajustarInsercao(ar, novo);
}

void imprimirArvoreAux(Arvore *ar, No *no, int nivel) {
    if (no != ar->NIL) {
        imprimirArvoreAux(ar, no->direita, nivel + 1);
        for (int i = 0; i < nivel; i++) {
            printf("    ");
        }
        printf("%d (%s)\n", no->chave, no->cor == VERMELHO ? "V" : "P");
        imprimirArvoreAux(ar, no->esquerda, nivel + 1);
    }
}

void imprimirArvore(Arvore *ar) {
    printf("=== Árvore Rubro-Negra (visualização vertical) ===\n");
    imprimirArvoreAux(ar, ar->raiz, 0);
}

No* minimo(Arvore *ar, No *x) {
    while (x->esquerda != ar->NIL) {
        x = x->esquerda;
    }
    return x;
}

void troca(Arvore *ar, No *u, No *v) {
    if (u->pai == ar->NIL) {
        ar->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

void ajustarRemocao(Arvore *ar, No *x) {
    while (x != ar->raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            No *w = x->pai->direita;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotEsquerda(ar, x->pai);
                w = x->pai->direita;
            }
            if (w->esquerda->cor == PRETO && w->direita->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->direita->cor == PRETO) {
                    w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    rotDireita(ar, w);
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->direita->cor = PRETO;
                rotEsquerda(ar, x->pai);
                x = ar->raiz;
            }
        } else {
            No *w = x->pai->esquerda;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotDireita(ar, x->pai);
                w = x->pai->esquerda;
            }
            if (w->direita->cor == PRETO && w->esquerda->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esquerda->cor == PRETO) {
                    w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    rotEsquerda(ar, w);
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esquerda->cor = PRETO;
                rotDireita(ar, x->pai);
                x = ar->raiz;
            }
        }
    }
    x->cor = PRETO;
}

void removeNo(Arvore *ar, int chave) {
    No *z = ar->raiz;
    while (z != ar->NIL && z->chave != chave) {
        if (chave < z->chave) {
            z = z->esquerda;
        } else {
            z = z->direita;
        }
    }

    if (z == ar->NIL) {
        printf("Elemento não encontrado.\n");
        return;
    }

    No *y = z;
    Cor corOriginal = y->cor;
    No *x;

    if (z->esquerda == ar->NIL) {
        x = z->direita;
        troca(ar, z, z->direita);
    } else if (z->direita == ar->NIL) {
        x = z->esquerda;
        troca(ar, z, z->esquerda);
    } else {
        y = minimo(ar, z->direita);
        corOriginal = y->cor;
        x = y->direita;
        if (y->pai == z) {
            x->pai = y;
        } else {
            troca(ar, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        troca(ar, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }

    if (corOriginal == PRETO) {
        ajustarRemocao(ar, x);
    }

    free(z);
    printf("Elemento %d removido com sucesso.\n", chave);
}

int main() {
    Arvore *arvore = (Arvore *)malloc(sizeof(Arvore));
    int op, num;

    inicializa(arvore);

    do {
        printf("\n1 - Inserir elemento\n");
        printf("2 - Ver árvore\n");
        printf("3 - Remover elemento\n");
        printf("4 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Elemento a inserir: ");
                scanf("%d", &num);
                inserir(arvore, num);
                break;
            case 2:
                imprimirArvore(arvore);
                break;
            case 3:
                printf("Elemento a remover: ");
                scanf("%d", &num);
                removeNo(arvore, num);
                break;
            case 4:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (op != 4);

    return 0;
}
