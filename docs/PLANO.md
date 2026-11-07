# Plano — Ciclo Hamiltoniano Direcionado

Trabalho final de Análise e Projeto de Algoritmos. Tema 21 do edital: existe um ciclo direcionado que visita cada vértice exatamente uma vez e retorna ao vértice inicial?

Este plano fecha as decisões técnicas para o grupo implementar sem reabrir o desenho. Nomes, matrículas e turma entram na capa quando a lista de quatro integrantes estiver fechada. O restante não depende desses dados.

## 1. O que o edital cobra

O trabalho implementa e compara estratégias de projeto de algoritmos para um problema em NP. A complexidade é estudada por contagem de operações e o desempenho, por tempo de execução. A documentação sai em PDF e inclui, nesta ordem:

1. Identificação (nomes completos em ordem alfabética e matrícula).
2. Introdução.
3. Prova de que o problema pertence a NP.
4. Implementação, com o código inserido no texto.
5. Análise de complexidade de tempo e de espaço.
6. Análise de resultados.
7. Conclusão, inclusive as dificuldades.
8. Bibliografia citada no ponto do texto em que é usada.

Estratégias pedidas: força bruta, backtracking, divisão e conquista, programação dinâmica, algoritmo guloso, algoritmo aproximado ou heurística. Onde uma estratégia não se sustenta, o edital aceita justificativa fundamentada com referência.

A nota é `(escrito + oral) / 2`. O maior peso está na profundidade, em especial na discussão das aplicações práticas. A apresentação tem 20 minutos. Um ou dois integrantes falam; os quatro precisam estar presentes. Entrega só pelo SIGAA, num ZIP `PRIMEIRO_ULTIMO.ZIP`. Atraso do escrito: 0,1 ponto por hora. Cópia entre grupos zera o trabalho de quem copiou e o de quem foi copiado.

Prazos do edital:

| Turma | Escrito | Oral |
| --- | --- | --- |
| A | 11/11/2026 | 17 e 24/11/2026 |
| B | 23/11/2026 | 30/11 e 07/12/2026 |

## 2. Definição que o grupo vai usar

**Ciclo Hamiltoniano Direcionado (CHD).**

Instância: digrafo simples `G = (V, A)`, sem laços, `n = |V| ≥ 2`.

Pergunta: existe uma sequência `(v1, v2, …, vn)` tal que

- os `vi` são todos distintos e cobrem `V`;
- `(vi, v{i+1}) ∈ A` para `i = 1, …, n−1`;
- `(vn, v1) ∈ A`?

O código resolve a **versão de busca**: devolve a sequência ou informa que não encontrou ciclo. A resposta de decisão é o sim ou não dessa busca. O certificado da prova é essa mesma sequência.

Convenção de simetria, usada em todos os métodos exatos: o ciclo, quando existe, é devolvido com `v1` igual a um vértice fixo (o de índice 0). Rotacionar um ciclo não cria outro ciclo. Inverter o sentido cria outro ciclo direcionado, então o sentido oposto continua valendo e entra na busca.

Dois exemplos mínimos, que viram testes:

Instância sim, ciclo `0 → 1 → 2 → 3 → 0`. Arcos: `(0,1), (1,2), (2,3), (3,0)`.

Instância não, digrafo forte sem ciclo hamiltoniano. Arcos: `(0,1), (1,2), (2,0), (1,3), (3,0)`. De 3 só se sai para 0, e em 3 só se entra por 1, então 3 teria de ocupar o trecho `1 → 3 → 0`. O vértice 2 não encaixa nesse trecho com os arcos existentes. O digrafo é fortemente conexo. Conectividade forte é condição necessária e não é suficiente. Todo método exato pode usar o filtro "se não é forte, responda não", e ainda precisa resolver o caso forte.

## 3. Decisões fechadas

