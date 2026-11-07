#pragma once

#include "00-comum/digrafo.hpp"

#include <random>
#include <vector>

namespace chd {

// Parâmetros declarados antes da bateria. O piloto pode trocar alfa e,
// depois da escolha, o valor usado no relatório fica congelado.
struct ParamsRecozimento {
  double t0 = 1.0;
  double alfa = 0.995;
  int movimentos_por_n = 200;
  int recomecos = 10;
};

enum class MotivoHeuristica { Ciclo, Orcamento };

// ciclo_valido só é preenchido quando o fitness chega a n.
// motivo Orcamento com ciclo vazio não é certificado de inexistência.
struct ResultadoHeuristica {
  std::vector<int> ciclo_valido;
  std::vector<int> melhor_permutacao;
  int fitness = 0;
  MotivoHeuristica motivo = MotivoHeuristica::Orcamento;
};

int fitness_permutacao(const Digrafo& g, const std::vector<int>& perm, Contador& contador);

// Variação do fitness ao mover o vértice que está em `origem` para o índice `destino`.
// Só os arcos da fronteira mudam, então a conta é O(1).
int delta_relocacao(const Digrafo& g, const std::vector<int>& perm, int origem, int destino,
                    Contador& contador);

void aplica_relocacao(std::vector<int>& perm, int origem, int destino);

ResultadoHeuristica recozimento(const Digrafo& g, Contador& contador, std::mt19937& rng,
                                const ParamsRecozimento& params = {});

}  // namespace chd
