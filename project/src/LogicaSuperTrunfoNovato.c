// Desafio Super Trunfo - Países
// Tema 03 - Nível Mestre
// Objetivo: Expandir o sistema do nível Aventureiro para incluir comparações
// entre cartas, cálculo de Super Poder e manipulação de grandes números.
// NOVIDADE: 30 cartas pré-preenchidas + 2 pelo usuário + sistema de comparação
// MODIFICAÇÕES: Estados A-H, cidades padronizadas como A01, B02, etc.
// NOTA: Aproveitando dos recursos de nossa era, este programa foi escrito com
// assistência de um LLM (Claude Sonnet 4), contudo, não foi apenas um <copy-
// paste>, mas sim uma colaboração entre humano e IA para garantir qualidade.

#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Constantes ——————————————————————————————————————————————————————————————————
// É assumido que trata-se de apenas um país com 8 estados (A a H) e 4 cidades
// cada um. As cidades têm nomes padronizados como A01, A02, B01, B02, etc.
#define NUM_ESTADOS 8               // 8 estados de A a H ——————————————————————
#define CIDADES_POR_ESTADO 4        // 4 cidades por estado ————————————————————
#define TOTAL_CARTAS (NUM_ESTADOS * CIDADES_POR_ESTADO)  // Total: 32 cartas ———
#define CARTAS_PREENCHIDAS 30       // 30 cartas pré-preenchidas automaticamente 
#define CARTAS_USUARIO 2            // 2 cartas preenchidas pelo usuário ———————
#define TAM_NOME_CIDADE 4           // Tamanho para A01, B02, etc. (3 chars + \0)

// Protótipos/declaração das funções ————[corpo de cada função após o main()]———
unsigned long  lerPopulacao(); // Lê população como unsigned long int ——————————
float          lerArea(); // Garante que área seja positiva maior que zero —————
float          lerPIB(); // Garante que PIB seja um número positivo ————————————
int            lerPontosTuristicos(); // Garante pontos turísticos positivos ———
void           limparBuffer(); // Limpa buffer do teclado ——————————————————————
void           gerarNomeCidade(char nome[], char estado, int cidade); // A01, B02...
// FUNÇÕES DO NÍVEL AVENTUREIRO (MANTIDAS) —————————————————————————————————————
double calcularDensidadePopulacional(unsigned long populacao, float area);
double calcularPIBPerCapita(float pib, unsigned long populacao);
// NOVAS FUNÇÕES DO NÍVEL MESTRE ———————————————————————————————————————————————
void   preencherCartasAutomaticamente(  char nomesCidades[][TAM_NOME_CIDADE],
                                        unsigned long populacoes[],
                                        float areas[],
                                        float pibs[],
                                        int pontosTuristicos[]); // Preenche 30 cartas
float  calcularSuperPoder(  unsigned long populacao,
                            float area,
                            float pib,
                            int pontosTuristicos,
                            double densidade,
                            double pibPerCapita); // Calcula Super Poder ———————
void   exibirIndiceCartas(  char nomesCidades[][TAM_NOME_CIDADE]); // Para comparação
int    obterEscolhaUsuario( int maxCartas); // Primeira carta (qualquer) ———————
int    obterEscolhaUsuarioExcluindo(int maxCartas, int cartaExcluida); // Segunda carta
void   compararCartas(      int carta1,
                            int carta2,
                            char nomesCidades[][TAM_NOME_CIDADE],
                            unsigned long populacoes[],
                            float areas[],
                            float pibs[],
                            int pontosTuristicos[],
                            double densidades[], 
                            double pibsPerCapita[],
                            float superPoderes[]); // Comparação de 2 cartas ———
void   exibirTabelaEstado(  char estado,
                            int inicioIndice, 
                            char nomesCidades[][TAM_NOME_CIDADE], 
                            unsigned long populacoes[],
                            float areas[],
                            float pibs[], 
                            int pontosTuristicos[],
                            double densidades[], 
                            double pibsPerCapita[], 
                            float superPoderes[]); // Tabela de exibição ———————

