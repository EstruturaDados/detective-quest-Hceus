#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 25

// =====================================
// ==== Estruturas =====================
// =====================================

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// =====================================
// ==== Protótipos =====================
// =====================================

Sala* criarSala(const char *nome, const char *pista);
PistaNode* inserirPista(PistaNode *raiz, const char *pista);
void explorarSalas(Sala *inicio, PistaNode **arvorePistas, HashNode *tabela[]);
void exibirTabelaPistas(PistaNode *raiz, HashNode *tabela[]);
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito);
char* encontrarSuspeito(HashNode *tabela[], const char *pista);
int contarPistasSuspeito(PistaNode *raiz, HashNode *tabela[], const char *suspeito);
void verificarSuspeitoFinal(HashNode *tabela[], PistaNode *arvorePistas);
int hash(const char *str);
void liberarArvoreSalas(Sala *raiz);
void liberarArvorePistas(PistaNode *raiz);
void liberarHash(HashNode *tabela[]);

// =====================================
// ==== Função Principal ===============
// =====================================

int main() {
    // ===== Criação das salas =====
    Sala *hall = criarSala("Hall de Entrada", "Marca de lama");
    Sala *salaEstar = criarSala("Sala de Estar", "Pano com produto de limpeza");
    Sala *cozinha = criarSala("Cozinha", "Faca faltando no suporte");

    Sala *quartoVisitas = criarSala("Quarto de Visitas", "Cogumelo venenoso");
    Sala *salaJantar = criarSala("Sala de Jantar", "Marca páginas");
    Sala *closet = criarSala("Closet", "Avental de cozinha molhado");
    Sala *banheiro = criarSala("Banheiro", "Folhas de grama");
    Sala *biblioteca = criarSala("Biblioteca", "Livro sobre venenos");
    Sala *quartoPrincipal = criarSala("Quarto Principal", "Luva solitária");

    Sala *lavanderia = criarSala("Lavanderia", "Caderno de anotações");
    Sala *quintal = criarSala("Quintal", "Avental de governanta molhado");
    Sala *dispensa = criarSala("Despensa", "Sapato sujo de lama");
    Sala *garagem = criarSala("Garagem", "Página solta sobre venenos");
    Sala *jardim = criarSala("Jardim", "Espanador");
    Sala *piscina = criarSala("Piscina", "Faca");

    // ===== Conexões entre as salas =====
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = quartoVisitas;
    salaEstar->direita = salaJantar;

    quartoVisitas->esquerda = closet;
    quartoVisitas->direita = banheiro;

    salaJantar->esquerda = biblioteca;
    salaJantar->direita = quartoPrincipal;

    cozinha->esquerda = lavanderia;
    cozinha->direita = quintal;

    lavanderia->esquerda = dispensa;
    lavanderia->direita = garagem;

    quintal->esquerda = jardim;
    quintal->direita = piscina;

    // ===== Criação da árvore de pistas e tabela hash =====
    PistaNode *arvorePistas = NULL;
    HashNode *tabela[TAM_HASH] = {NULL};

    // ===== Inserção das pistas e suspeitos =====
    inserirNaHash(tabela, "Marca de lama", "Jardineiro");
    inserirNaHash(tabela, "Pano com produto de limpeza", "Governanta");
    inserirNaHash(tabela, "Cogumelo venenoso", "Cozinheiro");
    inserirNaHash(tabela, "Avental de cozinha molhado", "Cozinheiro");
    inserirNaHash(tabela, "Folhas de grama", "Jardineiro");
    inserirNaHash(tabela, "Marca páginas", "Bibliotecário");
    inserirNaHash(tabela, "Livro sobre venenos", "Bibliotecário");
    inserirNaHash(tabela, "Luva solitária", "Governanta");
    inserirNaHash(tabela, "Faca faltando no suporte", "Cozinheiro");
    inserirNaHash(tabela, "Caderno de anotações", "Bibliotecário");
    inserirNaHash(tabela, "Sapato sujo de lama", "Jardineiro");
    inserirNaHash(tabela, "Página solta sobre venenos", "Bibliotecário");
    inserirNaHash(tabela, "Avental de governanta molhado", "Governanta");
    inserirNaHash(tabela, "Espanador", "Governanta");
    inserirNaHash(tabela, "Faca", "Cozinheiro");

    printf("=== Detective Quest ===\n");
    printf("Explore a mansão e colete pistas!\n");

    explorarSalas(hall, &arvorePistas, tabela);

    printf("\n=== PISTAS COLETADAS ===\n");
    exibirTabelaPistas(arvorePistas, tabela);

    verificarSuspeitoFinal(tabela, arvorePistas);

    liberarArvoreSalas(hall);
    liberarArvorePistas(arvorePistas);
    liberarHash(tabela);

    printf("\nExploração encerrada. Até logo, detetive!\n");
    return 0;
}

