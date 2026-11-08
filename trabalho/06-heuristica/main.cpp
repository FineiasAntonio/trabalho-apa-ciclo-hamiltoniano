#include "00-comum/gerador.hpp"
#include "00-comum/verifica.hpp"
#include "06-heuristica/recozimento.hpp"

#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "uso: " << argv[0] << " <tipo> <n> [semente_do_recozimento] [--semente=N]\n";
    std::cerr << "tipos: ciclo_simples, forte_sem_ciclo, pior_caso, aleatorio,\n";
    std::cerr << "       armadilha_indice, armadilha_warnsdorff\n";
    return 2;
  }

  const unsigned semente_digrafo = chd::semente_do_digrafo(argc, argv);
  const auto g = chd::gera(argv[1], std::stoi(argv[2]), semente_digrafo);
  if (!g) {
    std::cerr << "tipo desconhecido: " << argv[1] << "\n";
    return 2;
  }
  const unsigned semente = argc > 3 && std::string(argv[3]).rfind("--", 0) != 0 ? static_cast<unsigned>(std::stoul(argv[3])) : 20261107u;

  chd::Contador contador;
  const auto inicio = std::chrono::steady_clock::now();
  const auto resultado = chd::recozimento(*g, contador, semente);
  const double tempo_ms =
      std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - inicio).count();

  if (resultado.ciclo) {
    std::cout << "ciclo:";
    for (int v : *resultado.ciclo) {
      std::cout << " " << v;
    }
    std::cout << (chd::verifica_ciclo(*g, *resultado.ciclo) ? "  (conferido)" : "  (INVALIDO)") << "\n";
  } else {
    std::cout << "nao achou ciclo no limite de tentativas (isso NAO prova que ele nao existe)\n";
  }
  std::cout << "melhor nota:       " << resultado.melhor_nota << " de " << g->n() << "\n";
  std::cout << "movimentos:        " << contador.nos << "\n";
  std::cout << "consultas de arco: " << contador.consultas_arco << "\n";
  std::cout << "tempo (ms):        " << tempo_ms << "\n";
  std::cout << "semente do digrafo: " << semente_digrafo << "\n";
  return 0;
}
