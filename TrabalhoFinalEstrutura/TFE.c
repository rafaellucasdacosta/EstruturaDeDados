#include <stdio.h>
#include <stdlib.h>
#include "TFE.h"

//Funcoes de inicializar
PONT inicializaArvore() {
    return(NULL);
}
void inicializaLista(LISTA* l){
    l->inicio = NULL;
}
//

//Funcoes de inserir
void inserirDesordenado(LISTA* l, ALUNO novoAluno){
    PONTLIST atual = l->inicio;
    PONTLIST auxRepetir = l->inicio;
    PONTLIST novo = NULL;
    novo = (PONTLIST) malloc(sizeof(ELEMENTO));
    if (novo == NULL){
        printf("\n\nErro de alocação de memória\n\n");
        return;
    }
    while(auxRepetir != NULL){ //checa se o nome ou ra já estão cadastrados
        if(auxRepetir->aluno.RA == novoAluno.RA || strcmp(auxRepetir->aluno.nome, novoAluno.nome) == 0){
            printf("\n\nErro, aluno ja esta cadastrado!\n\n");
            return;
        }
        auxRepetir = auxRepetir->prox;
    }
    novo->aluno = novoAluno;//insere o novo aluno no inicio da lista
    l->inicio = novo;
    novo->prox = atual;
}

