#include <stdio.h>
#include <stdlib.h>

typedef struct No{
    int valor;
    struct No *esquerdo, *direito;
    int altura;    
};

No* novoNo(int num){
    No *novo = (No*)malloc(sizeof(No));

    novo->valor = num;
    novo->esquerdo = NULL;
    novo->direito = NULL;
    novo->altura = 0;

    return novo;
}

int maior(int a, int b){

    return(a > b)? a: b; // ternario = se verdadeiro: A | se falso: B
} 

int alturaNo(No *r){

    if(r == NULL){
        return -1;
    } else {
        return r->altura;
    }
}

int fatorBalanceamento(No *r){

    if(r){
        return (alturaNo(r->esquerdo) - alturaNo(r->direito));
    } else {
        return 0;
    }
}

No* rotEsquerda(No *r){
    No *y, *f;

    y = r->direito;
    f = y->esquerdo;

    y->esquerdo = r;
    r->direito = f;

    r->altura = maior(alturaNo(r->direito), alturaNo(r->esquerdo)) + 1;
    y->altura = maior(alturaNo(y->direito), alturaNo(y->esquerdo)) + 1;

    return y;
}

No* rotDireita(No *r){
    No *y, *f;
    
    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaNo(r->esquerdo), alturaNo(r->direito)) + 1;
    y->altura = maior(alturaNo(y->esquerdo), alturaNo(y->direito)) + 1;

    return y;
}

No* rotDireitaEsquerda(No *r){

    r->direito = rotDireita(r->direito);
    return rotEsquerda(r);
}

No* rotEsquerdaDireita(No *r){

    r->esquerdo = rotEsquerda(r->esquerdo);
    return rotDireita(r);
}

No* inserir(No *r, int num){
    
    if(r = NULL){
        return novoNo(num);
    } else {
        if(num < r->valor){
            r->esquerdo = inserir(r->esquerdo, num);
        } else if(num > r->valor){
            r->direito = inserir(r->direito, num);
        } else {
            printf("Valor repetido!");
        }
    }

    r->altura = maior (alturaNo(r->esquerdo), alturaNo(r->direito)) + 1;

    return r;
}

No* balancear(No *r){
    int fb = fatorBalanceamento(r);

    if(fb < 1 && fatorBalanceamento(r->esquerdo) >= 0){
        r = rotDireita(r);
    } else if(fb < 1 && fatorBalanceamento(r->esquerdo) < 0){
        r = rotDireitaEsquerda(r);
    } else if(fb > -1 && fatorBalanceamento(r->direito) <= 0){
        r = rotEsquerda(r);
    } else if(fb > -1 && fatorBalanceamento(r->direito) > 0){
        r = rotEsquerdaDireita(r);
    }

    return r;
}

No* remover(No *r, int num){


}

int main(){
    No raiz;

}
