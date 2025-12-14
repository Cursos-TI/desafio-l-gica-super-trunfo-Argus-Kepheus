//==============================================================================
// SUPER TRUNFO - PAÍSES
// Tema 02 - Nível Aventureiro
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
#define LINHA_SEPARADORA "================================================================================\n"

// Enums para melhor legibilidade e manutenção
typedef enum {
    MOD_SUPER_PODER = 1,
    MOD_ATRIBUTO_ISOLADO,
    MOD_MULTIPLOS_ATRIBUTOS
} Modalidade;

typedef enum {
    ATTR_POPULACAO = 1,
    ATTR_AREA,
    ATTR_PIB,
    ATTR_PONTOS_TURISTICOS,
    ATTR_DENSIDADE,
    ATTR_PIB_PER_CAPITA
} Atributo;

typedef enum {
    COMB_ECONOMICA = 1,
    COMB_DEMOGRAFICA,
    COMB_TERRITORIAL,
    COMB_TURISTICA
} Combinacao;

//==============================================================================
// ESTRUTURA PRINCIPAL - Carta
//==============================================================================
typedef struct {
    char codigo[5];              // Ex: A01, B02
    unsigned long populacao;
    float area;
    float pib;
    int pontos_turisticos;
    // Campos calculados
    double densidade;
    double pib_per_capita;
    float super_poder;
} Carta;

//==============================================================================
// ESTRUTURA DE DADOS PRÉ-CARREGADOS
//==============================================================================
typedef struct {
    unsigned long pop[30];
    float area[30];
    float pib[30];
    int pontos[30];
} DadosIniciais;

// Dados iniciais em estrutura mais compacta
static const DadosIniciais dados_iniciais = {
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
// PROTÓTIPOS DE FUNÇÕES
//==============================================================================
// Inicialização
void inicializar_cartas(Carta cartas[]);
void calcular_propriedades(Carta *carta);

// Entrada de dados
void limpar_buffer(void);
unsigned long ler_populacao(void);
float ler_numero_float(const char *prompt, const char *erro, float min);
int ler_numero_int(const char *prompt, const char *erro, int min);
void ler_dados_carta(Carta *carta);

// Interface
void exibir_cabecalho(void);
void exibir_tabela(const Carta cartas[]);
void exibir_opcoes_cartas(const Carta cartas[]);
void imprimir_linha_separadora(char c, int tamanho);
int obter_opcao(int min, int max, const char *prompt);
int obter_modalidade(void);
int obter_atributo(void);
int obter_combinacao(void);

// Comparação
double obter_valor_atributo(const Carta *carta, Atributo attr);
bool comparar_cartas_simples(const Carta *c1, const Carta *c2, Atributo attr);
bool comparar_multiplos_atributos(const Carta *c1, const Carta *c2, Combinacao comb);
void executar_comparacao(const Carta cartas[], Modalidade mod, int param);
void exibir_resultado_comparacao(const Carta *c1, const Carta *c2);

//==============================================================================
// FUNÇÃO PRINCIPAL
//==============================================================================
int main(void) {
    Carta cartas[TOTAL_CARTAS];
    
    exibir_cabecalho();
    inicializar_cartas(cartas);
    
    // Entrada de dados do usuário para as últimas 2 cartas
    for (int i = CARTAS_PREENCHIDAS; i < TOTAL_CARTAS; i++) {
        printf("\n--- Cadastrando carta %s ---\n", cartas[i].codigo);
        ler_dados_carta(&cartas[i]);
        calcular_propriedades(&cartas[i]);
        printf("✓ Carta %s cadastrada!\n", cartas[i].codigo);
    }
    
    // Exibição e comparação
    exibir_tabela(cartas);
    
    printf("\n");
    imprimir_linha_separadora('=', 100);
    printf("                                    SISTEMA DE COMPARAÇÃO\n");
    imprimir_linha_separadora('=', 100);
    
    Modalidade modalidade = obter_modalidade();
    int parametro = 0;
    
    if (modalidade == MOD_ATRIBUTO_ISOLADO) {
        parametro = obter_atributo();
    } else if (modalidade == MOD_MULTIPLOS_ATRIBUTOS) {
        parametro = obter_combinacao();
    }
    
    executar_comparacao(cartas, modalidade, parametro);
    
    printf("\n=== Sistema Super Trunfo concluído! ===\n");
    return 0;
}

//==============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
//==============================================================================

void imprimir_linha_separadora(char c, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        putchar(c);
    }
    putchar('\n');
}

