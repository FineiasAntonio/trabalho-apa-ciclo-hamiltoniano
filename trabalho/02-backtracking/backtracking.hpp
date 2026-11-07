#pragma once

#include "00-comum/digrafo.hpp"
#include "00-comum/tempo.hpp"

#include <optional>
#include <vector>

namespace chd {

// A ordem dos vizinhos muda o tamanho da árvore e preserva a resposta.
enum class OrdemBusca { Indice, Warnsdorff };

// Busca em profundidade a partir do vértice 0. usar_filtro desliga a
// conectividade forte só para a ablação em DAG; a resposta continua correta.
std::optional<std::vector<int>> backtracking(const Digrafo& g, Contador& contador,
                                             OrdemBusca ordem = OrdemBusca::Warnsdorff,
                                             bool usar_filtro = true,
                                             const Relogio& relogio = {});

}  // namespace chd
