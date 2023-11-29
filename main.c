#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <string.h>

int ID = 0;

// funcao para colocar o ponteiro do mouse em certa posicao do console
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// muda a cor do console
void setColor(int foreground, int background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, foreground | (background << 4));
}

// printa a estrutura principal da tela
void tela()
{
    int t;
    system("cls");
    gotoxy(01, 01);
    printf(" _________________________________________________________________________________________________");
    gotoxy(01, 02);
    printf("| UNICV");
    gotoxy(80, 02);
    printf("Estrutura de Dados |");
    gotoxy(01, 03);
    printf("|");
    gotoxy(40, 02);
    printf("JOGO DO MILHAO");
    gotoxy(99, 03);
    printf("|");
    gotoxy(01, 04);
    printf("|-------------------------------------------------------------------------------------------------|");
    for (t = 5; t < 24; t++)
    {
        gotoxy(01, t);
        printf("|");
        gotoxy(99, t);
        printf("|");
    }
    gotoxy(01, 22);
    printf("|-------------------------------------------------------------------------------------------------|");
    gotoxy(02, 23);
    printf("MSG.:");
    gotoxy(01, 24);
    printf("|-------------------------------------------------------------------------------------------------|");
}

// estrutura das perguntas
typedef struct 
{
    int id_questao;
    char ds_pergunta[100]; 
    char ds_alternativa1[100];
    char ds_alternativa2[100];
    char ds_alternativa3[100];
    char ds_alternativa4[100];
    int nivel_pergunta;
    int resp_ok;
    int pergunta_feita;
} reg_questoes;

// estrutura do save
typedef struct reg_save
{
    int num_pergunta;
    int cod_questao;
    int tem_carta;
    int tem_placa;
    int tem_univ;
    int pos_input;
} reg_save;
typedef struct reg_save *ApontadorSave;

// matriz com os valores das perguntas
int valores_pergunta[16][3] = {
        {1000, 0, 0},
        {2000, 1000, 500},
        {3000, 2000, 1000},
        {4000, 3000, 1500},
        {5000, 4000, 2000},
        {10000, 5000, 2500},
        {20000, 10000, 5000},
        {30000, 20000, 10000},
        {40000, 30000, 15000},
        {50000, 40000, 20000},
        {100000, 50000, 25000},
        {200000, 100000, 50000},
        {300000, 200000, 100000},
        {400000, 300000, 150000},
        {500000, 400000, 200000},
        {1000000, 500000, 0}
    };
    //  acertar  parar    errar

typedef struct TipoItem *TipoApontador;

typedef struct TipoItem {
    TipoApontador anterior;
    reg_questoes conteudo;
    TipoApontador proximo;
    int posicao;
} TipoItem;

typedef struct {
    TipoApontador Primeiro;
    TipoApontador Ultimo;
    int Tamanho;
} TipoLista;

void leStringDoConsole(char *strDeRetorno, int tamanhoMax) {
    fgets(strDeRetorno, tamanhoMax, stdin);
    size_t  len = strlen(strDeRetorno); 
    if (strDeRetorno[len-1] == '\n') { 
        strDeRetorno[len-1] = '\0';   
    }
}

// pesquisa a pergunta por codigo
TipoApontador pesquisa(TipoLista *L, int codigo)
{
    TipoApontador aux;
    aux = L->Primeiro;
    while (aux != NULL)
    {
        if (aux->conteudo.id_questao == codigo)
        {
            break;
        }
        aux = aux->proximo;
    }
    return aux;
}

// pesquisa a pergunta por posicao na lista
TipoApontador pesquisa_por_posicao(TipoLista *L, int posicao)
{
    TipoApontador aux;
    aux = L->Primeiro;
    while (aux != NULL)
    {
        if (aux->posicao == posicao)
        {
            break;
        }
        aux = aux->proximo;
    }
    return aux;
}

// organiza as posicoes da lista
void organizaIndices(TipoLista *lista) {
    TipoApontador p_auxiliar;
    p_auxiliar = lista->Primeiro;
    int pos = 0;
    while (p_auxiliar != NULL) {
        p_auxiliar->posicao = pos;// corrigir segmantation fault
        pos++;
        p_auxiliar = p_auxiliar->proximo;
    }
}

// le descricao da pergunta com 100 caracteres
char *le_ds_pergunta()
{
    char *ds_pergunta;
    ds_pergunta = malloc(sizeof(char) * 100);
    do
    {
        gotoxy(35, 07);
        printf("                                                                |");
        gotoxy(35, 07);
        fgets(ds_pergunta, 100, stdin);

        if ((strlen(ds_pergunta) == 1) && (strcmp(ds_pergunta, "0") == -1))
        {
            gotoxy(07, 23);
            printf("Descricao da pergunta e obrigatoria ");
            getch();
            gotoxy(07, 23);
            printf("                                    ");
        }
    } while ((strlen(ds_pergunta) == 1) && (strcmp(ds_pergunta, "0") == -1));
    return ds_pergunta;
}

// le alternativas
void le_ds_alternativa(char *ds_alternativa, int y)
{
    do
    {
        gotoxy(35, y);
        printf("                                                                |");
        gotoxy(35, y);
        fgets(ds_alternativa, 100, stdin);

        if ((strlen(ds_alternativa) == 1) && (strcmp(ds_alternativa, "0") == -1))
        {
            gotoxy(07, 23);
            printf("Alternativa obrigatoria ");
            getch();
            gotoxy(07, 23);
            printf("                       ");
        }
    } while ((strlen(ds_alternativa) == 1) && (strcmp(ds_alternativa, "0") == -1));
}

