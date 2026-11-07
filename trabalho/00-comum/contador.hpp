#pragma once

namespace chd {

// Contagem separada do tempo de parede. Os três campos não são somados
// entre estratégias: uma transição de Held-Karp e um teste de permutação
// não são a mesma operação.
struct Contador {
  long long consultas_aresta = 0;
  long long escritas = 0;
  long long nos = 0;

  void zera() {
    consultas_aresta = 0;
    escritas = 0;
    nos = 0;
  }
};

}  // namespace chd
