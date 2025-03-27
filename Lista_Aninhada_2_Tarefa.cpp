#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char string[40];

typedef struct tipoDisciplina {
	string nome;
	int cargaHoraria;
	tipoDisciplina *prox;
	TPreRequisito *preRequisitos;	
}TDisciplina;

typedef struct tipoPreRequisito {
	TDisciplina *preRequisito;
	tipoPreRequisito *prox;
}TPreRequisito;

typedef struct tipoGrade {
	TDisciplina *disciplina;
	int periodo;
	tipoGrade *prox;	
}TGrade;

typedef struct tipoCurso {
	string nome;
	tipoCurso *ante;
	tipoCurso *prox;
	TGrade *gradeCurricular;
}TCurso;

typedef struct tipoLista {
	TCurso *cursos;
	TDisciplina *disciplinas;
}TLista;


void inicializa(TLista *lista){
	lista->cursos = NULL;
	lista->disciplinas = NULL;	

}

int menu(){
	
	int opcao;
	system("CLS");
	
	printf("\n\n\n\t\t=======| MENU |=======\n\n");
	
	printf("\t0 - SAIR (Encerrar Aplicacao).\n\n");
	printf("\t1 - Inserir DISCIPLINAS.\n");
	printf("\t2 - Excluir DISCIPLINAS.\n\n");
	printf("\t3 - Inserir CURSOS.\n");
	printf("\t4 - Excluir CURSOS.\n");
	printf("\t5 - Registrar PRE-REQUISITOS para DISCIPLINA.\n");
	printf("\t6 - Excluir PRE-REQUISITO de DISCIPLINA.\n\n");
	printf("\t7 - Registrar GRADE CURRICULAR para CURSO.\n");
	printf("\t8 - Excluir item da GRADE CURRICULAR de CURSO.\n\n");
	
	printf("\t\tInforme a OPCAO desejada: ");
	scanf("%d", &opcao);
	
	if((opcao < 0) || (opcao > 8)){
		printf("\n\n\n\tERRO: OPCAO invalida!\n\tTente Novamente\n\n");
		system("PAUSE");
	}
	return opcao;
}

void insertDisciplina(TLista *lista) {
	TDisciplina *nova, *atual;
	
	nova = (TDisciplina *)malloc(sizeof(TDisciplina));
	nova->prox = NULL;

	printf("\nCadastro de DISCIPLINA: ");
	printf("\nNOME do DISCIPLINA: ");
	fflush(stdin);
	gets(nova->nome);

	printf("\nCARGA HORARIA da DISCIPLINA: ");
	fflush(stdin);
	scanf("%d", &nova->cargaHoraria);
	
	if (lista->disciplinas == NULL) {
		lista->disciplinas == nova;
	} else {
		atual = lista->disciplinas;
		while (atual->prox != NULL) {
			atual = atual->prox;
		}
		atual->prox = nova;
	}

}

void deleteDiscipline(TLista *lista) {

	
}

int main(){
	TLista *lista;
	int opcao;

	inicializa(lista);

	do{
		opcao = menu();
		
		switch(opcao){
			case 0: break;
			case 1: insertDiscipline(lista); break;
			case 2: deleteDiscipline(lista); break;
			case 3: break;
			case 4: break;
			case 5: break;
			case 6: break;
		}
		
	}while(opcao != 0);
	
}