#include "00-comum/digrafo.hpp"

#include <algorithm>
#include <stdexcept>

namespace chd {

Digrafo::Digrafo(int n)
    : n_(n),
      matriz_(n, std::vector<char>(n, 0)),
      saida_(n),
      entrada_(n) {
  if (n < 0) {
    throw std::invalid_argument("digrafo com ordem negativa");
  }
}

void Digrafo::adiciona_arco(int u, int v) {
  if (u < 0 || v < 0 || u >= n_ || v >= n_) {
    throw std::out_of_range("vertice fora do digrafo");
  }
  if (u == v || matriz_[u][v]) {
    return;
  }
  matriz_[u][v] = 1;
  auto insere = [](std::vector<int>& lista, int x) {
    auto it = std::lower_bound(lista.begin(), lista.end(), x);
    lista.insert(it, x);
  };
  insere(saida_[u], v);
  insere(entrada_[v], u);
}

bool Digrafo::tem_arco(int u, int v) const {
  return matriz_[u][v] != 0;
}

bool Digrafo::consulta_arco(int u, int v, Contador& contador) const {
  contador.consultas_aresta++;
  return matriz_[u][v] != 0;
}

bool Digrafo::fortemente_conexo() const {
  if (n_ == 0) {
    return true;
  }
  std::vector<char> visto(n_, 0);
  std::vector<int> ordem;
  ordem.reserve(n_);

  auto dfs_saida = [&](auto&& self, int u) -> void {
    visto[u] = 1;
    for (int v : saida_[u]) {
      if (!visto[v]) {
        self(self, v);
      }
    }
    ordem.push_back(u);
  };
  for (int u = 0; u < n_; ++u) {
    if (!visto[u]) {
      dfs_saida(dfs_saida, u);
    }
  }

  std::vector<char> visto_volta(n_, 0);
  int componentes = 0;
  auto dfs_entrada = [&](auto&& self, int u) -> void {
    visto_volta[u] = 1;
    for (int v : entrada_[u]) {
      if (!visto_volta[v]) {
        self(self, v);
      }
    }
  };
  for (int i = n_ - 1; i >= 0; --i) {
    int u = ordem[i];
    if (!visto_volta[u]) {
      dfs_entrada(dfs_entrada, u);
      componentes++;
    }
  }
  return componentes == 1;
}

bool Digrafo::eh_torneio() const {
  for (int u = 0; u < n_; ++u) {
    if (matriz_[u][u]) {
      return false;
    }
    for (int v = u + 1; v < n_; ++v) {
      if (matriz_[u][v] == matriz_[v][u]) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace chd
