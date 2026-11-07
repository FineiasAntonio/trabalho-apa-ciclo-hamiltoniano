#include "06-heuristica/recozimento.hpp"

#include <algorithm>
#include <cmath>

namespace chd {

int fitness_permutacao(const Digrafo& g, const std::vector<int>& perm, Contador& contador) {
  const int n = static_cast<int>(perm.size());
  int fitness = 0;
  for (int i = 0; i < n; ++i) {
    const int proximo = perm[(i + 1) % n];
    if (g.consulta_arco(perm[i], proximo, contador)) {
      fitness++;
    }
  }
  return fitness;
}

void aplica_relocacao(std::vector<int>& perm, int origem, int destino) {
  if (origem == destino) {
    return;
  }
  const int vertice = perm[origem];
  if (origem < destino) {
    for (int k = origem; k < destino; ++k) {
      perm[k] = perm[k + 1];
    }
    perm[destino] = vertice;
  } else {
    for (int k = origem; k > destino; --k) {
      perm[k] = perm[k - 1];
    }
    perm[destino] = vertice;
  }
}

int delta_relocacao(const Digrafo& g, const std::vector<int>& perm, int origem, int destino,
                    Contador& contador) {
  if (origem == destino) {
    return 0;
  }
  const int n = static_cast<int>(perm.size());
  auto presente = [&](int u, int v) {
    return g.consulta_arco(u, v, contador) ? 1 : 0;
  };
  auto sucessor = [&](int i) { return perm[(i + 1) % n]; };

  if (origem < destino) {
    const int anterior = perm[origem - 1];
    const int vertice = perm[origem];
    const int primeiro = perm[origem + 1];
    const int ultimo = perm[destino];
    const int depois = sucessor(destino);
    const int perdidos = presente(anterior, vertice) + presente(vertice, primeiro) + presente(ultimo, depois);
    const int ganhos = presente(anterior, primeiro) + presente(ultimo, vertice) + presente(vertice, depois);
    return ganhos - perdidos;
  }

  const int anterior = perm[destino - 1];
  const int primeiro = perm[destino];
  const int ultimo = perm[origem - 1];
  const int vertice = perm[origem];
  const int depois = sucessor(origem);
  const int perdidos = presente(anterior, primeiro) + presente(ultimo, vertice) + presente(vertice, depois);
  const int ganhos = presente(anterior, vertice) + presente(vertice, primeiro) + presente(ultimo, depois);
  return ganhos - perdidos;
}

ResultadoHeuristica recozimento(const Digrafo& g, Contador& contador, std::mt19937& rng,
                                const ParamsRecozimento& params) {
  ResultadoHeuristica resultado;
  const int n = g.n();
  if (n < 2) {
    return resultado;
  }

  std::vector<int> perm(n);
  perm[0] = 0;
  for (int i = 1; i < n; ++i) {
    perm[i] = i;
  }

  auto guarda_melhor = [&](const std::vector<int>& atual, int fitness) {
    if (fitness > resultado.fitness || resultado.melhor_permutacao.empty()) {
      resultado.fitness = fitness;
      resultado.melhor_permutacao = atual;
      contador.escritas++;
    }
  };

  std::uniform_int_distribution<int> indice(1, n - 1);
  std::uniform_real_distribution<double> unidade(0.0, 1.0);

  for (int recomeço = 0; recomeço < params.recomecos; ++recomeço) {
    std::shuffle(perm.begin() + 1, perm.end(), rng);
    int fitness = fitness_permutacao(g, perm, contador);
    guarda_melhor(perm, fitness);
    if (fitness == n) {
      resultado.ciclo_valido = perm;
      resultado.motivo = MotivoHeuristica::Ciclo;
      return resultado;
    }

    double temperatura = params.t0;
    const int movimentos = params.movimentos_por_n * n;
    for (int passo = 0; passo < movimentos; ++passo) {
      contador.nos++;
      int origem = indice(rng);
      int destino = indice(rng);
      if (destino == origem) {
        destino = origem == n - 1 ? 1 : origem + 1;
      }
      const int delta = delta_relocacao(g, perm, origem, destino, contador);
      const bool melhora = delta >= 0;
      const bool aceita =
          melhora || (temperatura > 0.0 && unidade(rng) < std::exp(static_cast<double>(delta) / temperatura));
      if (aceita) {
        aplica_relocacao(perm, origem, destino);
        fitness += delta;
        guarda_melhor(perm, fitness);
        if (fitness == n) {
          resultado.ciclo_valido = perm;
          resultado.motivo = MotivoHeuristica::Ciclo;
          return resultado;
        }
      }
      temperatura *= params.alfa;
    }
  }

  resultado.motivo = MotivoHeuristica::Orcamento;
  return resultado;
}

}  // namespace chd
