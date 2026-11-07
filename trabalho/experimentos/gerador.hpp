#pragma once

#include "00-comum/digrafo.hpp"

#include <random>
#include <string>

namespace chd {

enum class Familia { CicloExtra, Dag, Erdos, ErdosLimiar, Torneio };

Digrafo ciclo_direcionado(int n);
Digrafo ciclo_com_arcos_extras(int n, int extras, std::mt19937& rng);
Digrafo dag_aleatorio(int n, double p, std::mt19937& rng);
Digrafo erdos_renyi(int n, double p, std::mt19937& rng);
Digrafo torneio_aleatorio(int n, std::mt19937& rng);

Digrafo armadilha_indice();
Digrafo armadilha_warnsdorff();
Digrafo forte_sem_ciclo();
Digrafo ciclo_intercalado();
Digrafo ciclo_reverso(int n);
Digrafo digrafo_completo(int n);

const char* nome_familia(Familia familia);
Digrafo gera(Familia familia, int n, std::mt19937& rng);

}  // namespace chd
