//
// Created by pedro on 04/12/2017.
//

#include "BTree.h"


// Função para inserir uma chave na árvore

BTree::BTree(int _t, const char *_nomeArquivo) : gerenciador(
        GerenciadorDeBlocos(sizeof(int) * (_t*4), _nomeArquivo)) {
    nomeArquivo = _nomeArquivo;
    t = _t; // t é a ordem
    atual = -1; // Não tem nenhuma arvore ainda, então colocamos um valor invalido
    remove(nomeArquivo); // Remove o arquivo se ja existe
    gerenciador.NovoBloco(); // Bloco incial


}


// Função para imprimir a árvore
void BTree::traverse() {
    if (atual >= 0) {
        BTreeNode bTreeNode(t, false, &gerenciador);
        gerenciador.CarregarBloco(atual, &bTreeNode);
        bTreeNode.traverse();

    }
}

// Função para buscar uma chave na árvore
int BTree::search(int k) {
    if (atual < 0) {
        return -1;
    } else {
        BTreeNode bTreeNode(t, true, &gerenciador);
        gerenciador.CarregarBloco(atual, &bTreeNode);
        return bTreeNode.search(k);
    }
}
// Função que insere um elemento
void BTree::insert(int k) {
    // Se a árvore está vazia, cria o nó raíz e insere a chave nele
    if (atual < 0) {
        // Aloca memória para o nó
        BTreeNode *bTreeNode = new BTreeNode(t, true, &gerenciador, ++atual);
        bTreeNode->chaves[0] = k;  // Insere a chave
        for (int i = 0; i < (bTreeNode->ordem * 2 - 1); i++)
            bTreeNode->numero_chaves = 1;  // Atualiza o número de filhos no nó
        delete (bTreeNode);

    } else { // Se não é uma árvore vazia

        BTreeNode *raiz = new BTreeNode(t, false, &gerenciador); //Carrega a raiz
        gerenciador.CarregarBloco(atual, raiz);
        // Se a raíz estiver cheia, o nó é dividido e a árvore cresce
        if (raiz->numero_chaves >= 2 * raiz->ordem - 1) {
            // Elemento que vai ser o novo pai da raiz e de uma nova sub-arvore
            BTreeNode *novo_pai = new BTreeNode(t, false, &gerenciador, gerenciador.UltimoIndice());
            gerenciador.NovoBloco(); // Inicializa o novo bloco que vai ser usado pelo pai

            // Torna a velha raíz um nó filho da nova raíz
            novo_pai->filhos[0] = atual;

            // Divide a velha raíz
            novo_pai->splitChild(0, raiz);

            // Procede com a divisão, dividindo as filhos, enviando uma ao nó pai e decidindo qual dos nós filhos terá
            // A nova chave inserida
            int i = 0;
            if (novo_pai->chaves[0] < k)
                i++;
            // Carrega a sub-arvore onde vai ser inserido o elemento
            BTreeNode *ondeInserir = new BTreeNode(t, true, &gerenciador);
            gerenciador.CarregarBloco(novo_pai->filhos[i], ondeInserir);
            ondeInserir->insertNonFull(k);

            // Altera para a nova raíz
            atual = novo_pai->indice_no_arquivo;
            delete (novo_pai); // Salva o novo_pai para o arquivo e deleta-o
            delete (ondeInserir); // Salva o ondeInserir para o arquivo e deleta-o
        } else {
            //Se a raíz não está cheia, simplesmente insere nela
            raiz->insertNonFull(k);
        }
        delete (raiz); // Volta a raiz para o arquivo e deleta-a

    }

}
 // Retorna um BTreeNode usando um indice
BTreeNode BTree::ObterNo(int indice) {
    BTreeNode node(t, true, &gerenciador);
    gerenciador.CarregarBloco(indice, &node);
    return node;

}
