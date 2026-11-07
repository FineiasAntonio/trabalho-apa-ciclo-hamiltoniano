#include "05-guloso/guloso.hpp"

#include <algorithm>

namespace chd {
namespace {

struct Item {
  int vertice;
  int chave;
};

int saidas_livres(const Digrafo& g, int v, const std::vector<char>& usado, Contador& contador) {
  int livres = 0;
  for (int w : g.saida(v)) {
    contador.consultas_aresta++;
    if (!usado[w]) {
      livres++;
    }
  }
  return livres;
}

}  // namespace

std::optional<std::vector<int>> guloso(const Digrafo& g, Contador& contador, CriterioGuloso criterio) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }

  std::vector<int> caminho;
  caminho.reserve(n);
  caminho.push_back(0);
  std::vector<char> usado(n, 0);
  usado[0] = 1;
  contador.escritas++;

  while (static_cast<int>(caminho.size()) < n) {
    contador.nos++;
    const int u = caminho.back();
    std::vector<Item> candidatos;
    for (int v : g.saida(u)) {
      if (usado[v]) {
        continue;
      }
      int chave = v;
      if (criterio == CriterioGuloso::Warnsdorff) {
        chave = saidas_livres(g, v, usado, contador);
      }
      candidatos.push_back({v, chave});
    }
    if (candidatos.empty()) {
      return std::nullopt;
    }
    if (criterio == CriterioGuloso::Warnsdorff) {
      std::sort(candidatos.begin(), candidatos.end(), [](const Item& a, const Item& b) {
        if (a.chave != b.chave) {
          return a.chave < b.chave;
        }
        return a.vertice < b.vertice;
      });
    } else {
      std::sort(candidatos.begin(), candidatos.end(), [](const Item& a, const Item& b) {
        return a.vertice < b.vertice;
      });
    }
    const int escolhido = candidatos.front().vertice;
    usado[escolhido] = 1;
    caminho.push_back(escolhido);
    contador.escritas++;
  }

  if (!g.consulta_arco(caminho.back(), 0, contador)) {
    return std::nullopt;
  }
  return caminho;
}

}  // namespace chd
