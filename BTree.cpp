//
// Created by pedro on 04/12/2017.
//

#include "BTree.h"


// Função para inserir uma chave na árvore

BTree::BTree(int _t, const char *_nomeArquivo) : gerenciador(
        GerenciadorDeBlocos(50, _nomeArquivo)) {
    nomeArquivo = _nomeArquivo;
    t = _t;
    atual = -1;
    remove(nomeArquivo);
    gerenciador.NovoBloco();


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
        BTreeNode *bTreeNode = new BTreeNode(t,true,&gerenciador, ++atual);
        bTreeNode->chaves[0] = k;  // Insere a chave
        for(int i=0; i<(bTreeNode->ordem*2-1); i++)
            std::cout << bTreeNode->chaves[i] << std::endl;
        bTreeNode->numero_chaves = 1;  // Atualiza o número de filhos no nó
        delete (bTreeNode);
    } else // Se não é uma árvore vazia
    {
        BTreeNode *raiz = new BTreeNode;
        gerenciador.CarregarBloco(atual, raiz);
        // Se a raíz estiver cheia, o nó é dividido e a árvore cresce
        if (raiz->numero_chaves >= 2 * raiz->ordem - 1) {

            BTreeNode *novo_pai = new BTreeNode(t, true, &gerenciador, gerenciador.UltimoIndice());

            // Torna a velha raíz um nó filho da nova raíz
            novo_pai->filhos[0] = atual;

            // Divide a velha raíz
            novo_pai->splitChild(0, raiz);

            // Procede com a divisão, dividindo as filhos, enviando uma ao nó pai e decidindo qual dos nós filhos terá
            // A nova chave inserida
            int i = 0;
            if (novo_pai->chaves[0] < k)
                i++;
            BTreeNode *ondeInserir = new BTreeNode(t, true, &gerenciador);
            gerenciador.CarregarBloco(novo_pai->filhos[i], ondeInserir);
            ondeInserir->insertNonFull(k);

            // Altera para a nova raíz
            atual = novo_pai->indice_no_arquivo;
            delete (novo_pai);
            delete (ondeInserir);
        } else {
            //Se a raíz não está cheia, simplesmente insere nela
            raiz->insertNonFull(k);
        }
        delete (raiz);

    }

}