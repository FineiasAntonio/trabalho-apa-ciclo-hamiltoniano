#include "01-forca-bruta/forca_bruta.hpp"

#include <algorithm>

namespace chd {

std::optional<std::vector<int>> forca_bruta(const Digrafo& g, Contador& contador,
                                            const Relogio& relogio) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }
  // Filtro necessário. Não é poda da árvore de permutações.
  if (!g.fortemente_conexo()) {
    return std::nullopt;
  }

  std::vector<int> perm(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    perm[i] = i + 1;
  }

  std::vector<int> ciclo(n);
  long long passo = 0;
  do {
    if ((passo++ & 4095) == 0) {
      relogio.verifica();
    }
    contador.nos++;
    ciclo[0] = 0;
    for (int i = 0; i < n - 1; ++i) {
      ciclo[i + 1] = perm[i];
    }
    contador.escritas += n;

    bool fecha = true;
    for (int i = 0; i < n; ++i) {
      int proximo = ciclo[(i + 1) % n];
      if (!g.consulta_arco(ciclo[i], proximo, contador)) {
        fecha = false;
        break;
      }
    }
    if (fecha) {
      return ciclo;
    }
  } while (std::next_permutation(perm.begin(), perm.end()));

  return std::nullopt;
}

}  // namespace chd
