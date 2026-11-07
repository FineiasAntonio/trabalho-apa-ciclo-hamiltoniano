#pragma once

#include <chrono>
#include <optional>
#include <stdexcept>

namespace chd {

// Estouro de prazo não é resposta "inexistente". Quem chama distingue os dois.
class TempoEsgotado : public std::runtime_error {
 public:
  TempoEsgotado() : std::runtime_error("tempo esgotado") {}
};

struct Relogio {
  std::optional<std::chrono::steady_clock::time_point> limite;

  void verifica() const {
    if (limite && std::chrono::steady_clock::now() >= *limite) {
      throw TempoEsgotado();
    }
  }
};

}  // namespace chd