1. Linguagem: Python 3. O código entra no PDF, então cada algoritmo fica curto, comentado em português e legível em voz alta. A mesma lógica cabe em Java se a banca pedir a troca; a escolha atual favorece contagem de operações, testes e gráficos.
2. O programa devolve o ciclo, não apenas o booleano.
3. A origem fica fixa no vértice 0. O sentido não fica fixo.
4. Todo método exato recusa de imediato um digrafo que não é fortemente conexo.
5. Divisão e conquista fica como justificativa escrita, com contraexemplo. O grupo não entrega um algoritmo incorreto com esse nome.
6. O teorema de Camion (torneio forte tem ciclo hamiltoniano, com construção em tempo polinomial) entra em módulo próprio, como caso especial. Ele não substitui a estratégia do caso geral.
7. O guloso não tem garantia. Entram dois critérios, cada um com contraexemplo automático em teste.
8. A heurística é recozimento simulado. O texto não atribui razão de aproximação a ela.
9. A contagem de operações usa um contador do próprio algoritmo. O tempo usa `time.perf_counter`. As duas análises ficam separadas, como o edital pede.
10. Para `n` acima do alcance da programação dinâmica, a heurística nunca decreta inexistência. "Orçamento esgotado" e "não existe" são resultados diferentes.
11. Os parâmetros do recozimento são congelados num piloto, antes da bateria que vai para o relatório.
12. O PDF sai de LaTeX, com o código inserido via `listings`.
13. A semente dos experimentos é `20261107`.

## 4. Prova

O edital exige o pertencimento a NP. A NP-completude entra porque a apresentação é julgada pela profundidade. As duas partes ficam separadas no texto, para ficar claro o que é obrigação e o que é acréscimo.

### 4.1 CHD pertence a NP

Certificado: uma sequência de `n` vértices. O tamanho é polinomial na entrada.

Verificador, com matriz de adjacência:

1. Rejeita se o comprimento é diferente de `n`.
2. Rejeita se algum vértice se repete ou falta. Um vetor booleano de tamanho `n` resolve em `O(n)`.
3. Rejeita se algum arco consecutivo, inclusive o de volta, está ausente. São `n` consultas, cada uma `O(1)`.

O verificador roda em `O(n)`. Portanto CHD está em NP. Referências de apoio: Cormen et al. para a definição de verificação polinomial, e Ziviani para o enquadramento usado na disciplina. A prova precisa valer sozinha, com o verificador escrito por extenso.

### 4.2 CHD é NP-completo

O Ciclo Hamiltoniano não direcionado é NP-completo (Karp, 1972; Garey e Johnson, GT37). A redução para o caso direcionado:

Dado `G = (V, E)` não direcionado, construa `G' = (V, A)` com `A = { (u,v), (v,u) | {u,v} ∈ E }`. A construção percorre as arestas uma vez.

Se `(v1, …, vn)` é ciclo em `G`, cada aresta vira arco de `G'` nos dois sentidos, e a mesma sequência é ciclo direcionado. Se a sequência é ciclo direcionado em `G'`, cada arco usado veio de uma aresta de `G`, e a sequência é ciclo em `G`.

A redução é polinomial e correta. CHD é NP-difícil. Com a seção 4.1, CHD é NP-completo. O problema clássico correspondente é o Directed Hamiltonian Circuit (Garey e Johnson, GT38).

A redução produz um digrafo simétrico. Isso basta: o caso especial já é difícil, logo o caso geral também é.

### 4.3 Filtro que não resolve o problema

Ciclo hamiltoniano direcionado implica digrafo fortemente conexo. A volta é falsa, e o exemplo da seção 2 é o contraexemplo que vai para o relatório. O filtro entra na implementação como poda correta. A prova deixa explícito que ele não coloca o problema em P.

## 5. Estratégias

Representação comum: matriz booleana `n × n` e listas de saída. `n` de experimento heurístico fica na casa das centenas, então a matriz cabe.

Assinaturas previstas:

