#include "05-guloso/guloso.hpp"

#include <climits>

namespace chd {
namespace {

int saidas_livres(const Digrafo& g, int v, const std::vector<char>& usado, Contador& contador) {
  int livres = 0;
  for (int w = 0; w < g.n(); ++w) {
    if (!usado[w] && g.consulta_arco(v, w, contador)) {
      livres++;
    }
  }
  return livres;
}

}

std::optional<std::vector<int>> guloso(const Digrafo& g, Contador& contador, Criterio criterio) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }

  std::vector<int> caminho = {0};
  std::vector<char> usado(n, 0);
  usado[0] = 1;

  while (static_cast<int>(caminho.size()) < n) {
    contador.nos++;
    const int ultimo = caminho.back();

    int escolhido = -1;
    int menor = INT_MAX;
    for (int v = 0; v < n; ++v) {
      if (usado[v] || !g.consulta_arco(ultimo, v, contador)) {
        continue;
      }
      if (criterio == Criterio::Indice) {
        escolhido = v;
        break;
      }
      const int livres = saidas_livres(g, v, usado, contador);
      if (livres < menor) {
        menor = livres;
        escolhido = v;
      }
    }

    if (escolhido < 0) {
      return std::nullopt;
    }
    usado[escolhido] = 1;
    caminho.push_back(escolhido);
  }

  if (!g.consulta_arco(caminho.back(), 0, contador)) {
    return std::nullopt;
  }
  return caminho;
}

}
