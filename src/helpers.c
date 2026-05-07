#include "../include/helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define AVALIACOES_COUNT 5
const char *avaliacoes[] = {"av1", "av2", "trabalho", "av3", "av4"};

/**
 * Carrega todos os dados de um ano específico
 */
int load_year_data(int year, CSVData *output) {
    int total_loaded = 0;
    char filepath[512];
    
    for (int i = 0; i < AVALIACOES_COUNT; i++) {
        snprintf(filepath, sizeof(filepath), "dados/%d/%s.csv", year, avaliacoes[i]);
        
        CSVData *year_data = csv_read(filepath, year, avaliacoes[i]);
        if (year_data && year_data->count > 0) {
            total_loaded += merge_csv_data(output, year_data);
            csv_free(year_data);
        }
    }
    
    return total_loaded;
}

/**
 * Combina dados de múltiplos CSVData
 */
int merge_csv_data(CSVData *output, CSVData *input) {
    if (!output || !input) return 0;
    
    int added = 0;
    for (int i = 0; i < input->count; i++) {
        if (csv_add_record(output, input->records[i]) == 0) {
            added++;
        }
    }
    
    return added;
}

/**
 * Parse de argumentos de ano
 */
int parse_year_argument(const char *arg, int *years, int max_years) {
    if (!arg || !years || max_years <= 0) return 0;
    
    int count = 0;
    char buffer[256];
    strncpy(buffer, arg, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    // Verifica se tem hífen (intervalo)
    if (strchr(buffer, '-')) {
        int start_year, end_year;
        if (sscanf(buffer, "%d-%d", &start_year, &end_year) == 2) {
            // Intervalo: 2020-2022 → 2020, 2021, 2022
            for (int year = start_year; year <= end_year && count < max_years; year++) {
                years[count++] = year;
            }
            return count;
        }
    }
    
    // Verifica se tem vírgulas (múltiplos anos)
    if (strchr(buffer, ',')) {
        char *token = strtok(buffer, ",");
        while (token && count < max_years) {
            int year = atoi(token);
            if (year >= 2000 && year <= 2099) {  // Validação básica
                years[count++] = year;
            }
            token = strtok(NULL, ",");
        }
        return count;
    }
    
    // Single year
    int year = atoi(buffer);
    if (year >= 2000 && year <= 2099) {
        years[count++] = year;
    }
    
    return count;
}

/**
 * Calcula média
 */
double calculate_mean(double *values, int count) {
    if (!values || count <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += values[i];
    }
    
    return sum / count;
}

/**
 * Calcula desvio padrão
 */
double calculate_std_dev(double *values, int count, double mean) {
    if (!values || count <= 1) return 0.0;
    
    double sum_sq_diff = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = values[i] - mean;
        sum_sq_diff += diff * diff;
    }
    
    return sqrt(sum_sq_diff / (count - 1));
}

/**
 * Extrai valores de uma coluna específica (avaliação)
 */
int extract_column(CSVData *data, const char *avaliacao, double *values) {
    if (!data || !avaliacao || !values) return 0;
    
    int count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strcmp(data->records[i].avaliacao, avaliacao) == 0 && 
            data->records[i].nota >= 0.0) {
            values[count++] = data->records[i].nota;
        }
    }
    
    return count;
}
