// cliente.hpp define a classe cliente com nome e CPF 
//e o construtor permite criar um objeto com nome e CPF oo vazio se nenhum valor for fornecido.
//metodos get fornecem apenas acesso a leitura aos atributos privados.

#pragma once
#include <string>

using namespace std;

class Cliente {
private:
    string nome;
    string cpf;

public:
    Cliente(const string& nome = "", const string& cpf = "") //construtor com valores padrao
        : nome(nome), cpf(cpf) {}

    const string& getNome() const { return nome; }
    const string& getCpf()  const { return cpf;  }
};