```python
def verifica_ciclo(g: Digrafo, ciclo: list[int]) -> bool: ...
def forca_bruta(g: Digrafo, cont: Contador) -> list[int] | None: ...
def backtracking(g: Digrafo, cont: Contador, ordem: str = "warnsdorff") -> list[int] | None: ...
def held_karp(g: Digrafo, cont: Contador) -> list[int] | None: ...
def guloso(g: Digrafo, cont: Contador, criterio: str = "indice") -> list[int] | None: ...
def recozimento(g: Digrafo, cont: Contador, rng: random.Random, params: Params) -> ResultadoHeuristica: ...
def ciclo_em_torneio(g: Digrafo, cont: Contador) -> list[int] | None: ...
```

`None` nos métodos exatos significa que o ciclo não existe. No recozimento, o objeto de resultado separa `ciclo_valido`, `fitness` e o motivo (`ciclo` ou `orcamento`).

### 5.1 Força bruta

Fixa o vértice 0 na origem. Gera as `(n−1)!` permutações do restante. Para cada uma, consulta os `n` arcos do ciclo.

Tempo `Θ(n · (n−1)!)`. Espaço `O(n)` além da matriz, gerando a permutação no lugar. Não há poda: poda é o que distingue o backtracking na comparação.

Uso experimental: juiz das outras implementações até o `n` em que o fatorial ainda termina. Na prática, até `n = 11`.

### 5.2 Backtracking

Busca em profundidade. O caminho começa em 0. O próximo vértice é um vizinho de saída ainda fora do caminho. Com `n−1` vértices no caminho, o único teste é o arco de volta a 0.

Podas corretas:

- vizinho já usado;
- digrafo que falhou no filtro de conectividade forte, recusado antes da busca.

A ordem dos vizinhos muda o tamanho da árvore e preserva a resposta. O experimento compara duas ordens no mesmo código: índice crescente, e a regra de Warnsdorff adaptada (primeiro o vizinho com menos saídas ainda livres). A ordenação é detalhe de implementação do backtracking, não um algoritmo guloso.

Pior caso `O(n!)`. Na prática a árvore depende da densidade. Espaço `O(n)` de pilha. O limite de recursão do Python cobre os `n` deste trabalho.

O contador registra nós expandidos e consultas de arco. O relatório mostra mediana e máximo dos nós, porque a variância entre digrafos do mesmo `n` é parte do resultado.

### 5.3 Programação dinâmica (Held-Karp)

Estado: `dp[S][v]` é verdadeiro quando existe caminho simples que começa em 0, visita exatamente o conjunto `S` e termina em `v`, com `0 ∈ S` e `v ∈ S`.

Base: `dp[{0}][0] = verdadeiro`.

Transição: `dp[S][v]` é verdadeiro quando existe `u ∈ S \ {v}` com `dp[S \ {v}][u]` verdadeiro e arco `(u, v)`.

Há ciclo quando existe `v ≠ 0` com `dp[V][v]` verdadeiro e arco `(v, 0)`.

Implementação por máscara de bits. Máscaras em ordem crescente: a máscara sem o bit de `v` é menor e já foi calculada. A reconstrução guarda o predecessor de cada estado, para devolver o ciclo e não só o booleano.

Tempo `Θ(n² · 2ⁿ)`. Espaço `Θ(n · 2ⁿ)` com a tabela de predecessores. Em Python, o alvo confortável é `n ≤ 20`. O tempo depende de `n` e não do número de arcos, quando a matriz já está montada. Essa previsibilidade é o contraste com o backtracking na discussão dos resultados.

Referências: Held e Karp (1962); Bellman (1962).

### 5.4 Guloso

Dois critérios, mesma estrutura. Começa em 0. Enquanto houver vértice livre, escolhe um vizinho de saída livre. Se empacar antes de cobrir `V`, ou se o último não apontar para 0, devolve falha.

Critério `indice`: o menor índice. Contraexemplo que o teste automatiza.

