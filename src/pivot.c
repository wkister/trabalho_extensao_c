#include "../include/pivot.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void to_lower_copy(char *dst, size_t dst_size, const char *src) {
    if (!dst || dst_size == 0) return;

    dst[0] = '\0';
    if (!src) return;

    size_t i = 0;
    for (; src[i] && i < dst_size - 1; i++) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

static int find_row_index(const PivotTable *table, const char *numero, int ano) {
    if (!table || !numero) return -1;

    for (int i = 0; i < table->count; i++) {
        if (table->rows[i].ano == ano && strcmp(table->rows[i].numero, numero) == 0) {
            return i;
        }
    }

    return -1;
}

static int ensure_capacity(PivotTable *table) {
    if (!table) return -1;

    if (table->count < table->capacity) return 0;

    int new_capacity = table->capacity * 2;
    PivotRow *new_rows = (PivotRow *)realloc(table->rows, new_capacity * sizeof(PivotRow));
    if (!new_rows) return -1;

    table->rows = new_rows;
    table->capacity = new_capacity;
    return 0;
}

static void init_row(PivotRow *row, const StudentRecord *record) {
    if (!row || !record) return;

    strncpy(row->numero, record->numero, sizeof(row->numero) - 1);
    row->numero[sizeof(row->numero) - 1] = '\0';

    strncpy(row->pelotao, record->pelotao, sizeof(row->pelotao) - 1);
    row->pelotao[sizeof(row->pelotao) - 1] = '\0';

    strncpy(row->area, record->area, sizeof(row->area) - 1);
    row->area[sizeof(row->area) - 1] = '\0';

    row->av1 = PIVOT_EMPTY_GRADE;
    row->av2 = PIVOT_EMPTY_GRADE;
    row->trabalho = PIVOT_EMPTY_GRADE;
    row->av3 = PIVOT_EMPTY_GRADE;
    row->av4 = PIVOT_EMPTY_GRADE;
    row->nf_mati = PIVOT_EMPTY_GRADE;
    row->nf_matii = PIVOT_EMPTY_GRADE;
    row->ano = record->ano;
}

static void update_final_grades(PivotRow *row) {
    if (!row) return;

    if (row->av1 >= 0.0 && row->av2 >= 0.0 && row->trabalho >= 0.0) {
        row->nf_mati = (row->av1 + row->av2 + row->trabalho) / 2.0;
    } else {
        row->nf_mati = PIVOT_EMPTY_GRADE;
    }

    if (row->av3 >= 0.0 && row->av4 >= 0.0) {
        row->nf_matii = (row->av3 + row->av4) / 2.0;
    } else {
        row->nf_matii = PIVOT_EMPTY_GRADE;
    }
}

static void set_grade_by_avaliacao(PivotRow *row, const char *avaliacao, double nota) {
    if (!row || !avaliacao) return;

    char key[32];
    to_lower_copy(key, sizeof(key), avaliacao);

    if (strcmp(key, "av1") == 0) {
        row->av1 = nota;
    } else if (strcmp(key, "av2") == 0) {
        row->av2 = nota;
    } else if (strcmp(key, "trabalho") == 0) {
        row->trabalho = nota;
    } else if (strcmp(key, "av3") == 0) {
        row->av3 = nota;
    } else if (strcmp(key, "av4") == 0) {
        row->av4 = nota;
    }
}

PivotTable* pivot_create(void) {
    PivotTable *table = (PivotTable *)malloc(sizeof(PivotTable));
    if (!table) return NULL;

    table->capacity = 100;
    table->count = 0;
    table->rows = (PivotRow *)malloc(table->capacity * sizeof(PivotRow));
    if (!table->rows) {
        free(table);
        return NULL;
    }

    return table;
}

void pivot_free(PivotTable *table) {
    if (!table) return;

    free(table->rows);
    free(table);
}

PivotTable* pivot_from_csv(const CSVData *data) {
    if (!data) return NULL;

    PivotTable *table = pivot_create();
    if (!table) return NULL;

    for (int i = 0; i < data->count; i++) {
        const StudentRecord *record = &data->records[i];
        int index = find_row_index(table, record->numero, record->ano);

        if (index < 0) {
            if (ensure_capacity(table) != 0) {
                pivot_free(table);
                return NULL;
            }

            index = table->count;
            init_row(&table->rows[index], record);
            table->count++;
        } else {
            if (table->rows[index].pelotao[0] == '\0' && record->pelotao[0] != '\0') {
                strncpy(table->rows[index].pelotao, record->pelotao, sizeof(table->rows[index].pelotao) - 1);
                table->rows[index].pelotao[sizeof(table->rows[index].pelotao) - 1] = '\0';
            }
            if (table->rows[index].area[0] == '\0' && record->area[0] != '\0') {
                strncpy(table->rows[index].area, record->area, sizeof(table->rows[index].area) - 1);
                table->rows[index].area[sizeof(table->rows[index].area) - 1] = '\0';
            }
        }

        set_grade_by_avaliacao(&table->rows[index], record->avaliacao, record->nota);
        update_final_grades(&table->rows[index]);
    }

    return table;
}

static void print_grade(double value) {
    if (value < 0.0) {
        printf("%-10s", "-");
    } else {
        printf("%-10.2f", value);
    }
}

void pivot_print(const PivotTable *table, int max_rows) {
    if (!table || table->count == 0) {
        printf("Nenhum dado pivot para imprimir\n");
        return;
    }

    int limit = (max_rows > 0 && max_rows < table->count) ? max_rows : table->count;

        printf("\n============================= PIVOT POR ALUNO =============================\n");
        printf("%-15s %-30s %-18s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-6s\n",
            "NUMERO", "PELOTAO", "AREA", "AV1", "AV2", "TRABALHO", "AV3", "AV4", "NF_MATI", "NF_MATII", "ANO");
        printf("-------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < limit; i++) {
        const PivotRow *row = &table->rows[i];

        printf("%-15s %-30s %-18s ", row->numero, row->pelotao, row->area);
        print_grade(row->av1);
        print_grade(row->av2);
        print_grade(row->trabalho);
        print_grade(row->av3);
        print_grade(row->av4);
        print_grade(row->nf_mati);
        print_grade(row->nf_matii);
        printf("%-6d\n", row->ano);
    }

    printf("\nTotal no pivot: %d linhas\n", table->count);
}