// le numero
int le_nr_numero(int y, int limiteInf, int limiteSup, int x)
{
    int nr_numero;
    do
    {
        gotoxy(x, y);
        printf("                                                                |");
        gotoxy(x, y);
        scanf("%d", &nr_numero);
        // getchar();
        if (nr_numero < limiteInf || nr_numero > limiteSup)
        {
            gotoxy(07, 23);
            printf("Valores permitidos: de %d a %d ", limiteInf, limiteSup);
            getch();
            gotoxy(07, 23);
            printf("                               ");
        }
    } while (nr_numero < limiteInf || nr_numero > limiteSup);
    return nr_numero;
}

// le opcao no jogo
int le_nr_numero_jogo(int y, int limiteInf, int limiteSup, int x, char *desc)
{
    int nr_numero;
    do
    {
        gotoxy(x, y);
        printf("  %s.:                                                                           ", desc);
        gotoxy(25, y);
        scanf("%d", &nr_numero);
        // getch();
        if (nr_numero < limiteInf || nr_numero > limiteSup)
        {
            gotoxy(02, 23);
            printf("Valores permitidos: de %d a %d. ", limiteInf, limiteSup);
            // getch();
            system("pause");
            gotoxy(02, 23);
            printf("                               ");
        }
    } while (nr_numero < limiteInf || nr_numero > limiteSup);
    return nr_numero;
}

// mostra a estrutura da pergunta
void tela_questoes()
{
    tela();
    gotoxy(07, 05);
    printf("   ID...................: ");
    gotoxy(40, 05);
    printf("   Posicao: ");
    gotoxy(07, 07);
    printf("1- Descricao............: ");
    gotoxy(07, 9);
    printf("2- Alternativa 1........: ");
    gotoxy(07, 11);
    printf("3- Alternativa 2........: ");
    gotoxy(07, 13);
    printf("4- Alternativa 3........: ");
    gotoxy(07, 15);
    printf("5- Alternativa 4........: ");
    gotoxy(07, 17); 
    printf("6- Nivel................: ");
    gotoxy(07, 19);
    printf("7- Alternativa correta..: ");
    gotoxy(07, 21);
    printf("8- Pergunta feita.......: ");
    gotoxy(40, 21);
    printf("0 = Nao | 1 = Sim");
}

// le valores da pergunta
void leitura(reg_questoes *reg_clie)
{
    strcpy(reg_clie->ds_pergunta, le_ds_pergunta());
    le_ds_alternativa(reg_clie->ds_alternativa1, 9);
    le_ds_alternativa(reg_clie->ds_alternativa2, 11);
    le_ds_alternativa(reg_clie->ds_alternativa3, 13);
    le_ds_alternativa(reg_clie->ds_alternativa4, 15);
    reg_clie->nivel_pergunta = le_nr_numero(17, 1, 3, 35);
    reg_clie->resp_ok = le_nr_numero(19, 1, 4, 35);
    reg_clie->pergunta_feita = le_nr_numero(21, 0, 1, 35);

}

// insere pergunta no final da lista
void inserirNoFinal(TipoLista *lista, reg_questoes *questao, int copia) {
    if (lista->Primeiro == NULL)
    {
        lista->Primeiro = (TipoApontador) malloc (sizeof(TipoItem));
        lista->Primeiro->anterior = NULL;
        lista->Primeiro->posicao = lista->Tamanho;
        lista->Primeiro->conteudo = *questao;
        if (copia != 1)
        {
            lista->Primeiro->conteudo.id_questao = ++ID;
        }
        lista->Primeiro->proximo = NULL;
        lista->Ultimo = lista->Primeiro;
    }
    else
    {
        lista->Ultimo->proximo = (TipoApontador) malloc (sizeof(TipoItem));
        lista->Ultimo->proximo->anterior = lista->Ultimo;
        lista->Ultimo->proximo->posicao = lista->Tamanho;
        lista->Ultimo->proximo->conteudo = *questao;
        if (copia != 1)
        {
            lista->Primeiro->conteudo.id_questao = ++ID;
        }
        lista->Ultimo->proximo->proximo = NULL;
        lista->Ultimo = lista->Ultimo->proximo;
    }
    lista->Tamanho++;
}

// insere pergunta no inicio da lista
void inserirNoInicio(TipoLista *lista, reg_questoes *questao) {
    TipoApontador p_auxiliar = lista->Primeiro;
    
    lista->Primeiro = (TipoApontador) malloc (sizeof(TipoItem));
    lista->Primeiro->conteudo = *questao;
    lista->Primeiro->conteudo.id_questao = ++ID;
    lista->Primeiro->anterior = NULL;

    if (p_auxiliar != NULL) {
        p_auxiliar->anterior = lista->Primeiro;
        lista->Primeiro->proximo = p_auxiliar;
    } else {
        // Se a lista estava vazia, o próximo é NULL
        lista->Primeiro->proximo = NULL;
    }

    if(lista->Ultimo == NULL) {
        lista->Ultimo = lista->Primeiro;
    }

    organizaIndices(lista);
    lista->Tamanho++;
}

// insere pergunta na posicao espeficicada
void inserirEmPosicaoEspecifica(TipoLista *lista, reg_questoes *questao, int posicao) {
    if (posicao >= lista->Tamanho)
    {
        inserirNoFinal(lista, questao, 0);
    }
    else {
        TipoApontador p_atual = lista->Primeiro;
        // TipoApontador p_anterior = lista->Primeiro;

        while (p_atual != NULL) {
            if (p_atual->posicao == posicao)
            {
                TipoApontador p_novo = (TipoApontador) malloc (sizeof(TipoItem));
                p_novo->conteudo = *questao;
                p_novo->conteudo.id_questao = ++ID;
                p_atual->anterior->proximo = p_novo;
                p_novo->proximo = p_atual;
                break;
            }
            // p_anterior = p_atual;
            p_atual = p_atual->proximo;
        }
        organizaIndices(lista);
    }
    lista->Tamanho++;
}