Arcos: `(0,1), (0,2), (2,3), (3,1), (1,0)`. O ciclo `0 → 2 → 3 → 1 → 0` existe. O guloso por índice sai de 0 para 1, e de 1 só há arco para 0, que já está no caminho. Falha numa instância sim.

Critério `warnsdorff`: o vizinho livre com menos saídas livres. Contraexemplo:

Arcos: `(0,1), (0,2), (1,2), (1,3), (2,3), (3,0)`. O ciclo `0 → 1 → 2 → 3 → 0` existe. Na saída de 0, o vértice 1 tem duas saídas livres e o vértice 2 tem uma. A regra escolhe 2, segue a 3 e empaca com 1 ainda de fora.

Tempo `O(n²)` numa partida a partir da origem fixa. Espaço `O(n)`. O texto classifica os dois como heurísticas gulosas, sem garantia de otimalidade e sem razão de aproximação.

### 5.5 Heurística: recozimento simulado

A versão de otimização usada pela heurística: entre as permutações cíclicas com origem 0, maximizar o número de arcos presentes nas `n` posições. O fitness vale de 0 a `n`. A instância é sim exatamente quando o máximo vale `n`.

Essa reformulação é o que permite busca local. Ela não cria uma razão de aproximação, e o relatório diz isso com essa frase.

Movimento: retirar um vértice e inseri-lo noutra posição (relocação). A variação do fitness depende só dos arcos que entram e saem da posição antiga e da nova, e é `O(1)`.

Aceita melhora sempre. Aceita piora com probabilidade `exp(Δ / T)`. Esfriamento `T ← α T`. Vários recomeços a partir de permutações aleatórias. Para ao achar fitness `n` ou ao gastar o orçamento.

Parâmetros declarados antes da bateria final, ajustados só num piloto separado das instâncias do relatório:

- `T0 = 1`
- `α = 0,995`
- movimentos por recomeço: `200 · n`
- recomeços: 10

O piloto pode comparar `α ∈ {0,99, 0,995, 0,999}` e, depois da escolha, o valor fica congelado. A tabela do piloto entra no relatório como escolha de parâmetro, separada dos resultados finais.

Tempo proporcional ao orçamento. Espaço `O(n)`.

Referência do método: Kirkpatrick, Gelatt e Vecchi (1983).

### 5.6 Caso especial, fora das seis estratégias

Teorema de Camion (1959): um torneio tem ciclo hamiltoniano direcionado se e somente se é fortemente conexo.

Construção, quando o torneio é forte e `n ≥ 3`: obter um ciclo de comprimento 3 e inserir os vértices restantes. Para um vértice fora do ciclo, a propriedade de torneio garante uma posição `ci → v → c{i+1}`. Tempo `O(n²)`.

O módulo recusa o digrafo que não é torneio. No relatório, este algoritmo mostra a fronteira da dificuldade: a mesma pergunta, restrita a torneios fortes, cai em P. Ele não é a resposta de divisão e conquista do caso geral.

## 6. Divisão e conquista

Divisão e conquista, no sentido de Cormen, parte a entrada em subproblemas independentes, resolve cada um e combina. No CHD a combinação não fica independente do miolo.

Um ciclo hamiltoniano de `G` não é a união de ciclos hamiltonianos dos subdigrafos induzidos por uma partição de `V`. Para uma partição `L` e `R`, o ciclo pode alternar entre os dois lados várias vezes.

O atalho que parece divisão — permutar um bloco e depois o outro — responde outra pergunta. Contraexemplo, com origem 0, `L = {1, 2}` e `R = {3}`:

As concatenações de bloco são só `(0,1,2,3)`, `(0,2,1,3)`, `(0,3,1,2)` e `(0,3,2,1)`.

O digrafo com os arcos `(0,1), (1,3), (3,2), (2,0)` tem o ciclo `0 → 1 → 3 → 2 → 0` e nenhuma das quatro sequências. O método de blocos responde não numa instância sim.

A decomposição que funciona usa outro subproblema: "caminho simples sobre o conjunto `S` terminando em `v`". Esses subproblemas se repetem ao longo da busca. Resolver cada um uma vez e reutilizar é programação dinâmica, e é a seção 5.3.

