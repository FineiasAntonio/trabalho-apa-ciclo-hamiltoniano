#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

// Atalho que parece divisão e conquista e não resolve o CHD.
//
// Parte os vértices 1..n-1 em dois blocos L e R e só testa ciclos em que
// um bloco inteiro aparece antes do outro. Um ciclo que alterna os blocos
// fica de fora: o procedimento responde vazio numa instância que tem ciclo.
// nullopt aqui significa "nenhum ciclo com os blocos contíguos", e não
// "o digrafo não tem ciclo hamiltoniano".
//
// Para n = 4 a partição é L = {1, 2} e R = {3}, o contraexemplo do plano.
// O demonstrador só enumera até n = 10.
std::optional<std::vector<int>> ciclo_por_blocos_contiguos(const Digrafo& g, Contador& contador);

}  // namespace chd
