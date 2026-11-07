#pragma once

#include "00-comum/digrafo.hpp"
#include "00-comum/tempo.hpp"

#include <optional>
#include <vector>

namespace chd {

// Enumera as (n-1)! permutações dos vértices depois da origem fixa 0.
// O sentido não é fixado: a permutação inversa continua na lista.
// Devolve nullopt quando o ciclo não existe. Tempo esgotado lança TempoEsgotado.
std::optional<std::vector<int>> forca_bruta(const Digrafo& g, Contador& contador,
                                            const Relogio& relogio = {});

}  // namespace chd
