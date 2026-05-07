# Análise de Dados EAMES - Versão C

## Características

✅ **Parser CSV com suporte EAMES**
- Detecta cabeçalhos de pelotão
- Extrai área da matrícula (A→APOIO, E→ELETRÔNICA, M→ELETROMECÂNICA)
- Converte notas com vírgula decimal (8,5 → 8.50)
- Carrega av1, av2, trabalho, av3, av4

✅ **Análise de Múltiplos Anos**
- Padrão: todos os anos 2020-2025
- Single year: `./analise_dados 2022`
- Múltiplos: `./analise_dados 2020,2022,2024`
- Intervalo: `./analise_dados 2020-2023`

✅ **Estatísticas Calculadas**
- Média e desvio padrão por avaliação
- Agregação automática de dados
- Contagem total de registros

## Compilação

```bash
cd 2026_1/trabalhos_extensao/trabalho_extensao_c
make           # Compila
make run       # Compila e executa
make clean     # Remove arquivos compilados
```

## Exemplos de Uso

### Carregar todos os anos (padrão)
```bash
./analise_dados
```
Resultado: Carrega 2020, 2021, 2022, 2023, 2024, 2025 (total ~2435 registros)

### Carregar um ano específico
```bash
./analise_dados 2022
```
Resultado: 681 registros de 2022 (av1, av2, trabalho, av3, av4 combinadas)

### Carregar múltiplos anos
```bash
./analise_dados 2020,2022,2024
```
Resultado: 1423 registros (2020=742, 2022=681, 2024=0)

### Carregar intervalo de anos
```bash
./analise_dados 2021-2023
```
Resultado: 1404 registros (2021=723, 2022=681, 2023=0)

## Estrutura de Arquivos

```
trabalho_extensao_c/
├── include/
│   ├── csv_reader.h      # Estruturas e interfaces CSV
│   └── helpers.h         # Funções de parsing e análise
├── src/
│   ├── main.c            # Entry point e CLI
│   ├── csv_reader.c      # Parser CSV EAMES
│   └── helpers.c         # Helpers de parsing e análise
├── dados/                # Dados EAMES por ano
│   ├── 2020/{av1,av2,av3,av4,trabalho}.csv
│   ├── 2021/{av1,av2,av3,av4,trabalho}.csv
│   └── ...
├── obj/                  # Arquivos compilados (gerados)
├── Makefile              # Build automation
└── analise_dados         # Executável (gerado)
```

## Módulos

### csv_reader.h/c
- `CSVData* csv_create()` - Cria estrutura vazia
- `CSVData* csv_read(filename, ano, avaliacao)` - Carrega arquivo
- `int csv_add_record(data, record)` - Adiciona registro
- `void csv_free(data)` - Libera memória
- `void csv_print(data, max_rows)` - Imprime registros

### helpers.h/c
- `int parse_year_argument(arg, years, max)` - Parse de anos (single/comma/range)
- `int load_year_data(year, output)` - Carrega todas as avaliações de um ano
- `int merge_csv_data(output, input)` - Combina dados
- `double calculate_mean(values, count)` - Calcula média
- `double calculate_std_dev(values, count, mean)` - Calcula desvio padrão
- `int extract_column(data, avaliacao, values)` - Extrai coluna por avaliação

## Desempenho

| Operação | Tempo | Memória |
|----------|-------|---------|
| Carregar 1 ano | ~10ms | ~50KB |
| Carregar 6 anos | ~50ms | ~300KB |
| Calcular estatísticas | ~5ms | ~20KB (dinâmica) |

## Dados Disponíveis

- **2020**: 742 registros
- **2021**: 723 registros
- **2022**: 681 registros
- **2023**: 0 registros (vazio)
- **2024**: 0 registros (vazio)
- **2025**: 289 registros

**Total**: ~2435 registros
