//==============================================================================
// SUPER TRUNFO - PAÍSES
// Tema 01 - Nível Novato
//==============================================================================
//
// FUNCIONALIDADES:
// • 30 cartas pré-preenchidas automaticamente + 2 pelo usuário
// • Estados A-H, cidades padronizadas como A01, B02, etc.
// • Múltiplas modalidades de comparação escolhidas pelo usuário
// • Sistema de comparação entre cartas escolhidas pelo usuário
// • Cálculos automáticos: densidade populacional, PIB per capita, super poder
// • Interface tabular organizada por estado
// • Validação robusta de entrada com tratamento de erros
//
// NOTA: Este programa foi desenvolvido com assistência de IA (Claude Sonnet),
// numa colaboração humano-máquina para garantir qualidade e funcionalidade.
//==============================================================================

#include <stdio.h>
#include <ctype.h>
#include <string.h>

//==============================================================================
// CONSTANTES DO SISTEMA
//==============================================================================
// Estrutura do jogo: 1 país com 8 estados (A-H), 4 cidades por estado
#define NUM_ESTADOS 8                   // Estados de A a H
#define CIDADES_POR_ESTADO 4            // 4 cidades por estado  
#define TOTAL_CARTAS (NUM_ESTADOS * CIDADES_POR_ESTADO)  // Total: 32 cartas
#define CARTAS_PREENCHIDAS 30           // Cartas pré-preenchidas automaticamente
#define CARTAS_USUARIO 2                // Cartas preenchidas pelo usuário
#define TAM_NOME_CIDADE 4               // Tamanho para A01, B02, etc. (3 chars + \0)

//------------------------------------------------------------------------------
// Constantes para modalidades de jogo
//------------------------------------------------------------------------------
#define MODALIDADE_SUPER_PODER 1        // Comparação por Super Poder
#define MODALIDADE_ATRIBUTO_ISOLADO 2   // Comparação por atributo específico

//------------------------------------------------------------------------------
// Constantes para atributos específicos
//------------------------------------------------------------------------------
#define ATRIBUTO_POPULACAO 1            // População
#define ATRIBUTO_AREA 2                 // Área
#define ATRIBUTO_PIB 3                  // PIB
#define ATRIBUTO_PONTOS_TURISTICOS 4    // Pontos Turísticos
#define ATRIBUTO_DENSIDADE 5            // Densidade Populacional
#define ATRIBUTO_PIB_PER_CAPITA 6       // PIB per Capita
#define NUM_ATRIBUTOS 6                 // Total de atributos comparáveis

//==============================================================================
// PROTÓTIPOS DAS FUNÇÕES
//==============================================================================

//------------------------------------------------------------------------------
// Funções de entrada de dados (com validação)
//------------------------------------------------------------------------------
unsigned long lerPopulacao();          // Lê população como unsigned long
float         lerArea();               // Lê área garantindo valor positivo
float         lerPIB();                // Lê PIB garantindo valor positivo  
int           lerPontosTuristicos();   // Lê pontos turísticos (não negativos)
void          limparBuffer();          // Limpa buffer do teclado

//------------------------------------------------------------------------------
// Funções de geração e manipulação de dados
//------------------------------------------------------------------------------
void gerarNomeCidade(char nome[], char estado, int cidade); // Gera nomes A01, B02...
void preencherCartasAutomaticamente(char nomesCidades[][TAM_NOME_CIDADE],
                                   unsigned long populacoes[],
                                   float areas[],
                                   float pibs[],
                                   int pontosTuristicos[]); // Preenche 30 cartas

//------------------------------------------------------------------------------
// Funções de cálculos (propriedades derivadas)
//------------------------------------------------------------------------------
double calcularDensidadePopulacional(unsigned long populacao, float area);
double calcularPIBPerCapita(float pib, unsigned long populacao);
float  calcularSuperPoder(unsigned long populacao, float area, float pib,
                         int pontosTuristicos, double densidade, double pibPerCapita);

//------------------------------------------------------------------------------
// Funções do sistema de modalidades e comparação
//------------------------------------------------------------------------------
int  obterModalidadeJogo();            // Obtém modalidade escolhida pelo usuário
int  obterAtributoComparacao();        // Obtém atributo específico para comparação
void exibirMenuModalidades();          // Exibe opções de modalidades
void exibirMenuAtributos();            // Exibe opções de atributos

//------------------------------------------------------------------------------
// Funções do sistema de comparação de cartas
//------------------------------------------------------------------------------
void exibirIndiceCartas(char nomesCidades[][TAM_NOME_CIDADE]); 
int  obterEscolhaUsuario(int maxCartas); 
int  obterEscolhaUsuarioExcluindo(int maxCartas, int cartaExcluida);

