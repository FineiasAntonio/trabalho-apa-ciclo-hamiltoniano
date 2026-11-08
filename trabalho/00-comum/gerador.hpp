#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <string>

namespace chd {

std::optional<Digrafo> gera(const std::string& tipo, int n);

}
