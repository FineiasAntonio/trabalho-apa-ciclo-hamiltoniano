# Trabalho — onde cada código entra

Código em C++17 do Ciclo Hamiltoniano Direcionado. Cada estratégia fica na própria pasta. O digrafo, o contador, o verificador e o relógio ficam em `00-comum`.

O plano completo está em [docs/PLANO.md](../docs/PLANO.md). Compilação e testes: `make test` na raiz do repositório.

| Pasta | Arquivos | Papel |
| --- | --- | --- |
| `00-comum/` | `digrafo`, `contador`, `verifica`, `tempo` | base de todos |
| `01-forca-bruta/` | `forca_bruta.cpp` | exato, origem fixa em 0 |
| `02-backtracking/` | `backtracking.cpp` | exato, ordens índice e Warnsdorff |
| `03-divisao-e-conquista/` | `ciclo_por_blocos_contiguos.cpp` | atalho incorreto, com o falso negativo |
| `04-programacao-dinamica/` | `held_karp.cpp` | exato, com reconstrução do ciclo |
| `05-guloso/` | `guloso.cpp` | sem garantia, dois critérios |
| `06-heuristica/` | `recozimento.cpp` | recozimento simulado, sem razão de aproximação |
| `07-torneio/` | `torneio.cpp` | Camion, só em torneio |
| `testes/` | `testar.cpp` | concordância, armadilhas, torneio |
| `experimentos/` | `gerador.cpp`, `experimentos.cpp` | famílias e CSV |
| `dados/` | CSV gerado | bateria curta já pode estar gravada |
| `graficos/` | figuras do PDF | saem do CSV |

`nullopt` nos exatos significa que o ciclo não existe. No guloso, significa que a partida empacou. Em `ciclo_por_blocos_contiguos`, significa que nenhum ciclo tem os dois blocos contíguos. Em `ciclo_em_torneio`, um digrafo que não é torneio é recusado.
