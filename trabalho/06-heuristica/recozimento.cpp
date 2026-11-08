#include "06-heuristica/recozimento.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

namespace chd {
namespace {

int nota(const Digrafo& g, const std::vector<int>& ordem, Contador& contador) {
  const int n = static_cast<int>(ordem.size());
  int arcos = 0;
  for (int i = 0; i < n; ++i) {
    if (g.consulta_arco(ordem[i], ordem[(i + 1) % n], contador)) {
      arcos++;
    }
  }
  return arcos;
}

}

ResultadoRecozimento recozimento(const Digrafo& g, Contador& contador, unsigned semente,
                                 const Parametros& parametros) {
  ResultadoRecozimento resultado;
  const int n = g.n();
  if (n < 2) {
    return resultado;
  }

  std::mt19937 rng(semente);
  std::uniform_int_distribution<int> sorteia_posicao(1, n - 1);
  std::uniform_real_distribution<double> sorteia_chance(0.0, 1.0);

  std::vector<int> ordem(n);
  std::iota(ordem.begin(), ordem.end(), 0);

  for (int recomeco = 0; recomeco < parametros.recomecos; ++recomeco) {
    std::shuffle(ordem.begin() + 1, ordem.end(), rng);
    int atual = nota(g, ordem, contador);
    resultado.melhor_nota = std::max(resultado.melhor_nota, atual);
    if (atual == n) {
      resultado.ciclo = ordem;
      return resultado;
    }

    double temperatura = parametros.temperatura_inicial;
    const int movimentos = parametros.movimentos_por_vertice * n;
    for (int passo = 0; passo < movimentos; ++passo) {
      contador.nos++;
      const int origem = sorteia_posicao(rng);
      const int destino = sorteia_posicao(rng);

      std::vector<int> nova = ordem;
      const int vertice = nova[origem];
      nova.erase(nova.begin() + origem);
      nova.insert(nova.begin() + destino, vertice);

      const int nova_nota = nota(g, nova, contador);
      const int variacao = nova_nota - atual;
      if (variacao >= 0 || sorteia_chance(rng) < std::exp(variacao / temperatura)) {
        ordem = nova;
        atual = nova_nota;
        resultado.melhor_nota = std::max(resultado.melhor_nota, atual);
        if (atual == n) {
          resultado.ciclo = ordem;
          return resultado;
        }
      }
      temperatura *= parametros.resfriamento;
    }
  }
  return resultado;
}

}
