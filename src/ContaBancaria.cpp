#include "ContaBancaria.hpp"
#include "Banco.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

using namespace std;

// Implementação dos métodos da classe ContaBancaria
ContaBancaria::ContaBancaria(int numero, const Cliente& titular, double saldoInicial) 
    : numero(numero), saldo(saldoInicial), titular(titular) {}
// Construtor com valores padrão
int ContaBancaria::getNumero() const { return numero; }
const Cliente& ContaBancaria::getTitular() const { return titular; }

string ContaBancaria::agoraISO() { // retorna timestamp atual em formato ISO 8601
    using namespace chrono; // para facilitar o uso de funções de tempo
    auto t = system_clock::to_time_t(system_clock::now());// obtém o tempo atual
    tm tm{}; // estrutura para armazenar tempo local
    #if defined(_MSC_VER) // Compilador MSVC
        localtime_s(&tm, &t);
    #elif defined(__MINGW32__) || defined(__MINGW64__) // Compilador MinGW
        tm = *localtime(&t);
    #else
        localtime_r(&t, &tm); // Compilador GCC/Clang   
    #endif
    ostringstream oss; // fluxo de string para formatação
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S"); // formata como ISO 8601 
    return oss.str(); // retorna a string formatada
}

string ContaBancaria::formatValor(double v) { // formata valor monetario como string
    ostringstream oss;
    oss << fixed << setprecision(2) << v;
    return "R$ " + oss.str();
}

void ContaBancaria::registrar(const string& tipo, const string& detalhe, double valor) { // registra uma transacao no extrato
    extrato.push_back(Movimento{agoraISO(), tipo, detalhe, valor, saldo});
}

void ContaBancaria::depositar(double valor) { // metodo para depositar valor
    if (valor <= 0) { cout << "Valor de deposito invalido.\n"; return; }
    saldo += valor;
    registrar("DEP", "deposito em conta", valor); // registra o deposito no extrato
    cout << "Deposito de " << formatValor(valor) << " realizado com sucesso.\n"; // confirma o deposito
    exibirSaldo();// exibe o saldo apos o deposito
}

void ContaBancaria::sacar(double valor) { // metodo para sacar valor
    if (valor <= 0) { cout << "Valor de saque invalido.\n"; return; } // valor negativo ou zero nao e permitido
    if (saldo < valor) { cout << "Saldo insuficiente para saque.\n"; return; } // verifica saldo suficiente
    saldo -= valor;
    registrar("SAQ", "saque em conta", valor); // registra o saque no extrato
    cout << "Saque de " << formatValor(valor) << " realizado com sucesso.\n";// confirma o saque
    exibirSaldo();// exibe o saldo apos o saque
}

void ContaBancaria::transferir(double valor, ContaBancaria& destino) { // metodo para transferir valor para outra conta
    saldo -= valor;
    destino.saldo += valor;
    registrar("TRF ENVIADA", "para conta " + to_string(destino.numero), valor);// registra a transferencia no extrato
    destino.registrar("TRF RECEBIDA", "de conta " + to_string(numero), valor);// registra a transferencia no extrato da conta destino
    cout << "Transferido: " << formatValor(valor) << " da conta " << numero << " para a conta " << destino.numero << "\n";// confirma a transferencia
    destino.exibirSaldo();// exibe o saldo da conta destino apos a transferencia
}

bool ContaBancaria::transferirParaNumero(double valor, int numeroDestino, Banco& banco) { // metodo para transferir valor para conta por numero
    if (valor <= 0) { cout << "Valor de transferencia invalido.\n"; return false; } // valor negativo ou zero nao e permitido
    if (saldo < valor) { cout << "Saldo insuficiente para transferencia.\n"; return false; } // verifica saldo suficiente

    auto destinoPtr = banco.encontrarPorNumero(numeroDestino); // procura a conta destino no banco
    if (!destinoPtr) { 
        cout << "Conta destino inexistente.\n"; 
        return false; 
    }
    transferir(valor, *destinoPtr); // realiza a transferencia
    return true;
}


bool ContaBancaria::transferirPix(double valor, const string& chavePix, Banco& banco) { // metodo para transferir valor via chave PIX
    if (valor <= 0) { cout << "Valor de transferencia invalido.\n"; return false; }
    if (saldo < valor) { cout << "Saldo insuficiente para transferencia.\n"; return false; }

    auto destinoPtr = banco.encontrarPorPix(chavePix);
    if (!destinoPtr) {
        return false; // nao encontrou a chave PIX
    }
    
    saldo -= valor;
    destinoPtr->saldo += valor;// realiza a transferencia

    string detalheRemetente = "via PIX para chave " + chavePix; // detalhes para o extrato
    string detalheDestino = "via PIX (chave " + chavePix + ") de conta " + to_string(this->numero); // detalhes para o extrato
    registrar("TRF ENVIADA", detalheRemetente, valor);// registra no extrato do remetente
    destinoPtr->registrar("TRF RECEBIDA", detalheDestino, valor); // registra no extrato do destinatario

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