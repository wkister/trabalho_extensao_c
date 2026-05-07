#ifndef HELPERS_H
#define HELPERS_H

#include "csv_reader.h"

/**
 * Função para carregar todos os dados de um ano específico
 * 
 * @param year Ano a carregar
 * @param output Estrutura CSVData para armazenar dados combinados
 * @return Número de registros carregados (0 se nenhum arquivo encontrado)
 */
int load_year_data(int year, CSVData *output);

/**
 * Função para combinar dados de múltiplos CSVData em um
 * 
 * @param output CSVData destino
 * @param input CSVData origem
 * @return Número de registros adicionados
 */
int merge_csv_data(CSVData *output, CSVData *input);

/**
 * Parse de argumentos de ano
 * Suporta: 2022 | 2020,2022,2024 | 2020-2022
 * 
 * @param arg String com argumento de ano
 * @param years Array para armazenar anos
 * @param max_years Tamanho máximo do array
 * @return Número de anos parseados
 */
int parse_year_argument(const char *arg, int *years, int max_years);

/**
 * Função auxiliar: calcula média
 */
double calculate_mean(double *values, int count);

/**
 * Função auxiliar: calcula desvio padrão
 */
double calculate_std_dev(double *values, int count, double mean);

/**
 * Função auxiliar: extrai valores de uma coluna
 */
int extract_column(CSVData *data, const char *avaliacao, double *values);

#endif // HELPERS_H
