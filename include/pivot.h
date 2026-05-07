#ifndef PIVOT_H
#define PIVOT_H

#include "csv_reader.h"

#define PIVOT_EMPTY_GRADE (-1.0)

typedef struct {
    char numero[MAX_NUMERO];
    char pelotao[MAX_PELOTAO];
    char area[MAX_AREA];
    double av1;
    double av2;
    double trabalho;
    double av3;
    double av4;
    double nf_mati;
    double nf_matii;
    int ano;
} PivotRow;

typedef struct {
    PivotRow *rows;
    int count;
    int capacity;
} PivotTable;

/**
 * Cria uma tabela pivot vazia.
 */
PivotTable* pivot_create(void);

/**
 * Libera a memoria da tabela pivot.
 */
void pivot_free(PivotTable *table);

/**
 * Constrói uma tabela pivot no formato:
 * indice principal: numero do aluno
 * colunas: PELOTAO, AREA, AV1, AV2, TRABALHO, AV3, AV4, NF_MATI, NF_MATII, ANO
 */
PivotTable* pivot_from_csv(const CSVData *data);

/**
 * Imprime a tabela pivot no terminal.
 * max_rows: 0 para imprimir todas as linhas.
 */
void pivot_print(const PivotTable *table, int max_rows);

#endif // PIVOT_H