//------------------------------------------------------------------------------
// Funções de comparação (refatoradas)
//------------------------------------------------------------------------------
void executarComparacao(int modalidade, int atributo, int carta1, int carta2,
                       char nomesCidades[][TAM_NOME_CIDADE],
                       unsigned long populacoes[], float areas[], float pibs[],
                       int pontosTuristicos[], double densidades[], 
                       double pibsPerCapita[], float superPoderes[]);

void compararSuperPoder(int carta1, int carta2, char nomesCidades[][TAM_NOME_CIDADE],
                       float superPoderes[]);

void compararAtributoIsolado(int atributo, int carta1, int carta2,
                            char nomesCidades[][TAM_NOME_CIDADE],
                            unsigned long populacoes[], float areas[], float pibs[],
                            int pontosTuristicos[], double densidades[], 
                            double pibsPerCapita[]);

//------------------------------------------------------------------------------
// Funções de exibição de dados
//------------------------------------------------------------------------------
void exibirTabelaEstado(char estado, int inicioIndice, 
                       char nomesCidades[][TAM_NOME_CIDADE], 
                       unsigned long populacoes[], float areas[], float pibs[], 
                       int pontosTuristicos[], double densidades[], 
                       double pibsPerCapita[], float superPoderes[]);

void exibirResultadoTabular(int carta1, int carta2, char nomesCidades[][TAM_NOME_CIDADE],
                           unsigned long populacoes[], float areas[], float pibs[],
                           int pontosTuristicos[], double densidades[], 
                           double pibsPerCapita[], float superPoderes[]);

//------------------------------------------------------------------------------
// Funções utilitárias para comparação
//------------------------------------------------------------------------------
const char* obterNomeAtributo(int atributo);
double obterValorAtributo(int atributo, int carta, unsigned long populacoes[], 
                         float areas[], float pibs[], int pontosTuristicos[], 
                         double densidades[], double pibsPerCapita[]);
int compararAtributos(int atributo, double valor1, double valor2);

