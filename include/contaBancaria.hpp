#pragma once
#include <string>
#include <vector>
#include "cliente.hpp"

using namespace std;

class Banco; // Declaração avançada para evitar dependências

class ContaBancaria {
private:
    struct Movimento {
        string timestamp;
        string tipo;
        string detalhe;
        double valor;
        double saldoApos;
    };

    int numero;
    double saldo;
    Cliente titular;
    vector<Movimento> extrato;

    static string agoraISO();
    static string formatValor(double v);
    void registrar(const string& tipo, const string& detalhe, double valor);

public:
    ContaBancaria(int numero, const Cliente& titular, double saldoInicial = 0.0);
    int getNumero() const;
    const Cliente& getTitular() const;
    void depositar(double valor);
    void sacar(double valor);
    // Esta é a função de transferência base que usaremos
    void transferir(double valor, ContaBancaria& destino); 
    bool transferirParaNumero(double valor, int numeroDestino, Banco& banco);
    bool transferirPix(double valor, const string& chavePix, Banco& banco);
    void exibirSaldo() const;
    void exibirExtrato() const;
    void exibirInformacoes(const Banco& banco) const;
};