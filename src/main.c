#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

/**
 * Programa principal - Análise de Dados EAMES em C
 * Carrega arquivos CSV no formato EAMES e realiza análises estatísticas
 */

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("Análise de Dados EAMES - Versão C\n");
    printf("========================================\n\n");
    
    // Se passou argumentos, tenta carregar arquivo
    if (argc < 2) {
        printf("Uso: %s <arquivo_csv> [ano] [avaliacao]\n", argv[0]);
        printf("\nExemplos:\n");
        printf("  %s dados/2022/av1.csv 2022 av1\n", argv[0]);
        printf("  %s dados/2022/av2.csv 2022 av2\n", argv[0]);
        printf("\nFormatos suportados:\n");
        printf("  - Arquivos EAMES (pelotão/área + número + nota)\n");
        printf("  - Notas com vírgula decimal (ex: 7,5)\n");
        printf("  - Avaliações: av1, av2, trabalho, av3, av4\n\n");
        return 1;
    }
    
    const char *filename = argv[1];
    int ano = (argc > 2) ? atoi(argv[2]) : 2022;
    const char *avaliacao = (argc > 3) ? argv[3] : "av1";
    
    printf("Carregando arquivo: %s\n", filename);
    printf("  Ano: %d\n", ano);
    printf("  Avaliação: %s\n\n", avaliacao);
    
    // Carrega dados
    CSVData *data = csv_read(filename, ano, avaliacao);
    
    if (!data) {
        fprintf(stderr, "Erro: falha ao carregar arquivo\n");
        return 1;
    }
    
    int count = csv_count(data);
    printf("Registros carregados: %d\n\n", count);
    
    if (count > 0) {
        // Imprime primeiros 10 registros
        csv_print(data, 10);
    } else {
        printf("Nenhum registro encontrado no arquivo.\n");
    }
    
    // Libera memória
    csv_free(data);
    
    printf("\nPrograma finalizado com sucesso.\n");
    return 0;
}

