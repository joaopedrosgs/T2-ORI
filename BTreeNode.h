//
// Created by pedro on 04/12/2017.
//

#ifndef T1_ORI_BTREENODE_H
#define T1_ORI_BTREENODE_H


#include "GerenciadorDeBlocos.cpp"

class BTreeNode {
    int *keys;  // Vetor com as chaves do nó
    int ordem;      // Ordem do nó, que pode possuir de ordem-1 até 2t-1 chaves
    int *chaves; // Vetor de ponteiros para os nós filhos
    int numero_chaves;     // Numero atual de chaves no nó
    bool leaf; //Flag para declarar o nó como folha ou não
    int indice_no_arquivo;
    GerenciadorDeBlocos<BTreeNode> *gerenciador;
public:
    BTreeNode();   // Construtor padrao
    BTreeNode(int _t, bool _leaf,     GerenciadorDeBlocos<BTreeNode> *_gerenciador);   // Construtor

    // Função para inserir uma nova chave na sub-árvore que possui esse nó como raíz.
    //Assume-se que o nó não está cheio quando essa função é chamada
    void insertNonFull(int k);

    // Função que divide o nó filho y desse nó. i é o indice_no_arquivo de y no vetor de filhos chaves[]
    // O nó filho deve estar cheio quando essa função é chamada
    void splitChild(int i, BTreeNode *no_doador);

    // Função que imprime a árvore
    void traverse();

    // Função que busca uma chave k na subárvore que possui esse nó como raíz. Retorna NULL se não encontrar
    BTreeNode *search(int k);

    void SalvarNoArquivo();
    ~BTreeNode();

// Permite que BTree acesse atributos privados dessa classe
    friend class BTree;
};

#endif //T1_ORI_BTREENODE_H
