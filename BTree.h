//
// Created by pedro on 04/12/2017.
//

#ifndef T1_ORI_BTREE_H
#define T1_ORI_BTREE_H


#include "BTreeNode.h"

using std::string;

class BTree {
    int t;  // Ordem da árvore
    const char* nomeArquivo;
    int atual;
    GerenciadorDeBlocos<BTreeNode> gerenciador;
public:
    // Construtor
    BTree(int _t, const char *_nomeArquivo);

    // Função para imprimir a árvore
    void traverse();

    // Função para buscar uma chave na árvore
    BTreeNode *search(int k);

    // Função que insere uma chave nessa árvore
    void insert(int k);
};


#endif //T1_ORI_BTREE_H
