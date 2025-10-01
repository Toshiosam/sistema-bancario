#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ContaBancaria.hpp"

using namespace std;

class Banco {
private:
    unordered_map<int, shared_ptr<ContaBancaria>> contasPorNumero;
    unordered_map<string, shared_ptr<ContaBancaria>> contasPorPix;

public:
    Banco() = default;

    void cadastrarConta(const shared_ptr<ContaBancaria>& conta) {
        contasPorNumero[conta->getNumero()] = conta;
    }

    bool registrarChavePix(const string& chave, int numeroConta) {
        auto it = contasPorNumero.find(numeroConta);
        if (it == contasPorNumero.end()) {
            return false;
        }
        contasPorPix[chave] = it->second;
        return true;
    }

    shared_ptr<ContaBancaria> encontrarPorNumero(int numeroConta) {
        auto it = contasPorNumero.find(numeroConta);
        return (it != contasPorNumero.end()) ? it->second : nullptr;
    }

    shared_ptr<ContaBancaria> encontrarPorPix(const string& chavePix) {
        auto it = contasPorPix.find(chavePix);
        return (it != contasPorPix.end()) ? it->second : nullptr;
    }

    vector<string> encontrarChavesPorConta(int numeroConta) const {
        vector<string> chavesEncontradas;
        for (const auto& par : contasPorPix) {
            if (par.second->getNumero() == numeroConta) {
                chavesEncontradas.push_back(par.first);
            }
        }
        return chavesEncontradas;
    }
};