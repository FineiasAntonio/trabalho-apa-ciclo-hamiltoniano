#include "00-comum/gerador.hpp"
#include "00-comum/verifica.hpp"
#include "04-programacao-dinamica/held_karp.hpp"

#include <chrono>
#include <iostream>
#include <optional>
#include <exception>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "uso: " << argv[0] << " <tipo> <n>\n";
    std::cerr << "tipos: ciclo_simples, forte_sem_ciclo, pior_caso, aleatorio,\n       armadilha_indice, armadilha_warnsdorff\n";
    return 2;
  }

  const auto g = chd::gera(argv[1], std::stoi(argv[2]));
  if (!g) {
    std::cerr << "tipo desconhecido: " << argv[1] << "\n";
    return 2;
  }

  chd::Contador contador;
  const auto inicio = std::chrono::steady_clock::now();
  std::optional<std::vector<int>> ciclo;
  try {
    ciclo = chd::held_karp(*g, contador);
  } catch (const std::exception&) {
    std::cerr << "memoria insuficiente para n = " << argv[2] << "\n";
    return 2;
  }
  const double tempo_ms =
      std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - inicio).count();

  if (ciclo) {
    std::cout << "ciclo:";
    for (int v : *ciclo) {
      std::cout << " " << v;
    }
    std::cout << (chd::verifica_ciclo(*g, *ciclo) ? "  (conferido)" : "  (INVALIDO)") << "\n";
  } else {
    std::cout << "nao existe ciclo hamiltoniano direcionado\n";
  }
  std::cout << "estados expandidos: " << contador.nos << "\n";
  std::cout << "consultas de arco:  " << contador.consultas_arco << "\n";
  std::cout << "tempo (ms):         " << tempo_ms << "\n";
  return 0;
}
