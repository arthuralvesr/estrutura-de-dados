/*
   Matriz Dinâmica implementada através de alocação
   dinâmica de memória a ser usada como matriz de 
   adjacências para um Grafo.
   
   19-02-2025 (quarta-feira).
   
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct tipoCelula {
	int valor;
	tipoCelula *abaixo;
	tipoCelula *dir;
}TCelula;

TCelula *matriz;

//==| Assinatura das Funções |==========================
void inicializaMatriz(TCelula **c);
void defineMatriz(TCelula **c, int linha, int coluna);
TCelula *alocaCelula();
void atualizaCelula(TCelula **c, int linha, int coluna, int valor);
TCelula *localizaCelula(TCelula *c, int linha, int coluna);
int obterValor(TCelula *c);
void adicionarLinha(TCelula *c);
void adicionarColuna(TCelula *c);


//==| Programa Principal ===============================
int main(){
	inicializaMatriz(&matriz);
	
}

//===| Funções e Procedimentos |========================
void inicializaMatriz(TCelula **c){
	*c = NULL;
}
//======================================================
void defineMatriz(TCelula **c, int linha, int coluna){
	if(*c == NULL){
		int lin,col;
		TCelula *linhaAtual = NULL, *colunaAtual = NULL;
		
		*c = alocaCelula();
		linhaAtual = *c;
		
		for(lin=0; lin < linha; lin++){
			colunaAtual = linhaAtual;
			
			for(col=1; col < coluna; col++){
				colunaAtual->dir = alocaCelula();
				colunaAtual = colunaAtual->dir;	
			}//for
			
			if(lin < (linha - 1)) 
			   linhaAtual->abaixo = alocaCelula();
			
			linhaAtual = linhaAtual->abaixo;
		}//for
	}//if
}
//=======================================================
TCelula *alocaCelula(){
	TCelula *novo = (TCelula *)malloc(sizeof(TCelula));
	novo->dir = NULL;
	novo->abaixo = NULL;
	return novo;
}
//=======================================================
void atualizaCelula(TCelula **c, int linha, int coluna, int valor){
	TCelula *atual = *c;
	int lin = 0;
	int col = 0;
	
	while(lin < linha){
		lin++;
		atual = atual->abaixo;
	}//while
	
	while(col < coluna){
		col++;
		atual = atual->dir;
	}//while
	
	atual->valor = valor;
}
//===============================================================
TCelula *localizaCelula(TCelula *c, int linha, int coluna){
	TCelula *atual = c;
	int lin = 0, col = 0;
	
	while(lin < linha){
		atual = atual->abaixo;
		lin++;
	}//while
	
	while(col < coluna){
		atual = atual->dir;
		col++;
	}//while
	return atual;
}
//================================================================
int obterValor(TCelula *c){
	return c->valor;
}
//================================================================
void adicionarLinha(TCelula *c){
/* Acrescenta uma nova linha ao final da matriz*/

}
//================================================================
void adicionarColuna(TCelula *c){
/* Acrescenta uma nova coluna ao final da matriz*/
	
}
//================================================================


