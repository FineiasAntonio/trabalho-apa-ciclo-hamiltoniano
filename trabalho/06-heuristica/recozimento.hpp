#pragma once

#include "00-comum/digrafo.hpp"

#include <optional>
#include <vector>

namespace chd {

struct Parametros {
  double temperatura_inicial = 1.0;
  double resfriamento = 0.995;
  int movimentos_por_vertice = 200;
  int recomecos = 10;
};

struct ResultadoRecozimento {
  std::optional<std::vector<int>> ciclo;
  int melhor_nota = 0;
};

ResultadoRecozimento recozimento(const Digrafo& g, Contador& contador, unsigned semente,
                                 const Parametros& parametros = {});

}
