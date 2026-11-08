CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Itrabalho

COMUM := trabalho/00-comum/verifica.cpp trabalho/00-comum/gerador.cpp

.PHONY: all clean

all: trabalho/bin/forca_bruta trabalho/bin/backtracking trabalho/bin/held_karp trabalho/bin/guloso

trabalho/bin/forca_bruta: $(COMUM) trabalho/01-forca-bruta/forca_bruta.cpp trabalho/01-forca-bruta/main.cpp
	mkdir -p trabalho/bin
	$(CXX) $(CXXFLAGS) $^ -o $@

trabalho/bin/backtracking: $(COMUM) trabalho/02-backtracking/backtracking.cpp trabalho/02-backtracking/main.cpp
	mkdir -p trabalho/bin
	$(CXX) $(CXXFLAGS) $^ -o $@

trabalho/bin/held_karp: $(COMUM) trabalho/04-programacao-dinamica/held_karp.cpp trabalho/04-programacao-dinamica/main.cpp
	mkdir -p trabalho/bin
	$(CXX) $(CXXFLAGS) $^ -o $@

trabalho/bin/guloso: $(COMUM) trabalho/05-guloso/guloso.cpp trabalho/05-guloso/main.cpp
	mkdir -p trabalho/bin
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf trabalho/bin
