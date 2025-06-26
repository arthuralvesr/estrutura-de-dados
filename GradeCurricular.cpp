#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    return 0;
}
