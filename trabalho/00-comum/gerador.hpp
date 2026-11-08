#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <string>

namespace chd {

unsigned semente_do_digrafo(int argc, char** argv);

std::optional<Digrafo> gera(const std::string& tipo, int n, unsigned semente);

}
