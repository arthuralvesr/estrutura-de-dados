#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
/* -------------------------- TIPOS BÁSICOS -------------------------- */

typedef char string[40];

typedef struct tipoDisciplina {
    string nome;
    int cargaHoraria;
    struct tipoDisciplina *prox;
    struct tipoPreRequisito *preRequisito;
} TDisciplina;

typedef struct tipoPreRequisito {
    TDisciplina *preRequisito;
    struct tipoPreRequisito *prox;
} TPreRequisito;

typedef struct tipoGrade {
    TDisciplina *disciplina;
    int periodo;
    struct tipoGrade *prox;
} TGrade;

typedef struct tipoCurso {
    string nome;
    struct tipoCurso *ante;
    struct tipoCurso *prox;
    TGrade *gradeCurricular;
} TCurso;

typedef struct tipoLista {
    TCurso *cursos;
    TDisciplina *disciplinas;
} TLista;

/* ----------------------- FUNÇÕES UTILITÁRIAS ----------------------- */

void inicializa (TLista *lista) {
    lista->cursos      = NULL;
    lista->disciplinas = NULL;
}

/* Lê uma string com espaços descartando o newline */
void leLinha(string destino) {
    fgets(destino, sizeof(string), stdin);
    size_t n = strlen(destino);
    if (n && destino[n-1] == '\n') destino[n-1] = '\0';
}

TDisciplina *searchDisciplina (TLista *lista, const char *nome) {
    for (TDisciplina *p = lista->disciplinas; p; p = p->prox)
        if (strcmp(p->nome, nome) == 0) return p;
    return NULL;
}

TCurso *searchCurso (TLista *lista, const char *nome) {
    for (TCurso *p = lista->cursos; p; p = p->prox)
        if (strcmp(p->nome, nome) == 0) return p;
    return NULL;
}

int jaEhPrereq (const TDisciplina *principal, const TDisciplina *prereq) {
    for (TPreRequisito *n = principal->preRequisito; n; n = n->prox)
        if (n->preRequisito == prereq) return 1;
    return 0;
}

/* ---------------------- CADASTRO DE DISCIPLINAS -------------------- */

void listDisciplina (TLista *lista) {
    int i = 1;
    for (TDisciplina *p = lista->disciplinas; p; p = p->prox)
        printf("%d - %s (CH %d)\n", i++, p->nome, p->cargaHoraria);
}

void insertDisciplina (TLista *lista) {
    TDisciplina *nova = malloc(sizeof *nova);
    if (!nova) { puts("Falha de memória"); return; }
    nova->prox = NULL;
    nova->preRequisito = NULL;

    puts("\n== NOVA DISCIPLINA ==");
    printf("Nome: ");
    fflush(stdin);
    leLinha(nova->nome);
    printf("Carga Horária: ");
    scanf("%d", &nova->cargaHoraria); getchar();

    if (!lista->disciplinas) {
        lista->disciplinas = nova;
    } else {
        TDisciplina *p = lista->disciplinas;
        while (p->prox) p = p->prox;
        p->prox = nova;
    }
}

void deleteDisciplina (TLista *lista) {
    string nome; TDisciplina *ant = NULL, *p = lista->disciplinas;
    puts("\n== EXCLUIR DISCIPLINA ==");
    printf("Nome: "); fflush(stdin); leLinha(nome);

    while (p && strcmp(p->nome, nome)) { ant = p; p = p->prox; }
    if (!p) { puts("Disciplina não encontrada"); return; }

    /* REMOVER */
    if (!ant) lista->disciplinas = p->prox; else ant->prox = p->prox;

    /* liberar lista de pré‑requisitos */
    TPreRequisito *n = p->preRequisito;
    while (n) { TPreRequisito *tmp = n; n = n->prox; free(tmp); }
    free(p);
    puts("Disciplina removida com sucesso");
}

/* ------------------------ CADASTRO DE CURSOS ----------------------- */

void listCurso (TLista *lista) {
    int i = 1;
    for (TCurso *p = lista->cursos; p; p = p->prox)
        printf("%d - %s\n", i++, p->nome);
}

