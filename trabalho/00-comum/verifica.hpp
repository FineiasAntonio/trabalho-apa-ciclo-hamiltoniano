#pragma once

#include "00-comum/digrafo.hpp"

#include <vector>

namespace chd {

bool verifica_ciclo(const Digrafo& g, const std::vector<int>& ciclo);

}
