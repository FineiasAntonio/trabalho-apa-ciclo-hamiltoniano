#pragma once

#include "00-comum/contador.hpp"

#include <vector>

namespace chd {

class Digrafo {
 public:
  explicit Digrafo(int n) : n_(n), matriz_(n, std::vector<char>(n, 0)) {}

  int n() const { return n_; }

  void adiciona_arco(int u, int v) {
    if (u != v) {
      matriz_[u][v] = 1;
    }
  }

  bool consulta_arco(int u, int v, Contador& contador) const {
    contador.consultas_arco++;
    return matriz_[u][v] != 0;
  }

 private:
  int n_;
  std::vector<std::vector<char>> matriz_;
};

}
