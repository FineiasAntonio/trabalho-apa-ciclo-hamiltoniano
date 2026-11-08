#include "02-backtracking/backtracking.hpp"

namespace chd {
namespace {

bool busca(const Digrafo& g, std::vector<int>& caminho, std::vector<char>& usado, Contador& contador) {
  contador.nos++;
  const int n = g.n();
  const int ultimo = caminho.back();

  if (static_cast<int>(caminho.size()) == n) {
    return g.consulta_arco(ultimo, caminho[0], contador);
  }

  for (int v = 0; v < n; ++v) {
    if (usado[v] || !g.consulta_arco(ultimo, v, contador)) {
      continue;
    }
    usado[v] = 1;
    caminho.push_back(v);
    if (busca(g, caminho, usado, contador)) {
      return true;
    }
    caminho.pop_back();
    usado[v] = 0;
  }
  return false;
}

}

std::optional<std::vector<int>> backtracking(const Digrafo& g, Contador& contador) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }

  std::vector<int> caminho = {0};
  std::vector<char> usado(n, 0);
  usado[0] = 1;

  if (busca(g, caminho, usado, contador)) {
    return caminho;
  }
  return std::nullopt;
}

}
