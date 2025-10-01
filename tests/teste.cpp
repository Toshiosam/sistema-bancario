#include <cassert>
#include <memory>
#include <cmath>
#include "Cliente.hpp"
#include "ContaBancaria.hpp"
#include "Banco.hpp"

static bool eq(double a, double b) { return std::fabs(a - b) < 1e-9; }

int main() {
    // Setup
    Cliente c1("Teste1", "000.000.000-00");
    Cliente c2("Teste2", "111.111.111-11");
    Cliente c3("Teste3", "222.222.222-22");

    Banco banco;
    auto a1 = std::make_shared<ContaBancaria>(1, c1, 1000.0);
    auto a2 = std::make_shared<ContaBancaria>(2, c2, 0.0);
    auto a3 = std::make_shared<ContaBancaria>(3, c3, 0.0);
    banco.cadastrarConta(a1);
    banco.cadastrarConta(a2);
    banco.cadastrarConta(a3);

    banco.registrarChavePix("pix2", 2);
    banco.registrarChavePix("pix3", 3);

    // Transferência original (objeto)
    a1->transferir(200.0, *a2);
    assert(eq(a1->getSaldo(), 800.0));
    assert(eq(a2->getSaldo(), 200.0));

    // Transferência dupla (objeto)
    a1->transferir(300.0, *a2, *a3); // 150 para cada
    assert(eq(a1->getSaldo(), 500.0));
    assert(eq(a2->getSaldo(), 350.0));
    assert(eq(a3->getSaldo(), 150.0));

    // Transferência por numero
    bool okNum = a1->transferirParaNumero(50.0, 2, banco);
    assert(okNum);
    assert(eq(a1->getSaldo(), 450.0));
    assert(eq(a2->getSaldo(), 400.0));

    // Transferência via PIX
    bool okPix = a1->transferirPix(25.0, "pix3", banco);
    assert(okPix);
    assert(eq(a1->getSaldo(), 425.0));
    assert(eq(a3->getSaldo(), 175.0));

    // Saque inválido não altera
    a1->sacar(9999);
    assert(eq(a1->getSaldo(), 425.0));

    // Depósito válido
    a2->depositar(75.0);
    assert(eq(a2->getSaldo(), 475.0));

    // Tudo certo
    std::cout << "[OK] Testes passaram com extrato/numero/pix.\n";
    return 0;
}