void exibir_cabecalho(void) {
    imprimir_linha_separadora('=', 100);
    printf("                                 BEM-VINDO AO SUPER TRUNFO - PAÍSES\n");
    imprimir_linha_separadora('=', 100);
    printf("Sistema com %d cartas (%d pré-preenchidas + %d do usuário)\n\n",
           TOTAL_CARTAS, CARTAS_PREENCHIDAS, CARTAS_USUARIO);
}

void inicializar_cartas(Carta cartas[]) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        // Gerar código da carta
        int estado = i / CIDADES_POR_ESTADO;
        int cidade = (i % CIDADES_POR_ESTADO) + 1;
        sprintf(cartas[i].codigo, "%c%02d", 'A' + estado, cidade);
        
        // Inicializar dados
        if (i < CARTAS_PREENCHIDAS) {
            cartas[i].populacao = dados_iniciais.pop[i];
            cartas[i].area = dados_iniciais.area[i];
            cartas[i].pib = dados_iniciais.pib[i];
            cartas[i].pontos_turisticos = dados_iniciais.pontos[i];
            calcular_propriedades(&cartas[i]);
        } else {
            // Inicializar valores vazios para cartas do usuário
            cartas[i].populacao = 0;
            cartas[i].area = 0;
            cartas[i].pib = 0;
            cartas[i].pontos_turisticos = 0;
            cartas[i].densidade = 0;
            cartas[i].pib_per_capita = 0;
            cartas[i].super_poder = 0;
        }
    }
}

void calcular_propriedades(Carta *carta) {
    carta->densidade = (carta->area > 0) ? 
                      (double)carta->populacao / carta->area : 0.0;
    carta->pib_per_capita = (carta->populacao > 0) ? 
                           (double)carta->pib / carta->populacao : 0.0;
    carta->super_poder = (float)carta->populacao + carta->area + carta->pib + 
                        (float)carta->pontos_turisticos + 
                        (float)carta->densidade + (float)carta->pib_per_capita;
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

unsigned long ler_populacao(void) {
    unsigned long valor;
    while (1) {
        printf("Digite a população: ");
        if (scanf("%lu", &valor) == 1 && valor > 0) {
            return valor;
        }
        printf("ERRO: Digite um número válido maior que zero!\n");
        limpar_buffer();
    }
}

float ler_numero_float(const char *prompt, const char *erro, float min) {
    float valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &valor) == 1 && valor >= min) {
            return valor;
        }
        printf("%s\n", erro);
        limpar_buffer();
    }
}

int ler_numero_int(const char *prompt, const char *erro, int min) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) == 1 && valor >= min) {
            return valor;
        }
        printf("%s\n", erro);
        limpar_buffer();
    }
}

void ler_dados_carta(Carta *carta) {
    carta->populacao = ler_populacao();
    carta->area = ler_numero_float("Digite a área (km²): ", 
                                   "ERRO: A área deve ser maior que zero!", 0.01f);
    carta->pib = ler_numero_float("Digite o PIB (em bilhões): ",
                                  "ERRO: O PIB deve ser positivo!", 0.0f);
    carta->pontos_turisticos = ler_numero_int("Digite o número de pontos turísticos: ",
                                              "ERRO: Número deve ser positivo!", 0);
}

