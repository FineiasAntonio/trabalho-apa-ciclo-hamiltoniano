#include "04-programacao-dinamica/held_karp.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace chd {

std::optional<std::vector<int>> held_karp(const Digrafo& g, Contador& contador) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }

  const uint64_t limite = 1ull << n;
  std::vector<char> alcanca(static_cast<size_t>(limite) * n, 0);
  auto posicao = [n](uint64_t mascara, int v) { return static_cast<size_t>(mascara) * n + v; };

  alcanca[posicao(1u, 0)] = 1;

  for (uint64_t mascara = 1; mascara < limite; mascara += 2) {
    for (int v = 0; v < n; ++v) {
      if (!alcanca[posicao(mascara, v)]) {
        continue;
      }
      contador.nos++;
      for (int u = 0; u < n; ++u) {
        if ((mascara & (1ull << u)) != 0 || !g.consulta_arco(v, u, contador)) {
          continue;
        }
        alcanca[posicao(mascara | (1ull << u), u)] = 1;
      }
    }
  }

  const uint64_t cheia = limite - 1;
  int fim = -1;
  for (int v = 1; v < n; ++v) {
    if (alcanca[posicao(cheia, v)] && g.consulta_arco(v, 0, contador)) {
      fim = v;
      break;
    }
  }
  if (fim < 0) {
    return std::nullopt;
  }

  std::vector<int> ciclo;
  uint64_t mascara = cheia;
  int v = fim;
  while (v != 0) {
    ciclo.push_back(v);
    const uint64_t anterior = mascara ^ (1ull << v);
    int u = 0;
    while (!(((anterior >> u) & 1ull) && alcanca[posicao(anterior, u)] && g.consulta_arco(u, v, contador))) {
      ++u;
    }
    mascara = anterior;
    v = u;
  }
  ciclo.push_back(0);
  std::reverse(ciclo.begin(), ciclo.end());
  return ciclo;
}

}
