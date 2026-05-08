# USAGE

Guia rápido para compilar e executar o projeto `trabalho_extensao_c`.

## 1) Compilar

No diretório do projeto:

```bash
make
```

Outros comandos úteis:

```bash
make run
make clean
make help
```

## 2) Executar

Sintaxe:

```bash
./analise_dados [ANOS]
```

### Formatos aceitos para `ANOS`

- Omitido: usa padrão `2020-2025`
- Ano único: `2022`
- Lista: `2020,2022,2024`
- Intervalo: `2020-2023`

Exemplos:

```bash
./analise_dados
./analise_dados 2022
./analise_dados 2020,2022,2024
./analise_dados 2021-2023
```

## 3) O que aparece na saída

Durante o carregamento, o programa exibe por ano:

- avaliação sendo lida (`trabalho`, `av1`, `av2`, `av3`, `av4`)
- total de registros daquela avaliação
- total de registros carregados por ano

No final, exibe:

- total consolidado de registros
- correlação de Pearson entre `AV1` e `NF_MATII`
- quantidade de pares válidos usados
- classificação da correlação (`fraca`, `moderada`, `forte`, `muito forte`)

## 4) Fórmulas usadas no pivot

- `NF_MATI = (AV1 + AV2 + TRABALHO) / 2`
- `NF_MATII = (AV3 + AV4) / 2`

## 5) Exemplo resumido de saída

```text
Anos a analisar: 2020

Lendo avaliação 'trabalho' (dados/2020/trabalho.csv)...
 -> 187 registros lidos
Lendo avaliação 'av1' (dados/2020/av1.csv)...
 -> 186 registros lidos
...

========== CORRELAÇÃO ==========
Correlação de Pearson (AV1 x NF_MATII): 0.5288
Pares válidos usados: 184
Classificação da correlação: moderada
```

## 6) Erros comuns

- `Erro: argumento de ano inválido`:
	- informe ano/lista/intervalo no formato suportado.
- `Erro: não foi possível abrir arquivo ...`:
	- verifique se existe `dados/<ano>/<avaliacao>.csv`.
