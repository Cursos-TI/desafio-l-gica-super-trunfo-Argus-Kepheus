//==============================================================================
// SUPER TRUNFO - PAÍSES
// Tema 03 - Nível Mestre
//==============================================================================

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//==============================================================================
// CONSTANTES E CONFIGURAÇÕES
//==============================================================================
#define NUM_ESTADOS 8
#define CIDADES_POR_ESTADO 4
#define TOTAL_CARTAS 32
#define CARTAS_PREENCHIDAS 30
#define CARTAS_USUARIO 2

// Enums para modalidades e atributos
typedef enum {
    MOD_SUPER_PODER = 1,
    MOD_ATRIBUTO_ISOLADO,
    MOD_DOIS_ATRIBUTOS,      // NOVO: Nível Mestre
    MOD_MULTIPLOS_ATRIBUTOS
} Modalidade;

typedef enum {
    ATTR_POPULACAO = 1,
    ATTR_AREA,
    ATTR_PIB,
    ATTR_PONTOS_TURISTICOS,
    ATTR_DENSIDADE,
    ATTR_PIB_PER_CAPITA,
    NUM_ATRIBUTOS = 6
} Atributo;

typedef enum {
    COMB_AND = 1,  // Ambos atributos devem vencer
    COMB_OR,       // Pelo menos um atributo deve vencer
    COMB_XOR       // Exatamente um atributo deve vencer
} TipoCombinacao;

//==============================================================================
// ESTRUTURA PRINCIPAL
//==============================================================================
typedef struct {
    char codigo[5];
    unsigned long populacao;
    float area;
    float pib;
    int pontos_turisticos;
    double densidade;
    double pib_per_capita;
    float super_poder;
} Carta;

//==============================================================================
// DADOS PRÉ-CARREGADOS (estrutura compacta)
//==============================================================================
static const struct {
    unsigned long pop[30];
    float area[30];
    float pib[30];
    int pontos[30];
} dados = {
    .pop = {12325232, 6748000, 2886698, 2669342, 2521564, 2219580, 1948626, 1653461,
            3055149, 1536097, 1499641, 1488252, 1392121, 1213792, 1108975, 1084839,
            1025360, 924624, 906092, 890480, 868075, 844483, 817208, 817511,
            721368, 697886, 695956, 668949, 695737, 699097},
    .area = {1521.11, 1200.27, 692.82, 314.93, 331.40, 11401.09, 434.89, 218.44,
             5760.78, 739.49, 1059.46, 496.68, 318.68, 794.57, 834.78, 249.14,
             510.65, 467.62, 8096.05, 167.26, 1391.98, 408.45, 521.24, 211.47,
             175.78, 64.95, 258.69, 195.27, 650.95, 4115.82},
    .pib = {714.68, 254.51, 60.73, 58.26, 93.68, 75.88, 102.65, 66.27,
            259.49, 49.15, 36.25, 93.77, 55.45, 59.02, 19.31, 14.98,
            16.46, 15.21, 28.48, 18.28, 13.73, 46.27, 13.04, 16.78,
            56.90, 55.46, 12.34, 11.87, 18.45, 26.51},
    .pontos = {156, 189, 128, 89, 76, 94, 67, 82, 145, 54,
               78, 98, 23, 45, 67, 34, 89, 28, 112, 76,
               58, 67, 31, 87, 45, 32, 54, 43, 78, 89}
};

//==============================================================================
// PROTÓTIPOS SIMPLIFICADOS
//==============================================================================
// Core
void inicializar_cartas(Carta cartas[]);
void calcular_propriedades(Carta *carta);

// I/O
void limpar_buffer(void);
float ler_float(const char *prompt, float min);
int ler_int(const char *prompt, int min, int max);
void ler_dados_carta(Carta *carta);

// Interface
void exibir_linha(char c, int tam);
void exibir_tabela_resumida(const Carta cartas[]);
int obter_modalidade(void);
const char* nome_atributo(Atributo attr);

// Comparação
double valor_atributo(const Carta *c, Atributo a);
int comparar_atributo(const Carta *c1, const Carta *c2, Atributo attr);
void comparacao_dois_atributos(const Carta cartas[]);
void executar_comparacao_completa(const Carta cartas[]);

