//
// Created by pedro on 04/12/2017.
//

#include <iostream>
#include <cstring>
#include "BTreeNode.h"


BTreeNode::BTreeNode() {
    leaf = true;
    ordem = 3;
    chaves = new int[2 * ordem - 1];
    for(int i=0; i<ordem*2-1; i++)
        chaves[i]=0;

    filhos = new int[2 * ordem];
    for(int i=0; i<ordem*2; i++)
        filhos[i]=-1;


}

// Construtor da classe de nó de árvore B

BTreeNode::BTreeNode(int _ordem, bool _leaf, GerenciadorDeBlocos *_gerenciador) {
    // Copia o grau e a propriedade de ser folha ou não
    ordem = _ordem;
    leaf = _leaf;

    // Aloca memória para comportar o número máximo de filhos e ponteiros para nós-filho

    chaves = new int[2 * ordem - 1]();
    for(int i=0; i<_ordem*2-1; i++)
        chaves[i]=0;

    filhos = new int[2 * ordem];
    for(int i=0; i<_ordem*2; i++)
        filhos[i]=-1;

    // Inicializa o número de filhos como 0
    numero_chaves = 0;
    gerenciador = _gerenciador; // Adiciona referencia ao gerenciador, que é usado pra cuidar dos blocos
}

BTreeNode::BTreeNode(int _ordem, bool _leaf, GerenciadorDeBlocos *_gerenciador, int _indice_no_arquivo) {
    // Copia o grau e a propriedade de ser folha ou não
    ordem = _ordem;
    leaf = _leaf;

    // Aloca memória para comportar o número máximo de filhos e ponteiros para nós-filho
    chaves = new int[2 * ordem - 1];
    for(int i=0; i<_ordem*2-1; i++)
        chaves[i]=0;

    filhos = new int[2 * ordem];
    for(int i=0; i<_ordem*2; i++)
        filhos[i]=-1;




    // Inicializa o número de filhos como 0
    numero_chaves = 0;
    gerenciador = _gerenciador;
    indice_no_arquivo = _indice_no_arquivo;
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
    ImprimirInfo();


}