// funcao principal do cadastro, que chama as funcoes de insercao
void cadastrar(TipoLista *lista, int tipoCadastro, int posicao) { //1 = final, 2 = inicio, 3 = posicao especifica
    reg_questoes questao;

    tela_questoes();
    gotoxy(35, 03);
    printf("CADASTRAR PERGUNTA");

    leitura(&questao);

    int opt;
    gotoxy(07, 23);
    printf("Deseja salvar os dados? 1-Sim, 2-Nao | ");
    scanf("%d",&opt);

    if (opt == 1)
    {
        if (tipoCadastro == 1)
        {
            inserirNoFinal(lista, &questao, 0);
        }
        else if (tipoCadastro == 2)
        {
            inserirNoInicio(lista, &questao);
        }
        else {
            inserirEmPosicaoEspecifica(lista, &questao, posicao);
        }
    }
}

// remover pergunta do final da lista
void removerDoFinal(TipoLista *lista) {
    TipoApontador p_aux = lista->Primeiro;
    while (p_aux != NULL)
    {
        if (p_aux->proximo == NULL && lista->Tamanho == 1)
        {
            lista->Ultimo = NULL;
            lista->Primeiro = NULL;
            free(p_aux);
            break;
        }
        if (p_aux->proximo == NULL)
        {
            lista->Ultimo = p_aux->anterior;
            p_aux->anterior->proximo = NULL;
            free(p_aux);
            break;
        }
        p_aux = p_aux->proximo;
    }
    lista->Tamanho--;
}

// remover pergunta em uma posicao especifica da lista
void removerNaPosicaoEspecifica(TipoLista *lista, int posicao) {
    if (posicao >= lista->Tamanho)
    {
        printf("Posicao invalida. Digite uma posicao de 0 ate %d", (lista->Tamanho - 1));
    }
    else {
        TipoApontador p_aux = lista->Primeiro;
        if (posicao == 0)
        {
            lista->Primeiro = p_aux->proximo;
            free(p_aux);
        }
        else
        {
           while (p_aux != NULL)
            {
                if (p_aux->posicao == posicao)
                {
                    if (p_aux->proximo != NULL)
                    {
                        p_aux->proximo->anterior = p_aux->anterior;
                    }
                    if (p_aux->anterior != NULL)
                    {
                        p_aux->anterior->proximo = p_aux->proximo;
                    }
                    
                    free(p_aux);
                    break;
                }
                
                p_aux = p_aux->proximo;
            }
        }
        lista->Tamanho--;
        organizaIndices(lista);
    }
}

// remover pergunta do comeco da lista
void removerNoInicio(TipoLista *lista) {
    if (lista->Primeiro != NULL)
    {
        TipoApontador p_aux = lista->Primeiro;
        lista->Primeiro = p_aux->proximo;
        lista->Primeiro->anterior = NULL;
        free(p_aux);
        lista->Tamanho--;
        if (lista->Tamanho == 1)
        {
            lista->Ultimo = lista->Primeiro;
        }
        if (lista->Tamanho == 0)
        {
            lista->Primeiro = NULL;
            lista->Ultimo = NULL;
        }
        else {
            organizaIndices(lista);
        }
    }
}

// exibe as informacoes da pergunta
void mostra_pergunta(reg_questoes reg_quest, int pos)
{
    gotoxy(35, 05);
    printf("%d", reg_quest.id_questao);
    gotoxy(54, 05);
    printf("%d", pos);
    gotoxy(35, 07);
    printf("%s", reg_quest.ds_pergunta);
    gotoxy(35, 9);
    printf("%s", reg_quest.ds_alternativa1);
    gotoxy(35, 11);
    printf("%s", reg_quest.ds_alternativa2);
    gotoxy(35, 13);
    printf("%s", reg_quest.ds_alternativa3);
    gotoxy(35, 15);
    printf("%s", reg_quest.ds_alternativa4);
    gotoxy(35, 17);
    printf("%d", reg_quest.nivel_pergunta);
    gotoxy(35, 19);
    printf("%d", reg_quest.resp_ok);
    gotoxy(35, 21);
    printf("%d", reg_quest.pergunta_feita);
}

// inicia listas auxiliares
void iniciaListas(TipoLista *L, TipoLista *lista1, TipoLista *lista2, TipoLista *lista3, int copia) { // 0 = nao, 1 = sim
    TipoApontador p;
    p = L->Primeiro;
    while (p != NULL)
    {
        if (copia != 1)
        {
           if (lista1 != NULL)
            {
                if (p->conteudo.nivel_pergunta == 1)
                {
                    inserirNoFinal(lista1, &p->conteudo, 1);
                }
            }
            if (lista2 != NULL)
            {
                if (p->conteudo.nivel_pergunta == 2)
                {
                    inserirNoFinal(lista2, &p->conteudo, 1);
                }
            }
            if (lista3 != NULL)
            {
                if (p->conteudo.nivel_pergunta == 3)
                {
                    inserirNoFinal(lista3, &p->conteudo, 1);
                }
            }
        }
        else {
            inserirNoFinal(lista1, &p->conteudo, 1);
        }
        p = p->proximo;
    }
}

// ordena a lista com base no codigo da pergunta ou dificuldade, codigo é crescente e dificuldade decrescente
void ordena_codigo_ou_dificuldade(TipoLista *L, int opt) // 1 = por dificuldade, 2 = por codigo
{
    int i;
    int j;
    int qtde;
    reg_questoes reg;
    TipoApontador aux1;
    TipoApontador aux2;

    qtde = L->Tamanho;
    aux1 = L->Primeiro;
    aux2 = aux1->proximo;
    for (i = 1; i <= qtde - 1; i++)
    {
        for (j = i + 1; j <= qtde; j++)
        {
            if (opt == 1)
            {
                if (aux1->conteudo.nivel_pergunta < aux2->conteudo.nivel_pergunta)
                {
                    reg = aux1->conteudo;
                    aux1->conteudo = aux2->conteudo;
                    aux2->conteudo = reg;
                }
            }
            else
            {
                if (aux1->conteudo.id_questao > aux2->conteudo.id_questao)
                {
                    reg = aux1->conteudo;
                    aux1->conteudo = aux2->conteudo;
                    aux2->conteudo = reg;
                }
            }
            aux2 = aux2->proximo;
        }
        aux1 = aux1->proximo;
        aux2 = aux1->proximo;
    }
}