Há algoritmos de pesquisa mais recentes, em particular inclusão-exclusão para o caso não direcionado (Björklund). Eles ficam de fora do código. Uma frase no texto mostra que o grupo viu o estado da arte e que isso não é a divisão e conquista do curso. O algoritmo exato de referência do trabalho continua sendo Held-Karp, em `Θ(n² · 2ⁿ)`.

## 7. Medição

O edital separa contagem de instruções e tempo. O grupo mantém essa separação.

Contador, compartilhado por todos os algoritmos:

- `consultas_aresta`: cada leitura na matriz;
- `escritas`: extensão do caminho, ou preenchimento de um estado da PD;
- `nos`: nós da árvore, transições da PD, ou movimentos da heurística, conforme o algoritmo.

Os três números saem em colunas próprias. O relatório não soma contagens de algoritmos diferentes num "total de instruções" único: uma transição de Held-Karp e um teste de permutação não são a mesma operação. O eixo comum de comparação entre estratégias é o tempo de parede.

Tempo: `time.perf_counter`, em milissegundos, mediana de repetições quando uma execução é curta. Timeout de 30 s nos métodos exatos. Estouro de tempo entra na tabela como dado censurado, com o `n` e a família, e não é apagado.

`cProfile` pode aparecer numa frase de sanidade. Ele não é a contagem oficial, porque mistura o custo do interpretador.

## 8. Experimentos

Semente `20261107`. Cada família tem gerador próprio e resposta conhecida quando a construção garante sim ou não.

| Família | Resposta | Papel |
| --- | --- | --- |
| Ciclo direcionado mais arcos aleatórios | sim | custo quando a solução existe |
| DAG aleatório (arcos só para a frente) | não | efeito do filtro de conectividade |
| O mesmo DAG sem o filtro, só no backtracking | não | ablação: quantos nós a poda economiza |
| Erdős–Rényi direcionado, `p` fixo e `p` na ordem de `(ln n) / n` | mista | região fácil, região de limiar |
| Torneio aleatório | sim quando forte | guloso, heurística e Camion no mesmo terreno |
| Armadilhas da seção 5.4 | sim, e o guloso erra | o teste de correção vira figura do relatório |
| Forte sem ciclo, seção 2 | não | o filtro não substitui o algoritmo |

Juiz:

- até o alcance da força bruta, os três exatos precisam concordar, e o ciclo devolvido passa por `verifica_ciclo`;
- até o alcance de Held-Karp, a PD julga guloso, recozimento e Camion;
- acima desse `n`, o relatório publica "ciclo válido encontrado" ou "orçamento esgotado". A palavra "inexistente" fica reservada aos métodos exatos.

Tamanhos previstos, a ajustar se o tempo da máquina apertar, sem mudar as famílias:

- força bruta: `n = 6` a `11`;
- backtracking: `n = 6` a `16`, e além disso só nas famílias esparsas em que a mediana ficar abaixo do timeout;
- Held-Karp: `n = 6` a `20`;
- guloso, recozimento e Camion: `n = 6` a `150`, Camion só em torneio.

Repetições: 30 digrafos por célula na região de limiar, 20 nas outras. Armadilhas e o forte-sem-ciclo são instâncias fixas.

Gráficos que o script precisa produzir, e que a seção de resultados discute um a um:

1. Tempo mediano contra `n`, escala logarítmica, uma curva por estratégia, um painel por família.
2. Consultas de arco e nós contra `n`, para força bruta, backtracking e Held-Karp.
3. Nós do backtracking com filtro e sem filtro, na família DAG.
4. Taxa de ciclo válido do guloso e do recozimento contra `p`, com a PD como juiz no `n` em que ela cabe.
5. Fitness final dividido por `n` quando o recozimento para por orçamento.
6. Taxa de ciclo válido contra o número de movimentos, `n` fixo, para justificar o orçamento congelado.