void insertCurso (TLista *lista) {
    TCurso *nova = malloc(sizeof *nova);
    if (!nova) { puts("Falha de memória"); return; }
    nova->prox = nova->ante = NULL;
    nova->gradeCurricular = NULL;

    puts("\n== NOVO CURSO ==");
    printf("Nome: "); fflush(stdin); leLinha(nova->nome);

    if (!lista->cursos) {
        lista->cursos = nova;
    } else {
        TCurso *p = lista->cursos;
        while (p->prox) p = p->prox;
        p->prox = nova; nova->ante = p;
    }
}

void deleteCurso (TLista *lista) {
    string nome; TCurso *ant = NULL, *p = lista->cursos;
    puts("\n== EXCLUIR CURSO ==");
    printf("Nome: "); fflush(stdin); leLinha(nome);

    while (p && strcmp(p->nome, nome)) { ant = p; p = p->prox; }
    if (!p) { puts("Curso não encontrado"); return; }

    /* liberar grade curricular */
    TGrade *g = p->gradeCurricular;
    while (g) { TGrade *tmp = g; g = g->prox; free(tmp); }

    /* remover */
    if (!ant) lista->cursos = p->prox;
    else ant->prox = p->prox;
    if (p->prox) p->prox->ante = ant;
    free(p);
    puts("Curso removido");
}

/* -------------------- PRÉ‑REQUISITOS DAS DISCIPLINAS -------------- */

void registerPreRequisito (TLista *lista) {
    if (!lista->disciplinas) { puts("Nenhuma disciplina cadastrada"); return; }

    string nomePrin, nomePre;
    puts("\n== REGISTRAR PRÉ‑REQUISITO ==");
    listDisciplina(lista);
    printf("\nDisciplina que terá o pré‑requisito: "); fflush(stdin); leLinha(nomePrin);
    TDisciplina *principal = searchDisciplina(lista, nomePrin);
    if (!principal) { puts("Disciplina não encontrada"); return; }

    printf("Disciplina que será o pré‑requisito: "); leLinha(nomePre);
    TDisciplina *prereq = searchDisciplina(lista, nomePre);
    if (!prereq) { puts("Pré‑requisito não encontrado"); return; }
    if (principal == prereq) { puts("Não pode apontar para si mesma"); return; }
    if (jaEhPrereq(principal, prereq)) { puts("Já cadastrado"); return; }

    TPreRequisito *novo = malloc(sizeof *novo);
    novo->preRequisito = prereq;
    novo->prox = principal->preRequisito;
    principal->preRequisito = novo;
    puts("Pré‑requisito registrado");
}

void deletePreRequisito (TLista *lista) {
    if (!lista->disciplinas) { puts("Nenhuma disciplina cadastrada"); return; }
    string nomePrin, nomePre;
    puts("\n== EXCLUIR PRÉ‑REQUISITO ==");
    listDisciplina(lista);
    printf("\nDisciplina principal: "); fflush(stdin); leLinha(nomePrin);
    TDisciplina *principal = searchDisciplina(lista, nomePrin);
    if (!principal) { puts("Disciplina não encontrada"); return; }

    printf("Pré‑requisito a remover: "); leLinha(nomePre);
    TDisciplina *prereq = searchDisciplina(lista, nomePre);
    if (!prereq) { puts("Pré‑requisito não encontrado"); return; }

    TPreRequisito *ant = NULL, *n = principal->preRequisito;
    while (n && n->preRequisito != prereq) { ant = n; n = n->prox; }
    if (!n) { puts("Não era pré‑requisito"); return; }

    if (!ant) principal->preRequisito = n->prox; else ant->prox = n->prox;
    free(n);
    puts("Pré‑requisito removido");
}

void listPreRequisitos (TLista *lista) {
    if (!lista->disciplinas) { puts("Nenhuma disciplina cadastrada"); return; }
    string nome;
    puts("\n== LISTAR PRÉ‑REQUISITOS ==");
    listDisciplina(lista);
    printf("\nDisciplina: "); fflush(stdin); leLinha(nome);
    TDisciplina *d = searchDisciplina(lista, nome);
    if (!d) { puts("Disciplina não encontrada"); return; }

    printf("Pré‑requisitos de %s:\n", d->nome);
    if (!d->preRequisito) { puts("(nenhum)"); return; }
    for (TPreRequisito *n = d->preRequisito; n; n = n->prox)
        printf("- %s\n", n->preRequisito->nome);
}

/* -------------------- GRADE CURRICULAR DOS CURSOS ------------------ */