//==============================================================================
// MACROS COM OPERADOR TERNÁRIO
//==============================================================================
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define VENCEDOR_SIMPLES(v1, v2, inverso) ((inverso) ? ((v1) < (v2) ? 1 : ((v2) < (v1) ? 2 : 0)) : \
                                                       ((v1) > (v2) ? 1 : ((v2) > (v1) ? 2 : 0)))
#define FORMATAR_NUMERO(n) ((n) >= 1000000 ? (n)/1000000.0 : (n) >= 1000 ? (n)/1000.0 : (n))
#define UNIDADE_NUMERO(n) ((n) >= 1000000 ? "M" : (n) >= 1000 ? "K" : "")

//==============================================================================
// MAIN
//==============================================================================
int main(void) {
    Carta cartas[TOTAL_CARTAS];
    
    // Cabeçalho
    exibir_linha('=', 80);
    printf("               SUPER TRUNFO - PAÍSES\n");
    exibir_linha('=', 80);
    
    // Inicialização
    inicializar_cartas(cartas);
    
    // Entrada usuário (últimas 2 cartas)
    for (int i = CARTAS_PREENCHIDAS; i < TOTAL_CARTAS; i++) {
        printf("\n📝 Cadastro da carta %s:\n", cartas[i].codigo);
        ler_dados_carta(&cartas[i]);
        calcular_propriedades(&cartas[i]);
    }
    
    // Exibição e comparação
    exibir_tabela_resumida(cartas);
    executar_comparacao_completa(cartas);
    
    printf("\n✅ Sistema finalizado com sucesso!\n");
    return 0;
}

//==============================================================================
// IMPLEMENTAÇÃO OTIMIZADA
//==============================================================================

void inicializar_cartas(Carta cartas[]) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        sprintf(cartas[i].codigo, "%c%02d", 'A' + (i/4), (i%4) + 1);
        
        if (i < CARTAS_PREENCHIDAS) {
            cartas[i].populacao = dados.pop[i];
            cartas[i].area = dados.area[i];
            cartas[i].pib = dados.pib[i];
            cartas[i].pontos_turisticos = dados.pontos[i];
            calcular_propriedades(&cartas[i]);
        }
    }
}

void calcular_propriedades(Carta *c) {
    // Uso de operador ternário
    c->densidade = c->area > 0 ? (double)c->populacao / c->area : 0;
    c->pib_per_capita = c->populacao > 0 ? (double)c->pib / c->populacao : 0;
    c->super_poder = (float)c->populacao + c->area + c->pib + 
                     (float)c->pontos_turisticos + (float)c->densidade + 
                     (float)c->pib_per_capita;
}

void limpar_buffer(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF);
}

float ler_float(const char *prompt, float min) {
    float valor;
    do {
        printf("%s", prompt);
        if (scanf("%f", &valor) != 1) {
            printf("❌ Entrada inválida!\n");
            limpar_buffer();
            valor = min - 1;
        }
    } while (valor < min);
    return valor;
}

int ler_int(const char *prompt, int min, int max) {
    int valor;
    do {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("❌ Entrada inválida!\n");
            limpar_buffer();
            valor = min - 1;
        }
    } while (valor < min || valor > max);
    return valor;
}

void ler_dados_carta(Carta *c) {
    c->populacao = (unsigned long)ler_float("População: ", 1);
    c->area = ler_float("Área (km²): ", 0.01);
    c->pib = ler_float("PIB (bilhões): ", 0);
    c->pontos_turisticos = ler_int("Pontos turísticos: ", 0, 9999);
}

void exibir_linha(char c, int tam) {
    while (tam--) putchar(c);
    putchar('\n');
}

void exibir_tabela_resumida(const Carta cartas[]) {
    printf("\n📊 RESUMO DAS CARTAS:\n");
    exibir_linha('-', 60);
    printf("%-5s %-10s %-8s %-8s %-8s %-10s\n", 
           "Carta", "População", "Área", "PIB", "Pontos", "SuperPoder");
    exibir_linha('-', 60);
    
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        const Carta *c = &cartas[i];
        // Uso de operador ternário para formatação compacta
        printf("%-5s %7.1f%-3s %6.0f %7.1f %5d %10.0f\n",
               c->codigo, 
               FORMATAR_NUMERO(c->populacao), UNIDADE_NUMERO(c->populacao),
               c->area, c->pib, c->pontos_turisticos, c->super_poder);
    }
}

