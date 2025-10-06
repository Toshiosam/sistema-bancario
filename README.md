Sistema bancario feito em c++ para a disciplina de linguagem de programação 1

LP1 - Sistema Bancário Simples
Desenvolver um programa em C++ que simule um sistema bancário simples, incluindo classes para Clientes e Contas Bancárias, 
implementando as ações de depósito, saque e transferência. O programa deve utilizar conceitos de classes, objetos, construtores e sobrecarga de métodos.


Descrição da Atividade:

Classe Cliente:   Crie uma classe Cliente que represente um cliente do banco.
Atributos privados: nome (string): nome do cliente. cpf (string): CPF ou identificador único do cliente.
Construtor: Inicialize os atributos nome e cpf.
Métodos públicos:
Métodos getter para acessar o nome e o cpf.

Classe ContaBancaria:
Crie uma classe ContaBancaria que represente uma conta bancária.
Atributos privados: numero (int): número da conta. saldo (double): saldo da conta.
titular (Cliente): objeto do tipo Cliente que é o titular da conta.
Construtor: Inicialize o numero, o titular e, opcionalmente, o saldo (padrão zero).
Métodos públicos:
void depositar(double valor): método para depositar um valor na conta.
void sacar(double valor): método para sacar um valor da conta (verifique se há saldo suficiente).
Sobrecarga do método transferir:
void transferir(double valor, ContaBancaria &destino): método para transferir um valor para outra conta (verifique se há saldo suficiente).
void transferir(double valor, ContaBancaria &destino1, ContaBancaria &destino2): método para transferir o valor dividido igualmente entre duas contas destino (verifique se há saldo suficiente).
void exibirSaldo(): exibe o saldo atual da conta.
void exibirInformacoes(): exibe as informações do titular e da conta.
Sobrecarga de Métodos: Sobrecargue o método transferir criando duas versões:
Uma que recebe um valor e uma conta destino.Outra que recebe um valor e duas contas destino (nesse caso, o valor precisa ser dividido entre essas duas contas).

Função Principal (main):
Crie objetos Cliente e ContaBancaria.
Demonstre as operações de depósito, saque e transferência entre contas, incluindo a transferência para duas contas simultaneamente.
Exiba os saldos e informações dos clientes após as operações.