//==============================================================================
// FUNÇÃO PRINCIPAL
//==============================================================================
int main() {
    //--------------------------------------------------------------------------
    // Declaração de variáveis para armazenamento dos dados das cartas
    //--------------------------------------------------------------------------
    char          nomesCidades[TOTAL_CARTAS][TAM_NOME_CIDADE]; // A01, A02, B01, etc.
    unsigned long populacoes[TOTAL_CARTAS];                    // População em habitantes
    float         areas[TOTAL_CARTAS];                         // Área em km²
    float         pibs[TOTAL_CARTAS];                          // PIB em bilhões
    int           pontosTuristicos[TOTAL_CARTAS];              // Número de pontos turísticos
    
    //--------------------------------------------------------------------------
    // Variáveis para propriedades calculadas automaticamente
    //--------------------------------------------------------------------------
    double densidadesPopulacionais[TOTAL_CARTAS];              // Densidade: hab/km²
    double pibsPerCapita[TOTAL_CARTAS];                        // PIB per capita em bilhões/hab
    float  superPoderes[TOTAL_CARTAS];                         // Super Poder (soma de atributos)

    //--------------------------------------------------------------------------
    // Apresentação inicial do sistema
    //--------------------------------------------------------------------------
    printf("======================================================================================================\n");
    printf("================================= BEM-VINDO AO SUPER TRUNFO - PAÍSES =================================\n");
    printf("======================================================================================================\n");
    printf("Sistema avançado com 32 cartas e múltiplas modalidades de comparação!\n");
    printf("• Um total de %d cartas pré-preenchidas automaticamente;\n", CARTAS_PREENCHIDAS);
    printf("• Apenas %d cartas para você cadastrar;\n", CARTAS_USUARIO);
    printf("• Modalidades de comparação: Super Poder e Atributos Isolados\n");
    
    //--------------------------------------------------------------------------
    // Preenchimento automático das primeiras 30 cartas
    //--------------------------------------------------------------------------
    preencherCartasAutomaticamente(nomesCidades, populacoes, areas, pibs, pontosTuristicos);

    //--------------------------------------------------------------------------
    // Geração dos nomes padronizados para todas as cidades
    //--------------------------------------------------------------------------
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        int estado = i / CIDADES_POR_ESTADO;
        int cidade = (i % CIDADES_POR_ESTADO) + 1;
        char letraEstado = 'A' + estado;
        
        gerarNomeCidade(nomesCidades[i], letraEstado, cidade);
    }

    //--------------------------------------------------------------------------
    // Entrada de dados para as 2 últimas cartas (usuário)
    //--------------------------------------------------------------------------
    for (int i = CARTAS_PREENCHIDAS; i < TOTAL_CARTAS; i++) {
        int estado = i / CIDADES_POR_ESTADO;
        char letraEstado = 'A' + estado;
        
        printf("\n--- Cadastrando sua carta %s (Estado: %c) ---\n", 
               nomesCidades[i], letraEstado);

        // Lê apenas dados numéricos (nome já está predefinido)
        populacoes[i] = lerPopulacao();
        areas[i] = lerArea();
        pibs[i] = lerPIB();
        pontosTuristicos[i] = lerPontosTuristicos();

        printf("Carta %s cadastrada com sucesso no estado %c!\n", 
               nomesCidades[i], letraEstado);
    }

    //--------------------------------------------------------------------------
    // Cálculo das propriedades derivadas para todas as cartas
    //--------------------------------------------------------------------------
    printf("\nCalculando propriedades das cartas...\n"); // Barra de progresso
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        // Densidade Populacional (hab/km²)
        densidadesPopulacionais[i] = calcularDensidadePopulacional(populacoes[i], areas[i]);
        
        // PIB per Capita (bilhões/habitante)
        pibsPerCapita[i] = calcularPIBPerCapita(pibs[i], populacoes[i]);
        
        // Super Poder (soma de todos os atributos)
        superPoderes[i] = calcularSuperPoder(populacoes[i], areas[i], pibs[i],
                                           pontosTuristicos[i], densidadesPopulacionais[i],
                                           pibsPerCapita[i]);
    }
    printf("Cálculos concluídos!\n"); // tempo transcorrido

    //--------------------------------------------------------------------------
    // Exibição dos dados organizados por estado em formato tabular
    //--------------------------------------------------------------------------
    printf("\n=====================================================================================\n");
    printf("                          TODAS AS CARTAS - FORMATO TABULAR\n");
    printf("=======================================================================================\n");
    
    // Exibe tabela para cada estado
    for (int estado = 0; estado < NUM_ESTADOS; estado++) {
        char letraEstado = 'A' + estado;
        int inicioIndice = estado * CIDADES_POR_ESTADO;
        
        exibirTabelaEstado(letraEstado, inicioIndice, nomesCidades,
                          populacoes, areas, pibs, pontosTuristicos,
                          densidadesPopulacionais, pibsPerCapita, superPoderes);
    }

    //--------------------------------------------------------------------------
    // Sistema de escolha de modalidade de comparação
    //--------------------------------------------------------------------------
    printf("\n=====================================================================================================\n");
    printf("                                    SISTEMA DE COMPARAÇÃO\n");
    printf("=====================================================================================================\n");
    
    // Obtém modalidade de jogo
    exibirMenuModalidades();
    int modalidade = obterModalidadeJogo();
    
    // Se modalidade for atributo isolado, obtém atributo específico
    int atributo = 0;
    if (modalidade == MODALIDADE_ATRIBUTO_ISOLADO) {
        exibirMenuAtributos();
        atributo = obterAtributoComparacao();
    }

    //--------------------------------------------------------------------------
    // Seleção das cartas para comparação
    //--------------------------------------------------------------------------
    printf("\nEscolha duas cartas para comparar:\n\n");
    exibirIndiceCartas(nomesCidades);
    
    // Obtém as escolhas do usuário (garantindo cartas diferentes)
    printf("\nEscolha a primeira carta (1-%d): ", TOTAL_CARTAS);
    int carta1 = obterEscolhaUsuario(TOTAL_CARTAS) - 1; // Converte para índice 0-31

    printf("Escolha a segunda carta (1-%d, diferente da carta %d): ", 
           TOTAL_CARTAS, carta1 + 1);
    int carta2 = obterEscolhaUsuarioExcluindo(TOTAL_CARTAS, carta1 + 1) - 1;

    //--------------------------------------------------------------------------
    // Execução da comparação conforme modalidade escolhida
    //--------------------------------------------------------------------------
    printf("\n=====================================================================================\n");
    printf("                                RESULTADO DA COMPARAÇÃO\n");
    printf("=====================================================================================\n");
    
    executarComparacao(modalidade, atributo, carta1, carta2, nomesCidades,
                      populacoes, areas, pibs, pontosTuristicos,
                      densidadesPopulacionais, pibsPerCapita, superPoderes);

    //--------------------------------------------------------------------------
    // Exibição de tabela resumo com todos os atributos
    //--------------------------------------------------------------------------
    printf("\n================================================================================\n");
    printf("                           TABELA RESUMO DE COMPARAÇÃO\n");
    printf("================================================================================\n");
    
    exibirResultadoTabular(carta1, carta2, nomesCidades, populacoes, areas, pibs,
                          pontosTuristicos, densidadesPopulacionais, pibsPerCapita, superPoderes);

    //--------------------------------------------------------------------------
    // Finalização do programa
    //--------------------------------------------------------------------------
    printf("\n===========================================================\n");
    printf("Sistema Super Trunfo concluído!\n");
    printf("Total: %d cartas analisadas e comparadas\n", TOTAL_CARTAS);
    printf("Estados: A, B, C, D, E, F, G, H (8 estados)\n");
    printf("Cidades: ##01, ##02, ##03, ##04 (4 por estado)\n");
    printf("Propriedades calculadas: Densidade, PIB per Capita e Super Poder\n");
    printf("Obrigado por usar o Super Trunfo!\n");
    return 0;
}

