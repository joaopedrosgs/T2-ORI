
#include <stdlib.h>
#include "BTree.h"


int main() {
    srand (time(NULL));

        const char *NomeArquivo = "teste";
        BTree a(3, NomeArquivo);

        for (int i = 0; i < 52; i++) {
            int random = rand() % 200;
            a.insert(random);
        }
        a.insert(531);
        a.traverse();
        int indice_encontrado = a.search(531);
        BTreeNode no_encontrado = a.ObterNo(indice_encontrado);
        no_encontrado.ImprimirInfo();

    return 0;
}
