#include "04-programacao-dinamica/held_karp.hpp"

#include <algorithm>
#include <cstdint>
#include <stdexcept>

namespace chd {

std::optional<std::vector<int>> held_karp(const Digrafo& g, Contador& contador,
                                          const Relogio& relogio) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }
  if (n > 22) {
    throw std::invalid_argument("Held-Karp deste trabalho cobre n <= 22");
  }
  if (!g.fortemente_conexo()) {
    return std::nullopt;
  }

  const uint32_t limite = 1u << n;
  std::vector<uint32_t> alcanca(limite, 0);
  std::vector<int> predecessor(static_cast<size_t>(limite) * n, -1);

  // Base: o caminho que só contém a origem, terminando nela.
  alcanca[1u] = 1u;
  contador.escritas++;

  for (uint32_t mascara = 1; mascara < limite; ++mascara) {
    if ((mascara & 1023u) == 0) {
      relogio.verifica();
    }
    if ((mascara & 1u) == 0 || alcanca[mascara] == 0) {
      continue;
    }
    for (uint32_t bits = alcanca[mascara]; bits != 0; bits &= bits - 1) {
      const int v = __builtin_ctz(bits);
      contador.nos++;
      for (int u : g.saida(v)) {
        contador.consultas_aresta++;
        if ((mascara & (1u << u)) != 0) {
          continue;
        }
        const uint32_t nova = mascara | (1u << u);
        const uint32_t bit_u = 1u << u;
        if ((alcanca[nova] & bit_u) == 0) {
          alcanca[nova] |= bit_u;
          predecessor[static_cast<size_t>(nova) * n + u] = v;
          contador.escritas++;
        }
      }
    }
  }

  const uint32_t cheia = limite - 1;
  int fim = -1;
  for (int v = 1; v < n; ++v) {
    if ((alcanca[cheia] & (1u << v)) != 0 && g.consulta_arco(v, 0, contador)) {
      fim = v;
      break;
    }
  }
  if (fim < 0) {
    return std::nullopt;
  }

  std::vector<int> ciclo;
  ciclo.reserve(n);
  uint32_t mascara = cheia;
  int v = fim;
  while (true) {
    ciclo.push_back(v);
    if (v == 0) {
      break;
    }
    const int anterior = predecessor[static_cast<size_t>(mascara) * n + v];
    mascara ^= 1u << v;
    v = anterior;
  }
  std::reverse(ciclo.begin(), ciclo.end());
  return ciclo;
}

}  // namespace chd