// Função principal ————————————————————————————————————————————————————————————
int main() {
    // Área para definição das variáveis =======================================
    // Variáveis para armazenar os dados das cartas (ADAPTADAS PARA NÍVEL MESTRE)
    char           nomesCidades[TOTAL_CARTAS][TAM_NOME_CIDADE]; // A01, A02, B01, etc.
    unsigned long  populacoes[TOTAL_CARTAS]; // População (unsigned long) ———————
    float          areas[TOTAL_CARTAS]; // Área em km² de cada cidade ————————————
    float          pibs[TOTAL_CARTAS]; // PIB em bilhões de cada cidade ——————————
    int            pontosTuristicos[TOTAL_CARTAS]; // Número de pontos turísticos
    
    // VARIÁVEIS CALCULADAS (NÍVEL AVENTUREIRO + MESTRE) ———————————————————————
    double densidadesPopulacionais[TOTAL_CARTAS]; // Densidade: hab/km² ————————
    double pibsPerCapita[TOTAL_CARTAS]; // PIB per capita em bilhões/hab ———————
    float  superPoderes[TOTAL_CARTAS]; // NOVA: Super Poder de cada carta ————————

    // Área para entrada de dados ============================================== 
    printf("============ BEM-VINDO AO SUPER TRUNFO - PAÍSES ============\n");
    printf("Sistema avançado com 32 cartas e comparação entre cartas!\n");
    printf("- %d cartas pré-preenchidas automaticamente;\n", CARTAS_PREENCHIDAS);
    printf("- %d cartas para você cadastrar;\n", CARTAS_USUARIO);
    printf("- Cálculo automático de propriedades e Super Poder;\n");
    printf("- Sistema de comparação entre cartas;\n");
    printf("- Estados organizados de A a H com cidades numeradas;\n\n");

    // Pré-preenche 30 cartas automaticamente ————————————————————————————————
    preencherCartasAutomaticamente(nomesCidades, populacoes, areas, pibs, pontosTuristicos);

    // Gera nomes para TODAS as cidades (incluindo as que o usuário preencherá) ——
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        int estado = i / CIDADES_POR_ESTADO;
        int cidade = (i % CIDADES_POR_ESTADO) + 1;
        char letraEstado = 'A' + estado;
        
        gerarNomeCidade(nomesCidades[i], letraEstado, cidade);
    }

    // Loop para as 2 últimas cartas (índices 30 e 31) - APENAS DADOS NUMÉRICOS ——
    for (int i = CARTAS_PREENCHIDAS; i < TOTAL_CARTAS; i++) {
        int estado = i / CIDADES_POR_ESTADO;
        char letraEstado = 'A' + estado;
        
        printf("\n--- Cadastrando sua carta %s (Estado: %c) ---\n", 
               nomesCidades[i], letraEstado);

        // Lê APENAS dados numéricos (nome já está predefinido) ————————————————
        populacoes[i] = lerPopulacao();
        areas[i] = lerArea();
        pibs[i] = lerPIB();
        pontosTuristicos[i] = lerPontosTuristicos();

        printf("Carta %s cadastrada com sucesso no estado %c!\n", nomesCidades[i], letraEstado);
    }

    // Calcula propriedades derivadas para TODAS as cartas ————————————————————
    printf("\nCalculando propriedades derivadas e Super Poder...\n");
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        // Densidade Populacional ——————————————————————————————————————————————
        densidadesPopulacionais[i] = calcularDensidadePopulacional(populacoes[i], areas[i]);
        
        // PIB per Capita ——————————————————————————————————————————————————————
        pibsPerCapita[i] = calcularPIBPerCapita(pibs[i], populacoes[i]);
        
        // NOVA: Super Poder ———————————————————————————————————————————————————
        superPoderes[i] = calcularSuperPoder(populacoes[i], areas[i], pibs[i],
                                            pontosTuristicos[i], densidadesPopulacionais[i],
                                            pibsPerCapita[i]);
    }
    printf("Cálculos concluídos!\n");

    // Área para exibição dos dados das cidades ================================
    printf("\n\n===========================================================\n");
    printf("              TODAS AS CARTAS - FORMATO TABULAR\n");
    printf("===========================================================\n");
    
    // Exibe tabela de cada estado —————————————————————————————————————————————
    for (int estado = 0; estado < NUM_ESTADOS; estado++) {
        char letraEstado = 'A' + estado;
        int inicioIndice = estado * CIDADES_POR_ESTADO;
        
        exibirTabelaEstado(letraEstado, inicioIndice, nomesCidades,
                          populacoes, areas, pibs, pontosTuristicos,
                          densidadesPopulacionais, pibsPerCapita, superPoderes);
    }

    // NOVA FUNCIONALIDADE: Sistema de comparação de cartas ———————————————————
    printf("\n===========================================================\n");
    printf("                    COMPARAÇÃO DE CARTAS\n");
    printf("===========================================================\n");
    
    // Exibe índice simplificado para escolha ——————————————————————————————————
    printf("Escolha duas cartas para comparar:\n\n");
    exibirIndiceCartas(nomesCidades);
    
    // Obtém escolhas do usuário (EVITA CARTAS IGUAIS) ————————————————————————
    printf("\nEscolha a primeira carta (1-%d): ", TOTAL_CARTAS);
    int carta1 = obterEscolhaUsuario(TOTAL_CARTAS) - 1; // Converte para índice 0-31

    printf("Escolha a segunda carta (1-%d, diferente da carta %d): ", TOTAL_CARTAS, carta1 + 1);
    int carta2 = obterEscolhaUsuarioExcluindo(TOTAL_CARTAS, carta1 + 1) - 1; // Exclui carta1
    
    // Realiza e exibe a comparação ————————————————————————————————————————————
    printf("\n===========================================================\n");
    printf("                 RESULTADO DA COMPARAÇÃO\n");
    printf("===========================================================\n");
    
    compararCartas(carta1, carta2, nomesCidades, populacoes, areas, pibs,
                  pontosTuristicos, densidadesPopulacionais, pibsPerCapita, superPoderes);

    // Finalização do programa —————————————————————————————————————————————————
    printf("\n===========================================================\n");
    printf("Sistema Super Trunfo!\n");
    printf("Total: %d cartas analisadas e comparadas\n", TOTAL_CARTAS);
    printf("Estados: A, B, C, D, E, F, G, H (8 estados)\n");
    printf("Cidades: ##01, ##02, ##03, ##04 (4 por estado)\n");
    printf("Propriedades calculadas: Densidade, PIB per Capita e Super Poder\n");
    printf("Obrigado por usar o Super Trunfo!\n");
    return 0;
}

