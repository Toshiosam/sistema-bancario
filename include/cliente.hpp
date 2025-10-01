#pragma once
#include <string>

using namespace std;

class Cliente {
private:
    string nome;
    string cpf;

public:
    Cliente(const string& nome = "", const string& cpf = "")
        : nome(nome), cpf(cpf) {}

    const string& getNome() const { return nome; }
    const string& getCpf()  const { return cpf;  }
};