// funcao usada para exibir as perguntas sequencialmente
void consultar_sequencial(TipoLista *L, int reverso) { // 1 = a partir do comeco, 2 = a partir do final
    TipoApontador p;
    if (reverso == 1)
    {
        p = L->Primeiro;
    }
    else {
        p = L->Ultimo;
    }
    
    if (p == NULL)
    {
        tela();
        gotoxy(8, 23);
        printf("LISTA VAZIA...");
        getch();
    }
    else
    {
        tela();
        while (p != NULL)
        {
            int opt;
            tela_questoes();
            gotoxy(40, 03);
            printf("CONSULTAR PERGUNTAS");
            gotoxy(80, 03);
            printf("Total Perguntas:%d", L->Tamanho);
            mostra_pergunta(p->conteudo, p->posicao);

            gotoxy(8, 23);
            printf("4-Anterior | 6-Proximo | 5-Sair | ");
            gotoxy(43, 23);
            scanf("%d", &opt);
            if (opt == 4)
            {
                p = p->anterior;
                if (p == NULL)
                {
                    p = L->Ultimo;
                }
            }
            else if (opt == 6) {
                p = p->proximo;
                if (p == NULL)
                {
                    p = L->Primeiro;
                }
            }
            else {
                break;
            }
        }
    }
}

// funcao principal de consulta, exibe as opcoes e dps chama a funcao especifica para a opcao selecionada
void consultar(TipoLista *L)
{
    tela();
    gotoxy(30, 8);
    printf("1 - Consultar em Ordem de Codigo");
    gotoxy(30, 9);
    printf("2 - Consultar em Ordem de Nivel de Dificuldade");
    gotoxy(30, 10);
    printf("3 - Consultar do Ultimo para o Primeiro");
    gotoxy(8, 23);
    printf("Digite sua opcao.: ");
    int opt;
    scanf("%d", &opt);
    if (opt == 1)
    {
        TipoLista listaAux;
        listaAux.Primeiro = NULL;
        listaAux.Ultimo = NULL;
        listaAux.Tamanho = 0;     
        iniciaListas(L, &listaAux, NULL, NULL, 1);
        ordena_codigo_ou_dificuldade(&listaAux, 2);
        consultar_sequencial(&listaAux, 1);
    } else if (opt == 2)
    {
        TipoLista listaAux;
        listaAux.Primeiro = NULL;
        listaAux.Ultimo = NULL;
        listaAux.Tamanho = 0;
        iniciaListas(L, &listaAux, NULL, NULL, 1);  
        ordena_codigo_ou_dificuldade(&listaAux, 1); 
        consultar_sequencial(&listaAux, 1);
    } else if (opt == 3)
    {
        consultar_sequencial(L, 2);
    }
    
}

// funcao para editar a questao
void altera_questao(TipoLista *L)
{
    TipoApontador p;
    reg_questoes reg_questao;
    int codigo;
    int campo;
    int resp;
    p = L->Primeiro;
    if (p == NULL)
    {
        tela();
        gotoxy(8, 23);
        printf("LISTA VAZIA...");
        getch();
    }
    else
    {
        do
        {
            tela();
            tela_questoes();
            gotoxy(25, 03);
            printf("ALTERACAO DADOS DA PERGUNTA");
            gotoxy(60, 03);
            printf("Total Perguntas.: %d", L->Tamanho);
            gotoxy(35, 05);
            scanf("%d", &codigo);
            p = pesquisa(L, codigo);
            if (p == NULL)
            {
                gotoxy(07, 23);
                printf("Pergunta nao encontrada..");
                getch();
            }
        } while (p == NULL);

        mostra_pergunta(p->conteudo, p->posicao);
        reg_questao = p->conteudo;
        do
        {
            gotoxy(07, 23);
            printf("                                               ");
            gotoxy(07, 23);
            printf("Digite o Campo a ser Alterado (0=Sair).: ");
            scanf("%d", &campo);
            getchar();
            if (campo > 8)
            {
                gotoxy(07, 23);
                printf("                                           ");
                gotoxy(07, 23);
                printf("Campo invalido..");
                getch();
            }

            switch (campo)
            {
                case 1:
                    strcpy(reg_questao.ds_pergunta, le_ds_pergunta());
                    break;
                case 2:
                    le_ds_alternativa(reg_questao.ds_alternativa1, 9);
                    break;
                case 3:
                    le_ds_alternativa(reg_questao.ds_alternativa2, 11);
                    break;
                case 4:
                    le_ds_alternativa(reg_questao.ds_alternativa3, 13);
                    break;
                case 5:
                    le_ds_alternativa(reg_questao.ds_alternativa4, 15);
                    break;
                case 6:
                    reg_questao.nivel_pergunta = le_nr_numero(17, 1, 3, 35);
                    break;
                case 7:
                    reg_questao.resp_ok = le_nr_numero(19, 1, 4, 35);
                    break;
                case 8:
                    reg_questao.pergunta_feita = le_nr_numero(21, 0, 1, 35);
                    break;
                default:
                    break;
            }
        } while ((campo != 0));
        gotoxy(07, 23);
        printf("                                               ");
        gotoxy(07, 23);
        printf("Confirma a Alteracao dos Dados (1-Sim;2-Nao).: ");
        scanf("%d", &resp);
        if (resp == 1)
        {
            p->conteudo = reg_questao;
        }
    }
}

