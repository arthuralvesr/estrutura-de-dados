#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char string[40];

typedef struct tipoDisciplina {
	string nome;
	int cargaHoraria;
	tipoDisciplina *prox;
	struct tipoPreRequisito *preRequisito;
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
	printf("\n\n\n\t\t=======| MENU |=======\n\n");
	
	printf("\t0 - SAIR (Encerrar Aplicacao).\n\n");
	printf("\t1 - Inserir DISCIPLINA.\n");
	printf("\t2 - Excluir DISCIPLINA.\n\n");
	printf("\t3 - Inserir CURSOS.\n");
	printf("\t4 - Excluir CURSOS.\n");
	printf("\t5 - Registrar PRE-REQUISITOS para DISCIPLINA.\n");
	printf("\t6 - Excluir PRE-REQUISITO de DISCIPLINA.\n\n");
	printf("\t7 - Registrar GRADE CURRICULAR para CURSO.\n");
	printf("\t8 - Excluir item da GRADE CURRICULAR de CURSO.\n\n");
	printf("\t9 - Listar DISCIPLINAS.\n");
	printf("\t10- Listar CURSOS.\n");
	printf("\t11- Listar PRE_REQUISITOS de DISCIPLINA especifica.\n");
	printf("\t12- Listar GRADE CURRICULAR de CURSO especifico.\n\n");
	
	printf("\t\tInforme a OPCAO desejada: ");
	scanf("%d", &opcao);
	
	if((opcao < 0) || (opcao > 12)){
		printf("\n\n\n\tERRO: OPCAO invalida!\n\tTente Novamente\n\n");
		system("PAUSE");
	}
	return opcao;
}

void insertDisciplina(TLista *lista) {
	TDisciplina *nova, *atual;
	
	nova = (TDisciplina *)malloc(sizeof(TDisciplina));
	nova->prox = NULL;

	printf("\nCadastro de DISCIPLINA");
	printf("\nNOME do DISCIPLINA: ");
	fflush(stdin);
	gets(nova->nome);

	printf("CARGA HORARIA da DISCIPLINA: ");
	fflush(stdin);
	scanf("%d", &nova->cargaHoraria);

	if (lista->disciplinas == NULL) {
		lista->disciplinas = nova;
	} else {
		atual = lista->disciplinas;
		
		while (atual->prox != NULL) {
			atual = atual->prox;
		}
		atual->prox = nova;

	}
}

void deleteDisciplina(TLista *lista) {
	string disciplina;
	TDisciplina *atual, *anterior;

	if (lista == NULL) {
		printf("\nNenhum ator cadastrado!");
		return;
	}

	printf("\nEXCLUIR DISCIPLINA");
	printf("\nInsira a disciplina a ser excluida: ");
	fflush(stdin);
	gets(disciplina);

	atual = lista->disciplinas;
	anterior = NULL;

	while (atual != NULL && strcmp(atual->nome,disciplina) != 0) {
		anterior = atual;
		atual = atual->prox;
	}
	
	if (atual == NULL) {
		printf("Não existe NENHUMA disciplina CADASTRADA!");
		system("pause");
		return;
	}

	if (anterior == NULL) {
		lista->disciplinas = atual->prox;
	} else {
		anterior->prox = atual->prox;
	}

	printf("DISCIPLINA excluida com SUCESSO!");
}

void listDisciplina(TLista *lista) {
	TDisciplina *atual;
	int i = 1;

	if (lista->disciplinas == NULL) {
		printf("Não existe NENHUMA DISCIPLNAS CADASTRADA!");
		return;
	}
	
	atual = lista->disciplinas;

	while (atual != NULL) {
		printf("\n\n%d - %s" , i, atual->nome);
		printf("\nCarga Horaria: %d" , atual->cargaHoraria);
		
		atual = atual->prox;
		i++;
	}
	
}

void insertCurso(TLista *lista) {
	TCurso *nova, *atual;
	
	nova = (TCurso *)malloc(sizeof(TCurso));
	nova->prox = NULL;

	printf("\nCadastro de CURSO");
	printf("\nNOME do Curso: ");
	fflush(stdin);
	gets(nova->nome);

	if (nova->nome == NULL) {
		lista->cursos = nova;
	} else {
		atual = lista->cursos;
		
		while (atual->prox != NULL) {
			atual = atual->prox;
		}
		atual->prox = nova;

	}
}

int main(){
	TLista lista;
	int opcao;
	
	inicializa(&lista);

	do{
		opcao = menu();
		
		switch(opcao){
			case 0: 
				break;
			case 1: 
				insertDisciplina(&lista);
				break;
			case 2: 
				deleteDisciplina(&lista); 
				break;
			case 3: 
				insertCurso(&lista);
				break;
			case 4: 
				break;
			case 5: 
				break;
			case 6: 
				break;
			case 7: 
				break;
			case 8: 
				break;
			case 9:
				listDisciplina(&lista); 
				break;	
		}
		
	}while(opcao != 0);
	
}