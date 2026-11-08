#include "00-comum/gerador.hpp"
#include "00-comum/verifica.hpp"
#include "01-forca-bruta/forca_bruta.hpp"

#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "uso: " << argv[0] << " <tipo> <n>\n";
    std::cerr << "tipos: ciclo_simples, forte_sem_ciclo, pior_caso, aleatorio\n";
    return 2;
  }

  const auto g = chd::gera(argv[1], std::stoi(argv[2]));
  if (!g) {
    std::cerr << "tipo desconhecido: " << argv[1] << "\n";
    return 2;
  }

  chd::Contador contador;
  const auto inicio = std::chrono::steady_clock::now();
  const auto ciclo = chd::forca_bruta(*g, contador);
  const double tempo_ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - inicio).count();

  if (ciclo) {
    std::cout << "ciclo:";
    for (int v : *ciclo) {
      std::cout << " " << v;
    }
    std::cout << (chd::verifica_ciclo(*g, *ciclo) ? "  (conferido)" : "  (INVALIDO)") << "\n";
  } else {
    std::cout << "nao existe ciclo hamiltoniano direcionado\n";
  }
  std::cout << "permutacoes testadas: " << contador.permutacoes << "\n";
  std::cout << "consultas de arco:    " << contador.consultas_arco << "\n";
  std::cout << "tempo (ms):           " << tempo_ms << "\n";
  return 0;
}
