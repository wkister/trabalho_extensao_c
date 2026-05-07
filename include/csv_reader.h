#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 500
#define MAX_COLS 20
#define MAX_BUFFER 1024
#define MAX_PELOTAO 256
#define MAX_AREA 64
#define MAX_NUMERO 32

typedef struct {
    char pelotao[MAX_PELOTAO];
    char area[MAX_AREA];
    char numero[MAX_NUMERO];
    double nota;
    int ano;
    char avaliacao[10];  // av1, av2, trabalho, av3, av4
} StudentRecord;

typedef struct {
    StudentRecord *records;
    int count;
    int capacity;
} CSVData;

/**
 * Cria uma estrutura CSVData vazia
 * 
 * @return Ponteiro para estrutura CSVData inicializada
 */
CSVData* csv_create(void);

/**
 * Adiciona um registro à estrutura CSVData
 * 
 * @param data Estrutura CSVData
 * @param record Registro a adicionar
 * @return 0 em sucesso, -1 em erro
 */
int csv_add_record(CSVData *data, StudentRecord record);

/**
 * Lê um arquivo CSV no formato EAMES
 * 
 * @param filename Caminho do arquivo
 * @param ano Ano dos dados
 * @param avaliacao Tipo de avaliação (av1, av2, trabalho, av3, av4)
 * @return Estrutura com dados lidos, ou NULL em caso de erro
 */
CSVData* csv_read(const char *filename, int ano, const char *avaliacao);

/**
 * Libera memória alocada para dados CSV
 * 
 * @param data Ponteiro para estrutura CSVData
 */
void csv_free(CSVData *data);

/**
 * Imprime dados CSV (para debug)
 * 
 * @param data Estrutura com dados
 * @param max_rows Número máximo de linhas a imprimir (0 = todas)
 */
void csv_print(CSVData *data, int max_rows);

/**
 * Retorna número total de registros
 * 
 * @param data Estrutura com dados
 * @return Número de registros
 */
int csv_count(CSVData *data);

#endif // CSV_READER_H
