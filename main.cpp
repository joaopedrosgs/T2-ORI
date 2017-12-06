
#include <stdlib.h>
#include "BTree.h"


int main() {
    srand (time(NULL));

        const char *NomeArquivo = "teste";
        int ordem = 2;
        while(ordem < 3) {
            std::cout << "Digite a ordem da arvore (o tamanho maximo de chaves sera ordem*2-1), o valor minimo é 3" << endl;
            std::cin >> ordem;
        }
        BTree a(ordem, NomeArquivo);

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