// funcao para ler a lista
void le_arquivo(TipoLista *L)
{
    FILE *ptr;
    char *filename = "Perguntas.dat";
    char *modo_gravacao = "rb";
    reg_questoes reg_questoes;
    TipoApontador p;
    L->Primeiro = NULL;
    L->Ultimo = NULL;
    int pos = 0;
    tela();
    gotoxy(25, 03);
    printf("CARREGA DADOS DO ARQUIVO PARA A LISTA");

    // Abre o arquivo para Gravacao; Se ocorrer erro o Programa Aborta
    ptr = fopen(filename, modo_gravacao);
    if (ptr == NULL)
    {
        gotoxy(07, 23);
        printf("Erro ao Abrir o Arquivo");
        getch();
    }
    else
    {
        gotoxy(01, 05);
        while (!feof(ptr))
        {
            if (fread(&reg_questoes, sizeof(reg_questoes), 1, ptr) != 0)
            {
                if (L->Primeiro == NULL)
                {
                    p = (TipoApontador)malloc(sizeof(TipoItem));
                    p->conteudo = reg_questoes;
                    p->conteudo.id_questao = ++ID;
                    p->proximo = NULL;
                    p->anterior = NULL;
                    L->Primeiro = p;
                    L->Ultimo = p;
                    p->posicao = pos;
                    pos++;
                }
                else
                {
                    p->proximo = (TipoApontador)malloc(sizeof(TipoItem));
                    p->proximo->anterior = p;
                    p = p->proximo;
                    p->conteudo = reg_questoes;
                    p->conteudo.id_questao = ++ID;
                    p->proximo = NULL;
                    L->Ultimo = p;
                    p->posicao = pos;
                    pos++;
                }
                L->Tamanho++;
            }
        }
        
        fclose(ptr);
        // gotoxy(07, 23);
        // printf("Dados lidos com Sucesso");
        // getch();
    }
}

void gravar(TipoLista *L)
{
    FILE *ptr;
    char *filename = "Perguntas.dat";
    char *modo_gravacao = "wb";
    reg_questoes reg_questoes;
    TipoApontador p;
    p = L->Primeiro;
    tela();
    gotoxy(25, 03);
    printf("GRAVA DADOS DA LISTA NO ARQUIVO EM DISCO");
    if (p == NULL)
    {
        gotoxy(07, 23);
        printf("LISTA VAZIA..");
        getch();
    }
    else
    {
        // Abre o arquivo para Gravacao; Se ocorrer erro o Programa Aborta
        if ((ptr = fopen(filename, modo_gravacao)) == NULL)
        {
            gotoxy(07, 23);
            printf("Erro ao Abrir o Arquivo");
            getch();
        }
        else
        {
            while (p != NULL)
            {
                reg_questoes = p->conteudo;
                fwrite(&reg_questoes, sizeof(reg_questoes), 1, ptr);
                p = p->proximo;
            }
            fclose(ptr);
            gotoxy(07, 23);
            printf("Dados Gravados com Sucesso ");
            getch();
        }
    }
}

void salvar_progresso(reg_save *save)
{
    FILE *ptr;
    char *filename = "save.dat";
    char *modo_gravacao = "wb";
    if ((ptr = fopen(filename, modo_gravacao)) == NULL)
    {
        // printf("Erro ao Abrir o Arquivo");
    }
    else
    {
        fwrite(save, sizeof(reg_save), 1, ptr);
        fclose(ptr);
    }
}

void le_progresso(reg_save *save)
{
    FILE *ptr;
    char *filename = "save.dat";
    char *modo_gravacao = "rb";
    ptr = fopen(filename, modo_gravacao);
    if (ptr == NULL)
    {
        save->num_pergunta = 0;
        save->cod_questao = 0;
        save->tem_carta = 1;
        save->tem_univ = 1;
        save->tem_placa = 1;
        save->pos_input = 90;
    }
    else
    {
        fread(save, sizeof(reg_save), 1, ptr);
        fclose(ptr);
    }
}