//==============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
//==============================================================================

//------------------------------------------------------------------------------
// Função: gerarNomeCidade
// Objetivo: Gera nome padronizado da cidade no formato A01, B02, etc.
//------------------------------------------------------------------------------
void gerarNomeCidade(char nome[], char estado, int cidade) {
    sprintf(nome, "%c%02d", estado, cidade);
}

//------------------------------------------------------------------------------
// Função: lerPopulacao  
// Objetivo: Lê população garantindo valor válido (unsigned long)
//------------------------------------------------------------------------------
unsigned long lerPopulacao() {
    unsigned long populacao;
    
    do {
        printf("Digite a populacao: ");
        
        if (scanf("%lu", &populacao) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            populacao = 0; // Força o loop a continuar
        }
        else if (populacao == 0) {
            printf("ERRO: A populacao deve ser maior que zero!\n");
        }
    } while (populacao == 0);
    
    return populacao;
}

//------------------------------------------------------------------------------
// Função: lerArea
// Objetivo: Lê área garantindo valor positivo maior que zero
//------------------------------------------------------------------------------
float lerArea() {
    float area;
    
    do {
        printf("Digite a area (km²): ");
        
        if (scanf("%f", &area) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            area = -1;
        }
        else if (area <= 0) {
            printf("ERRO: A area deve ser maior que zero!\n");
        }
    } while (area <= 0);
    
    return area;
}

//------------------------------------------------------------------------------
// Função: lerPIB
// Objetivo: Lê PIB garantindo valor não negativo
//------------------------------------------------------------------------------
float lerPIB() {
    float pib;
    
    do {
        printf("Digite o PIB (em bilhoes): ");
        
        if (scanf("%f", &pib) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            pib = -1;
        }
        else if (pib < 0) {
            printf("ERRO: O PIB deve ser um numero positivo!\n");
        }
    } while (pib < 0);
    
    return pib;
}

//------------------------------------------------------------------------------
// Função: lerPontosTuristicos
// Objetivo: Lê número de pontos turísticos garantindo valor não negativo
//------------------------------------------------------------------------------
int lerPontosTuristicos() {
    int pontos;
    
    do {
        printf("Digite o numero de pontos turisticos: ");
        
        if (scanf("%d", &pontos) != 1) {
            printf("ERRO: Digite apenas numeros inteiros!\n");
            limparBuffer();
            pontos = -1;
        }
        else if (pontos < 0) {
            printf("ERRO: O numero de pontos turisticos deve ser positivo!\n");
        }
    } while (pontos < 0);
    
    return pontos;
}

//------------------------------------------------------------------------------
// Função: limparBuffer
// Objetivo: Remove caracteres inválidos do buffer de entrada
//------------------------------------------------------------------------------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//------------------------------------------------------------------------------
// Função: calcularDensidadePopulacional
// Objetivo: Calcula densidade populacional (habitantes por km²)
//------------------------------------------------------------------------------
double calcularDensidadePopulacional(unsigned long populacao, float area) {
    if (area <= 0) {
        printf("AVISO: Area invalida para calculo de densidade!\n");
        return 0.0;
    }
    
    // Conversão explícita para double para maior precisão
    return (double)populacao / (double)area;
}

//------------------------------------------------------------------------------
// Função: calcularPIBPerCapita
// Objetivo: Calcula PIB per capita (bilhões por habitante)
//------------------------------------------------------------------------------
double calcularPIBPerCapita(float pib, unsigned long populacao) {
    if (populacao == 0) {
        printf("AVISO: Populacao invalida para calculo de PIB per capita!\n");
        return 0.0;
    }
    
    // Conversão explícita para double para maior precisão
    return (double)pib / (double)populacao;
}

