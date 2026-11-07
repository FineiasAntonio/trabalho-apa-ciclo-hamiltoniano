# Ciclo Hamiltoniano Direcionado

Plano do trabalho final de Análise e Projeto de Algoritmos (APA). O tema é o problema 21 do edital: dado um digrafo, decidir se existe um ciclo direcionado que visita cada vértice exatamente uma vez e volta ao início.

O documento de trabalho do grupo está em [docs/PLANO.md](docs/PLANO.md). Ele fixa a definição formal, a prova de pertencimento a NP, o papel de cada estratégia, o protocolo de experimentos e a divisão entre quatro integrantes.

Os códigos ficam agrupados em [trabalho/](trabalho/LEIA-ME.md), uma pasta por item. A ordem de implementação está na seção 16 do plano.

## Como este repositório vai rodar

Quando o código entrar, a execução local prevista é:

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python -m pytest trabalho/testes
python trabalho/experimentos/experimentos.py
```

Linguagem escolhida: Python 3. Os gráficos e o CSV saem de `trabalho/experimentos/`. O PDF do relatório será gerado a partir de LaTeX em `docs/relatorio/`.

## Entrega exigida pelo edital

Um único ZIP no SIGAA, nomeado `PRIMEIRO_ULTIMO.ZIP`, com o código, os arquivos complementares e a documentação em PDF. O grupo tem quatro integrantes.