// =====================================
// ==== Implementações =================
// =====================================

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

int hash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += tolower(str[i]);
    return soma % TAM_HASH;
}

void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int indice = hash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int indice = hash(pista);
    HashNode *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return "Desconhecido";
}

void explorarSalas(Sala *atual, PistaNode **arvorePistas, HashNode *tabela[]) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        printf("Pista encontrada: \"%s\"\n", atual->pista);

        *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        printf("Pista associada ao suspeito: %s\n", encontrarSuspeito(tabela, atual->pista));

        printf("\nEscolha seu caminho:\n");
        if (atual->esquerda) printf(" - (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita) printf(" - (d) Ir para %s\n", atual->direita->nome);
        printf(" - (s) Sair da exploração\n");

        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há caminho à esquerda!\n");
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há caminho à direita!\n");
        } else if (escolha == 's' || escolha == 'S') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

// Exibe as pistas em formato de tabela
void exibirTabelaPistas(PistaNode *raiz, HashNode *tabela[]) {
    if (raiz == NULL) return;

    static int cabecalho = 0;
    if (!cabecalho) {
        printf("-------------------------------------------------------------------\n");
        printf("%-40s | %-25s\n", "PISTA", "SUSPEITO RELACIONADO");
        printf("-------------------------------------------------------------------\n");
        cabecalho = 1;
    }

    if (raiz->esquerda)
        exibirTabelaPistas(raiz->esquerda, tabela);

    printf("%-40s | %-25s\n", raiz->pista, encontrarSuspeito(tabela, raiz->pista));

    if (raiz->direita)
        exibirTabelaPistas(raiz->direita, tabela);
}

// Conta quantas pistas na BST apontam para o suspeito informado
int contarPistasSuspeito(PistaNode *raiz, HashNode *tabela[], const char *suspeito) {
    if (raiz == NULL)
        return 0;

    int contador = 0;
    char *suspeitoPista = encontrarSuspeito(tabela, raiz->pista);

    if (strcmp(suspeitoPista, suspeito) == 0)
        contador++;

    contador += contarPistasSuspeito(raiz->esquerda, tabela, suspeito);
    contador += contarPistasSuspeito(raiz->direita, tabela, suspeito);

    return contador;
}

// Realiza a fase de julgamento final
void verificarSuspeitoFinal(HashNode *tabela[], PistaNode *arvorePistas) {
    char suspeitoEscolhido[50];
    
    printf("\n=== JULGAMENTO FINAL ===\n");
    printf("Digite o nome do suspeito que você acredita ser o culpado: ");
    scanf(" %[^\n]", suspeitoEscolhido);

    int totalPistas = contarPistasSuspeito(arvorePistas, tabela, suspeitoEscolhido);

    printf("\nTotal de pistas que apontam para %s: %d\n", suspeitoEscolhido, totalPistas);

    if (totalPistas >= 2)
        printf("\nAcusação confirmada! %s é o culpado!\n", suspeitoEscolhido);
    else
        printf("\nAcusação falhou! %s não tem pistas suficientes contra si.\n", suspeitoEscolhido);
}

void liberarArvoreSalas(Sala *raiz) {
    if (!raiz) return;
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

void liberarArvorePistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

void liberarHash(HashNode *tabela[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *atual = tabela[i];
        while (atual) {
            HashNode *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
}