void inserirFinal(LISTA *l, ALUNO aluno){
    PONTLIST novo;
    PONTLIST aux = l->inicio;

    novo = (PONT) malloc(sizeof(ELEMENTO));
    novo->aluno=aluno;
    novo->prox=NULL;

    if(aux == NULL){
        l->inicio = novo;
    }
    else{
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

PONT insereRA(PONT raizRA, ALUNO *e) {//insere na arvore RA
    if (!raizRA) return(criaNovoNo(e));
    /*a chamada recursiva de insere ira percorrer a arvore ate encontrar o
     o lugar onde o no sera inserido, entao, o mesmo eh inserido com a chamada
     criaNovoNo(aluno)*/
    if (e->RA < raizRA->aluno->RA) {//caso o novo RA seja menor que o no atual, recursao para a esquerda da lista
        raizRA->esq = insereRA(raizRA->esq, e);
        raizRA = rotacao(raizRA);
    }
    else{
        if (e->RA > raizRA->aluno->RA){// caso contrario, para a direita da arvore
            raizRA->dir = insereRA(raizRA->dir, e);
            raizRA = rotacao(raizRA);
        }
    }
    raizRA->h = max(altura(raizRA->esq), altura(raizRA->dir)) + 1; //atualiza a altura
    return(raizRA);
}

PONT insereNome(PONT raizNome, ALUNO *e){
    if (!raizNome) return(criaNovoNo(e));
    if (strcmp(e->nome, raizNome->aluno->nome) < 0){
        raizNome->esq = insereNome(raizNome->esq, e);
        raizNome = rotacao(raizNome);
    }
    else{
        if (strcmp(e->nome, raizNome->aluno->nome) > 0){
            raizNome->dir = insereNome(raizNome->dir, e);
            raizNome = rotacao(raizNome);
        }

    }
    raizNome->h = max(altura(raizNome->esq), altura(raizNome->dir)) + 1;
    return(raizNome);
}
//

//funcoes de reinicializar
void reinicializar(LISTA* l){
    PONTLIST aux = l->inicio;
    while(l->inicio != NULL){//segue o mesmo princípio do inserir elemento, repetindo até não existir elementos na lista
        l->inicio = aux->prox; //inicio irá passar para o próximo elemento, assim ficando salvo e permitindo que o elemento atual seja liberado
        free(aux); //liberação da memória
        aux = l->inicio; //o elemento armazenado como inicio da lista é passado para o ponteiro
    }
}

void reinicializarArvore(PONT raiz){
    if (raiz != NULL) {//enquanto existirem elementos, recursividade para ambos os nós ate chegar ao ultimo e libera-los
        reinicializarArvore(raiz->esq);
        reinicializarArvore(raiz->dir);
        free(raiz);
    }
}
//

//funcao para criar um novo no a partir de uma struct aluno, com todos os dados do aluno ja inseridos
PONT criaNovoNo(ALUNO *a) {
    PONT novoNo = (PONT)malloc(sizeof(NO));
    novoNo->aluno = a;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->h = 0;
    return(novoNo);
}

//funcoes de auxilio para rotacoes
int max(int a, int b){//retorna o maior inteiro
    if (a > b) return a;
    return b;
}

int altura(PONT raiz) {//retorna altura do no
    if (!raiz) return -1;
    return raiz->h;
}

int fatorB(PONT raiz) {//fator para calcular rotacoes
    return (altura(raiz->esq) - altura(raiz->dir));
}

int defAltura(PONT p) {//determina altura do no
    return (max(altura(p->dir), altura(p->esq)) + 1);
}
//

//rotacao a direita
PONT direita(PONT r) {
    PONT aux;
    aux = r->esq; //aux eh a subarvore da esquerda do no desbalanceado
    r->esq = aux->dir; //r adota o filho de aux
    aux->dir = r; //aux passa a ser pai de r

    //atualiza as alturas dos nos que foram rotacionados
    r->h = max(altura(r->dir), altura(r->esq)) + 1;
    aux->h = max(altura(aux->esq), r->h) + 1;

    return(aux); //retorna a nova raiz
}

// Rotação a esquerda
PONT esquerda(PONT r) {
    PONT aux;
    aux = r->dir; //aux eh a subrvore da direita do nó desbalanceado
    r->dir = aux->esq; //r adota o filho de aux
    aux->esq = r; //aux passa a ser pai de r

    //atualiza as alturas dos nós que foram rotacionados
    r->h = max(altura(r->dir), altura(r->esq)) + 1;
    aux->h = max(altura(aux->esq), r->h) + 1;

    return(aux); //retorna a nova raiz
}

PONT esquerdaDireita(PONT r) { //LR
    r->esq = esquerda(r->esq); //rotaciona o filho a esquerda do no desbalanceado
    return(direita(r)); //rotacao a direita do no desbalanceado
}

PONT direitaEsquerda(PONT r) {
    r->dir = direita(r->dir); //rotaciona o filho a direita do no desbalanceado
    return(esquerda(r)); //rotacao a esquerda do no desbalanceado
}

PONT rotacao(PONT raiz) {//realiza a rotacao de acordo com o fator do no

    if (fatorB(raiz) >= 2){
        if (fatorB(raiz->esq) == 0 || fatorB(raiz->esq) == 1)
            raiz = direita(raiz);
        else
            raiz = esquerdaDireita(raiz);
    }
    if (fatorB(raiz) <= -2){
        if (fatorB(raiz->dir) == 0 || fatorB(raiz->dir) == -1)
            raiz = esquerda(raiz);
        else
            raiz = direitaEsquerda(raiz);
    }

    return raiz;
}

//funcoes de leitura e busca

void buscarRA(PONT raiz, int ra){//recebe a raiz ra e um numero de ra
    if (raiz != NULL){//recursividade de acordo com o numero do no atual em relacao ao desejado
        if(raiz->aluno->RA < ra){
            buscarRA(raiz->dir, ra);
        }
        else if (raiz->aluno->RA > ra){
            buscarRA(raiz->esq, ra);
        }
        else if (raiz->aluno->RA == ra){//caso seja igual, retorna os dados
            printf("\nAluno encontrado!\nNome: %s \nRA: %d \nCurso: %d\n", raiz->aluno->nome, raiz->aluno->RA, raiz->aluno->curso);
            return;
        }
    }
}

void buscarNome(PONT raiz, char nome[200]){//recebe a raiz nome e um nome
    if(raiz != NULL){//recursividade de acordo com o nome em ordem alfabetica do no atual em relacao ao desejado
        if(strcmp(raiz->aluno->nome, nome) < 0){
            buscarNome(raiz->dir, nome);
        }
        else if (strcmp(raiz->aluno->nome, nome) > 0){
            buscarNome(raiz->esq, nome);
        }
        else if (strcmp(raiz->aluno->nome, nome) == 0){
            printf("\nAluno encontrado!\nNome: %s \nRA: %d \nCurso: %d\n", raiz->aluno->nome, raiz->aluno->RA, raiz->aluno->curso);
            return;
        }
    }
}

void buscarTodosAlunosCurso(PONT raiz, int curso){//busca por curso, ao contrario das buscas por nome ou ra, nao executa return; ao encontrar um
                                                    //elemento correspondente
    if (raiz != NULL){
        if (raiz->aluno->curso == curso){
            printf("\nNome: %s \nRA: %d \nCurso: %d\n", raiz->aluno->nome, raiz->aluno->RA, raiz->aluno->curso);
            buscarTodosAlunosCurso(raiz->esq, curso);
            buscarTodosAlunosCurso(raiz->dir, curso);
        }
        else if(raiz->aluno->curso < curso){
            buscarTodosAlunosCurso(raiz->dir, curso);
        }
        else if (raiz->aluno->curso > curso){
            buscarTodosAlunosCurso(raiz->esq, curso);
        }
    }
}

void leituraPreOrdem(PONT raiz){
    if (raiz != NULL) {
        printf("\nNome: %s \nRA: %d \nCurso: %d\n", raiz->aluno->nome, raiz->aluno->RA, raiz->aluno->curso);
        leituraPreOrdem(raiz->esq);
        leituraPreOrdem(raiz->dir);
    }
}
//

//funcoes para remocao
    //auxiliares para remocao das arvores
PONT procuraNoNome(PONT q, PONT p){
    if (strcmp(q->aluno->nome, p->aluno->nome) == 0){
        q->h = defAltura(q);
        q = rotacao(q);
        return q;
    }
    if (strcmp(p->aluno->nome, q->aluno->nome) < 0){
        q = procuraNoNome(q->esq, p);
    }
    else{
        q = procuraNoNome(q->dir, p);
    }
    q->h = defAltura(q);
    q = rotacao(q);
    return q;
}

PONT procuraNoRA(PONT q, PONT p){
    if (q->aluno->RA == p->aluno->RA){
        q->h = defAltura(q);
        q = rotacao(q);
        return q;
    }
    if (p->aluno->RA < q->aluno->RA){
        q = procuraNoRA(q->esq, p);
    }
    else{
        q = procuraNoRA(q->dir, p);
    }
    q->h = defAltura(q);
    q = rotacao(q);
    return q;
}
    //

void removerListaRA(LISTA* l, int ra){//remove da lista caso um ra seja passado
    PONTLIST aux = l->inicio;
    PONTLIST anterior = NULL;
    if(aux == NULL){
        printf("\nLista vazia\n");
        return;
    }
    while(aux != NULL){//percorre a lista enquanto existirem elementos ou encontrar um correspondente
        if (aux->aluno.RA == ra){
            if(anterior == NULL){
                l->inicio = aux->prox;
                free(aux);
            }
            else{
                anterior->prox = aux->prox; //o ponteiro do elemento anterior será redefinido como o do próximo ao que se deseja excluir
                free(aux); //liberação de memória
                aux = NULL;
            }
            return;
        }
        else{
            anterior = aux;
            aux = aux->prox;
        }
    }
    printf("\nAluno não cadastrado!\n");
}

//funcoes de remocao
void removerListaNome(LISTA* l, char nome[200]){//remove da lista caso um nome seja passado
    PONTLIST aux = l->inicio;
    PONTLIST anterior = NULL;
    if(aux == NULL){
        printf("\nLista vazia\n");
        return;
    }
    while(aux != NULL){//percorre a lista enquanto existirem elementos ou encontrar um correspondente
        if (strcmp(aux->aluno.nome, nome) == 0){
            if(anterior == NULL){
                l->inicio = aux->prox;
                free(aux);
                aux = NULL;
            }
            else{
                anterior->prox = aux->prox; //o ponteiro do elemento anterior será redefinido como o do próximo ao que se deseja excluir
                free(aux); //liberação de memória
                aux = NULL;
            }
            return;
        }
        else{
            anterior = aux;
            aux = aux->prox;
        }
    }
    printf("\nAluno não cadastrado!\n");
}

ALUNO salvarInfo(LISTA*l, int opc){ //salva o aluno a ser excluido, possibilitando que ambas as
    //arvores tenham acesso aos dados necessarios para removerem corretamente o item
    ALUNO aluno;
    PONTLIST aux = l->inicio;
    char auxNome[200];
    int auxRA;
    if (opc == 1){
        printf("\nNome do aluno a ser excluido: ");
        gets(auxNome);
        while(aux != NULL){
            if(strcmp(aux->aluno.nome, auxNome) == 0){
                aluno = aux->aluno;
                return aluno;
            }
            aux = aux->prox;
        }
    }
    else{
        printf("\nRA do aluno a ser removido: ");
        scanf("%d", &auxRA);
        while(aux != NULL){
            if (aux->aluno.RA == auxRA){
                aluno = aux->aluno;
                return aluno;
            }
            aux = aux->prox;
        }
    }
    return aluno;
}

PONT removerRA(PONT raiz, int ra){//funcao para remover de acordo com o RA do aluno
    if (!raiz){
        return raiz;
    }

    PONT pai = NULL;

    if (ra < raiz->aluno->RA){

        pai = raiz->esq;
        raiz->esq = removerRA(raiz->esq, ra);

        raiz = rotacao(raiz);
    }
    else if (ra> raiz->aluno->RA){
        pai = raiz->dir;
        raiz->dir = removerRA(raiz->dir, ra);
        raiz = rotacao(raiz);
    }

    else if (ra == raiz->aluno->RA){
        PONT p = NULL, q;
        if (!raiz->esq || !raiz->dir){

            if(!raiz->esq)
                q = raiz->dir;
            else
                q = raiz->esq;

        }
        else{
            p = raiz;
            q = raiz->esq;

            while(q->dir){
                p = q;
                q = q->dir;
            }

            if (p != raiz) {
                p->dir = q->esq;
                q->esq = raiz->esq;
            }
            q->dir = raiz->dir;

            if (p == raiz)
                q = rotacao(q);

            else if (p->aluno->RA < q->aluno->RA)
                q->esq = procuraNoRA(q, p);

            else
                q->dir = procuraNoRA(q, p);

            q->h = defAltura(q);
            q = rotacao(q);
        }
        if(!pai){
            free(raiz);
            return(q);
        }

        if (ra < pai->aluno->RA)
            pai->esq = q;
        else
            pai->dir = q;

        free(raiz);

        q->h = defAltura(q);

        return q;
    }

    raiz->h = defAltura(raiz);
    return raiz;
}

PONT removerNome(PONT raiz, char nome[200]){//remove o aluno da arvore de acordo com o nome passado
    if (!raiz){
        return raiz;
    }

    PONT pai = NULL;
    if (strcmp(nome, raiz->aluno->nome) < 0){
        pai = raiz->esq;
        raiz->esq = removerNome(raiz->esq, nome);
        raiz = rotacao(raiz);
    }
    else if (strcmp(nome, raiz->aluno->nome) > 0){
        pai = raiz->dir;
        raiz->dir = removerNome(raiz->dir, nome);
        raiz = rotacao(raiz);
    }
    else if (strcmp(nome, raiz->aluno->nome) == 0){
        PONT p = NULL, q;
        if (!raiz->esq || !raiz->dir){

            if(!raiz->esq)
                q = raiz->dir;
            else
                q = raiz->esq;

        }
        else{
            p = raiz;
            q = raiz->esq;

            while(q->dir){
                p = q;
                q = q->dir;
            }

            if (p != raiz) { // o pai do n� substituto � o n� a ser removido?
                p->dir = q->esq;
                q->esq = raiz->esq;
            }
            q->dir = raiz->dir;

            if (p == raiz) //se q for igual o no removido, fa�o a rota��o somente em q
                q = rotacao(q);
            else if (strcmp(p->aluno->nome, q->aluno->nome) < 0)
                q->esq = procuraNoNome(q, p);
            else q->dir = procuraNoNome(q, p);
            q->h = defAltura(q);
            q = rotacao(q);
        }
        if(!pai) { //n� a ser removido � o n� raiz?
            free(raiz);
            return(q);
        }

        if (strcmp(nome, pai->aluno->nome) < 0)
            pai->esq = q;
        else
            pai->dir = q;

        free(raiz);

        q->h = defAltura(q);

        return q;
    }

    raiz->h = defAltura(raiz);
    return raiz;
}

void menu() //retorna um printf do menu
{
    printf("\nMENU \n\n 1- Inserir.\n 2- Remover por nome.\n 3- Remover por RA. \n 4- Buscar por nome. \n 5- Busca por RA.\n 6- Buscar todos alunos de um curso.\n 7- Leitura de ambas as arvores.\n 8- Sair do programa e desalocar endereços de memória.\n\n");
}

int setCurso(int ra){ //checa se o curso inserido é válido
    int curso = (ra / 10000);
    int listaCursos[6] = {10, 11, 12, 13, 14, 15};
    int x;
    for (x = 0; x <= 5; x++){
        if (listaCursos[x] == curso){
            return (curso);
        }
    }
    return (-1);
}
