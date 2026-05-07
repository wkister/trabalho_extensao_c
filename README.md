# Análise de Dados EAMES em C

API em C para leitura e análise de arquivos CSV no formato EAMES, com foco em cálculo de correlação entre disciplinas.

## Objetivo

Implementar em linguagem C uma solução de análise de dados estatísticos baseado no trabalho realizado em Python, permitindo:
- Leitura eficiente de arquivos CSV no formato EAMES
- Cálculo de estatísticas descritivas
- Análise de correlação de Pearson entre disciplinas (MAT I e MAT II)
- Geração de arquivos de resultado para visualização

## Estrutura do Projeto

```
trabalho_extensao_c/
│
├── src/
│   ├── main.c              # Programa principal
│   ├── csv_reader.c        # Leitura de CSV no formato EAMES
│   ├── statistics.c        # Cálculos estatísticos (média, desvio padrão, etc)
│   ├── correlation.c       # Correlação de Pearson
│   └── output.c            # Geração de arquivos de saída
│
├── include/
│   ├── csv_reader.h        # Interface para leitura de CSV
│   ├── statistics.h        # Interface para cálculos estatísticos
│   ├── correlation.h       # Interface para correlação
│   └── output.h            # Interface para geração de saída
│
├── dados/                  # Dados originais (CSVs da EAMES)
├── resultados/             # Arquivos gerados pelo programa
│   ├── medias_anuais.csv   # Para gráficos de série temporal
│   ├── correlacoes.csv     # Para gráficos de correlação
│   ├── taxa_conclusao.csv  # Para gráfico de evasão
│   └── dispersao.csv       # Dados para gráfico de dispersão
│
├── graficos/               # Gráficos gerados (por Python/gnuplot)
├── README.md
├── Makefile
└── .gitignore
```

## Compilação

```bash
make           # Compila o programa
make clean     # Remove objetos compilados
make run       # Executa o programa
make all       # Compila tudo
```

## Uso

```bash
./analise_dados <arquivo_csv>
```

## Módulos

### csv_reader
Responsável pela leitura de arquivos CSV no formato EAMES, com suporte a:
- Detecção automática de áreas (A1, A2, E1, etc)
- Parsing de cabeçalhos por área
- Leitura de notas por avaliação (AV1, AV2, Trabalho, AV3, AV4)

### statistics
Cálculos estatísticos:
- Média aritmética
- Desvio padrão
- Mínimo e máximo
- Quantis

### correlation
Análise de correlação:
- Correlação de Pearson
- P-value para significância estatística
- Análise por ano e por área

### output
Geração de arquivos de resultado:
- CSV para análises
- Dados normalizados para visualização
- Logs de execução

## Dependências

- C99 ou superior
- Make
- GCC ou Clang

## Status

Projeto em desenvolvimento.
