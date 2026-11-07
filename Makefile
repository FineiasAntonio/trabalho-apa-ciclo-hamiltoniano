CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Itrabalho

FONTES := \
	trabalho/00-comum/digrafo.cpp \
	trabalho/00-comum/verifica.cpp \
	trabalho/01-forca-bruta/forca_bruta.cpp \
	trabalho/02-backtracking/backtracking.cpp \
	trabalho/03-divisao-e-conquista/ciclo_por_blocos.cpp \
	trabalho/04-programacao-dinamica/held_karp.cpp \
	trabalho/05-guloso/guloso.cpp \
	trabalho/06-heuristica/recozimento.cpp \
	trabalho/07-torneio/torneio.cpp \
	trabalho/experimentos/gerador.cpp

TESTE := trabalho/testes/testar
EXPERIMENTO := trabalho/experimentos/experimentos

.PHONY: all test experimentos clean

all: $(TESTE) $(EXPERIMENTO)

$(TESTE): $(FONTES) trabalho/testes/testar.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(EXPERIMENTO): $(FONTES) trabalho/experimentos/experimentos.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TESTE)
	./$(TESTE)

experimentos: $(EXPERIMENTO)
	mkdir -p trabalho/dados trabalho/graficos
	./$(EXPERIMENTO) --saida trabalho/dados/resultados.csv --resumo trabalho/dados/resumo.csv

clean:
	rm -f $(TESTE) $(EXPERIMENTO)