// Funçao que busca uma chave k na sub-arvore que possui esse nó como raíz
int BTreeNode::search(int k) {
    // Encontra a primeira chave maior ou igual a k, pois indica a posição em que deve descer ou encontra a chave
    int i = 0;
    while (i < numero_chaves && k > chaves[i])
        i++;

    // Se encontrou a chave, retorna esse nó
    if (chaves[i] == k)
        return indice_no_arquivo;

    // Se não encontrou a chave e é um nó folha, então a chave não está na árvore
    if (leaf == true)
        return -1;

    // Desce no nó filho apropriado
    BTreeNode filho_apropriado(ordem, leaf, gerenciador);
    gerenciador->CarregarBloco(filhos[i], &filho_apropriado);
    return filho_apropriado.search(k);
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

    //Inicializa um novo nó no final do arquivo
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
// Salva o proprio nó no arquivo
void BTreeNode::SalvarNoArquivo() {
    gerenciador->SalvarBloco(indice_no_arquivo, this);
}
//Volta o nó para o arquivo e deleta
BTreeNode::~BTreeNode() {
    SalvarNoArquivo();
    if(chaves!= nullptr)
        delete[] chaves;
    if(filhos!= nullptr)
        delete[] filhos;
}
// Utilizado pra escrever no arquivo em um formato conhecido
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

void BTreeNode::SetInfo(int _ordem, int _numero_chaves, int *_chaves, int *_filhos, bool _leaf, int _indice_no_arquivo) {
    ordem = _ordem;
    numero_chaves = _numero_chaves;
    for(int i=0; i<numero_chaves; i++)
        chaves[i] = _chaves[i];
    for(int i=0; i<ordem*2; i++)
        filhos[i] = _filhos[i];
    leaf = _leaf;
    indice_no_arquivo = _indice_no_arquivo;
}

// Imprime informações na tela
void BTreeNode::ImprimirInfo() {
    {
        cout <<  endl << "###########################" << endl;
        cout << "Local no arquivo: " << indice_no_arquivo << endl;
        cout << "Ordem: " << ordem << endl;
        cout << "Chaves: ";
        for (int i = 0; i < numero_chaves; i++) {
            cout << chaves[i] << ", ";
        }
        cout << endl << "Filhos: ";
        int i = 0;
        if(!leaf){
            for(int i=0; i<ordem*2; i++)
                if(filhos[i]!=-1)
                    cout << filhos[i] << ", ";

        }else {
            cout << "Nao tem filhos";
        }
        cout <<  endl << "###########################" << endl;

    }
}

/*

 // A function to remove the key k from the sub-tree rooted with this node
void BTreeNode::remove(int k)
{
    int idx = findKey(k);

    // The key to be removed is present in this node
    if (idx < n && keys[idx] == k)
    {

        // If the node is a leaf node - removeFromLeaf is called
        // Otherwise, removeFromNonLeaf function is called
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {

        // If this node is a leaf node, then the key is not present in tree
        if (leaf)
        {
            cout << "The key "<< k <<" is does not exist in the tree\n";
            return;
        }

        // The key to be removed is present in the sub-tree rooted with this node
        // The flag indicates whether the key is present in the sub-tree rooted
        // with the last child of this node
        bool flag = ( (idx==n)? true : false );

        // If the child where the key is supposed to exist has less that t keys,
        // we fill that child
        if (C[idx]->n < t)
            fill(idx);

        // If the last child has been merged, it must have merged with the previous
        // child and so we recurse on the (idx-1)th child. Else, we recurse on the
        // (idx)th child which now has atleast t keys
        if (flag && idx > n)
            C[idx-1]->remove(k);
        else
            C[idx]->remove(k);
    }
    return;
}

// A function to remove the idx-th key from this node - which is a leaf node
void BTreeNode::removeFromLeaf (int idx)
{

    // Move all the keys after the idx-th pos one place backward
    for (int i=idx+1; i<n; ++i)
        keys[i-1] = keys[i];

    // Reduce the count of keys
    n--;

    return;
}

// A function to remove the idx-th key from this node - which is a non-leaf node
void BTreeNode::removeFromNonLeaf(int idx)
{

    int k = keys[idx];

    // If the child that precedes k (C[idx]) has atleast t keys,
    // find the predecessor 'pred' of k in the subtree rooted at
    // C[idx]. Replace k by pred. Recursively delete pred
    // in C[idx]
    if (C[idx]->n >= t)
    {
        int pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    }

    // If the child C[idx] has less that t keys, examine C[idx+1].
    // If C[idx+1] has atleast t keys, find the successor 'succ' of k in
    // the subtree rooted at C[idx+1]
    // Replace k by succ
    // Recursively delete succ in C[idx+1]
    else if  (C[idx+1]->n >= t)
    {
        int succ = getSucc(idx);
        keys[idx] = succ;
        C[idx+1]->remove(succ);
    }

    // If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
    // into C[idx]
    // Now C[idx] contains 2t-1 keys
    // Free C[idx+1] and recursively delete k from C[idx]
    else
    {
        merge(idx);
        C[idx]->remove(k);
    }
    return;
}

// A function to get predecessor of keys[idx]
int BTreeNode::getPred(int idx)
{
    // Keep moving to the right most node until we reach a leaf
    BTreeNode *cur=C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];

    // Return the last key of the leaf
    return cur->keys[cur->n-1];
}

int BTreeNode::getSucc(int idx)
{

    // Keep moving the left most node starting from C[idx+1] until we reach a leaf
    BTreeNode *cur = C[idx+1];
    while (!cur->leaf)
        cur = cur->C[0];

    // Return the first key of the leaf
    return cur->keys[0];
}

// A function to fill child C[idx] which has less than t-1 keys
void BTreeNode::fill(int idx)
{

    // If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
    // from that child
    if (idx!=0 && C[idx-1]->n>=t)
        borrowFromPrev(idx);

    // If the next child(C[idx+1]) has more than t-1 keys, borrow a key
    // from that child
    else if (idx!=n && C[idx+1]->n>=t)
        borrowFromNext(idx);

    // Merge C[idx] with its sibling
    // If C[idx] is the last child, merge it with with its previous sibling
    // Otherwise merge it with its next sibling
    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx-1);
    }
    return;
}

// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
void BTreeNode::borrowFromPrev(int idx)
{

    BTreeNode *child=C[idx];
    BTreeNode *sibling=C[idx-1];

    // The last key from C[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in C[idx]. Thus, the  loses
    // sibling one key and child gains one key

    // Moving all key in C[idx] one step ahead
    for (int i=child->n-1; i>=0; --i)
        child->keys[i+1] = child->keys[i];

    // If C[idx] is not a leaf, move all its child pointers one step ahead
    if (!child->leaf)
    {
        for(int i=child->n; i>=0; --i)
            child->C[i+1] = child->C[i];
    }

    // Setting child's first key equal to keys[idx-1] from the current node
    child->keys[0] = keys[idx-1];

    // Moving sibling's last child as C[idx]'s first child
    if (!leaf)
        child->C[0] = sibling->C[sibling->n];

    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    keys[idx-1] = sibling->keys[sibling->n-1];

    child->n += 1;
    sibling->n -= 1;

    return;
}

// A function to borrow a key from the C[idx+1] and place
// it in C[idx]
void BTreeNode::borrowFromNext(int idx)
{

    BTreeNode *child=C[idx];
    BTreeNode *sibling=C[idx+1];

    // keys[idx] is inserted as the last key in C[idx]
    child->keys[(child->n)] = keys[idx];

    // Sibling's first child is inserted as the last child
    // into C[idx]
    if (!(child->leaf))
        child->C[(child->n)+1] = sibling->C[0];

    //The first key from sibling is inserted into keys[idx]
    keys[idx] = sibling->keys[0];

    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling->n; ++i)
        sibling->keys[i-1] = sibling->keys[i];

    // Moving the child pointers one step behind
    if (!sibling->leaf)
    {
        for(int i=1; i<=sibling->n; ++i)
            sibling->C[i-1] = sibling->C[i];
    }

    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child->n += 1;
    sibling->n -= 1;

    return;
}

// A function to merge C[idx] with C[idx+1]
// C[idx+1] is freed after merging
void BTreeNode::merge(int idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx+1];

    // Pulling a key from the current node and inserting it into (t-1)th
    // position of C[idx]
    child->keys[t-1] = keys[idx];

    // Copying the keys from C[idx+1] to C[idx] at the end
    for (int i=0; i<sibling->n; ++i)
        child->keys[i+t] = sibling->keys[i];

    // Copying the child pointers from C[idx+1] to C[idx]
    if (!child->leaf)
    {
        for(int i=0; i<=sibling->n; ++i)
            child->C[i+t] = sibling->C[i];
    }

    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to C[idx]
    for (int i=idx+1; i<n; ++i)
        keys[i-1] = keys[i];

    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i=idx+2; i<=n; ++i)
        C[i-1] = C[i];

    // Updating the key count of child and the current node
    child->n += sibling->n+1;
    n--;

    // Freeing the memory occupied by sibling
    delete(sibling);
    return;
}
 */


