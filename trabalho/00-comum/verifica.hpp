#pragma once

#include "digrafo.hpp"

#include <vector>

namespace chd {

// Verificador do certificado: a sequência é um ciclo hamiltoniano direcionado.
// Roda em O(n) com a matriz. É a prova de que o problema está em NP, em código.
bool verifica_ciclo(const Digrafo& g, const std::vector<int>& ciclo, Contador& contador);

}  // namespace chd
