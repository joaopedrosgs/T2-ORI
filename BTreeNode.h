//
// Created by pedro on 04/12/2017.
//

#ifndef T1_ORI_BTREENODE_H
#define T1_ORI_BTREENODE_H


#include <ostream>
#include "GerenciadorDeBlocos.h"

class BTreeNode {
    int *chaves;  // Vetor com as filhos do nó
    int ordem;      // Ordem do nó, que pode possuir de ordem-1 até 2t-1 filhos
    int *filhos; // Vetor de ponteiros para os nós filhos
    int numero_chaves;     // Numero atual de filhos no nó
    bool leaf; //Flag para declarar o nó como folha ou não
    int indice_no_arquivo;
    GerenciadorDeBlocos *gerenciador;
public:
    BTreeNode();   // Construtor padrao
    BTreeNode(int _t, bool _leaf, GerenciadorDeBlocos *_gerenciador);   // Construtor
    BTreeNode(int _t, bool _leaf, GerenciadorDeBlocos *_gerenciador, int _indice_no_arquivo);   // Construtor




    // Função para inserir uma nova chave na sub-árvore que possui esse nó como raíz.
    //Assume-se que o nó não está cheio quando essa função é chamada
    void insertNonFull(int k);

    // Função que divide o nó filho y desse nó. i é o indice_no_arquivo de y no vetor de filhos filhos[]
    // O nó filho deve estar cheio quando essa função é chamada
    void splitChild(int i, BTreeNode *no_doador);

    // Função que imprime a árvore
    void traverse();

    // Função que busca uma chave k na subárvore que possui esse nó como raíz. Retorna NULL se não encontrar
    int search(int k);

    void SalvarNoArquivo();

    ~BTreeNode();

    friend fstream &operator<<(fstream &os, const BTreeNode &node);

    void ImprimirInfo();

// Permite que BTree acesse atributos privados dessa classe
    friend class BTree;

    void SetInfo(int _ordem, int numero_chaves, int *chaves, int *filhos, bool _leaf, int _indice_no_arquovo);
};

#endif //T1_ORI_BTREENODE_H
