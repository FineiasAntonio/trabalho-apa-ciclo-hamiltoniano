#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

std::optional<std::vector<int>> held_karp(const Digrafo& g, Contador& contador);

}
