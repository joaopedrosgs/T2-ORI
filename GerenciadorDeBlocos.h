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
class BTreeNode;

class BTreeNode;

using namespace std;

class GerenciadorDeBlocos {
    int tamanhoBloco;
    const char *nomeArquivo;
    fstream arquivo;

    bool ehValido();
    void zerarBlocoEm(int x);

public:
    int GetTamanhoBloco();

    GerenciadorDeBlocos(int tamanhoBloco, const char *nomeArquivo);

    void CarregarBloco(int indice, BTreeNode *destino);

    void SalvarBloco(int indice, BTreeNode *origem);

    void NovoBloco();

    void DeletarBloco(int indice);


    int UltimoIndice();

    void novoBloco();
};


#endif //T1_ORI_GERENCIADORDEBLOCOS_H
