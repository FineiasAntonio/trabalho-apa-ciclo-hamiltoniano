#include "experimentos/gerador.hpp"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace chd {
namespace {

Digrafo com_arcos(int n, std::initializer_list<std::pair<int, int>> arcos) {
  Digrafo g(n);
  for (auto [u, v] : arcos) {
    g.adiciona_arco(u, v);
  }
  return g;
}

}  // namespace

Digrafo ciclo_direcionado(int n) {
  Digrafo g(n);
  for (int i = 0; i < n; ++i) {
    g.adiciona_arco(i, (i + 1) % n);
  }
  return g;
}

Digrafo ciclo_com_arcos_extras(int n, int extras, std::mt19937& rng) {
  Digrafo g = ciclo_direcionado(n);
  if (n < 2 || extras <= 0) {
    return g;
  }
  std::vector<std::pair<int, int>> livres;
  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (u != v && !g.tem_arco(u, v)) {
        livres.push_back({u, v});
      }
    }
  }
  std::shuffle(livres.begin(), livres.end(), rng);
  const int quantidade = std::min(extras, static_cast<int>(livres.size()));
  for (int i = 0; i < quantidade; ++i) {
    g.adiciona_arco(livres[i].first, livres[i].second);
  }
  return g;
}

Digrafo dag_aleatorio(int n, double p, std::mt19937& rng) {
  std::bernoulli_distribution moeda(p);
  Digrafo g(n);
  for (int u = 0; u < n; ++u) {
    for (int v = u + 1; v < n; ++v) {
      if (moeda(rng)) {
        g.adiciona_arco(u, v);
      }
    }
  }
  return g;
}

Digrafo erdos_renyi(int n, double p, std::mt19937& rng) {
  std::bernoulli_distribution moeda(p);
  Digrafo g(n);
  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (u != v && moeda(rng)) {
        g.adiciona_arco(u, v);
      }
    }
  }
  return g;
}

Digrafo torneio_aleatorio(int n, std::mt19937& rng) {
  std::bernoulli_distribution moeda(0.5);
  Digrafo g(n);
  for (int u = 0; u < n; ++u) {
    for (int v = u + 1; v < n; ++v) {
      if (moeda(rng)) {
        g.adiciona_arco(u, v);
      } else {
        g.adiciona_arco(v, u);
      }
    }
  }
  return g;
}

Digrafo armadilha_indice() {
  return com_arcos(4, {{0, 1}, {0, 2}, {2, 3}, {3, 1}, {1, 0}});
}

Digrafo armadilha_warnsdorff() {
  return com_arcos(4, {{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}, {3, 0}});
}

Digrafo forte_sem_ciclo() {
  return com_arcos(4, {{0, 1}, {1, 2}, {2, 0}, {1, 3}, {3, 0}});
}

Digrafo ciclo_intercalado() {
  return com_arcos(4, {{0, 1}, {1, 3}, {3, 2}, {2, 0}});
}

Digrafo ciclo_reverso(int n) {
  Digrafo g(n);
  g.adiciona_arco(0, n - 1);
  for (int i = n - 1; i >= 1; --i) {
    g.adiciona_arco(i, i - 1);
  }
  return g;
}

Digrafo digrafo_completo(int n) {
  Digrafo g(n);
  for (int u = 0; u < n; ++u) {
    for (int v = 0; v < n; ++v) {
      if (u != v) {
        g.adiciona_arco(u, v);
      }
    }
  }
  return g;
}

const char* nome_familia(Familia familia) {
  switch (familia) {
    case Familia::CicloExtra:
      return "ciclo_extra";
    case Familia::Dag:
      return "dag";
    case Familia::Erdos:
      return "erdos";
    case Familia::ErdosLimiar:
      return "erdos_limiar";
    case Familia::Torneio:
      return "torneio";
  }
  return "desconhecida";
}

Digrafo gera(Familia familia, int n, std::mt19937& rng) {
  switch (familia) {
    case Familia::CicloExtra:
      return ciclo_com_arcos_extras(n, n, rng);
    case Familia::Dag:
      return dag_aleatorio(n, 0.5, rng);
    case Familia::Erdos:
      return erdos_renyi(n, 0.35, rng);
    case Familia::ErdosLimiar: {
      const double p = n <= 1 ? 0.0 : std::min(1.0, std::log(static_cast<double>(n)) / n);
      return erdos_renyi(n, p, rng);
    }
    case Familia::Torneio:
      return torneio_aleatorio(n, rng);
  }
  throw std::invalid_argument("familia desconhecida");
}

}  // namespace chd
