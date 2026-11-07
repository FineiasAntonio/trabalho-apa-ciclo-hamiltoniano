# Trabalho — onde cada código entra

Pasta para o grupo reunir o código do Ciclo Hamiltoniano Direcionado. Cada estratégia fica na própria pasta. O que é compartilhado (digrafo, contador e verificador) fica em `00-comum`, e os outros itens importam dali.

O plano completo continua em [docs/PLANO.md](../docs/PLANO.md). O PDF do relatório continua em `docs/relatorio/`.

| Pasta | O que colocar | Quem produz |
| --- | --- | --- |
| `00-comum/` | `digrafo.py`, `contador.py`, `verifica.py` | base de todos |
| `01-forca-bruta/` | `forca_bruta.py` | papel B |
| `02-backtracking/` | `backtracking.py` | papel B |
| `03-divisao-e-conquista/` | a justificativa e o contraexemplo dos blocos | papel C |
| `04-programacao-dinamica/` | `held_karp.py` | papel C |
| `05-guloso/` | `guloso.py` | papel D |
| `06-heuristica/` | `recozimento.py` | papel D |
| `07-torneio/` | `torneio.py` (Camion, só torneio) | papel C |
| `testes/` | concordância, armadilhas do guloso, torneio | quem implementou o item |
| `experimentos/` | `gerador.py`, `experimentos.py` | papel D |
| `dados/` | CSV gerado pela bateria | papel D |
| `graficos/` | figuras que entram no PDF | papel D |

Nomes de arquivo sem acento, para o caminho e o texto do relatório usarem a mesma palavra. Cada algoritmo permanece curto o bastante para ser colado no PDF.
