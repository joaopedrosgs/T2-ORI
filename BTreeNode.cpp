//
// Created by pedro on 04/12/2017.
//

#include <iostream>
#include "BTreeNode.h"


BTreeNode::BTreeNode() {
    leaf = true;
    ordem=0;

}

// Construtor da classe de nó de árvore B

BTreeNode::BTreeNode(int t1, bool leaf1,     GerenciadorDeBlocos<BTreeNode> *_gerenciador) {
    // Copia o grau e a propriedade de ser folha ou não
    ordem = t1;
    leaf = leaf1;

    // Aloca memória para comportar o número máximo de chaves e ponteiros para nós-filho
    keys = new int[2 * ordem - 1];
    chaves = new int[2 * ordem];

    // Inicializa o número de chaves como 0
    numero_chaves = 0;
    gerenciador = _gerenciador;
}

// Função para imprimir a árvore
void BTreeNode::traverse() {
    // O nó possui numero_chaves chaves e numero_chaves+1 filhos, a função inicialmente percorre as numero_chaves chaves e numero_chaves filhos
    int i;
    for (i = 0; i < numero_chaves; i++) {
        // Se não for um nó folha, percorre primeiro a árvore apontada por cada uma das chaves
        if (leaf == false) {
            BTreeNode no_apropriado;
            gerenciador->CarregarBloco(i, &no_apropriado);
            no_apropriado.traverse();
        }
        std::cout << " " << keys[i];
    }

    // Imprime a árvore apontada pela ultima chave
    if (leaf == false) {
        BTreeNode no_apropriado;
        gerenciador->CarregarBloco(i, &no_apropriado);
        no_apropriado.traverse();
    }
}

// Funçao que busca uma chave k na sub-arvore que possui esse nó como raíz
BTreeNode *BTreeNode::search(int k) {
    // Encontra a primeira chave maior ou igual a k, pois indica a posição em que deve descer ou encontra a chave
    int i = 0;
    while (i < numero_chaves && k > keys[i])
        i++;

    // Se encontrou a chave, retorna esse nó
    if (keys[i] == k)
        return this;

    // Se não encontrou a chave e é um nó folha, então a chave não está na árvore
    if (leaf == true)
        return nullptr;

    // Desce no nó filho apropriado
    BTreeNode no_apropriado;
    gerenciador->CarregarBloco(i, &no_apropriado);
    return no_apropriado.search(k);
}

void BTreeNode::insertNonFull(int k) {
    // Começa pelo indice_no_arquivo mais a direita
    int i = numero_chaves - 1;

    // Se for um nó folha, deve buscar onde inserir nele
    if (leaf == true) {
        // Loop para encontrar a nova posição a inserir a chave e mover as demais à frente
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insere a chave na posiçao encontrada
        keys[i + 1] = k;
        numero_chaves = numero_chaves + 1;
    } else // Se não é um nó folha, deve decidir qual nó filho recebe a chave
    {
        while (i >= 0 && keys[i] > k)
            i--;

        // Verifica se o nó filho encontrado já está cheio
        BTreeNode *no_encontrado = new BTreeNode;
        gerenciador->CarregarBloco(chaves[i + 1], no_encontrado);
        if (no_encontrado->numero_chaves == 2 * ordem - 1) {
            // Se estiver cheio, o divide
            splitChild(i + 1, no_encontrado);

            // Depois de dividir, decide qual nó novo recebe a chave
            if (keys[i + 1] < k)
                i++;
        }
        no_encontrado->insertNonFull(k);
        delete(no_encontrado);
    }
}


void BTreeNode::splitChild(int i, BTreeNode *no_doador) {
    // Cria um novo nó que vai receber ordem-1 chaves de no_doador (mínimo de chaves)
    BTreeNode *no_receptor = new BTreeNode(no_doador->ordem, no_doador->leaf, gerenciador);
    no_receptor->numero_chaves = ordem - 1;

    // Copia as últimas ordem-1 chaves de no_doador para o novo nó
    for (int j = 0; j < ordem - 1; j++)
        no_receptor->keys[j] = no_doador->keys[j + ordem];

    // Copia os últimos ordem-1 filhos apontados por no_doador para o novo nó
    if (no_doador->leaf == false) {
        for (int j = 0; j < ordem; j++)
            no_receptor->chaves[j] = no_doador->chaves[j + ordem];
    }

    // Reduz o número de chaves em no_doador
    no_doador->numero_chaves = ordem - 1;

    // Cria espaço para o novo nó filho que o nó que chamou a função terá
    for (int j = numero_chaves; j >= i + 1; j--)
        chaves[j + 1] = chaves[j];

    // Posiciona o novo nó filho
    chaves[i + 1] = no_receptor->indice_no_arquivo;

    // Posiciona a chave que sobe de no_doador neste nó
    for (int j = numero_chaves - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copia a chave do meio para esse nó
    keys[i] = no_doador->keys[ordem - 1];

    // Incrementa o número de chaves deste nó já que uma nova foi inserida
    numero_chaves = numero_chaves + 1;
    delete(no_receptor);
}

void BTreeNode::SalvarNoArquivo() {
    gerenciador->SalvarBloco(indice_no_arquivo,this);
}
BTreeNode::~BTreeNode() {
    SalvarNoArquivo();
    delete[] keys;
    delete[] chaves;
}

ostream &operator<<(ostream &os, const BTreeNode &node) {
    os << node.ordem << '|';
    for(int i=0; i<node.ordem*2-1; i++)
        os << node.keys[i] << '|';

    for(int i=0; i<node.ordem*2; i++)
        os << node.chaves[i] << '|';
    os << node.numero_chaves << '|';
    os << node.leaf << '|';
    os << node.indice_no_arquivo;
    return os;
}
