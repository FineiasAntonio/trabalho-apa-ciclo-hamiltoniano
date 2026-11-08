#include "00-comum/gerador.hpp"

#include <random>

namespace chd {
namespace {

Digrafo ciclo_simples(int n) {
  Digrafo g(n);
  for (int i = 0; i < n; ++i) {
    g.adiciona_arco(i, (i + 1) % n);
  }
  return g;
}

Digrafo forte_sem_ciclo() {
  Digrafo g(4);
  g.adiciona_arco(0, 1);
  g.adiciona_arco(1, 2);
  g.adiciona_arco(2, 0);
  g.adiciona_arco(1, 3);
  g.adiciona_arco(3, 0);
  return g;
}

Digrafo pior_caso(int n) {
  Digrafo g(n);
  for (int u = 0; u < n - 1; ++u) {
    for (int v = 0; v < n - 1; ++v) {
      g.adiciona_arco(u, v);
    }
  }
  g.adiciona_arco(1, n - 1);
  g.adiciona_arco(n - 1, 1);
  return g;
}

Digrafo armadilha_indice() {
  Digrafo g(4);
  g.adiciona_arco(0, 1);
  g.adiciona_arco(0, 2);
  g.adiciona_arco(2, 3);
  g.adiciona_arco(3, 1);
  g.adiciona_arco(1, 0);
  return g;
}

Digrafo armadilha_warnsdorff() {
  Digrafo g(4);
  g.adiciona_arco(0, 1);
  g.adiciona_arco(0, 2);
  g.adiciona_arco(1, 2);
  g.adiciona_arco(1, 3);
  g.adiciona_arco(2, 3);
  g.adiciona_arco(3, 0);
  return g;
}

Digrafo aleatorio(int n) {
  std::mt19937 rng(20261107u + static_cast<unsigned>(n));
  std::bernoulli_distribution moeda(0.35);
  Digrafo g(n);
  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (moeda(rng)) {
        g.adiciona_arco(u, v);
      }
    }
  }
  return g;
}

} 

std::optional<Digrafo> gera(const std::string& tipo, int n) {
  if (tipo == "ciclo_simples") {
    return ciclo_simples(n);
  }
  if (tipo == "forte_sem_ciclo") {
    return forte_sem_ciclo();
  }
  if (tipo == "pior_caso") {
    return pior_caso(n);
  }
  if (tipo == "armadilha_indice") {
    return armadilha_indice();
  }
  if (tipo == "armadilha_warnsdorff") {
    return armadilha_warnsdorff();
  }
  if (tipo == "aleatorio") {
    return aleatorio(n);
  }
  return std::nullopt;
}

} 