// NOVA FUNÇÃO: Gera nome da cidade no formato A01, B02, etc. —————————————————
void gerarNomeCidade(char nome[], char estado, int cidade) {
    sprintf(nome, "%c%02d", estado, cidade);
}

// Função para ler população (ADAPTADA PARA UNSIGNED LONG) ————————————————————
unsigned long lerPopulacao() {
    unsigned long populacao;
    
    do {
        printf("Digite a populacao: ");
        
        // scanf com %lu para unsigned long ———————————————————————————————————
        if (scanf("%lu", &populacao) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            populacao = 0; // Força o loop a continuar (0 é inválido) ————————————
        }
        else if (populacao == 0) {
            printf("ERRO: A populacao deve ser maior que zero!\n");
        }
    } while (populacao == 0);
    
    return populacao;
}

// Função para ler área (SEM ALTERAÇÕES DO NÍVEL ANTERIOR) ————————————————————
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

// Função para ler PIB (SEM ALTERAÇÕES DO NÍVEL ANTERIOR) —————————————————————
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

// Função para ler pontos turísticos (SEM ALTERAÇÕES DO NÍVEL ANTERIOR) ———————
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

// Função para limpar buffer (SEM ALTERAÇÕES) —————————————————————————————————
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para calcular densidade (ADAPTADA PARA UNSIGNED LONG) ———————————————
double calcularDensidadePopulacional(unsigned long populacao, float area) {
    if (area <= 0) {
        printf("AVISO: Area invalida para calculo de densidade!\n");
        return 0.0;
    }
    
    // Conversão cuidadosa de tipos ————————————————————————————————————————————
    return (double)populacao / (double)area;
}

// Função para calcular PIB per capita (ADAPTADA PARA UNSIGNED LONG) ——————————
double calcularPIBPerCapita(float pib, unsigned long populacao) {
    if (populacao == 0) {
        printf("AVISO: Populacao invalida para calculo de PIB per capita!\n");
        return 0.0;
    }
    
    // Conversão cuidadosa de tipos ————————————————————————————————————————————
    return (double)pib / (double)populacao;
}

