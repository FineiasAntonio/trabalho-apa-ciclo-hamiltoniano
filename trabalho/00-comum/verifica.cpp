#include "00-comum/verifica.hpp"

namespace chd {

bool verifica_ciclo(const Digrafo& g, const std::vector<int>& ciclo, Contador& contador) {
  const int n = g.n();
  if (static_cast<int>(ciclo.size()) != n || n < 2) {
    return false;
  }
  std::vector<char> visto(n, 0);
  for (int v : ciclo) {
    if (v < 0 || v >= n || visto[v]) {
      return false;
    }
    visto[v] = 1;
  }
  for (int i = 0; i < n; ++i) {
    int proximo = ciclo[(i + 1) % n];
    if (!g.consulta_arco(ciclo[i], proximo, contador)) {
      return false;
    }
  }
  return true;
}

}  // namespace chd