int obter_modalidade(void) {
    printf("\n🎮 MODALIDADES DE JOGO:\n");
    printf("1. Super Poder (soma total)\n");
    printf("2. Atributo Isolado\n");
    printf("3. Dois Atributos\n");
    printf("4. Múltiplos Atributos Pré-definidos\n");
    return ler_int("Escolha (1-4): ", 1, 4);
}

const char* nome_atributo(Atributo attr) {
    const char* nomes[] = {"", "População", "Área", "PIB", 
                          "Pontos Turísticos", "Densidade", "PIB per Capita"};
    return attr <= NUM_ATRIBUTOS ? nomes[attr] : "Desconhecido";
}

double valor_atributo(const Carta *c, Atributo a) {
    // Switch com retorno direto (mais eficiente)
    switch (a) {
        case ATTR_POPULACAO: return (double)c->populacao;
        case ATTR_AREA: return c->area;
        case ATTR_PIB: return c->pib;
        case ATTR_PONTOS_TURISTICOS: return c->pontos_turisticos;
        case ATTR_DENSIDADE: return c->densidade;
        case ATTR_PIB_PER_CAPITA: return c->pib_per_capita;
        default: return 0;
    }
}

int comparar_atributo(const Carta *c1, const Carta *c2, Atributo attr) {
    double v1 = valor_atributo(c1, attr);
    double v2 = valor_atributo(c2, attr);
    // Operador ternário aninhado
    return VENCEDOR_SIMPLES(v1, v2, attr == ATTR_DENSIDADE);
}

void comparacao_dois_atributos(const Carta cartas[]) {
    printf("\n🎯 COMPARAÇÃO DE DOIS ATRIBUTOS \n");
    exibir_linha('-', 50);
    
    // Escolha dos atributos
    printf("\nPrimeiro atributo:\n");
    for (int i = 1; i <= NUM_ATRIBUTOS; i++) {
        printf("%d. %s\n", i, nome_atributo(i));
    }
    int attr1 = ler_int("Escolha: ", 1, NUM_ATRIBUTOS);
    
    printf("\nSegundo atributo:\n");
    int attr2 = ler_int("Escolha: ", 1, NUM_ATRIBUTOS);
    
    // Escolha da lógica de combinação
    printf("\nComo combinar os atributos?\n");
    printf("1. AND (ambos devem vencer)\n");
    printf("2. OR (pelo menos um deve vencer)\n");
    printf("3. XOR (exatamente um deve vencer)\n");
    TipoCombinacao tipo = ler_int("Escolha: ", 1, 3);
    
    // Escolha das cartas
    printf("\nEscolha as cartas (1-%d):\n", TOTAL_CARTAS);
    int idx1 = ler_int("Carta 1: ", 1, TOTAL_CARTAS) - 1;
    int idx2 = ler_int("Carta 2: ", 1, TOTAL_CARTAS) - 1;
    
    const Carta *c1 = &cartas[idx1];
    const Carta *c2 = &cartas[idx2];
    
    // Comparação com lógica complexa
    int v1 = comparar_atributo(c1, c2, attr1);
    int v2 = comparar_atributo(c1, c2, attr2);
    
    printf("\n📊 RESULTADOS:\n");
    printf("%s: %.2f | %s: %.2f -> %s\n", 
           nome_atributo(attr1), valor_atributo(c1, attr1),
           nome_atributo(attr1), valor_atributo(c2, attr1),
           v1 == 1 ? "C1 vence" : v1 == 2 ? "C2 vence" : "Empate");
    
    printf("%s: %.2f | %s: %.2f -> %s\n",
           nome_atributo(attr2), valor_atributo(c1, attr2),
           nome_atributo(attr2), valor_atributo(c2, attr2),
           v2 == 1 ? "C1 vence" : v2 == 2 ? "C2 vence" : "Empate");
    
    // Resultado final com operador ternário complexo
    int vencedor = 0;
    switch (tipo) {
        case COMB_AND:
            vencedor = (v1 == 1 && v2 == 1) ? 1 : (v1 == 2 && v2 == 2) ? 2 : 0;
            break;
        case COMB_OR:
            vencedor = (v1 == 1 || v2 == 1) ? 1 : (v1 == 2 || v2 == 2) ? 2 : 0;
            break;
        case COMB_XOR:
            vencedor = ((v1 == 1) ^ (v2 == 1)) ? (v1 == 1 ? 1 : 2) : 
                      ((v1 == 2) ^ (v2 == 2)) ? (v1 == 2 ? 1 : 2) : 0;
            break;
    }
    
    printf("\n🏆 VENCEDOR FINAL: %s\n", 
           vencedor == 1 ? c1->codigo : vencedor == 2 ? c2->codigo : "EMPATE");
}

