#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

std::optional<std::vector<int>> backtracking(const Digrafo& g, Contador& contador);

}
