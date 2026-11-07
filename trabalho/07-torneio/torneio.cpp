#include "07-torneio/torneio.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace chd {
namespace {

std::vector<int> caminho_hamiltoniano(const Digrafo& g, Contador& contador) {
  const int n = g.n();
  std::vector<int> caminho;
  caminho.reserve(n);
  caminho.push_back(0);
  for (int v = 1; v < n; ++v) {
    int posicao = static_cast<int>(caminho.size());
    for (int i = 0; i < static_cast<int>(caminho.size()); ++i) {
      if (g.consulta_arco(v, caminho[i], contador)) {
        posicao = i;
        break;
      }
    }
    caminho.insert(caminho.begin() + posicao, v);
    contador.escritas++;
    contador.nos++;
  }
  return caminho;
}

}  // namespace

std::optional<std::vector<int>> ciclo_em_torneio(const Digrafo& g, Contador& contador) {
  const int n = g.n();
  if (!g.eh_torneio() || n < 3 || !g.fortemente_conexo()) {
    return std::nullopt;
  }

  const std::vector<int> caminho = caminho_hamiltoniano(g, contador);
  int fecho = -1;
  for (int i = n - 1; i >= 2; --i) {
    if (g.consulta_arco(caminho[i], caminho[0], contador)) {
      fecho = i;
      break;
    }
  }
  if (fecho < 0) {
    throw std::logic_error("torneio forte sem aresta de volta ao inicio do caminho");
  }

  std::vector<int> ciclo(caminho.begin(), caminho.begin() + fecho + 1);
  std::vector<char> dentro(n, 0);
  for (int v : ciclo) {
    dentro[v] = 1;
  }
  std::vector<int> fora;
  for (int v = 0; v < n; ++v) {
    if (!dentro[v]) {
      fora.push_back(v);
    }
  }

  while (!fora.empty()) {
    bool inseriu = false;
    for (size_t indice = 0; indice < fora.size() && !inseriu; ++indice) {
      const int v = fora[indice];
      const int m = static_cast<int>(ciclo.size());
      for (int i = 0; i < m; ++i) {
        const int atual = ciclo[i];
        const int proximo = ciclo[(i + 1) % m];
        if (g.consulta_arco(atual, v, contador) && g.consulta_arco(v, proximo, contador)) {
          ciclo.insert(ciclo.begin() + i + 1, v);
          fora.erase(fora.begin() + static_cast<std::ptrdiff_t>(indice));
          contador.escritas++;
          contador.nos++;
          inseriu = true;
          break;
        }
      }
    }
    if (inseriu) {
      continue;
    }

    std::vector<int> domina;
    std::vector<int> dominado;
    for (int v : fora) {
      bool vai_a_todos = true;
      bool recebe_de_todos = true;
      for (int c : ciclo) {
        if (!g.tem_arco(v, c)) {
          vai_a_todos = false;
        }
        if (!g.tem_arco(c, v)) {
          recebe_de_todos = false;
        }
      }
      if (vai_a_todos) {
        domina.push_back(v);
      } else if (recebe_de_todos) {
        dominado.push_back(v);
      } else {
        throw std::logic_error("vertice misto sem posicao de insercao no ciclo");
      }
    }
    if (domina.empty() || dominado.empty()) {
      throw std::logic_error("particao de Camion vazia num torneio forte");
    }

    int origem = -1;
    int destino = -1;
    for (int b : dominado) {
      for (int a : domina) {
        contador.consultas_aresta++;
        if (g.tem_arco(b, a)) {
          origem = b;
          destino = a;
          break;
        }
      }
      if (origem >= 0) {
        break;
      }
    }
    if (origem < 0) {
      throw std::logic_error("torneio forte sem arco do conjunto dominado para o dominante");
    }

    ciclo.insert(ciclo.begin() + 1, destino);
    ciclo.insert(ciclo.begin() + 1, origem);
    contador.escritas += 2;
    contador.nos++;
    fora.erase(std::remove_if(fora.begin(), fora.end(),
                              [&](int v) { return v == origem || v == destino; }),
               fora.end());
  }

  return ciclo;
}

}  // namespace chd
