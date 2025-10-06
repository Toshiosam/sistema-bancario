// banco.hpp define a classe Banco que gerencia contas bancarias
// funcionalidades para cadastrar contas, registrar chaves PIX e encontrar contas por numero ou chave PIX

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ContaBancaria.hpp"

using namespace std;

class Banco { // declara a classe Banco
private:
    unordered_map<int, shared_ptr<ContaBancaria>> contasPorNumero; // Mapeia numero da conta para o objeto ContaBancaria
    unordered_map<string, shared_ptr<ContaBancaria>> contasPorPix; // Mapeia chave PIX para o objeto ContaBancaria

public:
    Banco() = default;

    void cadastrarConta(const shared_ptr<ContaBancaria>& conta) { // Usa shared_ptr para gerenciar a conta
        contasPorNumero[conta->getNumero()] = conta;
    }

    bool registrarChavePix(const string& chave, int numeroConta) { // Registra uma chave PIX para uma conta existente
        auto it = contasPorNumero.find(numeroConta);
        if (it == contasPorNumero.end()) {
            return false;
        }
        contasPorPix[chave] = it->second;
        return true;
    }

    shared_ptr<ContaBancaria> encontrarPorNumero(int numeroConta) { // Encontra uma conta pelo numero
        auto it = contasPorNumero.find(numeroConta);
        return (it != contasPorNumero.end()) ? it->second : nullptr;
    }

    shared_ptr<ContaBancaria> encontrarPorPix(const string& chavePix) { // Encontra uma conta pela chave PIX
        auto it = contasPorPix.find(chavePix);
        return (it != contasPorPix.end()) ? it->second : nullptr;
    }

    vector<string> encontrarChavesPorConta(int numeroConta) const { // Encontra todas as chaves PIX associadas a uma conta
        vector<string> chavesEncontradas;
        for (const auto& par : contasPorPix) {
            if (par.second->getNumero() == numeroConta) {
                chavesEncontradas.push_back(par.first);
            }
        }
        return chavesEncontradas;
    }
};