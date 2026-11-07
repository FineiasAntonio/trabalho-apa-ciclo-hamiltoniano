#include "00-comum/tempo.hpp"
#include "00-comum/verifica.hpp"
#include "01-forca-bruta/forca_bruta.hpp"
#include "02-backtracking/backtracking.hpp"
#include "04-programacao-dinamica/held_karp.hpp"
#include "05-guloso/guloso.hpp"
#include "06-heuristica/recozimento.hpp"
#include "07-torneio/torneio.hpp"
#include "experimentos/gerador.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Linha {
  std::string familia;
  int n = 0;
  int rep = 0;
  std::string algoritmo;
  std::string ordem;
  std::string filtro;
  double tempo_ms = 0;
  long long consultas = 0;
  long long escritas = 0;
  long long nos = 0;
  std::string status;
  int fitness = -1;
  int dp_tem_ciclo = -1;
};

struct Chamada {
  std::string status;
  long long consultas = 0;
  long long escritas = 0;
  long long nos = 0;
  int fitness = -1;
  bool achou = false;
};

struct Medicao {
  Chamada chamada;
  double tempo_ms = 0;
};

template <typename Fn>
Medicao mede(Fn&& fn) {
  chd::Contador contador;
  chd::Relogio relogio;
  relogio.limite = std::chrono::steady_clock::now() + std::chrono::seconds(30);
  Medicao medicao;
  const auto inicio = std::chrono::steady_clock::now();
  try {
    fn(contador, relogio, medicao.chamada);
  } catch (const chd::TempoEsgotado&) {
    medicao.chamada.status = "censurado";
  }
  medicao.tempo_ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - inicio).count();
  medicao.chamada.consultas = contador.consultas_aresta;
  medicao.chamada.escritas = contador.escritas;
  medicao.chamada.nos = contador.nos;
  if (medicao.chamada.status.empty()) {
    medicao.chamada.status = "indefinido";
  }
  return medicao;
}

void registra(std::vector<Linha>& linhas, const std::string& familia, int n, int rep, const std::string& algoritmo,
              const std::string& ordem, const std::string& filtro, const Medicao& medicao, int dp_tem_ciclo) {
  Linha linha;
  linha.familia = familia;
  linha.n = n;
  linha.rep = rep;
  linha.algoritmo = algoritmo;
  linha.ordem = ordem;
  linha.filtro = filtro;
  linha.tempo_ms = medicao.tempo_ms;
  linha.consultas = medicao.chamada.consultas;
  linha.escritas = medicao.chamada.escritas;
  linha.nos = medicao.chamada.nos;
  linha.status = medicao.chamada.status;
  linha.fitness = medicao.chamada.fitness;
  linha.dp_tem_ciclo = dp_tem_ciclo;
  linhas.push_back(linha);
}

double mediana(std::vector<double> valores) {
  if (valores.empty()) {
    return 0;
  }
  std::sort(valores.begin(), valores.end());
  return valores[(valores.size() - 1) / 2];
}

void escreve_csv(const std::string& caminho, const std::vector<Linha>& linhas) {
  std::ofstream saida(caminho);
  if (!saida) {
    throw std::runtime_error("nao foi possivel escrever " + caminho);
  }
  saida << "familia,n,rep,algoritmo,ordem,filtro,tempo_ms,consultas_aresta,escritas,nos,status,fitness,dp_tem_ciclo\n";
  saida.setf(std::ios::fixed);
  saida.precision(3);
  for (const Linha& linha : linhas) {
    saida << linha.familia << "," << linha.n << "," << linha.rep << "," << linha.algoritmo << "," << linha.ordem
          << "," << linha.filtro << "," << linha.tempo_ms << "," << linha.consultas << "," << linha.escritas << ","
          << linha.nos << "," << linha.status << "," << linha.fitness << "," << linha.dp_tem_ciclo << "\n";
  }
}