void insertGradeItem (TLista *lista) {
    if (!lista->cursos || !lista->disciplinas) {
        puts("É preciso ter cursos e disciplinas cadastrados"); return; }

    string nomeCurso, nomeDisc; int periodo;
    puts("\n== INSERIR NA GRADE ==");
    listCurso(lista);
    printf("\nCurso: "); fflush(stdin); leLinha(nomeCurso);
    TCurso *curso = searchCurso(lista, nomeCurso);
    if (!curso) { puts("Curso não encontrado"); return; }

    listDisciplina(lista);
    printf("\nDisciplina: "); leLinha(nomeDisc);
    TDisciplina *disc = searchDisciplina(lista, nomeDisc);
    if (!disc) { puts("Disciplina não encontrada"); return; }

    printf("Período (1..?): "); scanf("%d", &periodo); getchar();

    /* Evita duplicidade */
    for (TGrade *g = curso->gradeCurricular; g; g = g->prox)
        if (g->disciplina == disc) { puts("Já está na grade"); return; }

    TGrade *novo = malloc(sizeof *novo);
    novo->disciplina = disc;
    novo->periodo = periodo;
    novo->prox = curso->gradeCurricular;
    curso->gradeCurricular = novo;
    puts("Inserido na grade");
}

void deleteGradeItem (TLista *lista) {
    if (!lista->cursos) { puts("Nenhum curso"); return; }
    string nomeCurso, nomeDisc;
    puts("\n== EXCLUIR ITEM DA GRADE ==");
    listCurso(lista);
    printf("\nCurso: "); fflush(stdin); leLinha(nomeCurso);
    TCurso *curso = searchCurso(lista, nomeCurso);
    if (!curso) { puts("Curso não encontrado"); return; }

    if (!curso->gradeCurricular) { puts("Grade vazia"); return; }
    listDisciplina(lista);
    printf("\nDisciplina a remover: "); leLinha(nomeDisc);
    TDisciplina *disc = searchDisciplina(lista, nomeDisc);
    if (!disc) { puts("Disciplina não encontrada"); return; }

    TGrade *ant = NULL, *g = curso->gradeCurricular;
    while (g && g->disciplina != disc) { ant = g; g = g->prox; }
    if (!g) { puts("Não estava na grade"); return; }

    if (!ant) curso->gradeCurricular = g->prox; else ant->prox = g->prox;
    free(g);
    puts("Removido da grade");
}

void listGradeCurso (TLista *lista) {
    if (!lista->cursos) { puts("Nenhum curso"); return; }
    string nomeCurso;
    puts("\n== LISTAR GRADE CURRICULAR ==");
    listCurso(lista);
    printf("\nCurso: "); fflush(stdin); leLinha(nomeCurso);
    TCurso *curso = searchCurso(lista, nomeCurso);
    if (!curso) { puts("Curso não encontrado"); return; }

    printf("Grade de %s:\n", curso->nome);
    if (!curso->gradeCurricular) { puts("(vazia)"); return; }
    for (TGrade *g = curso->gradeCurricular; g; g = g->prox)
        printf("Período %d -> %s\n", g->periodo, g->disciplina->nome);
}

/* ------------------------------ MENU ------------------------------- */

int menu () {
    puts("\n===== MENU =====");
    puts("0 - Sair");
    puts("1 - Inserir Disciplina");
    puts("2 - Excluir Disciplina");
    puts("3 - Inserir Curso");
    puts("4 - Excluir Curso");
    puts("5 - Registrar Pré‑requisito");
    puts("6 - Excluir Pré‑requisito");
    puts("7 - Inserir na Grade Curricular");
    puts("8 - Excluir da Grade Curricular");
    puts("9 - Listar Disciplinas");
    puts("10 - Listar Cursos");
    puts("11 - Listar Pré‑requisitos de Disciplina");
    puts("12 - Listar Grade Curricular do Curso");
    printf("Opção: ");
    int op; scanf("%d", &op); getchar();
    return op;
}

