# Análise de Dados EAMES em C

Aplicação em C para leitura de arquivos CSV da EAMES, consolidação dos dados em pivot por aluno e cálculo de correlação entre `AV1` e `NF_MATII`.

## O que o programa faz hoje

- Carrega dados por ano a partir de `dados/<ano>/{trabalho,av1,av2,av3,av4}.csv`
- Mostra no terminal qual avaliação está sendo lida e quantos registros foram carregados
- Consolida em tabela pivot por aluno (chave: `numero + ano`)
- Calcula notas finais:
	- `NF_MATI = (AV1 + AV2 + TRABALHO) / 2`
	- `NF_MATII = (AV3 + AV4) / 2`
- Calcula correlação de Pearson entre `AV1` e `NF_MATII`
- Classifica a intensidade da correlação (`fraca`, `moderada`, `forte`, `muito forte`)

## Estrutura atual

```
trabalho_extensao_c/
├── include/
│   ├── csv_reader.h
│   ├── helpers.h
│   ├── pivot.h
│   └── statistics.h
├── src/
│   ├── csv_reader.c
│   ├── helpers.c
│   ├── main.c
│   ├── pivot.c
│   └── statistics.c
├── dados/
├── Makefile
├── README.md
└── USAGE.md
```

## Principais módulos

### `csv_reader`

- Estruturas `StudentRecord` e `CSVData`
- Leitura do CSV EAMES com normalização de nota decimal
- Suporte aos formatos de cabeçalho usados nas avaliações
- Funções: `csv_create`, `csv_read`, `csv_add_record`, `csv_count`, `csv_print`, `csv_free`

### `helpers`

- Carregamento por ano (`load_year_data`)
- Merge de dados (`merge_csv_data`)
- Parse de argumento de ano (`parse_year_argument`)
- Funções auxiliares de média/desvio e extração de coluna

### `pivot`

- Geração de pivot por aluno/ano (`pivot_from_csv`)
- Preenchimento das colunas `AV1`, `AV2`, `TRABALHO`, `AV3`, `AV4`
- Cálculo de `NF_MATI` e `NF_MATII`
- Impressão opcional do pivot (`pivot_print`)

### `statistics`

- `stats_correlation_av1_nf_matii`: correlação de Pearson
- `stats_classify_correlation`: classificação por intensidade
	- `|r| < 0.3` -> `fraca`
	- `0.3 <= |r| < 0.6` -> `moderada`
	- `0.6 <= |r| < 0.9` -> `forte`
	- `|r| >= 0.9` -> `muito forte`

## Compilação

```bash
make
make clean
make run
make help
```

## Execução

```bash
./analise_dados [ANOS]
```

Exemplos:

```bash
./analise_dados
./analise_dados 2022
./analise_dados 2020,2022,2024
./analise_dados 2020-2023
```

## Saída esperada (resumo)

- Lista de anos analisados
- Para cada avaliação de cada ano:
	- nome da avaliação
	- total de registros lidos
- Total consolidado de registros
- Bloco de correlação AV1 x NF_MATII com valor, pares válidos e classificação

## Requisitos

- GCC com suporte a C99
- `make`
- Sistema com suporte a terminal UTF-8 (recomendado)