void escreve_resumo(const std::string& caminho, const std::vector<Linha>& linhas) {
  struct Chave {
    std::string familia;
    std::string algoritmo;
    std::string ordem;
    std::string filtro;
    int n;
    bool operator<(const Chave& outra) const {
      if (familia != outra.familia) return familia < outra.familia;
      if (algoritmo != outra.algoritmo) return algoritmo < outra.algoritmo;
      if (ordem != outra.ordem) return ordem < outra.ordem;
      if (filtro != outra.filtro) return filtro < outra.filtro;
      return n < outra.n;
    }
  };
  std::vector<Chave> chaves;
  for (const Linha& linha : linhas) {
    chaves.push_back({linha.familia, linha.algoritmo, linha.ordem, linha.filtro, linha.n});
  }
  std::sort(chaves.begin(), chaves.end());
  chaves.erase(std::unique(chaves.begin(), chaves.end(),
                           [](const Chave& a, const Chave& b) {
                             return a.familia == b.familia && a.algoritmo == b.algoritmo && a.ordem == b.ordem &&
                                    a.filtro == b.filtro && a.n == b.n;
                           }),
               chaves.end());

  std::ofstream saida(caminho);
  if (!saida) {
    throw std::runtime_error("nao foi possivel escrever " + caminho);
  }
  saida << "familia,algoritmo,ordem,filtro,n,repeticoes,mediana_tempo_ms,max_tempo_ms,mediana_nos,taxa_ciclo,censurados\n";
  saida.setf(std::ios::fixed);
  saida.precision(3);
  for (const Chave& chave : chaves) {
    std::vector<double> tempos;
    std::vector<double> nos;
    int ciclos = 0;
    int censurados = 0;
    for (const Linha& linha : linhas) {
      if (linha.familia == chave.familia && linha.algoritmo == chave.algoritmo && linha.ordem == chave.ordem &&
          linha.filtro == chave.filtro && linha.n == chave.n) {
        tempos.push_back(linha.tempo_ms);
        nos.push_back(static_cast<double>(linha.nos));
        if (linha.status == "ciclo") {
          ciclos++;
        }
        if (linha.status == "censurado") {
          censurados++;
        }
      }
    }
    double maximo = 0;
    for (double t : tempos) {
      maximo = std::max(maximo, t);
    }
    saida << chave.familia << "," << chave.algoritmo << "," << chave.ordem << "," << chave.filtro << "," << chave.n
          << "," << tempos.size() << "," << mediana(tempos) << "," << maximo << "," << mediana(nos) << ","
          << (tempos.empty() ? 0.0 : static_cast<double>(ciclos) / tempos.size()) << "," << censurados << "\n";
  }
}

}  // namespace