Pontos que a discussão precisa alcançar, porque é aqui que a nota se diferencia:

- Por volta de `n ≤ 7` o fatorial ainda compete com `n² 2ⁿ`. A partir daí Held-Karp cresce mais devagar. O gráfico teórico em log10 já mostra o cruzamento; a medição serve para ver se a constante da implementação em Python empurra esse cruzamento.
- Held-Karp é estável: o tempo muda pouco entre famílias do mesmo `n`. O backtracking é estável em DAG com filtro e tem cauda longa em digrafo denso. Mediana e máximo contam histórias diferentes; os dois aparecem.
- O guloso é rápido e erra nas armadilhas. Taxa alta em torneio denso não cancela o contraexemplo.
- O recozimento recupera parte das falhas do guloso e continua sem certificado negativo.
- A memória da PD limita `n` antes do tempo, na casa de 20 em Python. A conclusão cita esse teto com o número medido na máquina do grupo.

Referência para a família aleatória: Frieze (1988) trata ciclo hamiltoniano em digrafo aleatório; o limiar acompanha o aparecimento de grau de entrada e de saída pelo menos 1, na ordem de `(ln n) / n`. O experimento usa essa ordem como região interessante, sem tratar uma amostra finita como demonstração do teorema.

## 9. Aplicações

O edital dá o maior peso à profundidade da apresentação e, em particular, às aplicações. Cada aplicação cabe em cerca de uma página no escrito e em cerca de um minuto na oral. As quatro abaixo são as que o grupo sustenta. Cada uma nomeia o que é vértice, o que é arco e por que a volta ao início importa.

### 9.1 Montagem de genoma circular

Na formulação clássica por hibridização, fragmentos orientados são vértices e uma sobreposição compatível é um arco. Um ciclo hamiltoniano nesse digrafo corresponde a uma sequência circular que usa cada fragmento uma vez, o modelo de plasmídeo e de genoma circular. A prática migrou para caminho euleriano no grafo de de Bruijn, justamente porque o ciclo hamiltoniano é NP-completo e o caminho euleriano é polinomial. Essa migração é o ponto da seção: o problema do trabalho explica uma mudança real de modelo. Referência de leitura: Compeau, Pevzner e Tesler (2011).

### 9.2 Rotas em malha de mão única

Cruzamentos ou clientes são vértices. Um trecho que pode ser percorrido naquele sentido é um arco. O ciclo é a volta à base visitando cada ponto uma vez. Com custo nos arcos e um limite `B`, a pergunta vira o caixeiro viajante assimétrico, outro tema do edital. O CHD é o caso em que o arco permitido custa 1, o arco ausente é proibido e `B = n`. A heurística do trabalho é o tipo de ferramenta que resta quando a instância passa do alcance de Held-Karp. O texto não apresenta o recozimento como algoritmo aproximado do ATSP com razão provada.

### 9.3 Teste de máquinas de estados

Estados de um protocolo são vértices. Transições são arcos. Uma suíte que exercita cada estado e precisa recomeçar do estado inicial, de forma periódica, pede um ciclo que passe por todos os estados. Cobertura de transições é outra pergunta, ligada a percurso euleriano no grafo de transições. A seção separa as duas, para não misturar ciclo hamiltoniano com posteiro chinês. Referência de enquadramento: Lee e Yannakakis (1996). Na prática, protocolos grandes usam heurística pelo mesmo motivo da seção 9.2.

### 9.4 Ranking circular em torneio

Competidores são vértices. Vitória é arco. Todo par tem exatamente um sentido, então o digrafo é um torneio. Se ele é forte, Camion garante um ciclo hamiltoniano: uma ordenação circular coerente com as vitórias. Este é o caso em que a pergunta deixa de ser NP-completa. Ele fecha a discussão das aplicações mostrando que a dificuldade mora na estrutura do digrafo, não no nome do problema.

## 10. PDF

Orçamento para o escrito não estourar em listagem e ficar curto nas aplicações:

