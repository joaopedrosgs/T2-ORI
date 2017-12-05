//
// Created by pedro on 04/12/2017.
//

#ifndef T1_ORI_GERENCIADORDEBLOCOS_H
#define T1_ORI_GERENCIADORDEBLOCOS_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdio>

using namespace std;

template<typename T>
class GerenciadorDeBlocos {
    int tamanhoBloco;
    const char* nomeArquivo;
    ifstream leitura;
    ofstream escrita;

    bool ehValido();

public:

    GerenciadorDeBlocos(int tamanhoBloco, const char *nomeArquivo);

    void CarregarBloco(int indice, T *destino);

    void SalvarBloco(int indice, T *origem);

    void NovoBloco();

    void DeletarBloco(int indice);

};


#endif //T1_ORI_GERENCIADORDEBLOCOS_H