// NOVA FUNÇÃO: Preenche 30 cartas automaticamente (COM NOMES PADRONIZADOS) ———
void preencherCartasAutomaticamente(char nomesCidades[][TAM_NOME_CIDADE], 
                                   unsigned long populacoes[], float areas[],
                                   float pibs[], int pontosTuristicos[]) {
    
    // Dados de população (em habitantes) ——————————————————————————————————————
    unsigned long pops[30] = {
        12325232, 6748000, 2886698, 2669342,
        2521564, 2219580, 1948626, 1653461,
        3055149, 1536097, 1499641, 1488252,
        1392121, 1213792, 1108975, 1084839,
        1025360, 924624, 906092, 890480,
        868075, 844483, 817208, 817511,
        721368, 697886, 695956, 668949,
        695737, 699097
    };
    
    // Dados de área (em km²) ——————————————————————————————————————————————————
    float ars[30] = {
        1521.11, 1200.27, 692.82, 314.93,
        331.40, 11401.09, 434.89, 218.44,
        5760.78, 739.49, 1059.46, 496.68,
        318.68, 794.57, 834.78, 249.14,
        510.65, 467.62, 8096.05, 167.26,
        1391.98, 408.45, 521.24, 211.47,
        175.78, 64.95, 258.69, 195.27,
        650.95, 4115.82
    };
    
    // Dados de PIB (em bilhões) ———————————————————————————————————————————————
    float pibs_dados[30] = {
        714.68, 254.51, 60.73, 58.26,
        93.68, 75.88, 102.65, 66.27,
        259.49, 49.15, 36.25, 93.77,
        55.45, 59.02, 19.31, 14.98,
        16.46, 15.21, 28.48, 18.28,
        13.73, 46.27, 13.04, 16.78,
        56.90, 55.46, 12.34, 11.87,
        18.45, 26.51
    };
    
    // Dados de pontos turísticos ——————————————————————————————————————————————
    int pontos[30] = {
        156, 189, 128, 89, 76, 94, 67, 82,
        145, 54, 78, 98, 23, 45, 67, 34,
        89, 28, 112, 76, 58, 67, 31, 87,
        45, 32, 54, 43, 78, 89
    };
    
    // Preenche apenas os dados numéricos (nomes gerados depois) ———————————————
    for (int i = 0; i < CARTAS_PREENCHIDAS; i++) {
        populacoes[i] = pops[i];
        areas[i] = ars[i];
        pibs[i] = pibs_dados[i];
        pontosTuristicos[i] = pontos[i];
    }
}

// NOVA FUNÇÃO: Calcula Super Poder com conversões cuidadosas —————————————————
float calcularSuperPoder(unsigned long populacao, float area, float pib,
                        int pontosTuristicos, double densidade, double pibPerCapita) {
    
    // Soma com conversões explícitas para evitar erros de tipo ———————————————
    float superPoder = (float)populacao + area + pib + (float)pontosTuristicos + 
                      (float)densidade + (float)pibPerCapita;
    
    return superPoder;
}

// NOVA FUNÇÃO: Exibe índice simplificado para escolha de cartas ———————————————
void exibirIndiceCartas(char nomesCidades[][TAM_NOME_CIDADE]) {
    // Cabeçalho mais limpo e organizado ———————————————————————————————————————
    printf("%-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s\n", 
           "Nº", "Carta", "Nº", "Carta", "Nº", "Carta", "Nº", "Carta", 
           "Nº", "Carta", "Nº", "Carta", "Nº", "Carta", "Nº", "Carta");
           
    printf("%-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s    %-3s %-4s\n",
           "--", "----", "--", "----", "--", "----", "--", "----",
           "--", "----", "--", "----", "--", "----", "--", "----");
    
    // Exibe dados em 8 colunas com formatação consistente —————————————————————
    for (int i = 0; i < TOTAL_CARTAS; i += 8) {
        for (int j = 0; j < 8 && (i + j) < TOTAL_CARTAS; j++) {
            printf("%-3d %-4s", i + j + 1, nomesCidades[i + j]);
            if (j < 7 && (i + j + 1) < TOTAL_CARTAS) {
                printf("    "); // 4 espaços para separação entre colunas
            }
        }
        printf("\n");
    }
}

// NOVA FUNÇÃO: Obtém escolha válida do usuário ———————————————————————————————
int obterEscolhaUsuario(int maxCartas) {
    int escolha;
    
    do {
        if (scanf("%d", &escolha) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            escolha = 0; // Valor inválido para continuar loop ——————————————————
        }
        else if (escolha < 1 || escolha > maxCartas) {
            printf("ERRO: Digite um numero entre 1 e %d!\n", maxCartas);
        }
    } while (escolha < 1 || escolha > maxCartas);
    
    return escolha;
}