//------------------------------------------------------------------------------
// Função: preencherCartasAutomaticamente
// Objetivo: Preenche as primeiras 30 cartas com dados predefinidos
//------------------------------------------------------------------------------
void preencherCartasAutomaticamente(char nomesCidades[][TAM_NOME_CIDADE], 
                                   unsigned long populacoes[], float areas[],
                                   float pibs[], int pontosTuristicos[]) {
    
    // Dados de população (em habitantes)
    unsigned long pops[30] = {
        12325232, 6748000, 2886698, 2669342, 2521564, 2219580, 1948626, 1653461,
        3055149, 1536097, 1499641, 1488252, 1392121, 1213792, 1108975, 1084839,
        1025360, 924624, 906092, 890480, 868075, 844483, 817208, 817511,
        721368, 697886, 695956, 668949, 695737, 699097
    };
    
    // Dados de área (em km²)
    float ars[30] = {
        1521.11, 1200.27, 692.82, 314.93, 331.40, 11401.09, 434.89, 218.44,
        5760.78, 739.49, 1059.46, 496.68, 318.68, 794.57, 834.78, 249.14,
        510.65, 467.62, 8096.05, 167.26, 1391.98, 408.45, 521.24, 211.47,
        175.78, 64.95, 258.69, 195.27, 650.95, 4115.82
    };
    
    // Dados de PIB (em bilhões)
    float pibs_dados[30] = {
        714.68, 254.51, 60.73, 58.26, 93.68, 75.88, 102.65, 66.27,
        259.49, 49.15, 36.25, 93.77, 55.45, 59.02, 19.31, 14.98,
        16.46, 15.21, 28.48, 18.28, 13.73, 46.27, 13.04, 16.78,
        56.90, 55.46, 12.34, 11.87, 18.45, 26.51
    };
    
    // Dados de pontos turísticos
    int pontos[30] = {
        156, 189, 128, 89, 76, 94, 67, 82, 145, 54,
        78, 98, 23, 45, 67, 34, 89, 28, 112, 76,
        58, 67, 31, 87, 45, 32, 54, 43, 78, 89
    };
    
    // Preenche os arrays com os dados predefinidos
    for (int i = 0; i < CARTAS_PREENCHIDAS; i++) {
        populacoes[i] = pops[i];
        areas[i] = ars[i];
        pibs[i] = pibs_dados[i];
        pontosTuristicos[i] = pontos[i];
    }
}

//------------------------------------------------------------------------------
// Função: calcularSuperPoder
// Objetivo: Calcula Super Poder como soma de todos os atributos
//------------------------------------------------------------------------------
float calcularSuperPoder(unsigned long populacao, float area, float pib,
                        int pontosTuristicos, double densidade, double pibPerCapita) {
    
    // Soma com conversões explícitas para evitar erros de tipo
    float superPoder = (float)populacao + area + pib + (float)pontosTuristicos + 
                      (float)densidade + (float)pibPerCapita;
    
    return superPoder;
}

//------------------------------------------------------------------------------
// Função: exibirMenuModalidades
// Objetivo: Exibe as opções de modalidades de jogo disponíveis
//------------------------------------------------------------------------------
void exibirMenuModalidades() {
    printf("\n==================== MODALIDADES DE COMPARAÇÃO ====================\n");
    printf("1. Comparação por Super Poder (soma de todos os atributos)\n");
    printf("2. Comparação por Atributo Isolado (escolha específica)\n");
    printf("===============================================\n");
}

//------------------------------------------------------------------------------
// Função: exibirMenuAtributos
// Objetivo: Exibe as opções de atributos para comparação isolada
//------------------------------------------------------------------------------
void exibirMenuAtributos() {
    printf("\n========== ATRIBUTOS DISPONÍVEIS PARA COMPARAÇÃO ==========\n");
    printf("1. População (habitantes)\n");
    printf("2. Área (km²)\n");
    printf("3. PIB (bilhões)\n");
    printf("4. Pontos Turísticos\n");
    printf("5. Densidade Populacional (hab/km²) - MENOR vence\n");
    printf("6. PIB per Capita (bilhões/habitante)\n");
    printf("===========================================================\n");
}

//------------------------------------------------------------------------------
// Função: obterModalidadeJogo
// Objetivo: Obtém e valida a escolha da modalidade pelo usuário
//------------------------------------------------------------------------------
int obterModalidadeJogo() {
    int modalidade;
    
    do {
        printf("Escolha a modalidade de comparação (1 ou 2): ");
        
        if (scanf("%d", &modalidade) != 1) {
            printf("ERRO: Digite apenas números!\n");
            limparBuffer();
            modalidade = 0;
        }
        else if (modalidade < 1 || modalidade > 2) {
            printf("ERRO: Digite 1 para Super Poder ou 2 para Atributo Isolado!\n");
        }
    } while (modalidade < 1 || modalidade > 2);
    
    return modalidade;
}

