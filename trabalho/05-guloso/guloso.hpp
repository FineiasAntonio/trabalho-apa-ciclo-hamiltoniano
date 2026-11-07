#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

// Heurística sem garantia. nullopt significa que esta partida empacou,
// e não que o ciclo não existe.
enum class CriterioGuloso { Indice, Warnsdorff };

std::optional<std::vector<int>> guloso(const Digrafo& g, Contador& contador,
                                       CriterioGuloso criterio = CriterioGuloso::Indice);

}  // namespace chd
