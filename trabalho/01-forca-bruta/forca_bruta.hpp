#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

std::optional<std::vector<int>> forca_bruta(const Digrafo& g, Contador& contador);

}  // namespace chd
