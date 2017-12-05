//
// Created by pedro on 04/12/2017.
//

#include "BTree.h"


// Função para inserir uma chave na árvore

BTree::BTree(int _t, const char *_nomeArquivo) : gerenciador(
        GerenciadorDeBlocos(((_t*2)+ 4)*sizeof(int), _nomeArquivo)) {
    nomeArquivo = _nomeArquivo;
    t = _t;
    atual = -1;
}


// Função para imprimir a árvore
void BTree::traverse() {
    if (atual != -1) {
        int indice = 0;
        BTreeNode bTreeNode;
        gerenciador.CarregarBloco(indice, &bTreeNode);
        bTreeNode.traverse();

    }
}

// Função para buscar uma chave na árvore
BTreeNode *BTree::search(int k) {
    if (atual < 0) {
        return nullptr;
    } else {
        BTreeNode bTreeNode;
        gerenciador.CarregarBloco(0, &bTreeNode);
        bTreeNode.search(k);
    }
}

void BTree::insert(int k) {
    // Se a árvore está vazia, cria o nó raíz e insere a chave nele
    if (atual < 0) {
        // Aloca memória para o nó
        BTreeNode *bTreeNode = new BTreeNode(t,true,&gerenciador);
        bTreeNode->chaves[0] = k;  // Insere a chave
        for(int i=0; i<(bTreeNode->ordem*2-1); i++)
            std::cout << bTreeNode->chaves[i] << std::endl;
        bTreeNode->numero_chaves = 1;  // Atualiza o número de filhos no nó
        bTreeNode->indice_no_arquivo = atual + 1;
        delete (bTreeNode);
        atual++;
    } else // Se não é uma árvore vazia
    {
        BTreeNode *raiz = new BTreeNode;
        gerenciador.CarregarBloco(atual, raiz);
        // Se a raíz estiver cheia, o nó é dividido e a árvore cresce
        if (raiz->numero_chaves >= 2 * raiz->ordem - 1) {

            BTreeNode *novo_no = new BTreeNode(t, false, &gerenciador);

            // Torna a velha raíz um nó filho da nova raíz
            novo_no->filhos[0] = atual;

            // Divide a velha raíz
            novo_no->splitChild(0, raiz);

            // Procede com a divisão, dividindo as filhos, enviando uma ao nó pai e decidindo qual dos nós filhos terá
            // A nova chave inserida
            int i = 0;
            if (novo_no->chaves[0] < k)
                i++;
            BTreeNode *ondeInserir = new BTreeNode;
            gerenciador.CarregarBloco(novo_no->filhos[i], ondeInserir);
            ondeInserir->insertNonFull(k);

            // Altera para a nova raíz
            atual++;
            delete (novo_no);
            delete (ondeInserir);
        } else {
            //Se a raíz não está cheia, simplesmente insere nela
            raiz->insertNonFull(k);
        }
        delete (raiz);

    }

}