// Bateria sem --completo reduz o orçamento do recozimento para terminar na hora.
// Os parâmetros congelados do plano são o padrão de ParamsRecozimento e entram com --completo.
int main(int argc, char** argv) {
  bool completo = false;
  std::string saida = "trabalho/dados/resultados.csv";
  std::string resumo = "trabalho/dados/resumo.csv";
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--completo") {
      completo = true;
    } else if (arg == "--saida" && i + 1 < argc) {
      saida = argv[++i];
    } else if (arg == "--resumo" && i + 1 < argc) {
      resumo = argv[++i];
    } else {
      std::cerr << "uso: experimentos [--completo] [--saida csv] [--resumo csv]\n";
      return 2;
    }
  }

  const std::vector<chd::Familia> familias = {chd::Familia::CicloExtra, chd::Familia::Dag, chd::Familia::Erdos,
                                              chd::Familia::ErdosLimiar, chd::Familia::Torneio};
  const int n_min = 6;
  const int n_max = completo ? 11 : 8;
  const int reps = completo ? 8 : 3;
  const int n_dp_max = completo ? 14 : 10;
  const int n_heur_max = completo ? 30 : 16;

  std::vector<Linha> linhas;
  for (chd::Familia familia : familias) {
    const int limite_n = (familia == chd::Familia::Torneio) ? std::max(n_max, n_heur_max) : std::max(n_max, n_dp_max);
    for (int n = n_min; n <= limite_n; ++n) {
      const bool roda_exato = n <= n_max || (n <= n_dp_max && familia != chd::Familia::Torneio);
      const bool roda_heur = n == n_min || n == n_max || n == n_heur_max || (completo && n % 2 == 0);
      if (!roda_exato && !roda_heur && familia != chd::Familia::Torneio) {
        continue;
      }
      for (int rep = 0; rep < reps; ++rep) {
        std::mt19937 rng(20261107u + static_cast<unsigned>(familia) * 100000u + static_cast<unsigned>(n) * 1000u +
                         static_cast<unsigned>(rep));
        const chd::Digrafo g = chd::gera(familia, n, rng);
        const std::string nome = chd::nome_familia(familia);

        int dp_tem_ciclo = -1;
        if (n <= n_dp_max) {
          auto medicao = mede([&](chd::Contador& contador, const chd::Relogio& relogio, Chamada& chamada) {
            auto ciclo = chd::held_karp(g, contador, relogio);
            if (ciclo) {
              chd::Contador extra;
              chamada.achou = chd::verifica_ciclo(g, *ciclo, extra);
              chamada.status = chamada.achou ? "ciclo" : "indefinido";
            } else {
              chamada.status = "inexistente";
            }
          });
          dp_tem_ciclo = medicao.chamada.achou ? 1 : 0;
          if (medicao.chamada.status == "censurado") {
            dp_tem_ciclo = -1;
          }
          registra(linhas, nome, n, rep, "held_karp", "-", "sim", medicao, dp_tem_ciclo);
        }

        if (n <= n_max && n <= 11) {
          auto medicao = mede([&](chd::Contador& contador, const chd::Relogio& relogio, Chamada& chamada) {
            auto ciclo = chd::forca_bruta(g, contador, relogio);
            chamada.status = ciclo ? "ciclo" : "inexistente";
            chamada.achou = ciclo.has_value();
          });
          registra(linhas, nome, n, rep, "forca_bruta", "-", "sim", medicao, dp_tem_ciclo);
        }

        if (n <= n_max) {
          for (chd::OrdemBusca ordem : {chd::OrdemBusca::Indice, chd::OrdemBusca::Warnsdorff}) {
            const char* nome_ordem = ordem == chd::OrdemBusca::Indice ? "indice" : "warnsdorff";
            auto medicao = mede([&](chd::Contador& contador, const chd::Relogio& relogio, Chamada& chamada) {
              auto ciclo = chd::backtracking(g, contador, ordem, true, relogio);
              chamada.status = ciclo ? "ciclo" : "inexistente";
              chamada.achou = ciclo.has_value();
            });
            registra(linhas, nome, n, rep, "backtracking", nome_ordem, "sim", medicao, dp_tem_ciclo);
          }
          if (familia == chd::Familia::Dag) {
            auto medicao = mede([&](chd::Contador& contador, const chd::Relogio& relogio, Chamada& chamada) {
              auto ciclo = chd::backtracking(g, contador, chd::OrdemBusca::Indice, false, relogio);
              chamada.status = ciclo ? "ciclo" : "inexistente";
              chamada.achou = ciclo.has_value();
            });
            registra(linhas, nome, n, rep, "backtracking", "indice", "nao", medicao, dp_tem_ciclo);
          }
        }

        if (roda_heur) {
          for (chd::CriterioGuloso criterio : {chd::CriterioGuloso::Indice, chd::CriterioGuloso::Warnsdorff}) {
            const char* nome_criterio = criterio == chd::CriterioGuloso::Indice ? "indice" : "warnsdorff";
            auto medicao = mede([&](chd::Contador& contador, const chd::Relogio&, Chamada& chamada) {
              auto ciclo = chd::guloso(g, contador, criterio);
              if (ciclo) {
                chd::Contador extra;
                chamada.achou = chd::verifica_ciclo(g, *ciclo, extra);
                chamada.status = chamada.achou ? "ciclo" : "nao_encontrou";
              } else {
                chamada.status = "nao_encontrou";
              }
            });
            registra(linhas, nome, n, rep, "guloso", nome_criterio, "-", medicao, dp_tem_ciclo);
          }

          auto medicao_sa = mede([&](chd::Contador& contador, const chd::Relogio&, Chamada& chamada) {
            std::mt19937 rng_sa(20261107u + static_cast<unsigned>(n) * 17u + static_cast<unsigned>(rep));
            chd::ParamsRecozimento params;
            if (!completo) {
              params.recomecos = 4;
              params.movimentos_por_n = 40;
            }
            auto resultado = chd::recozimento(g, contador, rng_sa, params);
            chamada.fitness = resultado.fitness;
            if (resultado.motivo == chd::MotivoHeuristica::Ciclo) {
              chd::Contador extra;
              chamada.achou = chd::verifica_ciclo(g, resultado.ciclo_valido, extra);
              chamada.status = chamada.achou ? "ciclo" : "orcamento";
            } else {
              chamada.status = "orcamento";
            }
          });
          registra(linhas, nome, n, rep, "recozimento", "-", "-", medicao_sa, dp_tem_ciclo);
        }

        if (familia == chd::Familia::Torneio && (n <= n_max || roda_heur)) {
          auto medicao = mede([&](chd::Contador& contador, const chd::Relogio&, Chamada& chamada) {
            auto ciclo = chd::ciclo_em_torneio(g, contador);
            if (ciclo) {
              chd::Contador extra;
              chamada.achou = chd::verifica_ciclo(g, *ciclo, extra);
              chamada.status = chamada.achou ? "ciclo" : "indefinido";
            } else {
              chamada.status = "inexistente";
            }
          });
          registra(linhas, nome, n, rep, "camion", "-", "sim", medicao, dp_tem_ciclo);
        }
      }
      std::cout << chd::nome_familia(familia) << " n=" << n << " pronto\n";
    }
  }

  escreve_csv(saida, linhas);
  escreve_resumo(resumo, linhas);
  std::cout << linhas.size() << " medicoes em " << saida << "\n";
  std::cout << "resumo em " << resumo << "\n";
  return 0;
}