//------------------------------------------------------------------------------
// Função: obterAtributoComparacao
// Objetivo: Obtém e valida a escolha do atributo específico pelo usuário
//------------------------------------------------------------------------------
int obterAtributoComparacao() {
    int atributo;
    
    do {
        printf("Escolha o atributo para comparação (1 a %d): ", NUM_ATRIBUTOS);
        
        if (scanf("%d", &atributo) != 1) {
            printf("ERRO: Digite apenas números!\n");
            limparBuffer();
            atributo = 0;
        }
        else if (atributo < 1 || atributo > NUM_ATRIBUTOS) {
            printf("ERRO: Digite um número entre 1 e %d!\n", NUM_ATRIBUTOS);
        }
    } while (atributo < 1 || atributo > NUM_ATRIBUTOS);
    
    return atributo;
}

//------------------------------------------------------------------------------
// Função: executarComparacao
// Objetivo: Executa comparação conforme modalidade escolhida
//------------------------------------------------------------------------------
void executarComparacao(int modalidade, int atributo, int carta1, int carta2,
                       char nomesCidades[][TAM_NOME_CIDADE],
                       unsigned long populacoes[], float areas[], float pibs[],
                       int pontosTuristicos[], double densidades[], 
                       double pibsPerCapita[], float superPoderes[]) {
    
    printf("Comparando: %s vs %s\n\n", nomesCidades[carta1], nomesCidades[carta2]);
    
    if (modalidade == MODALIDADE_SUPER_PODER) {
        compararSuperPoder(carta1, carta2, nomesCidades, superPoderes);
    }
    else if (modalidade == MODALIDADE_ATRIBUTO_ISOLADO) {
        compararAtributoIsolado(atributo, carta1, carta2, nomesCidades,
                               populacoes, areas, pibs, pontosTuristicos,
                               densidades, pibsPerCapita);
    }
}

//------------------------------------------------------------------------------
// Função: compararSuperPoder
// Objetivo: Compara duas cartas baseado no Super Poder
//------------------------------------------------------------------------------
void compararSuperPoder(int carta1, int carta2, char nomesCidades[][TAM_NOME_CIDADE],
                       float superPoderes[]) {
    
    printf("=== COMPARAÇÃO POR SUPER PODER ===\n");
    printf("Carta 1 (%s): %.2f\n", nomesCidades[carta1], superPoderes[carta1]);
    printf("Carta 2 (%s): %.2f\n", nomesCidades[carta2], superPoderes[carta2]);
    
    if (superPoderes[carta1] > superPoderes[carta2]) {
        printf("\n🏆 VENCEDOR: Carta 1 (%s) com Super Poder de %.2f!\n",
               nomesCidades[carta1], superPoderes[carta1]);
        printf("Diferença: %.2f pontos\n", superPoderes[carta1] - superPoderes[carta2]);
    }
    else if (superPoderes[carta2] > superPoderes[carta1]) {
        printf("\n🏆 VENCEDOR: Carta 2 (%s) com Super Poder de %.2f!\n",
               nomesCidades[carta2], superPoderes[carta2]);
        printf("Diferença: %.2f pontos\n", superPoderes[carta2] - superPoderes[carta1]);
    }
    else {
        printf("\n🤝 EMPATE! Ambas as cartas têm Super Poder de %.2f!\n",
               superPoderes[carta1]);
    }
}

//------------------------------------------------------------------------------
// Função: compararAtributoIsolado
// Objetivo: Compara duas cartas baseado em um atributo específico
//------------------------------------------------------------------------------
void compararAtributoIsolado(int atributo, int carta1, int carta2,
                            char nomesCidades[][TAM_NOME_CIDADE],
                            unsigned long populacoes[], float areas[], float pibs[],
                            int pontosTuristicos[], double densidades[], 
                            double pibsPerCapita[]) {
    
    const char* nomeAtributo = obterNomeAtributo(atributo);
    double valor1 = obterValorAtributo(atributo, carta1, populacoes, areas, pibs,
                                      pontosTuristicos, densidades, pibsPerCapita);
    double valor2 = obterValorAtributo(atributo, carta2, populacoes, areas, pibs,
                                      pontosTuristicos, densidades, pibsPerCapita);
    
    printf("=== COMPARAÇÃO POR %s ===\n", nomeAtributo);
    printf("Carta 1 (%s): %.2f\n", nomesCidades[carta1], valor1);
    printf("Carta 2 (%s): %.2f\n", nomesCidades[carta2], valor2);
    
    int resultado = compararAtributos(atributo, valor1, valor2);
    
    if (resultado == 1) {
        printf("\n🏆 VENCEDOR: Carta 1 (%s)!\n", nomesCidades[carta1]);
        printf("Critério: %s ", nomeAtributo);
        if (atributo == ATRIBUTO_DENSIDADE) {
            printf("(menor valor vence)\n");
        } else {
            printf("(maior valor vence)\n");
        }
    }
    else if (resultado == 2) {
        printf("\n🏆 VENCEDOR: Carta 2 (%s)!\n", nomesCidades[carta2]);
        printf("Critério: %s ", nomeAtributo);
        if (atributo == ATRIBUTO_DENSIDADE) {
            printf("(menor valor vence)\n");
        } else {
            printf("(maior valor vence)\n");
        }
    }
    else {
        printf("\n🤝 EMPATE! Ambas as cartas têm o mesmo valor!\n");
    }
}

