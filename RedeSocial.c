#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char string[20];

typedef struct tipoCelula {
	int valor;
	string nome;
	struct tipoCelula *abaixo;
	struct tipoCelula *dir;
}TCelula;

TCelula *relacionamentos, *usuarios;

//==| Assinatura das Fun��es |==========================
void inicializaMatriz(TCelula **c);
void defineMatriz(TCelula **c, int linha, int coluna);
TCelula *alocaCelula();
void atualizaCelula(TCelula **c, int linha, int coluna, int valor);
TCelula *localizaCelula(TCelula *c, int linha, int coluna);
int obterValor(TCelula *c);
void adicionarLinha(TCelula **c);
void adicionarColuna(TCelula **c);
void exibeMatriz(TCelula *c);
int menu();
void defineAmizade(TCelula *relacionamentos, int iUser1, int iUser2);
void inserirUser(TCelula *usuarios, TCelula *relacionamentos, int *qntdUser);
void registrarAmizade(TCelula *usuarios, TCelula *relacionamentos);
void recomendarAmizade(TCelula *relacionamentos, TCelula *usuarios, int qntdUser);

//==| Programa Principal ===============================
int main(){
	int opcao, qntdUser = 4;
	
	inicializaMatriz(&relacionamentos);
	inicializaMatriz(&usuarios);
	
	defineMatriz(&relacionamentos, 4, 4);
	defineMatriz(&usuarios,4,1);

	strcpy(localizaCelula(usuarios, 0, 0)->nome, "Asdrobal");
	strcpy(localizaCelula(usuarios, 1, 0)->nome, "Jambira");
	strcpy(localizaCelula(usuarios, 2, 0)->nome, "Carlos");
	strcpy(localizaCelula(usuarios, 3, 0)->nome, "Marta");

	defineAmizade(relacionamentos, 0, 1);
	defineAmizade(relacionamentos, 0, 2);
	defineAmizade(relacionamentos, 1, 3);
	defineAmizade(relacionamentos, 2, 3);
	
	do {
		opcao = menu();
		switch (opcao) {
			case 0:
				system("PAUSE");
				break;
			case 1:
				inserirUser(usuarios, relacionamentos, &qntdUser);
				break;
			case 2:
				registrarAmizade(usuarios, relacionamentos);
				break;
			case 3:
				recomendarAmizade(relacionamentos, usuarios, qntdUser);
				break;
			default:
				printf("\n\nOpção inválida!\n\n");
		}
		system("PAUSE");

	} while (opcao != 0);

}
//===| Funcoes Novas |===================================
void defineAmizade(TCelula *relacionamentos, int iUser1, int iUser2) {

	localizaCelula(relacionamentos, iUser1, iUser2)->valor = 1;
	localizaCelula(relacionamentos, iUser2, iUser1)->valor = 1;

}
//======================================================
void inserirUser(TCelula *usuarios, TCelula *relacionamentos, int *qntdUser) {
	string nomeNew;

	printf("Insira o nome do novo user: ");
	scanf("%s" , nomeNew);

	adicionarLinha(&usuarios);
	adicionarColuna(&relacionamentos);
	adicionarLinha(&relacionamentos);
	
	strcpy(localizaCelula(usuarios, *qntdUser, 0)->nome, nomeNew);
	
	(*qntdUser)++;
}
//======================================================
void registrarAmizade(TCelula *usuarios, TCelula *relacionamentos) {

	TCelula *usuarioAtual = usuarios;
	int iUser = 0, iUser1, iUser2;

	while (usuarioAtual != NULL) {
		printf("%d - %s\n" , iUser, usuarioAtual->nome);
		iUser++;
		usuarioAtual = usuarioAtual->abaixo;
	}

	printf("Insira o ID do primeiro user: ");
	scanf("%d" , &iUser1);
	printf("Insira o ID do segundo user: ");
	scanf("%d" , &iUser2);

	defineAmizade(relacionamentos, iUser1, iUser2);

	exibeMatriz(relacionamentos);

	system("pause");

}
//======================================================
void recomendarAmizade(TCelula *relacionamentos, TCelula *usuarios, int qntdUser){

	TCelula *recomendacoes = NULL;
	TCelula *linhaAtual, *colunaAtual, *colunaAtualAux, *usuarioAtual;

	int i, j, k;

	defineMatriz(&recomendacoes, qntdUser, qntdUser);

	linhaAtual = relacionamentos;
	for (i = 0; i < qntdUser; i++) {
		colunaAtual = linhaAtual;
		for (j = 0; j < qntdUser; j++) {
			colunaAtualAux = linhaAtual;
			if (colunaAtual->valor == 1) {
				for (k = 0; k < qntdUser; k++) {
					if (j != k && colunaAtualAux->valor == 1) {
						localizaCelula(recomendacoes, k, j)->valor++;
					}
					colunaAtualAux = colunaAtualAux->dir;
				}
			}
			colunaAtual = colunaAtual->dir;
		}
		linhaAtual = linhaAtual->abaixo;
	}

	usuarioAtual = usuarios;
	linhaAtual = recomendacoes;

	printf("Recomendações:\n\n");
	for (i = 0; i < qntdUser; i++) {
		printf("%s:\n", usuarioAtual->nome);
		colunaAtual = linhaAtual;
		for (j = 0; j < qntdUser; j++) {
			if (colunaAtual->valor >= 2) {
				if (localizaCelula(relacionamentos, i, j)->valor == 0) {
					printf("   %s\n", localizaCelula(usuarios, j, 0)->nome);
				}
			} 

			colunaAtual = colunaAtual->dir;
		}
		linhaAtual = linhaAtual->abaixo;
		usuarioAtual = usuarioAtual->abaixo;
	}

	system("pause");

}
//===| Funcoes e Procedimentos |========================
void inicializaMatriz(TCelula **c){
	*c = NULL;
}
//======================================================
void defineMatriz(TCelula **c, int linha, int coluna){
	
	if(*c == NULL){
		int lin,col;
		TCelula *linhaAtual = NULL, *colunaAtual = NULL;
		TCelula *colunaAux = NULL;
		
		*c = alocaCelula();
		linhaAtual = *c;
		
		for(lin=0; lin < linha; lin++){
			colunaAtual = linhaAtual;
			
			for(col=1; col < coluna; col++){
				colunaAtual->dir = alocaCelula();
				colunaAtual = colunaAtual->dir;
				
				if(colunaAux != NULL)	{
					colunaAux->abaixo = colunaAtual;
					colunaAux = colunaAux->dir;	
				}//if
				
			}//for
			
			colunaAux = linhaAtual->dir;
			
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
	novo->valor = 0;
	strcpy(novo->nome,"");
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
void adicionarLinha(TCelula **c){
/* Acrescenta uma nova linha ao final da matriz*/
	TCelula *atual, *novo, *ultimo;
	
	if((*c) != NULL){

	   atual = (*c);
	   while(atual->abaixo != NULL){
	    	atual = atual->abaixo;
	   }//while
	   
	   ultimo = NULL;
	   
	   while(atual != NULL){
	   		novo = alocaCelula();
			
			if(ultimo != NULL){
				ultimo->dir = novo;
			}//if
			
			atual->abaixo = novo;
			
			ultimo = novo;
			
			atual = atual->dir;
					
	   }//while
	   
    }//if
	
	printf("\n\n\tLinha INSERIDA com SUCESSO!!!");
	
}
//================================================================
void adicionarColuna(TCelula **c){
/* Acrescenta uma nova coluna ao final da matriz*/
	TCelula *atual, *novo, *ultimo;
	
	if((*c) != NULL){
		atual = (*c);
		
		while(atual->dir != NULL){
			atual = atual->dir;
		}//while
		
		ultimo = NULL;
		
		while(atual != NULL){
	   		novo = alocaCelula();
			
			if(ultimo != NULL){
				ultimo->abaixo = novo;
			}//if
			
			atual->dir = novo;
			
			ultimo = novo;
			
			atual = atual->abaixo;
			
		}//while
		
    }//if
    
    printf("\n\n\tColuna INSERIDA com SUCESSO!!!");
}
//================================================================
void exibeMatriz(TCelula *c){
	system("CLS");
	printf("\n\n\n\t\t=====| EXIBE MATRIZ |=====\n\n");
	
	TCelula *atual;
	TCelula *proxima = c;
	
	while(proxima != NULL){
		atual = proxima;
		while (atual != NULL)	{
			printf("  %d", atual->valor);
			atual = atual->dir;
		}//while
		printf("\n");
		proxima = proxima->abaixo;
	}//while
	
	printf("\n\n");
	system("PAUSE");
}
//================================================================
int menu(){
	int opcao;
	system("CLS");
	printf("\n\n\n\t\t=====| MENU |======\n\n");
	printf("\t0 - Sair (Encerrar Aplicacao).\n");
	printf("\t1 - Inserir NOVO usuario.\n");
	printf("\t2 - Registrar amizade entre usuarios.\n");
	printf("\t3 - Indicar novas amizades para usuarios.\n\n");
	printf("\t\tInforme OPCAO desejada: ");
	scanf("%d", &opcao);
	
	if((opcao > 3) || (opcao < 0)){
		printf("\n\n\tERRO:\n\tOPCAO invalida. Tente novamente.\n\n");
		system("PAUSE");
	}//if
	


	return opcao;
}
//================================================================