// NOVA FUNÇÃO: Obtém escolha válida excluindo uma carta já selecionada ————————
int obterEscolhaUsuarioExcluindo(int maxCartas, int cartaExcluida) {
    int escolha;
    
    do {
        if (scanf("%d", &escolha) != 1) {
            printf("ERRO: Digite apenas numeros!\n");
            limparBuffer();
            escolha = 0; // Valor inválido para continuar loop ——————————————————
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

// NOVA FUNÇÃO: Compara duas cartas e exibe resultados ————————————————————————
void compararCartas(int carta1, int carta2, char nomesCidades[][TAM_NOME_CIDADE],
                   unsigned long populacoes[], float areas[], float pibs[],
                   int pontosTuristicos[], double densidades[], 
                   double pibsPerCapita[], float superPoderes[]) {
    
    printf("Comparando: %s vs %s\n\n", nomesCidades[carta1], nomesCidades[carta2]);
    
    // População: maior vence ——————————————————————————————————————————————————
    int popVence = (populacoes[carta1] > populacoes[carta2]) ? 1 : 0;
    printf("População: Carta %d venceu (%d)\n", popVence ? 1 : 2, popVence);
    
    // Área: maior vence ———————————————————————————————————————————————————————
    int areaVence = (areas[carta1] > areas[carta2]) ? 1 : 0;
    printf("Área: Carta %d venceu (%d)\n", areaVence ? 1 : 2, areaVence);
    
    // PIB: maior vence ————————————————————————————————————————————————————————
    int pibVence = (pibs[carta1] > pibs[carta2]) ? 1 : 0;
    printf("PIB: Carta %d venceu (%d)\n", pibVence ? 1 : 2, pibVence);
    
    // Pontos Turísticos: maior vence ——————————————————————————————————————————
    int pontosVence = (pontosTuristicos[carta1] > pontosTuristicos[carta2]) ? 1 : 0;
    printf("Pontos Turísticos: Carta %d venceu (%d)\n", pontosVence ? 1 : 2, pontosVence);
    
    // Densidade: MENOR vence (regra especial) —————————————————————————————————
    int densVence = (densidades[carta1] < densidades[carta2]) ? 1 : 0;
    printf("Densidade Populacional: Carta %d venceu (%d)\n", densVence ? 1 : 2, densVence);
    
    // PIB per Capita: maior vence —————————————————————————————————————————————
    int pibPcVence = (pibsPerCapita[carta1] > pibsPerCapita[carta2]) ? 1 : 0;
    printf("PIB per Capita: Carta %d venceu (%d)\n", pibPcVence ? 1 : 2, pibPcVence);
    
    // Super Poder: maior vence —————————————————————————————————————————————————
    int superVence = (superPoderes[carta1] > superPoderes[carta2]) ? 1 : 0;
    printf("Super Poder: Carta %d venceu (%d)\n", superVence ? 1 : 2, superVence);
    
    // Resumo final —————————————————————————————————————————————————————————————
    int totalVitoriasCarta1 = popVence + areaVence + pibVence + pontosVence + densVence + pibPcVence + superVence;
    printf("\nResumo: Carta 1 venceu %d/7 comparações\n", totalVitoriasCarta1);
    printf("Carta vencedora geral: Carta %d (%s)\n", 
           (totalVitoriasCarta1 > 3) ? 1 : 2,
           (totalVitoriasCarta1 > 3) ? nomesCidades[carta1] : nomesCidades[carta2]);
}

// Função para exibir tabela (ADAPTADA COM NOMES PADRONIZADOS) ————————————————
void exibirTabelaEstado(char estado, int inicioIndice, 
                       char nomesCidades[][TAM_NOME_CIDADE], 
                       unsigned long populacoes[], float areas[], float pibs[], 
                       int pontosTuristicos[], double densidades[], 
                       double pibsPerCapita[], float superPoderes[]) {
    
    printf("\n========== ESTADO %c - TABELA RESUMO ==========\n", estado);
    
    // Cabeçalho da tabela ————————————————————————————————————————————————————
    printf("%-5s %-12s %-10s %-12s %-8s %-12s %-12s %-12s\n",
           "Carta", "Pop.(hab)", "Area(km²)", "PIB(bi)", "Pontos",
           "Densidade", "PIB pc(R$)", "SuperPoder");
    
    printf("%-5s %-12s %-10s %-12s %-8s %-12s %-12s %-12s\n",
           "-----", "------------", "----------", "------------", "--------", 
           "------------", "------------", "------------");

    // Dados das 4 cidades do estado ——————————————————————————————————————————
    for (int i = 0; i < CIDADES_POR_ESTADO; i++) {
        int indice = inicioIndice + i;
        
        printf("%-5s %-12lu %-10.2f %-12.2f %-8d %-12.2f %-12.2e %-12.2f\n",
               nomesCidades[indice],               // A01, A02, etc.
               populacoes[indice],                 // População (unsigned long) —
               areas[indice],                      // Área —————————————————————
               pibs[indice],                       // PIB ——————————————————————
               pontosTuristicos[indice],           // Pontos turísticos ————————
               densidades[indice],                 // Densidade —————————————————
               pibsPerCapita[indice] * 1e9,        // PIB per capita ———————————
               superPoderes[indice]                // Super Poder ——————————————
        );
    }
    
    printf("\n"); // Linha em branco para separar as tabelas ———————————————————
}