# Ciclo Hamiltoniano Direcionado

Trabalho final de Análise e Projeto de Algoritmos (APA). O tema é o problema 21 do edital: dado um digrafo, decidir se existe um ciclo direcionado que visita cada vértice exatamente uma vez e volta ao início.

O plano está em [docs/PLANO.md](docs/PLANO.md). Os códigos em C++17 ficam em [trabalho/](trabalho/), uma pasta por estratégia. Por enquanto só existe a força bruta.

## Como rodar

É preciso o `g++` com C++17.

```bash
make
./trabalho/bin/forca_bruta pior_caso 8
```

O programa recebe o tipo do digrafo e o tamanho `n`. Tipos: `ciclo_simples`, `forte_sem_ciclo`, `pior_caso` e `aleatorio`. A saída traz o ciclo (ou o aviso de que não existe), as permutações testadas, as consultas de arco e o tempo.

## Entrega exigida pelo edital

Um único ZIP no SIGAA, nomeado `PRIMEIRO_ULTIMO.ZIP`, com o código, os arquivos complementares e a documentação em PDF. O grupo tem quatro integrantes.
