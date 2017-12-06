//
// Created by pedro on 04/12/2017.
//

#include <iostream>
#include <cstring>
#include "BTreeNode.h"


BTreeNode::BTreeNode() {
    leaf = true;
    ordem = 0;

}

// Construtor da classe de nó de árvore B

BTreeNode::BTreeNode(int _ordem, bool leaf1, GerenciadorDeBlocos *_gerenciador) {
    // Copia o grau e a propriedade de ser folha ou não
    ordem = _ordem;
    leaf = leaf1;

    // Aloca memória para comportar o número máximo de filhos e ponteiros para nós-filho
    chaves = new int[2 * ordem - 1]();
    filhos = new int[2 * ordem]();
    memset(filhos, -1, _ordem * 2 * sizeof(int));



    // Inicializa o número de filhos como 0
    numero_chaves = 0;
    gerenciador = _gerenciador;
}

// Função para imprimir a árvore
void BTreeNode::traverse() {
    int i = 0;


    while (filhos[i] != -1) {
        BTreeNode no_debaixo(ordem, false, gerenciador);
        gerenciador->CarregarBloco(filhos[i], &no_debaixo);
        no_debaixo.traverse();
        i++;
    }
    i = 0;
    if ((filhos[i] != -1)) {
        cout << "Filhos de " << indice_no_arquivo << ": ";
        while (filhos[i] != -1) {
            cout << filhos[i] << ",";
            i++;
        }
        cout << endl;
    } else {
        cout << indice_no_arquivo << " Nao tem filhos " << endl;
    }
    for (i = 0; i < numero_chaves; i++) {
        std::cout << indice_no_arquivo << ":" << chaves[i] << std::endl;
    }

}

// Funçao que busca uma chave k na sub-arvore que possui esse nó como raíz
BTreeNode *BTreeNode::search(int k) {
    // Encontra a primeira chave maior ou igual a k, pois indica a posição em que deve descer ou encontra a chave
    int i = 0;
    while (i < numero_chaves && k > chaves[i])
        i++;

    // Se encontrou a chave, retorna esse nó
    if (chaves[i] == k)
        return this;

    // Se não encontrou a chave e é um nó folha, então a chave não está na árvore
    if (leaf == true)
        return nullptr;

    // Desce no nó filho apropriado
    BTreeNode no_apropriado(ordem, leaf, gerenciador);
    gerenciador->CarregarBloco(i, &no_apropriado);
    return no_apropriado.search(k);
}

void BTreeNode::insertNonFull(int k) {
    // Começa pelo indice_no_arquivo mais a direita
    int i = numero_chaves - 1;

    // Se for um nó folha, deve buscar onde inserir nele
    if (leaf == true) {
        // Loop para encontrar a nova posição a inserir a chave e mover as demais à frente
        while (i >= 0 && chaves[i] > k) {
            chaves[i + 1] = chaves[i];
            i--;
        }

        // Insere a chave na posiçao encontrada
        chaves[i + 1] = k;
        numero_chaves++;
    } else // Se não é um nó folha, deve decidir qual nó filho recebe a chave
    {
        while (i >= 0 && chaves[i] > k)
            i--;

        // Verifica se o nó filho encontrado já está cheio
        BTreeNode *no_encontrado = new BTreeNode(ordem, leaf, gerenciador);
        gerenciador->CarregarBloco(filhos[i + 1], no_encontrado);
        if (no_encontrado->numero_chaves == 2 * ordem - 1) {
            // Se estiver cheio, o divide
            splitChild(i + 1, no_encontrado);

            // Depois de dividir, decide qual nó novo recebe a chave
            if (chaves[i + 1] < k)
                i++;
            BTreeNode *local_insercao = new BTreeNode(ordem, leaf, gerenciador);
            gerenciador->CarregarBloco(filhos[i + 1], local_insercao);
            local_insercao->insertNonFull(k);
            delete (local_insercao);
        } else {
            no_encontrado->insertNonFull(k);
        }
        delete (no_encontrado);
    }
}


void BTreeNode::splitChild(int i, BTreeNode *no_doador) {
    // Quem chama esse metodo vai ser o novo pai de dois novos nós
    // Cria um novo nó que vai receber ordem-1 filhos de no_doador (mínimo de filhos)
    // Um dos nós que vai receber metade dos elementos

    BTreeNode *no_receptor = new BTreeNode(no_doador->ordem, no_doador->leaf, gerenciador, gerenciador->UltimoIndice());
    no_receptor->numero_chaves = ordem - 1;

    // Copia as últimas ordem-1 filhos de no_doador para o novo nó
    for (int j = 0; j < ordem - 1; j++) {
        no_receptor->chaves[j] = no_doador->chaves[j + ordem];
    }

    // Copia os últimos ordem-1 filhos apontados por no_doador para o novo nó
    if (no_doador->leaf == false) {
        for (int j = 0; j < ordem; j++) {
            no_receptor->filhos[j] = no_doador->filhos[j + ordem];
            no_doador->filhos[j + ordem] = -1;
        }
    }

    // Reduz o número de filhos em no_doador
    no_doador->numero_chaves = ordem - 1;

    // Cria espaço para o novo nó filho que o nó que chamou a função terá
    for (int j = numero_chaves; j >= i + 1; j--)
        filhos[j + 1] = filhos[j];

    // Posiciona o novo nó filho
    filhos[i + 1] = no_receptor->indice_no_arquivo;

    // Posiciona a chave que sobe de no_doador neste nó
    for (int j = numero_chaves - 1; j >= i; j--)
        chaves[j + 1] = chaves[j];

    // Copia a chave do meio para esse nó
    chaves[i] = no_doador->chaves[ordem - 1];
    no_doador->chaves[ordem - 1] = 0;

    // Incrementa o número de filhos deste nó já que uma nova foi inserida
    numero_chaves++;
    delete (no_receptor);
}

void BTreeNode::SalvarNoArquivo() {
    gerenciador->SalvarBloco(indice_no_arquivo, this);
}

BTreeNode::~BTreeNode() {
    SalvarNoArquivo();
    delete[] chaves;
    delete[] filhos;
}

fstream &operator<<(fstream &os, const BTreeNode &node) {
    os << node.ordem << "|";
    os << node.numero_chaves << "|";
    for (int i = 0; i < node.numero_chaves; i++)
        os << node.chaves[i] << "|";
    for (int i = 0; i < node.ordem * 2; i++)
        os << node.filhos[i] << "|";
    os << node.leaf << "|";
    os << node.indice_no_arquivo;
    return os;
}

void
BTreeNode::SetInfo(int _ordem, int _numero_chaves, int *_chaves, int *_filhos, bool _leaf, int _indice_no_arquivo) {
    ordem = _ordem;
    numero_chaves = _numero_chaves;
    chaves = _chaves;
    filhos = _filhos;
    leaf = _leaf;
    indice_no_arquivo = _indice_no_arquivo;
}

BTreeNode::BTreeNode(int _ordem, bool _leaf, GerenciadorDeBlocos *_gerenciador, int _indice_no_arquivo) {
    // Copia o grau e a propriedade de ser folha ou não
    ordem = _ordem;
    leaf = _leaf;

    // Aloca memória para comportar o número máximo de filhos e ponteiros para nós-filho
    chaves = new int[2 * ordem - 1]();
    filhos = new int[2 * ordem]();
    memset(filhos, -1, _ordem * 2 * sizeof(int));



    // Inicializa o número de filhos como 0
    numero_chaves = 0;
    gerenciador = _gerenciador;
    indice_no_arquivo = _indice_no_arquivo;
}