void exibir_tabela(const Carta cartas[]) {
    printf("\n");
    imprimir_linha_separadora('=', 100);
    printf("                          TODAS AS CARTAS - FORMATO TABULAR\n");
    imprimir_linha_separadora('=', 100);
    
    for (int estado = 0; estado < NUM_ESTADOS; estado++) {
        printf("\n====== ESTADO %c ======\n", 'A' + estado);
        printf("%-5s %-12s %-10s %-12s %-8s %-12s %-12s\n",
               "Carta", "População", "Área", "PIB", "Pontos", "Densidade", "SuperPoder");
        imprimir_linha_separadora('-', 80);
        
        for (int i = 0; i < CIDADES_POR_ESTADO; i++) {
            const Carta *c = &cartas[estado * CIDADES_POR_ESTADO + i];
            printf("%-5s %-12lu %-10.2f %-12.2f %-8d %-12.2f %-12.2f\n",
                   c->codigo, c->populacao, c->area, c->pib,
                   c->pontos_turisticos, c->densidade, c->super_poder);
        }
    }
}

void exibir_opcoes_cartas(const Carta cartas[]) {
    printf("\nCartas disponíveis:\n");
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        printf("%2d.%-4s ", i + 1, cartas[i].codigo);
        if ((i + 1) % 8 == 0) printf("\n");
    }
}

int obter_opcao(int min, int max, const char *prompt) {
    int opcao;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &opcao) == 1 && opcao >= min && opcao <= max) {
            return opcao;
        }
        printf("ERRO: Escolha entre %d e %d!\n", min, max);
        limpar_buffer();
    }
}

int obter_modalidade(void) {
    printf("\n=== MODALIDADES DE COMPARAÇÃO ===\n");
    printf("1. Super Poder (soma de todos atributos)\n");
    printf("2. Atributo Isolado\n");
    printf("3. Múltiplos Atributos\n");
    return obter_opcao(1, 3, "Escolha a modalidade (1-3): ");
}

int obter_atributo(void) {
    printf("\n=== ATRIBUTOS DISPONÍVEIS ===\n");
    const char *nomes[] = {"", "População", "Área", "PIB", "Pontos Turísticos",
                          "Densidade (menor vence)", "PIB per Capita"};
    for (int i = 1; i <= 6; i++) {
        printf("%d. %s\n", i, nomes[i]);
    }
    return obter_opcao(1, 6, "Escolha o atributo (1-6): ");
}

int obter_combinacao(void) {
    printf("\n=== COMBINAÇÕES DISPONÍVEIS ===\n");
    printf("1. Econômica: PIB && PIB per Capita\n");
    printf("2. Demográfica: População && Densidade\n");
    printf("3. Territorial: Área && População\n");
    printf("4. Turística: Pontos Turísticos || PIB\n");
    return obter_opcao(1, 4, "Escolha a combinação (1-4): ");
}

double obter_valor_atributo(const Carta *carta, Atributo attr) {
    switch (attr) {
        case ATTR_POPULACAO: return (double)carta->populacao;
        case ATTR_AREA: return carta->area;
        case ATTR_PIB: return carta->pib;
        case ATTR_PONTOS_TURISTICOS: return carta->pontos_turisticos;
        case ATTR_DENSIDADE: return carta->densidade;
        case ATTR_PIB_PER_CAPITA: return carta->pib_per_capita;
        default: return 0.0;
    }
}

bool comparar_cartas_simples(const Carta *c1, const Carta *c2, Atributo attr) {
    double v1 = obter_valor_atributo(c1, attr);
    double v2 = obter_valor_atributo(c2, attr);
    
    // Para densidade, menor vence
    if (attr == ATTR_DENSIDADE) {
        return v1 < v2;
    }
    return v1 > v2;
}

