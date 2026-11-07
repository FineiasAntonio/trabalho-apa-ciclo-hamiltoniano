#include "02-backtracking/backtracking.hpp"

#include <algorithm>
#include <utility>

namespace chd {
namespace {

struct Item {
  int vertice;
  int chave;
};

}  // namespace

std::optional<std::vector<int>> backtracking(const Digrafo& g, Contador& contador,
                                             OrdemBusca ordem, bool usar_filtro,
                                             const Relogio& relogio) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }
  if (usar_filtro && !g.fortemente_conexo()) {
    return std::nullopt;
  }

  std::vector<int> caminho;
  caminho.reserve(n);
  caminho.push_back(0);
  std::vector<char> usado(n, 0);
  usado[0] = 1;
  long long passo = 0;

  auto saidas_livres = [&](int v) {
    int livres = 0;
    for (int w : g.saida(v)) {
      contador.consultas_aresta++;
      if (!usado[w]) {
        livres++;
      }
    }
    return livres;
  };

  auto dfs = [&](auto&& self) -> bool {
    if ((passo++ & 1023) == 0) {
      relogio.verifica();
    }
    contador.nos++;
    if (static_cast<int>(caminho.size()) == n) {
      // Os n vértices já estão no caminho. Só resta o arco de volta à origem.
      return g.consulta_arco(caminho.back(), 0, contador);
    }

    const int u = caminho.back();
    std::vector<Item> candidatos;
    candidatos.reserve(g.saida(u).size());
    for (int v : g.saida(u)) {
      if (usado[v]) {
        continue;
      }
      int chave = 0;
      if (ordem == OrdemBusca::Warnsdorff) {
        chave = saidas_livres(v);
      }
      candidatos.push_back({v, chave});
    }
    if (ordem == OrdemBusca::Warnsdorff) {
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

    for (const Item& item : candidatos) {
      usado[item.vertice] = 1;
      caminho.push_back(item.vertice);
      contador.escritas++;
      if (self(self)) {
        return true;
      }
      caminho.pop_back();
      usado[item.vertice] = 0;
    }
    return false;
  };

  if (dfs(dfs)) {
    return caminho;
  }
  return std::nullopt;
}

}  // namespace chd
