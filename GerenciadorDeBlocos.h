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

using namespace std;

class GerenciadorDeBlocos {
    int tamanhoBloco;
    const char *nomeArquivo;
    fstream arquivo;

    void zerarBlocoEm(int indice);

    void novoBloco();

public:

    GerenciadorDeBlocos(int tamanhoBloco, const char *nomeArquivo);

    //Pega um indice e copia seu conteudo para o BTreeNode
    void CarregarBloco(int indice, BTreeNode *destino);

    // Salva um BTreeNode em um indice (serializando-o antes, tambem zera o bloco antes)
    void SalvarBloco(int indice, BTreeNode *origem);

    //Cria um novo bloco (e zera ele)
    void NovoBloco();

    //Pega o tamanho do arquivo e divide pelo tamanho do bloco, obtendo assim o indice
    int UltimoIndice();

    //Converte um buffer para um BTreeNode
    void Deserializar(BTreeNode *destino, char *buffer);
};


#endif //T1_ORI_GERENCIADORDEBLOCOS_H
