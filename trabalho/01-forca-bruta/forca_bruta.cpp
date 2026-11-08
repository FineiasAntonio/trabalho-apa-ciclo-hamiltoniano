#include "01-forca-bruta/forca_bruta.hpp"

#include <algorithm>

namespace chd {

std::optional<std::vector<int>> forca_bruta(const Digrafo& g, Contador& contador) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }

  std::vector<int> ciclo(n);
  for (int i = 0; i < n; ++i) {
    ciclo[i] = i;
  }

  do {
    contador.permutacoes++;

    bool fecha = true;
    for (int i = 0; i < n; ++i) {
      if (!g.consulta_arco(ciclo[i], ciclo[(i + 1) % n], contador)) {
        fecha = false;
        break;
      }
    }
    if (fecha) {
      return ciclo;
    }
  } while (std::next_permutation(ciclo.begin() + 1, ciclo.end()));

  return std::nullopt;
}

} 
