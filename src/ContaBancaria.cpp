#include "ContaBancaria.hpp"
#include "Banco.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

// ... (todo o início do arquivo, até transferirPix, permanece igual) ...

ContaBancaria::ContaBancaria(int numero, const Cliente& titular, double saldoInicial)
    : numero(numero), saldo(saldoInicial), titular(titular) {}

int ContaBancaria::getNumero() const { return numero; }
const Cliente& ContaBancaria::getTitular() const { return titular; }

string ContaBancaria::agoraISO() {
    using namespace chrono;
    auto t = system_clock::to_time_t(system_clock::now());
    tm tm{};
    #if defined(_MSC_VER)
        localtime_s(&tm, &t);
    #elif defined(__MINGW32__) || defined(__MINGW64__)
        tm = *localtime(&t);
    #else
        localtime_r(&t, &tm);
    #endif
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

string ContaBancaria::formatValor(double v) {
    ostringstream oss;
    oss << fixed << setprecision(2) << v;
    return "R$ " + oss.str();
}

void ContaBancaria::registrar(const string& tipo, const string& detalhe, double valor) {
    extrato.push_back(Movimento{agoraISO(), tipo, detalhe, valor, saldo});
}

void ContaBancaria::depositar(double valor) {
    if (valor <= 0) { cout << "Valor de deposito invalido.\n"; return; }
    saldo += valor;
    registrar("DEP", "deposito em conta", valor);
    cout << "Deposito de " << formatValor(valor) << " realizado com sucesso.\n";
    exibirSaldo();
}

void ContaBancaria::sacar(double valor) {
    if (valor <= 0) { cout << "Valor de saque invalido.\n"; return; }
    if (saldo < valor) { cout << "Saldo insuficiente para saque.\n"; return; }
    saldo -= valor;
    registrar("SAQ", "saque em conta", valor);
    cout << "Saque de " << formatValor(valor) << " realizado com sucesso.\n";
    exibirSaldo();
}

void ContaBancaria::transferir(double valor, ContaBancaria& destino) {
    saldo -= valor;
    destino.saldo += valor;
    registrar("TRF ENVIADA", "para conta " + to_string(destino.numero), valor);
    destino.registrar("TRF RECEBIDA", "de conta " + to_string(numero), valor);
    cout << "Transferido: " << formatValor(valor) << " da conta " << numero << " para a conta " << destino.numero << "\n";
    destino.exibirSaldo();
}

bool ContaBancaria::transferirParaNumero(double valor, int numeroDestino, Banco& banco) {
    if (valor <= 0) { cout << "Valor de transferencia invalido.\n"; return false; }
    if (saldo < valor) { cout << "Saldo insuficiente para transferencia.\n"; return false; }

    auto destinoPtr = banco.encontrarPorNumero(numeroDestino);
    if (!destinoPtr) { 
        cout << "Conta destino inexistente.\n"; 
        return false; 
    }
    transferir(valor, *destinoPtr);
    return true;
}

// LÓGICA DE TRANSFERÊNCIA PIX CORRIGIDA
bool ContaBancaria::transferirPix(double valor, const string& chavePix, Banco& banco) {
    if (valor <= 0) { cout << "Valor de transferencia invalido.\n"; return false; }
    if (saldo < valor) { cout << "Saldo insuficiente para transferencia.\n"; return false; }

    auto destinoPtr = banco.encontrarPorPix(chavePix);
    if (!destinoPtr) {
        return false; // Retorna false silenciosamente se a chave não existe
    }
    
    // Se encontrou a chave, faz a transferência
    saldo -= valor;
    destinoPtr->saldo += valor;

    string detalheRemetente = "via PIX para chave " + chavePix;
    string detalheDestino = "via PIX (chave " + chavePix + ") de conta " + to_string(this->numero);
    registrar("TRF ENVIADA", detalheRemetente, valor);
    destinoPtr->registrar("TRF RECEBIDA", detalheDestino, valor);

    cout << "Transferido (PIX): " << formatValor(valor)
         << " da conta " << this->numero
         << " para chave " << chavePix << "\n";
    destinoPtr->exibirSaldo();
    return true;
}

void ContaBancaria::exibirSaldo() const {
    cout << "--> Saldo atual da conta " << numero << ": " << formatValor(saldo) << "\n";
}

void ContaBancaria::exibirExtrato() const {
    cout << "=== Extrato da conta " << numero << " (" << titular.getNome() << ") ===\n";
    if (extrato.empty()) { cout << "Sem movimentacoes.\n"; return; }
    for (const auto& m : extrato) {
        cout << "[" << m.timestamp << "] " << m.tipo << " - " << m.detalhe << " - valor: " << formatValor(m.valor) << " - saldo atual: " << formatValor(m.saldoApos) << "\n";
    }
}

void ContaBancaria::exibirInformacoes(const Banco& banco) const {
    cout << "Titular: " << titular.getNome() << ", CPF: " << titular.getCpf() << "\n";
    cout << "Numero da Conta: " << numero << ", Saldo: " << formatValor(saldo) << "\n";
    vector<string> chaves = banco.encontrarChavesPorConta(numero);
    if (chaves.empty()) {
        cout << "Chaves PIX: Nenhuma cadastrada.\n";
    } else {
        cout << "Chaves PIX: ";
        for (size_t i = 0; i < chaves.size(); ++i) {
            cout << chaves[i] << (i == chaves.size() - 1 ? "" : ", ");
        }
        cout << "\n";
    }
}