int main () {
    TLista lista; inicializa(&lista);
    int op;
    do {
        op = menu();
        switch (op) {
            case 1: insertDisciplina(&lista); break;
            case 2: deleteDisciplina(&lista); break;
            case 3: insertCurso(&lista); break;
            case 4: deleteCurso(&lista); break;
            case 5: registerPreRequisito(&lista); break;
            case 6: deletePreRequisito(&lista); break;
            case 7: insertGradeItem(&lista); break;
            case 8: deleteGradeItem(&lista); break;
            case 9: listDisciplina(&lista); break;
            case 10: listCurso(&lista); break;
            case 11: listPreRequisitos(&lista); break;
            case 12: listGradeCurso(&lista); break;
            case 0: puts("Encerrando..."); break;
            default: puts("Opção inválida");
        }
    } while (op != 0);

=======
#define MAX 40

typedef char string[MAX];

// Declaração antecipada das estruturas
typedef struct TDisciplina TDisciplina;
typedef struct TPreRequisito TPreRequisito;
typedef struct TGrade TGrade;
typedef struct TCurso TCurso;

// Estrutura de pré-requisito
struct TPreRequisito {
    TDisciplina *preRequisito;
    TPreRequisito *prox;
};

// Estrutura de disciplina
struct TDisciplina {
    string nome;
    int cargaHoraria;
    TDisciplina *prox;
    TPreRequisito *preRequisito;
};

// Estrutura de grade curricular
struct TGrade {
    TDisciplina *disciplina;
    int periodo;
    TGrade *prox;
};

// Estrutura de curso
struct TCurso {
    string nome;
    TCurso *ante;
    TCurso *prox;
    TGrade *gradeCurricular;
};

typedef struct {
    TCurso *cursos;
    TDisciplina *disciplinas;
} TLista;

void inicializa(TLista *lista) {
    lista->cursos = NULL;
    lista->disciplinas = NULL;    
}

int menu() {
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
    while(getchar() != '\n'); // Limpa o buffer do teclado
    
    if((opcao < 0) || (opcao > 12)) {
        printf("\n\n\n\tERRO: OPCAO invalida!\n\tTente Novamente\n\n");
        system("PAUSE");
    }
    return opcao;
}

TDisciplina* searchDisciplina(TLista *lista, string disc) {
    TDisciplina *atual = lista->disciplinas;
    while(atual != NULL) {
        if(strcmp(atual->nome, disc) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

TCurso* searchCurso(TLista *lista, string curso) {
    TCurso *atual = lista->cursos;
    while(atual != NULL) {
        if(strcmp(atual->nome, curso) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void insertDisciplina(TLista *lista) {
    TDisciplina *nova = (TDisciplina*)malloc(sizeof(TDisciplina));
    nova->prox = NULL;
    nova->preRequisito = NULL;

    printf("\nCadastro de DISCIPLINA");
    printf("\nNOME da DISCIPLINA: ");
    fgets(nova->nome, MAX, stdin);
    nova->nome[strcspn(nova->nome, "\n")] = '\0'; // Remove o \n do final

    printf("CARGA HORARIA da DISCIPLINA: ");
    scanf("%d", &nova->cargaHoraria);
    while(getchar() != '\n'); // Limpa o buffer

    if(lista->disciplinas == NULL) {
        lista->disciplinas = nova;
    } else {
        TDisciplina *atual = lista->disciplinas;
        while(atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = nova;
    }
}

void deleteDisciplina(TLista *lista) {
    string disciplina;
    printf("\nEXCLUIR DISCIPLINA");
    printf("\nInsira a disciplina a ser excluida: ");
    fgets(disciplina, MAX, stdin);
    disciplina[strcspn(disciplina, "\n")] = '\0';

    TDisciplina *atual = lista->disciplinas;
    TDisciplina *anterior = NULL;

    while(atual != NULL && strcmp(atual->nome, disciplina) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    // Remove referências como pré-requisito
    TDisciplina *disc;
    for(disc = lista->disciplinas; disc != NULL; disc = disc->prox) {
        TPreRequisito *preReq = disc->preRequisito;
        TPreRequisito *antPreReq = NULL;
        
        while(preReq != NULL) {
            if(preReq->preRequisito == atual) {
                if(antPreReq == NULL) {
                    disc->preRequisito = preReq->prox;
                } else {
                    antPreReq->prox = preReq->prox;
                }
                TPreRequisito *temp = preReq;
                preReq = preReq->prox;
                free(temp);
            } else {
                antPreReq = preReq;
                preReq = preReq->prox;
            }
        }
    }

    // Remove a disciplina
    if(anterior == NULL) {
        lista->disciplinas = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    // Libera os pré-requisitos da disciplina
    TPreRequisito *preReq = atual->preRequisito;
    while(preReq != NULL) {
        TPreRequisito *temp = preReq;
        preReq = preReq->prox;
        free(temp);
    }

    free(atual);
    printf("Disciplina removida com sucesso!\n");
}

void listDisciplina(TLista *lista) {
    if(lista->disciplinas == NULL) {
        printf("Nenhuma disciplina cadastrada!\n");
        return;
    }

    TDisciplina *atual = lista->disciplinas;
    int i = 1;
    while(atual != NULL) {
        printf("\n%d - %s", i, atual->nome);
        printf("\nCarga Horaria: %d\n", atual->cargaHoraria);
        atual = atual->prox;
        i++;
    }
}

void insertCurso(TLista *lista) {
    TCurso *nova = (TCurso*)malloc(sizeof(TCurso));
    nova->prox = NULL;
    nova->ante = NULL;
    nova->gradeCurricular = NULL;

    printf("\nCadastro de CURSO");
    printf("\nNOME do CURSO: ");
    fgets(nova->nome, MAX, stdin);
    nova->nome[strcspn(nova->nome, "\n")] = '\0';

    if(lista->cursos == NULL) {
        lista->cursos = nova;
    } else {
        TCurso *atual = lista->cursos;
        while(atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = nova;
        nova->ante = atual;
    }
}

void deleteCurso(TLista *lista) {
    string curso;
    printf("\nEXCLUIR CURSO");
    printf("\nInsira o curso a ser excluido: ");
    fgets(curso, MAX, stdin);
    curso[strcspn(curso, "\n")] = '\0';

    TCurso *atual = lista->cursos;
    TCurso *anterior = NULL;

    while(atual != NULL && strcmp(atual->nome, curso) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL) {
        printf("Curso não encontrado!\n");
        return;
    }

    // Remove a grade curricular
    TGrade *grade = atual->gradeCurricular;
    while(grade != NULL) {
        TGrade *temp = grade;
        grade = grade->prox;
        free(temp);
    }

    // Remove o curso
    if(anterior == NULL) {
        lista->cursos = atual->prox;
        if(lista->cursos != NULL) {
            lista->cursos->ante = NULL;
        }
    } else {
        anterior->prox = atual->prox;
        if(atual->prox != NULL) {
            atual->prox->ante = anterior;
        }
    }

    free(atual);
    printf("Curso removido com sucesso!\n");
}

void listCurso(TLista *lista) {
    if(lista->cursos == NULL) {
        printf("Nenhum curso cadastrado!\n");
        return;
    }

    TCurso *atual = lista->cursos;
    int i = 1;
    while(atual != NULL) {
        printf("%d - %s\n", i, atual->nome);
        atual = atual->prox;
        i++;
    }
}

void registerPreRequisito(TLista *lista) {
    if(lista->disciplinas == NULL) {
        printf("Nenhuma disciplina cadastrada!\n");
        return;
    }

    string principal, pre;
    printf("\nREGISTRAR PRE-REQUISITO\n");

    printf("Disciplinas disponíveis:\n");
    listDisciplina(lista);

    printf("\nDigite o nome da disciplina PRINCIPAL: ");
    fgets(principal, MAX, stdin);
    principal[strcspn(principal, "\n")] = '\0';

    TDisciplina *discPrincipal = searchDisciplina(lista, principal);
    if(discPrincipal == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    printf("\nDigite o nome da disciplina PRE-REQUISITO: ");
    fgets(pre, MAX, stdin);
    pre[strcspn(pre, "\n")] = '\0';

    TDisciplina *discPre = searchDisciplina(lista, pre);
    if(discPre == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    // Verifica se já não é pré-requisito
    TPreRequisito *atual = discPrincipal->preRequisito;
    while(atual != NULL) {
        if(atual->preRequisito == discPre) {
            printf("Esta disciplina já é pré-requisito!\n");
            return;
        }
        atual = atual->prox;
    }

    TPreRequisito *novo = (TPreRequisito*)malloc(sizeof(TPreRequisito));
    novo->preRequisito = discPre;
    novo->prox = discPrincipal->preRequisito;
    discPrincipal->preRequisito = novo;

    printf("Pré-requisito registrado com sucesso!\n");
}

void deletePreRequisito(TLista *lista) {
    if(lista->disciplinas == NULL) {
        printf("Nenhuma disciplina cadastrada!\n");
        return;
    }

    string principal, pre;
    printf("\nREMOVER PRE-REQUISITO\n");

    printf("Disciplinas disponíveis:\n");
    listDisciplina(lista);

    printf("\nDigite o nome da disciplina PRINCIPAL: ");
    fgets(principal, MAX, stdin);
    principal[strcspn(principal, "\n")] = '\0';

    TDisciplina *discPrincipal = searchDisciplina(lista, principal);
    if(discPrincipal == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    if(discPrincipal->preRequisito == NULL) {
        printf("Esta disciplina não tem pré-requisitos!\n");
        return;
    }

    printf("\nPré-requisitos de %s:\n", discPrincipal->nome);
    TPreRequisito *preReq = discPrincipal->preRequisito;
    int i = 1;
    while(preReq != NULL) {
        printf("%d - %s\n", i, preReq->preRequisito->nome);
        preReq = preReq->prox;
        i++;
    }

    printf("\nDigite o nome do pré-requisito a ser removido: ");
    fgets(pre, MAX, stdin);
    pre[strcspn(pre, "\n")] = '\0';

    TDisciplina *discPre = searchDisciplina(lista, pre);
    if(discPre == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    TPreRequisito *atual = discPrincipal->preRequisito;
    TPreRequisito *anterior = NULL;

    while(atual != NULL && atual->preRequisito != discPre) {
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL) {
        printf("Pré-requisito não encontrado!\n");
        return;
    }

    if(anterior == NULL) {
        discPrincipal->preRequisito = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    printf("Pré-requisito removido com sucesso!\n");
}

void listPreRequisitos(TLista *lista) {
    if(lista->disciplinas == NULL) {
        printf("Nenhuma disciplina cadastrada!\n");
        return;
    }

    string disciplina;
    printf("\nLISTAR PRE-REQUISITOS\n");
    printf("Disciplinas disponíveis:\n");
    listDisciplina(lista);

    printf("\nDigite o nome da disciplina: ");
    fgets(disciplina, MAX, stdin);
    disciplina[strcspn(disciplina, "\n")] = '\0';

    TDisciplina *disc = searchDisciplina(lista, disciplina);
    if(disc == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    if(disc->preRequisito == NULL) {
        printf("Esta disciplina não tem pré-requisitos!\n");
        return;
    }

    printf("\nPré-requisitos de %s:\n", disc->nome);
    TPreRequisito *preReq = disc->preRequisito;
    int i = 1;
    while(preReq != NULL) {
        printf("%d - %s\n", i, preReq->preRequisito->nome);
        preReq = preReq->prox;
        i++;
    }
}

void registerGrade(TLista *lista) {
    if(lista->cursos == NULL) {
        printf("Nenhum curso cadastrado!\n");
        return;
    }

    if(lista->disciplinas == NULL) {
        printf("Nenhuma disciplina cadastrada!\n");
        return;
    }

    string curso, disciplina;
    int periodo;

    printf("\nREGISTRAR GRADE CURRICULAR\n");
    printf("Cursos disponíveis:\n");
    listCurso(lista);

    printf("\nDigite o nome do curso: ");
    fgets(curso, MAX, stdin);
    curso[strcspn(curso, "\n")] = '\0';

    TCurso *c = searchCurso(lista, curso);
    if(c == NULL) {
        printf("Curso não encontrado!\n");
        return;
    }

    printf("\nDisciplinas disponíveis:\n");
    listDisciplina(lista);

    printf("\nDigite o nome da disciplina: ");
    fgets(disciplina, MAX, stdin);
    disciplina[strcspn(disciplina, "\n")] = '\0';

    TDisciplina *d = searchDisciplina(lista, disciplina);
    if(d == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    printf("Digite o período: ");
    scanf("%d", &periodo);
    while(getchar() != '\n');

    // Verifica se já está na grade
    TGrade *grade = c->gradeCurricular;
    while(grade != NULL) {
        if(grade->disciplina == d) {
            printf("Esta disciplina já está na grade deste curso!\n");
            return;
        }
        grade = grade->prox;
    }

    TGrade *nova = (TGrade*)malloc(sizeof(TGrade));
    nova->disciplina = d;
    nova->periodo = periodo;
    nova->prox = c->gradeCurricular;
    c->gradeCurricular = nova;

    printf("Disciplina adicionada à grade curricular com sucesso!\n");
}

void deleteGradeItem(TLista *lista) {
    if(lista->cursos == NULL) {
        printf("Nenhum curso cadastrado!\n");
        return;
    }

    string curso, disciplina;
    printf("\nREMOVER ITEM DA GRADE CURRICULAR\n");
    printf("Cursos disponíveis:\n");
    listCurso(lista);

    printf("\nDigite o nome do curso: ");
    fgets(curso, MAX, stdin);
    curso[strcspn(curso, "\n")] = '\0';

    TCurso *c = searchCurso(lista, curso);
    if(c == NULL) {
        printf("Curso não encontrado!\n");
        return;
    }

    if(c->gradeCurricular == NULL) {
        printf("Este curso não tem disciplinas em sua grade curricular!\n");
        return;
    }

    printf("\nDisciplinas na grade de %s:\n", c->nome);
    TGrade *grade = c->gradeCurricular;
    int i = 1;
    while(grade != NULL) {
        printf("%d - %s (Período: %d)\n", i, grade->disciplina->nome, grade->periodo);
        grade = grade->prox;
        i++;
    }

    printf("\nDigite o nome da disciplina a ser removida: ");
    fgets(disciplina, MAX, stdin);
    disciplina[strcspn(disciplina, "\n")] = '\0';

    TDisciplina *d = searchDisciplina(lista, disciplina);
    if(d == NULL) {
        printf("Disciplina não encontrada!\n");
        return;
    }

    grade = c->gradeCurricular;
    TGrade *anterior = NULL;

    while(grade != NULL && grade->disciplina != d) {
        anterior = grade;
        grade = grade->prox;
    }

    if(grade == NULL) {
        printf("Esta disciplina não está na grade deste curso!\n");
        return;
    }

    if(anterior == NULL) {
        c->gradeCurricular = grade->prox;
    } else {
        anterior->prox = grade->prox;
    }

    free(grade);
    printf("Disciplina removida da grade curricular com sucesso!\n");
}

void listGradeCurricular(TLista *lista) {
    if(lista->cursos == NULL) {
        printf("Nenhum curso cadastrado!\n");
        return;
    }

    string curso;
    printf("\nLISTAR GRADE CURRICULAR\n");
    printf("Cursos disponíveis:\n");
    listCurso(lista);

    printf("\nDigite o nome do curso: ");
    fgets(curso, MAX, stdin);
    curso[strcspn(curso, "\n")] = '\0';

    TCurso *c = searchCurso(lista, curso);
    if(c == NULL) {
        printf("Curso não encontrado!\n");
        return;
    }

    if(c->gradeCurricular == NULL) {
        printf("Este curso não tem disciplinas em sua grade curricular!\n");
        return;
    }

    printf("\nGrade Curricular de %s:\n", c->nome);
    printf("Disciplina\t\tCarga Horária\tPeríodo\n");
    printf("--------------------------------------------\n");
    
    TGrade *grade = c->gradeCurricular;
    while(grade != NULL) {
        printf("%-20s\t%d\t\t%d\n", 
               grade->disciplina->nome, 
               grade->disciplina->cargaHoraria, 
               grade->periodo);
        grade = grade->prox;
    }
}

int main() {
    TLista lista;
    int opcao;
    
    inicializa(&lista);

    do {
        opcao = menu();
        
        switch(opcao) {
            case 0: 
                printf("Encerrando aplicação...\n");
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
                deleteCurso(&lista); 
                break;
            case 5: 
                registerPreRequisito(&lista); 
                break;
            case 6: 
                deletePreRequisito(&lista); 
                break;
            case 7: 
                registerGrade(&lista); 
                break;
            case 8: 
                deleteGradeItem(&lista); 
                break;
            case 9: 
                listDisciplina(&lista); 
                break;
            case 10: 
                listCurso(&lista); 
                break;
            case 11: 
                listPreRequisitos(&lista); 
                break;
            case 12: 
                listGradeCurricular(&lista); 
                break;
        }
        
        system("PAUSE");
    } while(opcao != 0);
    
>>>>>>> 5ae89ca81849500a8a8b802defb340100a11026e
    return 0;
}
