//
// Created by pedro on 04/12/2017.
//

#ifndef T1_ORI_BTREENODE_H
#define T1_ORI_BTREENODE_H


#include <ostream>
#include "GerenciadorDeBlocos.h"

class BTreeNode {
    int *chaves;  // Vetor com as chaves do nó
    int ordem;      // Ordem do nó, que pode possuir de ordem-1 até 2*ordem-1 filhos
    int *filhos; // Vetor de indices para os filhos
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
    /*
// A function that returns the index of the first key that is greater
    // or equal to k
    int findKey(int k);

    // A wrapper function to remove the key k in subtree rooted with
    // this node.
    void remove(int k);

    // A function to remove the key present in idx-th position in
    // this node which is a leaf
    void removeFromLeaf(int idx);

    // A function to remove the key present in idx-th position in
    // this node which is a non-leaf node
    void removeFromNonLeaf(int idx);

    // A function to get the predecessor of the key- where the key
    // is present in the idx-th position in the node
    int getPred(int idx);

    // A function to get the successor of the key- where the key
    // is present in the idx-th position in the node
    int getSucc(int idx);

    // A function to fill up the child node present in the idx-th
    // position in the C[] array if that child has less than t-1 keys
    void fill(int idx);

    // A function to borrow a key from the C[idx-1]-th node and place
    // it in C[idx]th node
    void borrowFromPrev(int idx);

    // A function to borrow a key from the C[idx+1]-th node and place it
    // in C[idx]th node
    void borrowFromNext(int idx);

    // A function to merge idx-th child of the node with (idx+1)th child of
    // the node
    void merge(int idx);

     */

};

#endif //T1_ORI_BTREENODE_H