//------------------------------------------------------------------------------
// Função: exibirResultadoTabular
// Objetivo: Exibe resultado em formato de tabela com 1s e 0s
//------------------------------------------------------------------------------
void exibirResultadoTabular(int carta1, int carta2, char nomesCidades[][TAM_NOME_CIDADE],
                           unsigned long populacoes[], float areas[], float pibs[],
                           int pontosTuristicos[], double densidades[], 
                           double pibsPerCapita[], float superPoderes[]) {
    
    printf("%-8s %-10s %-8s %-8s %-8s %-8s %-12s %-12s\n",
           "Carta", "Pop.", "Área", "PIB", "Pontos", "Densidade", "PIB_pc", "SuperPoder");
    
    printf("%-8s %-10s %-8s %-8s %-8s %-8s %-12s %-12s\n",
           "--------", "----------", "--------", "--------", "--------", 
           "--------", "------------", "------------");
    
    // Carta 1
    int vPop1 = (populacoes[carta1] > populacoes[carta2]) ? 1 : 0;
    int vArea1 = (areas[carta1] > areas[carta2]) ? 1 : 0;
    int vPib1 = (pibs[carta1] > pibs[carta2]) ? 1 : 0;
    int vPontos1 = (pontosTuristicos[carta1] > pontosTuristicos[carta2]) ? 1 : 0;
    int vDens1 = (densidades[carta1] < densidades[carta2]) ? 1 : 0; // Menor vence
    int vPibPc1 = (pibsPerCapita[carta1] > pibsPerCapita[carta2]) ? 1 : 0;
    int vSuper1 = (superPoderes[carta1] > superPoderes[carta2]) ? 1 : 0;
    
    printf("%-8s %-10d %-8d %-8d %-8d %-8d %-12d %-12d\n",
           nomesCidades[carta1], vPop1, vArea1, vPib1, vPontos1, vDens1, vPibPc1, vSuper1);
    
    // Carta 2
    printf("%-8s %-10d %-8d %-8d %-8d %-8d %-12d %-12d\n",
           nomesCidades[carta2], 1-vPop1, 1-vArea1, 1-vPib1, 1-vPontos1, 1-vDens1, 1-vPibPc1, 1-vSuper1);
    
    // Total de vitórias
    int totalVitorias1 = vPop1 + vArea1 + vPib1 + vPontos1 + vDens1 + vPibPc1 + vSuper1;
    printf("\nTotal de vitórias:\n");
    printf("Carta 1 (%s): %d/7\n", nomesCidades[carta1], totalVitorias1);
    printf("Carta 2 (%s): %d/7\n", nomesCidades[carta2], 7 - totalVitorias1);
}

//------------------------------------------------------------------------------
// Função: obterNomeAtributo
// Objetivo: Retorna o nome do atributo conforme código
//------------------------------------------------------------------------------
const char* obterNomeAtributo(int atributo) {
    switch (atributo) {
        case ATRIBUTO_POPULACAO: return "POPULAÇÃO";
        case ATRIBUTO_AREA: return "ÁREA";
        case ATRIBUTO_PIB: return "PIB";
        case ATRIBUTO_PONTOS_TURISTICOS: return "PONTOS TURÍSTICOS";
        case ATRIBUTO_DENSIDADE: return "DENSIDADE POPULACIONAL";
        case ATRIBUTO_PIB_PER_CAPITA: return "PIB PER CAPITA";
        default: return "ATRIBUTO DESCONHECIDO";
    }
}

//------------------------------------------------------------------------------
// Função: obterValorAtributo
// Objetivo: Retorna o valor do atributo específico de uma carta
//------------------------------------------------------------------------------
double obterValorAtributo(int atributo, int carta, unsigned long populacoes[], 
                         float areas[], float pibs[], int pontosTuristicos[], 
                         double densidades[], double pibsPerCapita[]) {
    switch (atributo) {
        case ATRIBUTO_POPULACAO: return (double)populacoes[carta];
        case ATRIBUTO_AREA: return (double)areas[carta];
        case ATRIBUTO_PIB: return (double)pibs[carta];
        case ATRIBUTO_PONTOS_TURISTICOS: return (double)pontosTuristicos[carta];
        case ATRIBUTO_DENSIDADE: return densidades[carta];
        case ATRIBUTO_PIB_PER_CAPITA: return pibsPerCapita[carta];
        default: return 0.0;
    }
}

