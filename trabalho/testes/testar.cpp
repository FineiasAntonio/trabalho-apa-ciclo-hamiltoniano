#include "00-comum/verifica.hpp"
#include "01-forca-bruta/forca_bruta.hpp"
#include "02-backtracking/backtracking.hpp"
#include "03-divisao-e-conquista/ciclo_por_blocos.hpp"
#include "04-programacao-dinamica/held_karp.hpp"
#include "05-guloso/guloso.hpp"
#include "06-heuristica/recozimento.hpp"
#include "07-torneio/torneio.hpp"
#include "experimentos/gerador.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <random>
#include <string>

namespace {

int falhas = 0;

void espera(bool condicao, const char* texto, int linha) {
  if (!condicao) {
    std::cerr << "falha na linha " << linha << ": " << texto << "\n";
    falhas++;
  }
}

std::string texto_ciclo(const std::optional<std::vector<int>>& ciclo) {
  if (!ciclo) {
    return "vazio";
  }
  std::string s;
  for (int v : *ciclo) {
    s += std::to_string(v);
    s += " ";
  }
  return s;
}

bool tem_ciclo(const std::optional<std::vector<int>>& ciclo) { return ciclo.has_value(); }

void confere_sequencia(const chd::Digrafo& g, const std::vector<int>& ciclo, int linha) {
  chd::Contador contador;
  if (!chd::verifica_ciclo(g, ciclo, contador)) {
    std::cerr << "falha na linha " << linha << ": ciclo invalido";
    for (int v : ciclo) {
      std::cerr << " " << v;
    }
    std::cerr << "\n";
    falhas++;
  }
}

void confere_valido(const chd::Digrafo& g, const std::optional<std::vector<int>>& ciclo, int linha) {
  if (!ciclo) {
    std::cerr << "falha na linha " << linha << ": ciclo ausente\n";
    falhas++;
    return;
  }
  confere_sequencia(g, *ciclo, linha);
}

int fitness_puro(const chd::Digrafo& g, const std::vector<int>& perm) {
  int fitness = 0;
  const int n = static_cast<int>(perm.size());
  for (int i = 0; i < n; ++i) {
    if (g.tem_arco(perm[i], perm[(i + 1) % n])) {
      fitness++;
    }
  }
  return fitness;
}

}  // namespace

#define ESPERA(cond) espera((cond), #cond, __LINE__)

