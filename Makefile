CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Itrabalho

COMUM := trabalho/00-comum/verifica.cpp trabalho/00-comum/gerador.cpp

.PHONY: all clean

all: trabalho/bin/forca_bruta

trabalho/bin/forca_bruta: $(COMUM) trabalho/01-forca-bruta/forca_bruta.cpp trabalho/01-forca-bruta/main.cpp
	mkdir -p trabalho/bin
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf trabalho/bin
