#ifndef STATISTICS_H
#define STATISTICS_H

#include "pivot.h"

/**
 * Calcula a correlacao de Pearson entre AV1 e NF_MATII.
 *
 * @param table Tabela pivot com os dados dos alunos
 * @param correlation Resultado da correlacao (saida)
 * @param pair_count Quantidade de pares validos usados no calculo (saida)
 * @return 0 em sucesso, -1 em erro/pares insuficientes
 */
int stats_correlation_av1_nf_matii(const PivotTable *table, double *correlation, int *pair_count);

/**
 * Classifica a intensidade da correlação de Pearson.
 * Retorna uma string literal em português: "fraca", "moderada", "forte", "muito forte".
 */
const char *stats_classify_correlation(double r);

#endif // STATISTICS_H