bool comparar_multiplos_atributos(const Carta *c1, const Carta *c2, Combinacao comb) {
    switch (comb) {
        case COMB_ECONOMICA:
            return c1->pib > c2->pib && c1->pib_per_capita > c2->pib_per_capita;
            
        case COMB_DEMOGRAFICA:
            return c1->populacao > c2->populacao && c1->densidade < c2->densidade;
            
        case COMB_TERRITORIAL:
            return c1->area > c2->area && c1->populacao > c2->populacao;
            
        case COMB_TURISTICA:
            return c1->pontos_turisticos > c2->pontos_turisticos || c1->pib > c2->pib;
            
        default:
            return false;
    }
}

void executar_comparacao(const Carta cartas[], Modalidade mod, int param) {
    exibir_opcoes_cartas(cartas);
    
    int idx1 = obter_opcao(1, TOTAL_CARTAS, "\nEscolha a primeira carta: ") - 1;
    int idx2;
    do {
        idx2 = obter_opcao(1, TOTAL_CARTAS, "Escolha a segunda carta (diferente): ") - 1;
    } while (idx2 == idx1);
    
    const Carta *c1 = &cartas[idx1];
    const Carta *c2 = &cartas[idx2];
    
    printf("\n=== COMPARANDO: %s vs %s ===\n", c1->codigo, c2->codigo);
    
    const char *vencedor = NULL;
    
    switch (mod) {
        case MOD_SUPER_PODER:
            printf("Super Poder: %.2f vs %.2f\n", c1->super_poder, c2->super_poder);
            if (c1->super_poder > c2->super_poder) vencedor = c1->codigo;
            else if (c2->super_poder > c1->super_poder) vencedor = c2->codigo;
            break;
            
        case MOD_ATRIBUTO_ISOLADO: {
            double v1 = obter_valor_atributo(c1, param);
            double v2 = obter_valor_atributo(c2, param);
            printf("Valores: %.2f vs %.2f\n", v1, v2);
            if (comparar_cartas_simples(c1, c2, param)) vencedor = c1->codigo;
            else if (comparar_cartas_simples(c2, c1, param)) vencedor = c2->codigo;
            break;
        }
            
        case MOD_MULTIPLOS_ATRIBUTOS:
            if (comparar_multiplos_atributos(c1, c2, param)) vencedor = c1->codigo;
            else if (comparar_multiplos_atributos(c2, c1, param)) vencedor = c2->codigo;
            break;
    }
    
    if (vencedor) {
        printf("🏆 VENCEDOR: %s\n", vencedor);
    } else {
        printf("🤝 EMPATE!\n");
    }
    
    exibir_resultado_comparacao(c1, c2);
}

void exibir_resultado_comparacao(const Carta *c1, const Carta *c2) {
    printf("\n=== RESUMO COMPARATIVO ===\n");
    printf("%-6s | Pop: %lu | Area: %.0f | PIB: %.1f | Pts: %d | Dens: %.1f | SP: %.0f\n",
           c1->codigo, c1->populacao, c1->area, c1->pib, 
           c1->pontos_turisticos, c1->densidade, c1->super_poder);
    printf("%-6s | Pop: %lu | Area: %.0f | PIB: %.1f | Pts: %d | Dens: %.1f | SP: %.0f\n",
           c2->codigo, c2->populacao, c2->area, c2->pib,
           c2->pontos_turisticos, c2->densidade, c2->super_poder);
    
    int vitorias_c1 = 0;
    vitorias_c1 += (c1->populacao > c2->populacao);
    vitorias_c1 += (c1->area > c2->area);
    vitorias_c1 += (c1->pib > c2->pib);
    vitorias_c1 += (c1->pontos_turisticos > c2->pontos_turisticos);
    vitorias_c1 += (c1->densidade < c2->densidade);
    vitorias_c1 += (c1->pib_per_capita > c2->pib_per_capita);
    vitorias_c1 += (c1->super_poder > c2->super_poder);
    
    printf("\nPlacar geral: %s: %d/7 | %s: %d/7\n", 
           c1->codigo, vitorias_c1, c2->codigo, 7 - vitorias_c1);
}