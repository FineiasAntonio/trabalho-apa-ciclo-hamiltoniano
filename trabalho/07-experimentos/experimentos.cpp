#include "00-comum/gerador.hpp"
#include "00-comum/verifica.hpp"
#include "01-forca-bruta/forca_bruta.hpp"
#include "02-backtracking/backtracking.hpp"
#include "04-programacao-dinamica/held_karp.hpp"
#include "05-guloso/guloso.hpp"
#include "06-heuristica/recozimento.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace chd;
using Relogio = std::chrono::steady_clock;

namespace {

template <class F>
double mede(F f) {
  const auto t0 = Relogio::now();
  f();
  return std::chrono::duration<double, std::milli>(Relogio::now() - t0).count();
}

template <class F>
void mediana5(F algoritmo, const Digrafo& g, long long& passos, double& ms) {
  std::vector<double> t;
  for (int i = 0; i < 5; ++i) {
    Contador c;
    t.push_back(mede([&] { algoritmo(g, c); }));
    passos = c.nos + c.permutacoes;
  }
  std::sort(t.begin(), t.end());
  ms = t[2];
}

void pior_caso() {
  for (int n = 6; n <= 20; ++n) {
    const Digrafo g = *gera("pior_caso", n, 0);
    long long pf = -1, pb = -1, ph = -1;
    double tf = -1, tb = -1, th = -1;
    if (n <= 12) mediana5([](const Digrafo& d, Contador& c) { forca_bruta(d, c); }, g, pf, tf);
    if (n <= 13) mediana5([](const Digrafo& d, Contador& c) { backtracking(d, c); }, g, pb, tb);
    mediana5([](const Digrafo& d, Contador& c) { held_karp(d, c); }, g, ph, th);
    long long pg, pr;
    double tg, tr;
    mediana5([](const Digrafo& d, Contador& c) { guloso(d, c, Criterio::Indice); }, g, pg, tg);
    mediana5([](const Digrafo& d, Contador& c) { recozimento(d, c, 20261107u); }, g, pr, tr);
    std::printf("P\t%d\t%lld\t%.6f\t%lld\t%.6f\t%lld\t%.6f\t%lld\t%.6f\t%lld\t%.6f\n", n, pf, tf, pb, tb, ph, th, pg, tg, pr, tr);
  }
}

void extra() {
  long long p;
  double ms;
  mediana5([](const Digrafo& d, Contador& c) { forca_bruta(d, c); }, *gera("pior_caso", 13, 0), p, ms);
  std::printf("FB13\t%lld\t%.6f\n", p, ms);
  std::fflush(stdout);
  mediana5([](const Digrafo& d, Contador& c) { backtracking(d, c); }, *gera("pior_caso", 14, 0), p, ms);
  std::printf("BT14\t%lld\t%.6f\n", p, ms);
}

void varredura(int n, double p, int total, bool com_hk, int rodadas) {
  const std::string tipo = "aleatorio:" + std::to_string(p);
  std::vector<std::string> linhas(total);
#pragma omp parallel for schedule(dynamic)
  for (int s = 1; s <= total; ++s) {
    const Digrafo g = *gera(tipo, n, s);
    Contador cb;
    const auto rb = backtracking(g, cb);
    long long estados = -1;
    int hk_ok = 1;
    if (com_hk) {
      Contador ch;
      const auto rh = held_karp(g, ch);
      estados = ch.nos;
      hk_ok = rh.has_value() == rb.has_value();
    }
    Contador cg;
    const int gi = guloso(g, cg, Criterio::Indice).has_value();
    const int gw = guloso(g, cg, Criterio::Warnsdorff).has_value();
    int rec = 0;
    for (int r = 1; r <= rodadas; ++r) {
      Contador cr;
      rec += recozimento(g, cr, 1000u * s + r).ciclo.has_value();
    }
    char buf[256];
    std::snprintf(buf, sizeof buf, "B\t%d\t%.2f\t%d\t%d\t%lld\t%lld\t%d\t%d\t%d\t%d",
                  n, p, s, static_cast<int>(rb.has_value()), cb.nos, estados, hk_ok, gi, gw, rec);
    linhas[s - 1] = buf;
  }
  for (const auto& l : linhas) std::puts(l.c_str());
}

void grandes(int n, double p, int total) {
  const std::string tipo = "aleatorio:" + std::to_string(p);
  std::vector<std::string> linhas(total);
#pragma omp parallel for schedule(dynamic)
  for (int s = 1; s <= total; ++s) {
    const Digrafo g = *gera(tipo, n, s);
    Contador c;
    const int gi = guloso(g, c, Criterio::Indice).has_value();
    const int gw = guloso(g, c, Criterio::Warnsdorff).has_value();
    const int rec = recozimento(g, c, 1000u * s + 1).ciclo.has_value();
    char buf[128];
    std::snprintf(buf, sizeof buf, "G\t%d\t%.2f\t%d\t%d\t%d\t%d", n, p, s, gi, gw, rec);
    linhas[s - 1] = buf;
  }
  for (const auto& l : linhas) std::puts(l.c_str());
}

void ciclo_simples() {
  for (int n : {5, 8, 10, 12, 15, 20, 25, 30, 40, 50}) {
    const Digrafo g = *gera("ciclo_simples", n, 0);
    int ok = 0;
    for (int r = 1; r <= 100; ++r) {
      Contador c;
      ok += recozimento(g, c, r).ciclo.has_value();
    }
    Contador c;
    const int gi = guloso(g, c, Criterio::Indice).has_value();
    std::printf("C\t%d\t%d\t%d\n", n, ok, gi);
  }
}

}

int main(int argc, char** argv) {
  const std::string modo = argc > 1 ? argv[1] : "";
  if (modo == "pior") pior_caso();
  else if (modo == "extra") extra();
  else if (modo == "ciclo") ciclo_simples();
  else if (modo == "varre") varredura(std::atoi(argv[2]), std::atof(argv[3]), std::atoi(argv[4]), argv[5][0] == '1', 5);
  else if (modo == "grandes") grandes(std::atoi(argv[2]), std::atof(argv[3]), std::atoi(argv[4]));
  return 0;
}
