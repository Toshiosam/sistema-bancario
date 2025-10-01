#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <string>
#include "Banco.hpp"
#include "ContaBancaria.hpp"
#include "cliente.hpp"

using namespace std;

// Função robusta para ler entradas numéricas, corrigindo o problema do menu
template <typename T>
T lerEntradaNumerica(const string& mensagem) {
    T valor;
    while (true) {
        cout << mensagem;
        cin >> valor;
        if (cin.good()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer
            return valor;
        } else {
            cout << "Entrada invalida. Por favor, digite um valor valido.\n";
            cin.clear(); // Limpa o estado de erro
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta a entrada ruim
        }
    }
}

void exibirMenu() {
    cout << "\n===== MENU DO BANCO =====\n";
    cout << "1. Depositar\n2. Sacar\n3. Transferir (por numero da conta)\n4. Transferir (PIX)\n5. Exibir Saldo\n6. Exibir Extrato\n7. Listar Todas as Contas\n8. Criar Nova Conta\n9. Cadastrar Chave PIX\n0. Sair" << endl;
}

int main() {
    Banco banco;
    vector<shared_ptr<ContaBancaria>> todasAsContas;
    todasAsContas.push_back(make_shared<ContaBancaria>(1001, Cliente("Ana", "111.111.111-11"), 1000.0));
    todasAsContas.push_back(make_shared<ContaBancaria>(1002, Cliente("Bruno", "222.222.222-22"), 500.0));
    todasAsContas.push_back(make_shared<ContaBancaria>(1003, Cliente("Carla", "333.333.333-33"), 250.0));
    for(const auto& conta : todasAsContas) { banco.cadastrarConta(conta); }
    banco.registrarChavePix("bruno@email.com", 1002);
    int proximoNumeroConta = 1004;
    cout << "Bem-vindo ao Sistema Bancario!" << endl;

    while (true) {
        exibirMenu();
        int opcao = lerEntradaNumerica<int>("Escolha uma opcao: ");

        if (opcao == 0) { cout << "Obrigado por usar o sistema. Ate logo!\n"; break; }

        shared_ptr<ContaBancaria> conta;
        int numeroContaOrigem;

        switch (opcao) {
            case 1:
                numeroContaOrigem = lerEntradaNumerica<int>("Digite o numero da conta para depositar: ");
                conta = banco.encontrarPorNumero(numeroContaOrigem);
                if (conta) {
                    double valor = lerEntradaNumerica<double>("Digite o valor do deposito: ");
                    conta->depositar(valor);
                } else { cout << "Conta nao encontrada.\n"; }
                break;
            case 2:
                numeroContaOrigem = lerEntradaNumerica<int>("Digite o numero da sua conta: ");
                conta = banco.encontrarPorNumero(numeroContaOrigem);
                if (conta) {
                    double valor = lerEntradaNumerica<double>("Digite o valor do saque: ");
                    conta->sacar(valor);
                } else { cout << "Conta nao encontrada.\n"; }
                break;
            case 3:
                numeroContaOrigem = lerEntradaNumerica<int>("Digite o numero da sua conta (origem): ");
                conta = banco.encontrarPorNumero(numeroContaOrigem);
                if (conta) {
                    int numeroContaDestino = lerEntradaNumerica<int>("Digite o numero da conta de destino: ");
                    double valor = lerEntradaNumerica<double>("Digite o valor a transferir: ");
                    conta->transferirParaNumero(valor, numeroContaDestino, banco);
                } else { cout << "Sua conta de origem nao foi encontrada.\n"; }
                break;
            case 4: {
                numeroContaOrigem = lerEntradaNumerica<int>("Digite o numero da sua conta (origem): ");
                conta = banco.encontrarPorNumero(numeroContaOrigem);
                if (!conta) { cout << "Sua conta de origem nao foi encontrada.\n"; break; }

                cout << "Digite a chave PIX de destino: ";
                string chavePix;
                getline(cin, chavePix);

                double valor = lerEntradaNumerica<double>("Digite o valor a transferir: ");

                bool sucesso = conta->transferirPix(valor, chavePix, banco);

                if (!sucesso) {
                    cout << "Chave PIX '" << chavePix << "' nao encontrada.\n";
                    char resposta;
                    cout << "Deseja tentar a transferencia por numero da conta? (S/N): ";
                    cin >> resposta;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (resposta == 'S' || resposta == 's') {
                        int numeroContaDestino = lerEntradaNumerica<int>("Digite o numero da conta de destino: ");
                        conta->transferirParaNumero(valor, numeroContaDestino, banco);
                    } else {
                        cout << "Transferencia cancelada.\n";
                    }
                }
                break;
            }
            case 5:
                numeroContaOrigem = lerEntradaNumerica<int>("Digite o numero da conta: ");
                conta = banco.encontrarPorNumero(numeroContaOrigem);
                if (conta) { conta->exibirSaldo(); } else { cout << "Conta nao encontrada.\n"; }
                break;
            case 6:
                numeroContaOrigem = lerEntradaNumerica<int>("Digite o numero da conta: ");
                conta = banco.encontrarPorNumero(numeroContaOrigem);
                if (conta) { conta->exibirExtrato(); } else { cout << "Conta nao encontrada.\n"; }
                break;
            case 7:
                cout << "\n--- Contas Cadastradas no Banco ---\n";
                for (const auto& c : todasAsContas) {
                    c->exibirInformacoes(banco);
                    cout << "---------------------------------\n";
                }
                break;
            case 8: {
                cout << "--- Cadastro de Novo Cliente e Conta ---\n";
                cout << "Digite o nome completo do cliente: ";
                string nome;
                getline(cin, nome);
                cout << "Digite o CPF do cliente: ";
                string cpf;
                getline(cin, cpf);
                double saldoInicial = lerEntradaNumerica<double>("Digite o valor do deposito inicial (ou 0 para nenhum): ");
                auto novaConta = make_shared<ContaBancaria>(proximoNumeroConta, Cliente(nome, cpf), saldoInicial);
                todasAsContas.push_back(novaConta);
                banco.cadastrarConta(novaConta);
                cout << "\nConta criada com sucesso! O numero da nova conta e: " << proximoNumeroConta++ << endl;
                break;
            }
            case 9: {
                int numeroConta = lerEntradaNumerica<int>("Digite o numero da conta para vincular a chave: ");
                if (banco.encontrarPorNumero(numeroConta)) {
                    cout << "Digite a nova chave PIX (ex: email, cpf, telefone): ";
                    string chavePix;
                    getline(cin, chavePix);
                    if (banco.registrarChavePix(chavePix, numeroConta)) {
                        cout << "Chave PIX '" << chavePix << "' cadastrada com sucesso para a conta " << numeroConta << "!\n";
                    }
                } else { cout << "Conta com numero " << numeroConta << " nao encontrada.\n"; }
                break;
            }
            default:
                cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    }
    return 0;
}