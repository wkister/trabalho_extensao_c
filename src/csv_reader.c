#include "../include/csv_reader.h"
#include <ctype.h>
#include <regex.h>

/**
 * Estrutura de mapeamento de áreas
 */
typedef struct {
    char prefix;
    char area[MAX_AREA];
} AreaMapping;

static AreaMapping area_map[] = {
    {'A', "APOIO"},
    {'E', "ELETROELETRÔNICA"},
    {'M', "ELETROMECÂNICA"},
    {0, ""}
};

/**
 * Função auxiliar: converte string para maiúsculas
 */
static void strtoupper_custom(char *str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

/**
 * Função auxiliar: remove espaços no início e fim
 */
static char* trim(char *str) {
    if (!str) return str;
    
    // Remove espaços do início
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    
    // Remove espaços do fim
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    
    return str;
}

/**
 * Função auxiliar: substitui vírgula por ponto em string numérica
 */
static void replace_decimal_comma(char *str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
}

/**
 * Extrai a área a partir do prefixo do número do aluno
 * Exemplo: A1-1006 → A → APOIO
 */
static void extract_area_from_numero(const char *numero, char *area) {
    area[0] = '\0';
    
    if (!numero || strlen(numero) < 3) return;
    
    char prefix = toupper(numero[0]);
    
    for (int i = 0; area_map[i].prefix != 0; i++) {
        if (area_map[i].prefix == prefix) {
            strcpy(area, area_map[i].area);
            return;
        }
    }
}

/**
 * Converte nota string para double
 * Substitui vírgula por ponto e converte
 */
static double parse_nota(const char *nota_str) {
    if (!nota_str || strlen(nota_str) == 0) {
        return -1.0;  // Valor inválido
    }
    
    char buffer[64];
    strcpy(buffer, nota_str);
    
    // Remover aspas se houver
    char *p = buffer;
    if (*p == '"') p++;
    if (p[strlen(p) - 1] == '"') {
        p[strlen(p) - 1] = '\0';
    }
    
    replace_decimal_comma(p);
    
    char *endptr;
    double value = strtod(p, &endptr);
    
    // Se não converteu nada, retorna inválido
    if (endptr == p) {
        return -1.0;
    }
    
    return value;
}

/**
 * Divide string por delimitador
 * Retorna ponteiros para as partes
 */
static int split_csv_line(char *line, char **parts, int max_parts) {
    int count = 0;
    char *start = line;
    int in_quotes = 0;
    
    for (int i = 0; line[i] && count < max_parts; i++) {
        if (line[i] == '"') {
            in_quotes = !in_quotes;
        } else if (line[i] == ',' && !in_quotes) {
            line[i] = '\0';
            parts[count] = start;
            count++;
            start = line + i + 1;
        }
    }
    
    // Última parte
    if (count < max_parts && *start) {
        parts[count] = start;
        count++;
    }
    
    return count;
}

/**
 * Cria estrutura CSVData vazia
 */
CSVData* csv_create(void) {
    CSVData *data = (CSVData *)malloc(sizeof(CSVData));
    if (!data) return NULL;
    
    data->capacity = 100;
    data->count = 0;
    data->records = (StudentRecord *)malloc(data->capacity * sizeof(StudentRecord));
    
    if (!data->records) {
        free(data);
        return NULL;
    }
    
    return data;
}

/**
 * Adiciona registro à estrutura
 */
int csv_add_record(CSVData *data, StudentRecord record) {
    if (!data) return -1;
    
    // Redimensionar se necessário
    if (data->count >= data->capacity) {
        data->capacity *= 2;
        StudentRecord *new_records = (StudentRecord *)realloc(data->records, 
                                                              data->capacity * sizeof(StudentRecord));
        if (!new_records) return -1;
        data->records = new_records;
    }
    
    data->records[data->count] = record;
    data->count++;
    
    return 0;
}

/**
 * Lê arquivo CSV no formato EAMES
 */
CSVData* csv_read(const char *filename, int ano, const char *avaliacao) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Erro: não foi possível abrir arquivo %s\n", filename);
        return NULL;
    }
    
    CSVData *data = csv_create();
    if (!data) {
        fclose(fp);
        return NULL;
    }
    
    char line[MAX_BUFFER];
    char pelotao[MAX_PELOTAO] = "";
    int reading_data = 0;
    int line_num = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        
        // Remove quebra de linha
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Trim
        char *trimmed = trim(line);
        
        // Linha vazia - pular
        if (strlen(trimmed) == 0) {
            continue;
        }
        
        // Se encontrou "Nº" ou "NOTA" ou similar, é cabeçalho de colunas
        char upper_line[MAX_BUFFER];
        strcpy(upper_line, trimmed);
        strtoupper_custom(upper_line);
        
        if ((strstr(upper_line, "NO") && strstr(upper_line, "NOTA")) ||
            strstr(upper_line, "PROVA")) {
            reading_data = 1;
            continue;
        }
        
        // Se estamos lendo dados
        if (reading_data && strchr(trimmed, ',')) {
            // Verifica se é nova seção de pelotão (tem padrão A1/E2/M3 e PEL/CIA)
            if (strstr(trimmed, "PEL") || strstr(trimmed, "CIA")) {
                strcpy(pelotao, trimmed);
                reading_data = 0;
                continue;
            }
            
            char *parts[10];
            char line_copy[MAX_BUFFER];
            strcpy(line_copy, trimmed);
            
            int num_parts = split_csv_line(line_copy, parts, 10);
            
            if (num_parts >= 2) {
                StudentRecord record;
                
                // Inicializa record
                strcpy(record.pelotao, pelotao);
                strcpy(record.numero, "");
                strcpy(record.area, "");
                record.nota = -1.0;
                record.ano = ano;
                strcpy(record.avaliacao, avaliacao);
                
                char *p0 = trim(parts[0]);
                char *p1 = trim(parts[1]);
                
                // Parse: Nº,NOTA ou ÁREA,Nº,NOTA
                
                // Se primeira coluna tem padrão "LETRA-NUMERO" é o número do aluno
                if (strchr(p0, '-') && isalpha(p0[0])) {
                    strcpy(record.numero, p0);
                    extract_area_from_numero(record.numero, record.area);
                    record.nota = parse_nota(p1);
                }
                // Se primeira coluna é só letra (A, E, M), é a área
                else if (strlen(p0) == 1 && isalpha(p0[0]) && num_parts >= 3) {
                    strcpy(record.area, p0);
                    strcpy(record.numero, p1);
                    record.nota = parse_nota(trim(parts[2]));
                }
                
                // Adiciona se tem número válido e nota válida
                if (strlen(record.numero) > 0 && record.nota >= 0.0) {
                    csv_add_record(data, record);
                }
            }
        }
        else if (!reading_data && !strchr(trimmed, ',') && strchr(trimmed, '-')) {
            // Linhas tipo "A1   -   1º PEL / 1ª CIA"
            if (isalpha(trimmed[0])) {
                strcpy(pelotao, trimmed);
                continue;
            }
        }
    }
    
    fclose(fp);
    return data;
}

/**
 * Libera memória de CSVData
 */
void csv_free(CSVData *data) {
    if (!data) return;
    
    if (data->records) {
        free(data->records);
    }
    free(data);
}

/**
 * Imprime dados CSV
 */
void csv_print(CSVData *data, int max_rows) {
    if (!data || data->count == 0) {
        printf("Nenhum dado para imprimir\n");
        return;
    }
    
    printf("\n========== DADOS CSV ==========\n");
    printf("%-30s %-20s %-15s %-8s %s\n", 
           "PELOTÃO", "ÁREA", "Nº", "NOTA", "ANO");
    printf("-----------------------------------------------------\n");
    
    int limit = max_rows > 0 ? max_rows : data->count;
    
    for (int i = 0; i < limit && i < data->count; i++) {
        printf("%-30s %-20s %-15s %-8.2f %d\n",
               data->records[i].pelotao,
               data->records[i].area,
               data->records[i].numero,
               data->records[i].nota,
               data->records[i].ano);
    }
    
    printf("\nTotal: %d registros\n", data->count);
}

/**
 * Retorna contagem de registros
 */
int csv_count(CSVData *data) {
    if (!data) return 0;
    return data->count;
}
