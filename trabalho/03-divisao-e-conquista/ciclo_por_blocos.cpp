#include "03-divisao-e-conquista/ciclo_por_blocos.hpp"

#include <algorithm>
#include <stdexcept>

namespace chd {

std::optional<std::vector<int>> ciclo_por_blocos_contiguos(const Digrafo& g, Contador& contador) {
  const int n = g.n();
  if (n < 2) {
    return std::nullopt;
  }
  if (n > 10) {
    throw std::invalid_argument("ciclo por blocos so enumera ate n = 10");
  }

  std::vector<int> resto;
  resto.reserve(n - 1);
  for (int v = 1; v < n; ++v) {
    resto.push_back(v);
  }
  const int meio = (n - 1 + 1) / 2;
  std::vector<int> esquerda(resto.begin(), resto.begin() + meio);
  std::vector<int> direita(resto.begin() + meio, resto.end());
  std::sort(esquerda.begin(), esquerda.end());

  auto fecha = [&](const std::vector<int>& ordem) {
    contador.nos++;
    contador.escritas += n;
    for (int i = 0; i < n; ++i) {
      if (!g.consulta_arco(ordem[i], ordem[(i + 1) % n], contador)) {
        return false;
      }
    }
    return true;
  };

  do {
    std::vector<int> bloco_direito = direita;
    std::sort(bloco_direito.begin(), bloco_direito.end());
    do {
      std::vector<int> esquerda_depois;
      esquerda_depois.reserve(n);
      esquerda_depois.push_back(0);
      esquerda_depois.insert(esquerda_depois.end(), esquerda.begin(), esquerda.end());
      esquerda_depois.insert(esquerda_depois.end(), bloco_direito.begin(), bloco_direito.end());
      if (fecha(esquerda_depois)) {
        return esquerda_depois;
      }

      std::vector<int> direita_depois;
      direita_depois.reserve(n);
      direita_depois.push_back(0);
      direita_depois.insert(direita_depois.end(), bloco_direito.begin(), bloco_direito.end());
      direita_depois.insert(direita_depois.end(), esquerda.begin(), esquerda.end());
      if (fecha(direita_depois)) {
        return direita_depois;
      }
    } while (std::next_permutation(bloco_direito.begin(), bloco_direito.end()));
  } while (std::next_permutation(esquerda.begin(), esquerda.end()));

  return std::nullopt;
}

}  // namespace chd
