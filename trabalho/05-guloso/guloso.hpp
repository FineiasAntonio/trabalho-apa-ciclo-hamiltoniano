#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

enum class Criterio { Indice, Warnsdorff };

std::optional<std::vector<int>> guloso(const Digrafo& g, Contador& contador, Criterio criterio);

}
