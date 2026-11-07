# Ciclo Hamiltoniano Direcionado

Trabalho final de Análise e Projeto de Algoritmos (APA). O tema é o problema 21 do edital: dado um digrafo, decidir se existe um ciclo direcionado que visita cada vértice exatamente uma vez e volta ao início.

O plano está em [docs/PLANO.md](docs/PLANO.md). Os códigos em C++17 ficam em [trabalho/](trabalho/LEIA-ME.md), uma pasta por estratégia.

## Como rodar

É preciso o `g++` com C++17.

```bash
make test
make experimentos
```

`make test` confere a concordância dos métodos exatos, as armadilhas do guloso, o falso negativo dos blocos contíguos, o recozimento e o teorema de Camion.

`make experimentos` grava a bateria curta em `trabalho/dados/resultados.csv` e a mediana em `trabalho/dados/resumo.csv`. Nessa bateria o recozimento usa orçamento reduzido, para a execução terminar na hora. A bateria do relatório, com os parâmetros congelados no código (`T0 = 1`, `alfa = 0,995`, `200·n` movimentos e 10 recomeços) e com a grade maior, é:

```bash
make all
./trabalho/experimentos/experimentos --completo \
  --saida trabalho/dados/resultados.csv \
  --resumo trabalho/dados/resumo.csv
```

Cada chamada exata para aos 30 segundos e a linha sai com status `censurado`. `inexistente` fica reservado aos métodos exatos. O guloso que empaca grava `nao_encontrou`. O recozimento que gasta o orçamento grava `orcamento`.

## Entrega exigida pelo edital

Um único ZIP no SIGAA, nomeado `PRIMEIRO_ULTIMO.ZIP`, com o código, os arquivos complementares e a documentação em PDF. O grupo tem quatro integrantes.