int main() {
  using namespace chd;

  {
    Contador contador;
    auto g = ciclo_direcionado(4);
    std::vector<int> ciclo = {0, 1, 2, 3};
    ESPERA(verifica_ciclo(g, ciclo, contador));
    ciclo = {0, 1, 3, 2};
    ESPERA(!verifica_ciclo(g, ciclo, contador));
    ESPERA(!verifica_ciclo(g, {0, 1, 2}, contador));
    ESPERA(!verifica_ciclo(g, {0, 1, 1, 2}, contador));
  }

  {
    auto g = forte_sem_ciclo();
    ESPERA(g.fortemente_conexo());
    Contador contador;
    ESPERA(!forca_bruta(g, contador));
    ESPERA(!backtracking(g, contador, OrdemBusca::Indice));
    ESPERA(!backtracking(g, contador, OrdemBusca::Warnsdorff));
    ESPERA(!held_karp(g, contador));
  }

  {
    auto g = ciclo_direcionado(5);
    Contador contador;
    auto bruto = forca_bruta(g, contador);
    auto indice = backtracking(g, contador, OrdemBusca::Indice);
    auto warnsdorff = backtracking(g, contador, OrdemBusca::Warnsdorff);
    auto dinamico = held_karp(g, contador);
    confere_valido(g, bruto, __LINE__);
    confere_valido(g, indice, __LINE__);
    confere_valido(g, warnsdorff, __LINE__);
    confere_valido(g, dinamico, __LINE__);
    ESPERA(bruto && bruto->front() == 0);
  }

  {
    auto g = ciclo_reverso(6);
    Contador contador;
    auto bruto = forca_bruta(g, contador);
    auto dinamico = held_karp(g, contador);
    confere_valido(g, bruto, __LINE__);
    confere_valido(g, dinamico, __LINE__);
    ESPERA(bruto && (*bruto)[1] == 5);
  }

  {
    auto g = digrafo_completo(2);
    Contador contador;
    auto bruto = forca_bruta(g, contador);
    confere_valido(g, bruto, __LINE__);
    Digrafo um_arco(2);
    um_arco.adiciona_arco(0, 1);
    ESPERA(!um_arco.fortemente_conexo());
    ESPERA(!forca_bruta(um_arco, contador));
    ESPERA(!held_karp(um_arco, contador));
  }

  std::mt19937 rng(20261107);
  for (int n = 2; n <= 8; ++n) {
    for (int rep = 0; rep < 12; ++rep) {
      Digrafo g = (rep % 3 == 0) ? erdos_renyi(n, 0.4, rng)
                                 : (rep % 3 == 1) ? ciclo_com_arcos_extras(n, n, rng)
                                                  : torneio_aleatorio(n, rng);
      Contador c1;
      Contador c2;
      Contador c3;
      Contador c4;
      auto bruto = forca_bruta(g, c1);
      auto indice = backtracking(g, c2, OrdemBusca::Indice);
      auto warnsdorff = backtracking(g, c3, OrdemBusca::Warnsdorff);
      auto dinamico = held_karp(g, c4);
      const bool existe = tem_ciclo(bruto);
      if (existe != tem_ciclo(indice) || existe != tem_ciclo(warnsdorff) || existe != tem_ciclo(dinamico)) {
        std::cerr << "discordancia n=" << n << " rep=" << rep << " bruto=" << texto_ciclo(bruto)
                  << " indice=" << texto_ciclo(indice) << " warnsdorff=" << texto_ciclo(warnsdorff)
                  << " dp=" << texto_ciclo(dinamico) << "\n";
        falhas++;
      }
      if (bruto) {
        confere_valido(g, bruto, __LINE__);
      }
      if (indice) {
        confere_valido(g, indice, __LINE__);
      }
      if (warnsdorff) {
        confere_valido(g, warnsdorff, __LINE__);
      }
      if (dinamico) {
        confere_valido(g, dinamico, __LINE__);
      }
    }
  }

  {
    auto g = armadilha_indice();
    Contador contador;
    auto exato = held_karp(g, contador);
    confere_valido(g, exato, __LINE__);
    ESPERA(!guloso(g, contador, CriterioGuloso::Indice));
  }
  {
    auto g = armadilha_warnsdorff();
    Contador contador;
    auto exato = held_karp(g, contador);
    confere_valido(g, exato, __LINE__);
    ESPERA(!guloso(g, contador, CriterioGuloso::Warnsdorff));
  }

  {
    auto intercalado = ciclo_intercalado();
    Contador contador;
    ESPERA(!ciclo_por_blocos_contiguos(intercalado, contador));
    auto exato = held_karp(intercalado, contador);
    confere_valido(intercalado, exato, __LINE__);

    auto direto = ciclo_direcionado(4);
    auto blocos = ciclo_por_blocos_contiguos(direto, contador);
    confere_valido(direto, blocos, __LINE__);
  }

  {
    auto dag = dag_aleatorio(7, 0.6, rng);
    ESPERA(!dag.fortemente_conexo());
    Contador com_filtro;
    Contador sem_filtro;
    ESPERA(!backtracking(dag, com_filtro, OrdemBusca::Indice, true));
    ESPERA(!backtracking(dag, sem_filtro, OrdemBusca::Indice, false));
    ESPERA(com_filtro.nos == 0);
    ESPERA(sem_filtro.nos > com_filtro.nos);
    Contador contador;
    ESPERA(!forca_bruta(dag, contador));
    ESPERA(!held_karp(dag, contador));
  }

  {
    auto g = digrafo_completo(7);
    Contador contador;
    ParamsRecozimento params;
    params.recomecos = 1;
    params.movimentos_por_n = 1;
    std::mt19937 local(20261107);
    auto resultado = recozimento(g, contador, local, params);
    ESPERA(resultado.motivo == MotivoHeuristica::Ciclo);
    ESPERA(resultado.fitness == g.n());
    ESPERA(static_cast<int>(resultado.ciclo_valido.size()) == g.n());
    confere_sequencia(g, resultado.ciclo_valido, __LINE__);
  }

  {
    auto g = dag_aleatorio(8, 0.5, rng);
    Contador contador;
    ParamsRecozimento params;
    params.recomecos = 2;
    params.movimentos_por_n = 20;
    std::mt19937 local(7);
    auto resultado = recozimento(g, contador, local, params);
    ESPERA(resultado.motivo == MotivoHeuristica::Orcamento);
    ESPERA(resultado.ciclo_valido.empty());
    ESPERA(resultado.fitness < g.n());
  }

  {
    auto g = armadilha_indice();
    Contador contador;
    std::mt19937 local(20261107);
    auto resultado = recozimento(g, contador, local);
    ESPERA(resultado.motivo == MotivoHeuristica::Ciclo);
    confere_sequencia(g, resultado.ciclo_valido, __LINE__);
  }

  {
    std::mt19937 local(11);
    for (int n = 4; n <= 8; ++n) {
      for (int rep = 0; rep < 30; ++rep) {
        auto g = torneio_aleatorio(n, local);
        std::vector<int> perm(n);
        for (int i = 0; i < n; ++i) {
          perm[i] = i;
        }
        std::shuffle(perm.begin() + 1, perm.end(), local);
        for (int passo = 0; passo < 20; ++passo) {
          std::uniform_int_distribution<int> indice(1, n - 1);
          int origem = indice(local);
          int destino = indice(local);
          const int antes = fitness_puro(g, perm);
          Contador contador;
          const int delta = delta_relocacao(g, perm, origem, destino, contador);
          aplica_relocacao(perm, origem, destino);
          const int depois = fitness_puro(g, perm);
          if (depois != antes + delta) {
            std::cerr << "delta inconsistente n=" << n << " antes=" << antes << " delta=" << delta
                      << " depois=" << depois << "\n";
            falhas++;
            passo = 20;
            rep = 30;
            n = 9;
          }
        }
      }
    }
  }

  {
    Digrafo transitivo(6);
    for (int u = 0; u < 6; ++u) {
      for (int v = u + 1; v < 6; ++v) {
        transitivo.adiciona_arco(u, v);
      }
    }
    ESPERA(transitivo.eh_torneio());
    ESPERA(!transitivo.fortemente_conexo());
    Contador contador;
    ESPERA(!ciclo_em_torneio(transitivo, contador));
    ESPERA(!ciclo_em_torneio(ciclo_direcionado(5), contador));
  }

  {
    std::mt19937 local(99);
    for (int n = 3; n <= 9; ++n) {
      for (int rep = 0; rep < 20; ++rep) {
        auto g = torneio_aleatorio(n, local);
        Contador c_dp;
        Contador c_cam;
        auto dinamico = held_karp(g, c_dp);
        auto camion = ciclo_em_torneio(g, c_cam);
        if (tem_ciclo(dinamico) != tem_ciclo(camion)) {
          std::cerr << "Camion discorda da PD n=" << n << " rep=" << rep << "\n";
          falhas++;
        }
        if (camion) {
          confere_valido(g, camion, __LINE__);
        }
      }
    }
  }

  {
    auto g = ciclo_direcionado(18);
    Contador contador;
    auto dinamico = held_karp(g, contador);
    confere_valido(g, dinamico, __LINE__);
    auto busca = backtracking(g, contador, OrdemBusca::Indice);
    confere_valido(g, busca, __LINE__);
  }

  if (falhas == 0) {
    std::cout << "testes ok\n";
    return 0;
  }
  std::cerr << falhas << " falha(s)\n";
  return 1;
}