| Seção | Conteúdo | Páginas-alvo |
| --- | --- | --- |
| Capa | nomes em ordem alfabética, matrículas, tema, turma | 1 |
| Introdução | definição, exemplos sim e não, as quatro aplicações | 7 |
| Prova | verificador, redução, filtro que não basta | 4 |
| Implementação | as cinco estratégias com código, Camion, justificativa da divisão e conquista com o contraexemplo dos blocos | 10 |
| Complexidade | fórmulas, o que o contador mede, espaço da PD | 4 |
| Resultados | os seis gráficos e a discussão da seção 8 | 7 |
| Conclusão | teto de `n`, guloso sem garantia, heurística sem certificado negativo, dificuldade de implementar a PD com reconstrução | 2 |
| Bibliografia | só obras citadas no texto | 1 |

O código colado é o das funções de algoritmo, não o gerador nem o script de gráfico. Comentários curtos ficam no que a voz da apresentação precisaria explicar: a origem fixa, o sentido livre, o significado de `dp[S][v]`, a parada da heurística.

## 11. Quatro papéis

O edital exige quatro integrantes. A divisão abaixo dá a cada pessoa um trecho que ela consegue defender se a banca perguntar.

| Papel | Produz | Defende |
| --- | --- | --- |
| A, teoria | introdução, prova, aplicação do genoma, bibliografia cruzada | verificador e redução |
| B, exatos combinatórios | força bruta, backtracking, contador, testes de concordância | simetria da origem e efeito da poda |
| C, programação dinâmica | Held-Karp com reconstrução, texto da divisão e conquista, Camion, aplicação do torneio | recorrência e o falso negativo dos blocos |
| D, inexatos e medição | dois gulosos, recozimento, geradores, CSV, gráficos, aplicação das rotas | a diferença entre orçamento esgotado e inexistência |

Os quatro escrevem juntos a aplicação de máquinas de estados, a conclusão e a revisão. Quem escreveu uma seção não é o único revisor dela. Os dois apresentadores saem de quem ensaiar com mais folga; a escolha fecha depois de um ensaio cronometrado. Os quatro permanecem na arguição.

## 12. Oral de 20 minutos

| Minuto | Bloco | Quem sustenta |
| --- | --- | --- |
| 0–2 | Definição, exemplo sim, exemplo forte sem ciclo | A |
| 2–5 | Verificador em NP e a redução, numa figura | A |
| 5–9 | As estratégias, o cruzamento fatorial versus `2ⁿ`, a justificativa da divisão e conquista | B e C |
| 9–14 | Três gráficos: tempo, ablação do filtro, taxa da heurística. A armadilha do guloso | D |
| 14–18 | Aplicações: genoma, rota de mão única, estados, torneio | A e D |
| 18–20 | Dificuldades: memória da PD, heurística sem certificado negativo | C |

O ensaio usa cronômetro. O notebook da apresentação abre os gráficos já gerados e, se sobrar pergunta, roda o contraexemplo do guloso. A banca não precisa esperar uma bateria de `n = 20`.

## 13. Árvore prevista

```text
chd/
  digrafo.py          # matriz, listas de saída, conectividade forte
  contador.py
  verifica.py
  forca_bruta.py
  backtracking.py
  held_karp.py
  guloso.py
  recozimento.py
  torneio.py          # Camion, só torneio
  gerador.py
  experimentos.py
testes/
  test_concordancia.py
  test_armadilhas.py
  test_torneio.py
docs/
  PLANO.md
  relatorio/          # LaTeX, quando a redação começar
dados/                # CSV gerado, não escrito à mão
graficos/
```

Cada arquivo de algoritmo permanece pequeno o bastante para entrar no PDF sem corte opaco. Nomes de função em português, sem acento, para o código e o texto usarem as mesmas palavras.

## 14. Bibliografia de partida

As fichas completas fecham na redação, no formato que o grupo adotar no LaTeX. A lista mínima, toda ela com um ponto de citação já marcado neste plano:

