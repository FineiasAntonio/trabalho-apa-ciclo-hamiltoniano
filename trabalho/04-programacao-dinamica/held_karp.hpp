#pragma once

#include "00-comum/digrafo.hpp"
#include "00-comum/tempo.hpp"

#include <optional>
#include <vector>

namespace chd {

// Held-Karp direcionado, com reconstrução do ciclo.
// dp[S][v] é verdadeiro quando existe caminho simples que começa em 0,
// visita exatamente o conjunto S e termina em v.
// Aceita n até 22. Acima disso a tabela não cabe no recorte deste trabalho
// e a função lança std::invalid_argument, em vez de responder "não existe".
std::optional<std::vector<int>> held_karp(const Digrafo& g, Contador& contador,
                                          const Relogio& relogio = {});

}  // namespace chd
