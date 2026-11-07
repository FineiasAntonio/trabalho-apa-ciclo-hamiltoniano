#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

// Teorema de Camion: torneio tem ciclo hamiltoniano direcionado
// exatamente quando é fortemente conexo.
// nullopt em digrafo que não é torneio significa recusa, não inexistência.
// nullopt em torneio significa que o ciclo não existe.
// Se o torneio é forte e a construção falhar, a função lança std::logic_error.
std::optional<std::vector<int>> ciclo_em_torneio(const Digrafo& g, Contador& contador);

}  // namespace chd