- Bellman, R. Dynamic programming treatment of the travelling salesman problem. Journal of the ACM, 1962.
- Björklund, A. Determinant sums for undirected Hamiltonicity. FOCS, 2010. Uma frase na justificativa da divisão e conquista.
- Camion, P. Chemins et circuits hamiltoniens des graphes complets. Comptes Rendus de l'Académie des Sciences, 1959.
- Compeau, P.; Pevzner, P.; Tesler, G. How to apply de Bruijn graphs to genome assembly. Nature Biotechnology, 2011.
- Cormen, T. H.; Leiserson, C. E.; Rivest, R. L.; Stein, C. Introduction to Algorithms. 3. ed. MIT Press, 2009. Referência [1] do edital.
- Frieze, A. M. An algorithm for finding Hamilton cycles in random directed graphs. Journal of Algorithms, 1988.
- Garey, M. R.; Johnson, D. S. Computers and Intractability. Freeman, 1979. GT37 e GT38.
- Held, M.; Karp, R. M. A dynamic programming approach to sequencing problems. Journal of the Society for Industrial and Applied Mathematics, 1962.
- Karp, R. M. Reducibility among combinatorial problems. In: Complexity of Computer Computations. 1972.
- Kirkpatrick, S.; Gelatt, C. D.; Vecchi, M. P. Optimization by simulated annealing. Science, 1983.
- Lee, D.; Yannakakis, M. Principles and methods of testing finite state machines — a survey. Proceedings of the IEEE, 1996.
- Ziviani, N. Projeto de Algoritmos com implementações em Java e C++. Cengage Learning, 2006. Referência [2] do edital.

Warnsdorff entra como adaptação da regra de passeio do cavalo, com a frase de que a ordem dos vizinhos no backtracking herda a ideia e não herda uma prova.

## 15. Riscos

- Entregar um "divisão e conquista" que erra o contraexemplo da seção 6. A defesa escrita existe para evitar isso.
- Tratar falha do recozimento como prova de que o ciclo não existe. O protocolo da seção 8 fecha essa leitura.
- Publicar razão de aproximação para o recozimento ou para o guloso. Nenhum dos dois tem.
- Colar código de outro grupo. O edital zera os dois lados.
- Gastar a oral lendo listagem. O roteiro da seção 12 põe aplicações e resultados em 9 dos 20 minutos.
- Deixar a PD sem reconstrução e mostrar só sim ou não. A versão de busca está na decisão 2.
- Somar contadores de estratégias diferentes e chamar a soma de complexidade. A seção 7 proíbe essa conta.

## 16. Ordem de execução

A ordem deixa um resultado verificável cedo: primeiro o verificador e a força bruta, que julgam o resto.

1. Fechar os quatro nomes, as matrículas, a turma e o nome do ZIP.
2. Implementar digrafo, conectividade forte, verificador e contador.
3. Implementar a força bruta e os testes dos exemplos da seção 2.
4. Implementar o backtracking e exigir concordância com a força bruta até `n = 10`, nas famílias sim, não e aleatória pequena.
5. Implementar Held-Karp com reconstrução e repetir a concordância.
6. Escrever a justificativa da seção 6 enquanto o contraexemplo dos blocos vira teste que documenta a falha daquele atalho.
7. Implementar Camion e testar em torneio forte e em torneio que não é forte.
8. Implementar os dois gulosos com as armadilhas da seção 5.4.
9. Implementar o recozimento, rodar o piloto de `α`, congelar o parâmetro.
10. Rodar a bateria da seção 8, gerar CSV e os seis gráficos.
11. Redigir o PDF na ordem das seções, com citação no ponto de uso.
12. Ensaio cronometrado e geração do ZIP.

O passo 4 é o primeiro marco em que o grupo já tem um programa correto para mostrar. Os passos 5 e 8 são os marcos seguintes. Gráfico sem concordância prévia não entra no relatório.
