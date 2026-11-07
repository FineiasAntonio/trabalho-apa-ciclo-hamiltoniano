#pragma once

#include "contador.hpp"

#include <vector>

namespace chd {

// Digrafo simples, vértices 0..n-1, sem laço. Matriz para consulta O(1)
// e listas de saída e de entrada para a busca.
class Digrafo {
 public:
  explicit Digrafo(int n);

  int n() const { return n_; }

  void adiciona_arco(int u, int v);

  // Leitura da matriz sem contar. Geradores e checagens internas usam esta.
  bool tem_arco(int u, int v) const;

  // Cada chamada conta uma consulta de aresta, mesmo quando o arco falta.
  bool consulta_arco(int u, int v, Contador& contador) const;

  const std::vector<int>& saida(int u) const { return saida_[u]; }
  const std::vector<int>& entrada(int u) const { return entrada_[u]; }

  // Condição necessária para um ciclo hamiltoniano, e não suficiente.
  bool fortemente_conexo() const;

  // Exatamente um sentido em cada par, sem laço.
  bool eh_torneio() const;

 private:
  int n_;
  std::vector<std::vector<char>> matriz_;
  std::vector<std::vector<int>> saida_;
  std::vector<std::vector<int>> entrada_;
};

}  // namespace chd
