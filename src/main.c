#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "helpers.h"
#include "pivot.h"
#include "statistics.h"

#define MAX_YEARS 10

/**
 * Programa principal - Análise de Dados EAMES em C
 * Carrega arquivos CSV no formato EAMES e realiza análises estatísticas
 */

void print_usage(const char *program_name) {
    printf("Uso: %s [ANOS]\n", program_name);
    printf("\nPar\u00e2metros:\n");
    printf("  ANOS - Anos a analisar (opcional)\n");
    printf("    Se omitido: analisa todos os anos (2020-2025)\n");
    printf("    Single: %s 2022\n", program_name);
    printf("    M\u00faltiplos: %s 2020,2022,2024\n", program_name);
    printf("    Intervalo: %s 2020-2023\n", program_name);
}

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("An\u00e1lise de Dados EAMES - Versão C\n");
    printf("========================================\n\n");
    
    int years[MAX_YEARS];
    int years_count = 0;
    
    // Parse de anos
    if (argc > 1) {
        years_count = parse_year_argument(argv[1], years, MAX_YEARS);
        if (years_count == 0) {
            fprintf(stderr, "Erro: argumento de ano inv\u00e1lido: %s\n", argv[1]);
            print_usage(argv[0]);
            return 1;
        }
    } else {
        // Padrão: todos os anos de 2020 a 2025
        for (int i = 0; i < 6; i++) {
            years[i] = 2020 + i;
        }
        years_count = 6;
        // years[0] = 2023;
        // years_count = 1;
    }
    
    printf("Anos a analisar: ");
    for (int i = 0; i < years_count; i++) {
        printf("%d%s", years[i], (i < years_count - 1) ? ", " : "\n");
    }
    printf("\n");
    
    // Cria estrutura para dados combinados
    CSVData *all_data = csv_create();
    if (!all_data) {
        fprintf(stderr, "Erro: falha ao criar estrutura de dados\n");
        return 1;
    }
    
    // Carrega dados de todos os anos
    int total_records = 0;
    for (int i = 0; i < years_count; i++) {
        int loaded = load_year_data(years[i], all_data);
        printf("Ano %d: %d registros carregados\n", years[i], loaded);
        total_records += loaded;
    }
    
    printf("\nTotal de registros: %d\n\n", total_records);

    // exit(0);
    
    if (total_records > 0) {
        // Imprime primeiros 20 registros
        // csv_print(all_data, 20);
        // csv_print(all_data, total_records);

        PivotTable *pivot = pivot_from_csv(all_data);
        if (pivot) {
            // pivot_print(pivot, 0);

            double corr_av1_nf_matii = 0.0;
            int pairs = 0;
            if (stats_correlation_av1_nf_matii(pivot, &corr_av1_nf_matii, &pairs) == 0) {
                printf("\n========== CORRELAÇÃO ==========" "\n");
                printf("Correlação de Pearson (AV1 x NF_MATII): %.4f\n", corr_av1_nf_matii);
                printf("Pares válidos usados: %d\n", pairs);
                const char *corr_class = stats_classify_correlation(corr_av1_nf_matii);
                if (corr_class) {
                    printf("Classificação da correlação: %s\n", corr_class);
                }
            } else {
                printf("\n========== CORRELAÇÃO ==========" "\n");
                printf("Não foi possível calcular AV1 x NF_MATII (pares válidos insuficientes).\n");
            }

            pivot_free(pivot);
        } else {
            fprintf(stderr, "Erro: falha ao gerar pivot\n");
        }        
    } else {
        printf("Nenhum registro encontrado nos arquivos.\n");
    }
    
    // Libera memória
    csv_free(all_data);
    
    printf("\nPrograma finalizado com sucesso.\n");
    return 0;
}


