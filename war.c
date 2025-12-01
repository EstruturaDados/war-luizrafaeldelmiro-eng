#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// -----------------------------------------------------------
//  CONSTANTES
// -----------------------------------------------------------
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 3
#define TAM_STRING 50

// -----------------------------------------------------------
//  ESTRUTURA TERRITÓRIO
// -----------------------------------------------------------
typedef struct {
    char nome[TAM_STRING];
    char corDono[TAM_STRING];
    int tropas;
} Territorio;


// -----------------------------------------------------------
//  PROTÓTIPOS
// -----------------------------------------------------------

// Setup
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMapa(Territorio *mapa);

// Interface
void exibirMapa(const Territorio *mapa);
void exibirMenu();
void exibirMissao(int idMissao);

// Lógica do jogo
void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *mapa, int origem, int destino, const char *corJogador);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, const char *corJogador, int idMissao);

// Utilidades
void limparBufferEntrada();


// -----------------------------------------------------------
//  FUNÇÃO PRINCIPAL
// -----------------------------------------------------------
int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (!mapa) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    char corJogador[TAM_STRING] = "Azul";
    int idMissao = sortearMissao();

    int opcao;
    int venceu = 0;

    do {
        system("clear || cls");

        printf("\n===== MAPA ATUAL =====\n");
        exibirMapa(mapa);

        printf("\n===== SUA MISSÃO =====\n");
        exibirMissao(idMissao);

        printf("\n===== MENU =====\n");
        exibirMenu();

        printf("\nEscolha: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;

            case 2:
                venceu = verificarVitoria(mapa, corJogador, idMissao);
                if (venceu)
                    printf("\n🎉 Você cumpriu a missão! Vitória!\n");
                else
                    printf("\n❌ Você ainda não cumpriu a missão.\n");
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

        printf("\nPressione ENTER para continuar...");
        getchar();

    } while (opcao != 0 && !venceu);

    liberarMapa(mapa);
    return 0;
}


// -----------------------------------------------------------
//  IMPLEMENTAÇÕES
// -----------------------------------------------------------

Territorio* alocarMapa() {
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio *mapa) {
    const char *nomes[] = {
        "África do Sul", "Egito", "Madagascar", "Brasil",
        "Argentina", "EUA", "México", "China",
        "Índia", "Japão"
    };

    const char *cores[] = {
        "Vermelho", "Azul", "Amarelo", "Verde",
        "Vermelho", "Azul", "Amarelo", "Verde",
        "Vermelho", "Azul"
    };

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].corDono, cores[i]);
        mapa[i].tropas = (rand() % 5) + 1;
    }
}

void liberarMapa(Territorio *mapa) {
    free(mapa);
}

void exibirMapa(const Territorio *mapa) {
    printf("\n%-20s %-12s %s\n", "Território", "Dono", "Tropas");
    printf("----------------------------------------------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-20s %-12s %d\n",
               mapa[i].nome,
               mapa[i].corDono,
               mapa[i].tropas);
    }
}

void exibirMenu() {
    printf("1 - Atacar\n");
    printf("2 - Verificar vitória\n");
    printf("0 - Sair\n");
}

void exibirMissao(int idMissao) {
    switch (idMissao) {
        case 1:
            printf("Missão: Conquistar 5 territórios.\n");
            break;
        case 2:
            printf("Missão: Eliminar todos os territórios de uma cor inimiga.\n");
            break;
        case 3:
            printf("Missão: Controlar todos os territórios que começam com a letra 'A'.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
    }
}

void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    char origemNome[TAM_STRING];
    char destinoNome[TAM_STRING];
    int origem = -1, destino = -1;

    printf("\nDigite o território de origem: ");
    fgets(origemNome, TAM_STRING, stdin);
    origemNome[strcspn(origemNome, "\n")] = '\0';

    printf("Digite o território de destino: ");
    fgets(destinoNome, TAM_STRING, stdin);
    destinoNome[strcspn(destinoNome, "\n")] = '\0';

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].nome, origemNome) == 0)
            origem = i;
        if (strcmp(mapa[i].nome, destinoNome) == 0)
            destino = i;
    }

    if (origem == -1 || destino == -1) {
        printf("\n❌ Território inválido.\n");
        return;
    }

    simularAtaque(mapa, origem, destino, corJogador);
}

void simularAtaque(Territorio *mapa, int origem, int destino, const char *corJogador) {
    if (strcmp(mapa[origem].corDono, corJogador) != 0) {
        printf("\n❌ Você não controla o território de origem!\n");
        return;
    }

    if (mapa[origem].tropas <= 1) {
        printf("\n❌ Você precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n🎲 Você rolou %d | Defesa rolou %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("✔ Vitória! Tropas inimigas reduzidas.\n");
        mapa[destino].tropas--;

        if (mapa[destino].tropas == 0) {
            printf("🏆 Você conquistou %s!\n", mapa[destino].nome);
            strcpy(mapa[destino].corDono, corJogador);
            mapa[destino].tropas = 1;
            mapa[origem].tropas--;
        }
    } else {
        printf("❌ Derrota! Você perdeu uma tropa.\n");
        mapa[origem].tropas--;
    }
}

int sortearMissao() {
    return (rand() % NUM_MISSOES) + 1;
}

int verificarVitoria(const Territorio *mapa, const char *corJogador, int idMissao) {

    if (idMissao == 1) {
        int conquistados = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++)
            if (strcmp(mapa[i].corDono, corJogador) == 0)
                conquistados++;

        return conquistados >= 5;
    }

    if (idMissao == 2) {
        char corAlvo[TAM_STRING] = "Vermelho"; 
        int vivos = 0;

        for (int i = 0; i < NUM_TERRITORIOS; i++)
            if (strcmp(mapa[i].corDono, corAlvo) == 0)
                vivos++;

        return vivos == 0;
    }

    if (idMissao == 3) {
        int ok = 1;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (mapa[i].nome[0] == 'A' &&
                strcmp(mapa[i].corDono, corJogador) != 0)
                ok = 0;
        }
        return ok;
    }

    return 0;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
