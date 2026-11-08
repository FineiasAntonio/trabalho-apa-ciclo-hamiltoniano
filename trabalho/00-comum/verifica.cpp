#include "00-comum/verifica.hpp"

namespace chd {

bool verifica_ciclo(const Digrafo& g, const std::vector<int>& ciclo) {
  const int n = g.n();
  if (n < 2 || static_cast<int>(ciclo.size()) != n) {
    return false;
  }
  std::vector<char> visto(n, 0);
  for (int v : ciclo) {
    if (v < 0 || v >= n || visto[v]) {
      return false;
    }
    visto[v] = 1;
  }
  Contador descartavel;
  for (int i = 0; i < n; ++i) {
    if (!g.consulta_arco(ciclo[i], ciclo[(i + 1) % n], descartavel)) {
      return false;
    }
  }
  return true;
}

}
