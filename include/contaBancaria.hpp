//contaBancaria.hpp define a classe ContaBancaria que representa uma conta bancaria 
//com funcionalidades como deposito, saque, transferencia e exibicao de saldo e extrato.

//o struct Movimento registra cada transacao com timestamp, tipo, detalhe, valor e saldo apos a transacao.
//representa uma unica transação no extrato da conta. 
// a declaração da classe Banco é feita antecipadamente para permitir o uso de ponteiros compartilhados
//para objetos ContaBancaria dentro da classe Banco, evitando dependências circulares.

#pragma once
#include <string>
#include <vector>
#include "cliente.hpp"

using namespace std;

class Banco; // Declaração antecipada para evitar dependência circular

class ContaBancaria { // declara a classe ContaBancaria
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

    static string agoraISO();// retorna timestamp atual em formato ISO 8601
    static string formatValor(double v);// formata valor monetario como string
    void registrar(const string& tipo, const string& detalhe, double valor);// registra uma transacao no extrato

public:
    ContaBancaria(int numero, const Cliente& titular, double saldoInicial = 0.0); // construtor com valores padrao
    int getNumero() const; // metodo get para numero da conta
    const Cliente& getTitular() const; // metodo get para titular da conta
    void depositar(double valor); // metodo para depositar valor
    void sacar(double valor); // metodo para sacar valor
   
    void transferir(double valor, ContaBancaria& destino); // metodo para transferir valor para outra conta
    bool transferirParaNumero(double valor, int numeroDestino, Banco& banco); // metodo para transferir valor para conta por numero
    bool transferirPix(double valor, const string& chavePix, Banco& banco); // metodo para transferir valor via chave PIX
    void exibirSaldo() const; // metodo para exibir saldo atual
    void exibirExtrato() const; // metodo para exibir extrato de transacoes
    void exibirInformacoes(const Banco& banco) const; // metodo para exibir informacoes da conta, incluindo chaves PIX
};