void executar_comparacao_completa(const Carta cartas[]) {
    printf("\n");
    exibir_linha('=', 60);
    printf("                   SISTEMA DE COMPARAÇÃO\n");
    exibir_linha('=', 60);
    
    Modalidade mod = obter_modalidade();
    
    switch (mod) {
        case MOD_SUPER_PODER: {
            int idx1 = ler_int("Carta 1 (1-32): ", 1, TOTAL_CARTAS) - 1;
            int idx2 = ler_int("Carta 2 (1-32): ", 1, TOTAL_CARTAS) - 1;
            
            const Carta *c1 = &cartas[idx1];
            const Carta *c2 = &cartas[idx2];
            
            // Comparação com operador ternário aninhado
            printf("\n🏆 Vencedor Super Poder: %s (%.0f vs %.0f)\n",
                   c1->super_poder > c2->super_poder ? c1->codigo :
                   c2->super_poder > c1->super_poder ? c2->codigo : "EMPATE",
                   c1->super_poder, c2->super_poder);
            break;
        }
        
        case MOD_ATRIBUTO_ISOLADO: {
            printf("\nEscolha o atributo:\n");
            for (int i = 1; i <= NUM_ATRIBUTOS; i++) {
                printf("%d. %s%s\n", i, nome_atributo(i),
                       i == ATTR_DENSIDADE ? " (menor vence)" : "");
            }
            int attr = ler_int("Atributo: ", 1, NUM_ATRIBUTOS);
            
            int idx1 = ler_int("Carta 1 (1-32): ", 1, TOTAL_CARTAS) - 1;
            int idx2 = ler_int("Carta 2 (1-32): ", 1, TOTAL_CARTAS) - 1;
            
            const Carta *c1 = &cartas[idx1];
            const Carta *c2 = &cartas[idx2];
            
            int resultado = comparar_atributo(c1, c2, attr);
            
            // Uso intensivo de operador ternário
            printf("\n🏆 Vencedor %s: %s (%.2f vs %.2f)\n",
                   nome_atributo(attr),
                   resultado == 1 ? c1->codigo : resultado == 2 ? c2->codigo : "EMPATE",
                   valor_atributo(c1, attr), valor_atributo(c2, attr));
            break;
        }
        
        case MOD_DOIS_ATRIBUTOS:
            // Função específica para Nível Mestre
            comparacao_dois_atributos(cartas);
            break;
            
        case MOD_MULTIPLOS_ATRIBUTOS: {
            printf("\nCombinações pré-definidas:\n");
            printf("1. Econômica (PIB && PIB pc)\n");
            printf("2. Demográfica (Pop && Densidade)\n");
            printf("3. Territorial (Área && Pop)\n");
            int combo = ler_int("Escolha: ", 1, 3);
            
            int idx1 = ler_int("Carta 1 (1-32): ", 1, TOTAL_CARTAS) - 1;
            int idx2 = ler_int("Carta 2 (1-32): ", 1, TOTAL_CARTAS) - 1;
            
            const Carta *c1 = &cartas[idx1];
            const Carta *c2 = &cartas[idx2];
            
            // Lógica complexa com operadores ternários aninhados
            bool vitoria = false;
            switch (combo) {
                case 1: 
                    vitoria = c1->pib > c2->pib && c1->pib_per_capita > c2->pib_per_capita;
                    break;
                case 2:
                    vitoria = c1->populacao > c2->populacao && c1->densidade < c2->densidade;
                    break;
                case 3:
                    vitoria = c1->area > c2->area && c1->populacao > c2->populacao;
                    break;
            }
            
            printf("\n🏆 Vencedor: %s\n", 
                   vitoria ? c1->codigo : 
                   (!vitoria && combo == 1 && c2->pib > c1->pib && c2->pib_per_capita > c1->pib_per_capita) ? c2->codigo :
                   (!vitoria && combo == 2 && c2->populacao > c1->populacao && c2->densidade < c1->densidade) ? c2->codigo :
                   (!vitoria && combo == 3 && c2->area > c1->area && c2->populacao > c1->populacao) ? c2->codigo :
                   "EMPATE");
            break;
        }
    }
}