void shuffleArray(int array[], int size) {
    srand((unsigned int)time(NULL));
    int i;
    for (i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        // Troca os elementos nas posições i e j
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void tela_univ() {
    int positions[] = {1, 2, 3, 4, 4, 3, 2, 1, 2, 3, 4, 1, 1, 3, 2, 4}; //alternativas
    int numPositions = sizeof(positions) / sizeof(positions[0]);

    shuffleArray(positions, numPositions);
    setColor(0, 1);
    gotoxy(06, 9);
    printf("+----------------------------+");
    gotoxy(06, 10);
    printf("| U N I V E R S I T A R I O S|");
    gotoxy(06, 11);
    printf("|----------------------------|");
    gotoxy(06, 12);
    printf("| Univ1| Univ2| Univ3| Univ4 |");
    gotoxy(06, 13);
    printf("|----------------------------|");
    gotoxy(06, 14);
    printf("| %d    | %d    | %d    | %d     |", positions[0], positions[1], positions[2], positions[3]);
    gotoxy(06, 15);
    printf("+----------------------------+");
    setColor(7, 0);
}

void tela_placas() {
    int per1 = 0; 
    int per2 = 0;
    int per3 = 0;
    int per4 = 0;

    int i = 0;
    while (i <= 100)
    {
        int j = rand() % 5;
        // if (j == 0)
        // {
        //     j++;
        // }
        switch (j)
        {
        case 1:
            per1++;
            break;
        case 2:
            per2++;
            break;
        case 3:
            per3++;
            break;
        case 4:
            per4++;
            break;
        
        default:
            break;
        }
        i++;
    }

    double percent1, percent2, percent3, percent4;
    percent1 = ((double)per1 / (per1 + per2 + per3 + per4)) *100;
    percent2 = ((double)per2 / (per1 + per2 + per3 + per4)) *100;
    percent3 = ((double)per3 / (per1 + per2 + per3 + per4)) *100;
    percent4 = ((double)per4 / (per1 + per2 + per3 + per4)) *100;

    setColor(0, 1);
    gotoxy(06, 9);
    printf("+----------------------------+");
    gotoxy(06, 10);
    printf("| P L A C A S - 100 pessoas  |");
    gotoxy(06, 11);
    printf("|----------------------------|");
    gotoxy(06, 12);
    printf("|     1|     2|     3|     4 |");
    gotoxy(06, 13);
    printf("|----------------------------|");
    gotoxy(06, 14);
    printf("|%.2f%%|%.2f%%|%.2f%%|%.2f%% |", percent1, percent2, percent3, percent4);
    gotoxy(06, 15);
    printf("+----------------------------+");
    setColor(7, 0);
}

void tela_questao_jogo(int tem_carta, int tem_univ, int tem_placa)
{
    tela();
    gotoxy(35, 05);
    printf("Pergunta.: ");
    gotoxy(01, 06);
    printf("|-------------------------------------------------------------------------------------------------|");
    gotoxy(01, 8);
    printf("|-------------------------------------------------------------------------------------------------|");
    gotoxy(01, 18);
    printf("|-------------------------------------------------------------------------------------------------|");
    gotoxy(02, 19);
    printf("            ERRAR             |              PARAR               |             ACERTAR           |");
    gotoxy(01, 20);
    printf("|-------------------------------------------------------------------------------------------------|");
    gotoxy(02, 23);
    char str1[200] = "  0-Parar  |  1-Responder  |  2-Pular  |";
    
    if (tem_carta == 1)
    {
        strcat(str1, "  3-Cartas  |");
    }
    if (tem_univ == 1)
    {
        strcat(str1, "  4-Universitarios  |");
    }
    if (tem_placa == 1)
    {
        strcat(str1, "  5-Placas  |         ");
    }
    printf("%s", str1);
    
}

void mostra_questao_jogo(reg_questoes quest, int num, int *qtde_a_ocultar) {
    int qtde_ocultada = 0;
    gotoxy(47, 05);
    printf("%d - ", (num + 1));
    gotoxy(51, 05);
    printf("(%d)", quest.id_questao);
    gotoxy(10, 07);
    printf("%s", quest.ds_pergunta);
    gotoxy(47, 9);
    if (*qtde_a_ocultar > 0 && quest.resp_ok != 1)
    {
        printf("1 - ");
        setColor(7, 1);
        gotoxy(51, 9);
        printf("                                          ");
        setColor(7, 0);
        qtde_ocultada++;
    }
    else
    {
        printf("1 - %s", quest.ds_alternativa1);
    }
    gotoxy(47, 11);
    if (*qtde_a_ocultar > 0 && quest.resp_ok != 2 && qtde_ocultada < *qtde_a_ocultar)
    {
        printf("2 - ");
        setColor(7, 1);
        gotoxy(51, 11);
        printf("                                          ");
        setColor(7, 0);
        qtde_ocultada++;
    }
    else
    {
        printf("2 - %s", quest.ds_alternativa2);
    }
    gotoxy(47, 13);
    if (*qtde_a_ocultar > 0 && quest.resp_ok != 3 && qtde_ocultada < *qtde_a_ocultar)
    {
        printf("3 - ");
        setColor(7, 1);
        gotoxy(51, 13);
        printf("                                          ");
        setColor(7, 0);
        qtde_ocultada++;
    }
    else
    {
        printf("3 - %s", quest.ds_alternativa3);
    }
    gotoxy(47, 15);
    if (*qtde_a_ocultar > 0 && quest.resp_ok != 4 && qtde_ocultada < *qtde_a_ocultar)
    {
        printf("2 - ");
        setColor(7, 1);
        gotoxy(51, 15);
        printf("                                          ");
        setColor(7, 0);
        qtde_ocultada++;
    }
    else
    {
        printf("4 - %s", quest.ds_alternativa4);
    }
    gotoxy(02, 21);
    printf("                              |                                  |                               |");
    gotoxy(05, 21);
    printf("R$ %d", valores_pergunta[num][2]);
    gotoxy(35, 21);
    printf("R$ %d", valores_pergunta[num][1]);
    gotoxy(70, 21);
    printf("R$ %d", valores_pergunta[num][0]);
    *qtde_a_ocultar = 0;
}

TipoApontador sorteia_pergunta(TipoLista *L) {
    TipoApontador questao = NULL;
    int r = rand()  % (L->Tamanho + 1);      // Returns a pseudo-random integer between 0 and RAND_MAX.

    while (questao == NULL) {
        questao = pesquisa(L, r);
        if (questao != NULL)
        {
            if (questao->conteudo.pergunta_feita == 0)
            {
                return questao;
            }
            else
            {
                questao = NULL;
            }
        }
    }
    return questao;
}

void tela_cartas() {
    //carta 1
    gotoxy(06, 9);
    printf("+--------+");
    gotoxy(06, 10);
    printf("|        |");
    gotoxy(06, 11);
    printf("| Carta1 |");
    gotoxy(06, 12);
    printf("|        |");
    gotoxy(06, 13);
    printf("+--------+");
    //carta 3
    gotoxy(06, 14);
    printf("|        |");
    gotoxy(06, 15);
    printf("| Carta3 |");
    gotoxy(06, 16);
    printf("|        |");
    gotoxy(06, 17);
    printf("+--------+");
    //carta 2
    gotoxy(19, 9);
    printf("+--------+");
    gotoxy(19, 10);
    printf("|        |");
    gotoxy(19, 11);
    printf("| Carta2 |");
    gotoxy(19, 12);
    printf("|        |");
    gotoxy(19, 13);
    printf("+--------+");
    //carta 4
    gotoxy(19, 14);
    printf("|        |");
    gotoxy(19, 15);
    printf("| Carta4 |");
    gotoxy(19, 16);
    printf("|        |");
    gotoxy(19, 17);
    printf("+--------+");
}

void responder(TipoItem *questao, int *valor_ganho, int *continua_jogo, reg_save *save) {
    int opt2;
    gotoxy(02, 23);
    printf("                                                                                                ");
    gotoxy(02, 23);
    printf("  Qual sua resposta.: ");
    gotoxy(25, 23);
    opt2 = le_nr_numero_jogo(23, 1, 4, 02, "Qual sua resposta");
    gotoxy(02, 26);
    if (opt2 == questao->conteudo.resp_ok)
    {
        questao->conteudo.pergunta_feita = 1;
        gotoxy(02, 23);
        printf("  Resposta correta. ");
        *valor_ganho = valores_pergunta[save->num_pergunta][0];
        save->num_pergunta = save->num_pergunta +1;
        if (save->num_pergunta == 16)
        {
            *continua_jogo = 0;
            save->num_pergunta = 0;
            save->cod_questao = 0;
            save->tem_carta = 1;
            save->tem_placa = 1;
            save->tem_univ = 1;
            save->pos_input = 90;
            tela();
            gotoxy(20, 10);
            printf("PARABENS POR VENCER O JOGO DO MILHAO E GANHAR R$ %d !!", *valor_ganho);
            gotoxy(8, 23);
            system("pause");
        }
        salvar_progresso(save);
    }
    else
    {
        gotoxy(0, 23);
        printf("Resposta incorreta. Voce ganhou R$ %d. ", *valor_ganho);
        system("pause");
        *continua_jogo = 0;
        save->num_pergunta = 0;
        salvar_progresso(save);
    }
}

void selecionar_carta(int opt, int *qtde_a_ocultar) {
    int positions[] = {1, 2, 3, 4}; //1=reis-nenhuma alt 2=as-uma alt 3=2 duas alt 4=3 tres alt
    int numPositions = sizeof(positions) / sizeof(positions[0]);

    shuffleArray(positions, numPositions);
    setColor(7, 1);
    int x, y;

    if (opt == 1 || opt == 2)
    {
        y = 9;
    }
    else
    {
        y = 14;
    }
    if (opt == 1 || opt == 3)
    {
       x = 06;
    }
    else {
        x = 19;
    }
    
    char nome_carta[3];
    int c = positions[0];
    if (c == 1)
    {
        strcpy(nome_carta, "Rei");
        *qtde_a_ocultar = 0;
    }
    else if (c == 2)
    {
        strcpy(nome_carta, "As");
        *qtde_a_ocultar = 1;
    }
    else if (c == 3)
    {
        strcpy(nome_carta, "2");
        *qtde_a_ocultar = 2;
    }
    else {
        strcpy(nome_carta, "3");
        *qtde_a_ocultar = 3;
    }
   
    if (opt < 3)
    {
        gotoxy(x, y);
        printf("+--------+");
        gotoxy(x, y +1);
        printf("|        |");
        gotoxy(x, y+2);
        printf("|        |");
        gotoxy(x+3, y+2);
        printf("%s", nome_carta);
        gotoxy(x, y+3);
        printf("|        |");
        gotoxy(x, y+4);
        printf("+--------+");
    }
    else
    {
        gotoxy(x, y-1);
        printf("+--------+");
        gotoxy(x, y);
        printf("|        |");
        gotoxy(x, y+1);
        printf("|        |");
        gotoxy(x+3, y+1);
        printf("%s", nome_carta);
        gotoxy(x, y+2);
        printf("|        |");
        gotoxy(x, y+3);
        printf("+--------+");
    }
    
    setColor(7, 0);
    gotoxy(02, 23);
    printf("Voce tirou um %s. %d alternativas erradas serao ocultadas. ", nome_carta, *qtde_a_ocultar);
    getch();
}

void jogar(TipoLista *L) {
    srand(time(NULL));
    tela();
    if (L->Primeiro == NULL)
    {
        gotoxy(8, 23);
        printf("Sem perguntas cadastradas!"); 
        getch();
        return;
    }

    ApontadorSave save = (ApontadorSave) malloc (sizeof(reg_save));
    
    int continua_jogo = 1;
    int valor_ganho = 0;
    int qtde_a_ocultar = 0;

    le_progresso(save);

    valor_ganho = valores_pergunta[save->num_pergunta][1];

    TipoLista listaNivel1;
    listaNivel1.Primeiro = NULL;
    listaNivel1.Ultimo = NULL;
    listaNivel1.Tamanho = 0;

    TipoLista listaNivel2;
    listaNivel2.Primeiro = NULL;
    listaNivel2.Ultimo = NULL;
    listaNivel2.Tamanho = 0;

    TipoLista listaNivel3;
    listaNivel3.Primeiro = NULL;
    listaNivel3.Ultimo = NULL;
    listaNivel3.Tamanho = 0;

    iniciaListas(L, &listaNivel1, &listaNivel2, &listaNivel3, 0);
    
    gotoxy(20, 10);
    printf("BEM VINDO AO JOGO DO MILHAO! PRONTO PARA A PRIMEIRA PERGUNTA?");
    gotoxy(8, 23);
    system("pause");

    TipoApontador questao;
    if (save->cod_questao != 0)
    {
        questao = pesquisa(L, save->cod_questao);
    }
    else
    {
        questao = sorteia_pergunta(&listaNivel1);
        save->cod_questao = questao->conteudo.id_questao;
    }
    
    while (continua_jogo == 1)
    {
        int opt;
        int opt2;
        tela_questao_jogo(save->tem_carta, save->tem_univ, save->tem_placa);
        mostra_questao_jogo(questao->conteudo, save->num_pergunta, &qtde_a_ocultar);
        gotoxy(save->pos_input, 23);
        scanf("%d", &opt);
        switch (opt)
        {
            case 0:
                gotoxy(02, 23);
                printf("                                                                                                ");
                gotoxy(02, 23);
                printf("Voce ganhou R$ %d. ", valor_ganho);
                system("pause");
                continua_jogo = 0;
                save->num_pergunta = 0;
                save->cod_questao = 0;
                salvar_progresso(save);
                continue;
            case 1:
                responder(questao, &valor_ganho, &continua_jogo, save);
                if (save->num_pergunta < 6)
                {
                    questao = sorteia_pergunta(&listaNivel1);
                }
                else if (save->num_pergunta < 11)
                {
                    questao = sorteia_pergunta(&listaNivel2);
                }
                else {
                    questao = sorteia_pergunta(&listaNivel3);
                }
                save->cod_questao = questao->conteudo.id_questao;
                salvar_progresso(save);
                system("pause");
                continue;
            case 2:
                questao->conteudo.pergunta_feita = 1;
                if (save->num_pergunta < 6)
                {
                    questao = sorteia_pergunta(&listaNivel1);
                }
                else if (save->num_pergunta < 11)
                {
                    questao = sorteia_pergunta(&listaNivel2);
                }
                else {
                    questao = sorteia_pergunta(&listaNivel3);
                }
                save->cod_questao = questao->conteudo.id_questao;
                salvar_progresso(save);
                continue;
            case 3:
                tela_cartas();
                gotoxy(02, 23);
                printf("                                                                                                ");
                gotoxy(02, 23);
                printf("  Escolha uma carta.: ");
                gotoxy(25, 23);
                opt2 = le_nr_numero_jogo(23, 1, 4, 02, "Escolha uma carta");
                selecionar_carta(opt2, &qtde_a_ocultar);
                save->tem_carta = 0;
                save->pos_input = save->pos_input - 13;
                salvar_progresso(save);
                continue;
            case 4:
                tela_univ();
                save->tem_univ = 0;
                save->pos_input = save->pos_input - 21;
                responder(questao, &valor_ganho, &continua_jogo, save);
                if (save->num_pergunta < 6)
                {
                    questao = sorteia_pergunta(&listaNivel1);
                }
                else if (save->num_pergunta < 11)
                {
                    questao = sorteia_pergunta(&listaNivel2);
                }
                else {
                    questao = sorteia_pergunta(&listaNivel3);
                }
                save->cod_questao = questao->conteudo.id_questao;
                salvar_progresso(save);
                system("pause");
                continue;
            case 5:
                tela_placas();
                save->tem_placa = 0;
                save->pos_input = save->pos_input - 13;
                responder(questao, &valor_ganho, &continua_jogo, save);
                if (save->num_pergunta < 6)
                {
                    questao = sorteia_pergunta(&listaNivel1);
                }
                else if (save->num_pergunta < 11)
                {
                    questao = sorteia_pergunta(&listaNivel2);
                }
                else {
                    questao = sorteia_pergunta(&listaNivel3);
                }
                save->cod_questao = questao->conteudo.id_questao;
                salvar_progresso(save);
                system("pause");
                continue;
            
            default:
                continue;
        }
    }
    
    
}

int main() {
    TipoLista lista;
    lista.Primeiro = NULL;
    lista.Ultimo = NULL;
    lista.Tamanho = 0;
    system("cls");

    setlocale(LC_ALL, "portuguese-brazilian");
    le_arquivo(&lista);

    int continua = 0;
    while (continua == 0)
    {
        int opt;
        tela();
        gotoxy(40, 03);
        printf("MENU PRINCIPAL");
        gotoxy(30, 8);
        printf("1 - Cadastrar Pergunta no Inicio");
        gotoxy(30, 9);
        printf("2 - Cadastrar Pergunta no Final");
        gotoxy(30, 10);
        printf("3 - Cadastrar Pergunta em uma Posicao");
        gotoxy(30, 11);
        printf("4 - Remover Pergunta no Inicio");
        gotoxy(30, 12);
        printf("5 - Remover Pergunta no Final");
        gotoxy(30, 13);
        printf("6 - Remover Pergunta em uma Posicao");
        gotoxy(30, 14);
        printf("7 - Consultar o cadastro das Perguntas");
        gotoxy(30, 15);
        printf("8 - Alterar uma Pergunta");
        gotoxy(30, 16);
        printf("9 - JOGAR SHOW DO MILHAO");
        gotoxy(30, 17);
        printf("10 - Finalizar Programa");
        gotoxy(8, 23);
        printf("Digite sua opcao.: ");
        gotoxy(28, 23);
        scanf("%d", &opt);
        int resp, posicao;

        switch (opt)
        {
            case 1:
                cadastrar(&lista, 2, 0);
                continue;
            case 2:
                cadastrar(&lista, 1, 0);
                continue;
            case 3:
                gotoxy(07, 23);
                printf("Em que posicao deseja cadastrar?(digite apenas o numero).: ");
                scanf("%d", &posicao);
                cadastrar(&lista, 3, posicao);
                continue;
            case 4:
                gotoxy(07, 23);
                printf("Deseja realmente remover a pergunta? 1-Sim, 2-Nao.: ");
                scanf("%d", &resp);
                if (resp == 1)
                {
                    removerNoInicio(&lista);
                }
                continue;
            case 5:
                gotoxy(07, 23);
                printf("Deseja realmente remover a ultima pergunta? 1-Sim, 2-Nao.: ");
                scanf("%d", &resp);
                if (resp == 1)
                {
                    removerDoFinal(&lista);
                }
                continue;
            case 6:
                gotoxy(07, 23);
                printf("Deseja realmente remover a pergunta? 1-Sim, 2-Nao.: ");
                scanf("%d", &resp);
                if (resp == 1)
                {
                    gotoxy(07, 23);
                    printf("                                                         ");
                    gotoxy(07, 23);
                    printf("Em que posicao deseja remover?(digite apenas o numero).: ");
                    scanf("%d", &posicao);
                    removerNaPosicaoEspecifica(&lista, posicao);
                }
                continue;
            case 7:
                consultar(&lista);
                continue;
            case 8:
                altera_questao(&lista);
                continue;
            case 9:
                jogar(&lista);
                continue;
            case 10:
                continua = 1;
                continue;
            default:
                continua = 1;
                continue;
        }
    }
    gravar(&lista);
    return 0;
}