#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char string[40];

typedef struct tipoAtor {
	string nome;
	tipoAtor *prox;
}TAtor;

typedef struct tipoElenco {
	TAtor *ator;
	tipoElenco *prox;
}TElenco;

typedef struct tipoFilme {
	string titulo;
	int anoProducao;
	TElenco *elenco;
	tipoFilme *ante;
	tipoFilme *prox;
}TFilme;

typedef struct tipoListas {
	TFilme *filmes;
	TAtor *atores;	
}TListas;

TListas lista;

void inicializa(TListas *L);
int menu();
void cadastroAtor(TListas *L);
void listagemAtor(TListas L);
void cadastroFilme(TListas *L);
void cadastroElenco(TListas *L);
void listagemFilme(TListas L);

//=============================================================
int main(){
	int opcao;
	
	inicializa(&lista);
	
	do{
		opcao = menu();
		
		switch(opcao){
			case 1: cadastroAtor(&lista); break;
			case 2: cadastroFilme(&lista); break;
			case 3: cadastroElenco(&lista); break;
			case 4: listagemAtor(lista); break;
			case 5: listagemFilme(lista); break;
		}//switch
		
	}while(opcao != 0);
}
//======| Fun��es |===========================================
void inicializa(TListas *L){
	L->atores = NULL;
	L->filmes = NULL;
}
//============================================================
int menu(){
	int opcao;
	system("CLS");
	printf("\n\n\n\t\t=====| MENU |======\n\n");
	printf("0 - Sair (Encerrar Aplicacao).\n\n");
	printf("1 - Cadastro de ATORES.\n");
	printf("2 - Cadastro de FILMES.\n");
	printf("3 - Cadastro de ELENCOS.\n\n");
	printf("4 - Listagem de ATORES.\n");
	printf("5 - Listagem de FILMES.\n\n");
	printf("6 - Exclus�o de FILMES.\n\n");
	printf("\tinforme OPCAO desejada: ");
	scanf("%d",&opcao);
	
	if(opcao > 6 || opcao < 0){
		printf("\n\n\n\t\tERRO:\n\t\tOPCAO invalida.\n\n");
		system("PAUSE");
	}
	return opcao;
}
//============================================================
void cadastroAtor(TListas *L){
	TAtor *novo, *atual;
	
	novo = (TAtor *)malloc(sizeof(TAtor));
	novo->prox = NULL;
	
	
	printf("\n\n\t\t=====| CADASTRO de ATORES |=====\n\n");
	printf("\tNOME do ATOR: ");
	fflush(stdin);
	gets(novo->nome);
	
	if(L->atores == NULL){
		L->atores = novo;
	} else {
    	atual = L->atores;
    	while(atual->prox != NULL){
    		atual = atual->prox;
    	}//while
		atual->prox = novo;
	}
}
//=============================================================
void listagemAtor(TListas L){
	TAtor *atual = L.atores;
	int cont = 0;
	
	printf("\n\n\t\t=======| Listagem de ATORES |=======\n\n");
	
	while(atual!= NULL){
		printf("\n\t(%d) - %s.", ++cont, atual->nome);
		atual = atual->prox;
	}//while
	printf("\n\n");
	system("PAUSE");
}
//=============================================================
void cadastroFilme(TListas *L){
	TFilme *atual, *novo;
	
	printf("\n\n\t\t=======| Cadastro de FILME |=======\n\n");
	novo = (TFilme *)malloc(sizeof(TFilme));
	novo->ante = NULL;
	novo->prox = NULL;
	
	printf("\tInforme TITULO do FILME: ");
	fflush(stdin);
	gets(novo->titulo);
	
	printf("\n\n\tANO de Producao: ");
	scanf("%d", &novo->anoProducao);
	
	novo->elenco = NULL;
	
	atual = L->filmes;
	
	if(atual == NULL){
		//Lista vazia.
		L->filmes = novo;		
	} else {
		//Lista com pelo menos um filme.
		while(atual->prox != NULL){
			atual = atual->prox;
		}//while
		atual->prox = novo;
	}//if else
}
//=============================================================
void cadastroElenco(TListas *L){
	TFilme *atualFilme = L->filmes;
	TElenco *novo, *atualElenco;
	TAtor *atualAtor = L->atores;
	string titulo, nome;
	
	printf("\n\n\n\t\t=======| Cadastro de ELENCO |=======\n\n");
	printf("\tInforme TITULO do FILME: ");
	fflush(stdin);
	gets(titulo);
	
	while(atualFilme != NULL){
		//Busca pelo Filme que receber� ELENCO.
		if(strcmp(atualFilme->titulo, titulo) == 0){
			//Encontrado Filme procurado.
			
			printf("\n\n\tNome do ATOR: ");
			fflush(stdin);
			gets(nome);
			
			while(atualAtor != NULL){
				//Busca pelo ATOR informado...
				if(strcmp(atualAtor->nome, nome) == 0){
					//Ator Encontrado.
					break;
				}//if
				atualAtor = atualAtor->prox;
			}//while
			
			if(atualAtor != NULL){
				novo = (TElenco *)malloc(sizeof(TElenco));
				novo->prox = NULL;
				novo->ator = atualAtor;
			}//if
			
			atualElenco = atualFilme->elenco;
			
			if(atualElenco == NULL){
				//Nenhum elenco foi cadastrado ainda para esse filme.
				atualFilme->elenco = novo;
			} else {
				while(atualElenco->prox != NULL){
					atualElenco = atualElenco->prox;
				}//while
				
				atualElenco->prox = novo;
			}
			break;
		}//if
		atualFilme = atualFilme->prox; //move para o pr�ximo Filme.
	}//while

}
//=============================================================
void listagemFilme(TListas L){
	TFilme *atualF;
	TElenco *atualE;
	TAtor	*atualA;
	int contF = 0;
	int contA = 0;
	
	printf("\n\n\n\t\t=======| Listagem de FILMES |=======\n\n");
	
	atualF = L.filmes;
	while(atualF != NULL){
		printf("\t[%d] - %s (%d).\n", ++contF, atualF->titulo, atualF->anoProducao);
		
		contA = 0;
		atualE = atualF->elenco;
		while(atualE != NULL){
			printf("\t\t[%d] - %s.\n", ++contA, atualE->ator->nome);
			atualE = atualE->prox;
		}//while
		
		atualF = atualF->prox;
	}//while
	printf("\n\n");
	system("PAUSE");
}
//==============================================================