//------------------------------------------------------------------------------
// Função: compararAtributos
// Objetivo: Compara valores de atributos conforme regras específicas
//------------------------------------------------------------------------------
int compararAtributos(int atributo, double valor1, double valor2) {
    if (atributo == ATRIBUTO_DENSIDADE) {
        // Para densidade, menor valor vence
        if (valor1 < valor2) return 1;
        else if (valor2 < valor1) return 2;
        else return 0; // Empate
    }
    else {
        // Para outros atributos, maior valor vence
        if (valor1 > valor2) return 1;
        else if (valor2 > valor1) return 2;
        else return 0; // Empate
    }
}

//------------------------------------------------------------------------------
// Funções mantidas do código original (com pequenas adaptações)
//------------------------------------------------------------------------------

void exibirIndiceCartas(char nomesCidades[][TAM_NOME_CIDADE]) {
    // Cabeçalho organizado em 8 colunas
    printf("%-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s\n", 
           "Nº", "Carta", "Nº", "Carta", "Nº", "Carta", "Nº", "Carta", 
           "Nº", "Carta", "Nº", "Carta", "Nº", "Carta", "Nº", "Carta");
           
    printf("%-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s\n",
           "--", "----", "--", "----", "--", "----", "--", "----",
           "--", "----", "--", "----", "--", "----", "--", "----");
    
    // Exibe dados em 8 colunas com formatação consistente
    for (int i = 0; i < TOTAL_CARTAS; i += 8) {
        for (int j = 0; j < 8 && (i + j) < TOTAL_CARTAS; j++) {
            printf("%-3d %-4s", i + j + 1, nomesCidades[i + j]);
            if (j < 7 && (i + j + 1) < TOTAL_CARTAS) {
                printf("    "); // Separação entre colunas
            }
        }
        printf("\n");
    }
}

int obterEscolhaUsuario(int maxCartas) {
    int escolha;
    
    do {
        if (scanf("%d", &escolha) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            escolha = 0; // Valor inválido para continuar loop
        }
        else if (escolha < 1 || escolha > maxCartas) {
            printf("ERRO: Digite um numero entre 1 e %d!\n", maxCartas);
        }
    } while (escolha < 1 || escolha > maxCartas);
    
    return escolha;
}

int obterEscolhaUsuarioExcluindo(int maxCartas, int cartaExcluida) {
    int escolha;
    
    do {
        if (scanf("%d", &escolha) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            escolha = 0; // Valor inválido para continuar loop
        }
        else if (escolha < 1 || escolha > maxCartas) {
            printf("ERRO: Digite um numero entre 1 e %d!\n", maxCartas);
        }
        else if (escolha == cartaExcluida) {
            printf("ERRO: Você já escolheu a carta %d! Escolha uma carta diferente.\n", cartaExcluida);
        }
    } while (escolha < 1 || escolha > maxCartas || escolha == cartaExcluida);
    
    return escolha;
}

void exibirTabelaEstado(char estado, int inicioIndice, 
                       char nomesCidades[][TAM_NOME_CIDADE], 
                       unsigned long populacoes[], float areas[], float pibs[], 
                       int pontosTuristicos[], double densidades[], 
                       double pibsPerCapita[], float superPoderes[]) {
    
    printf("\n============================== ESTADO %c - TABELA RESUMO ==============================\n", estado);
    printf("\n———————————————————————————————————————————————————————————————————————————————————————\n", estado);
    
    // Cabeçalho da tabela
    printf("%-5s %-12s %-10s %-12s %-8s %-12s %-12s %-12s\n",
           "Carta", "Pop.(hab)", "Area(km²)", "PIB(bi)", "Pontos",
           "Densidade", "PIB pc(R$)", "SuperPoder");
    
    printf("%-5s %-12s %-10s %-12s %-8s %-12s %-12s %-12s\n",
           "-----", "------------", "----------", "------------", "--------", 
           "------------", "------------", "------------");

    // Dados das 4 cidades do estado
    for (int i = 0; i < CIDADES_POR_ESTADO; i++) {
        int indice = inicioIndice + i;
        
        printf("%-5s %-12lu %-10.2f %-12.2f %-8d %-12.2f %-12.2e %-12.2f\n",
               nomesCidades[indice],               // Código da carta (A01, A02, etc.)
               populacoes[indice],                 // População (unsigned long)
               areas[indice],                      // Área em km²
               pibs[indice],                       // PIB em bilhões
               pontosTuristicos[indice],           // Pontos turísticos
               densidades[indice],                 // Densidade populacional
               pibsPerCapita[indice] * 1e9,        // PIB per capita (convertido para reais)
               superPoderes[indice]                // Super Poder
        );
    }
    
    printf("\n"); // Linha em branco para separar as tabelas
}