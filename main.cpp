
#include <stdlib.h>
#include "BTree.h"


int main() {
    srand(time(NULL));

    const char *NomeArquivo = "teste";
    int ordem = 2;
    while (ordem < 3) {
        std::cout << "Digite a ordem da arvore (o tamanho maximo de chaves sera ordem*2-1), o valor minimo é 3: "
                  << endl;
        std::cin >> ordem;
    }

    BTree a(ordem, NomeArquivo);

    int quantos_numeros = 0;
    while (quantos_numeros < 1) {
        std::cout << "Digite a quantidade de numeros aleatorios que voce quer inserir:" << endl;
        std::cin >> quantos_numeros;
    }


    //Insere numeros aleatorios
    for (int i = 0; i < quantos_numeros; i++) {
        int random = rand() % 200;
        a.insert(random);
    }

    //Printa a arvore
    a.traverse();

    // Procurar um numero

    int numero_procura=0;
    while(numero_procura!=-1) {
        std::cout << "Digite um numero para procura-lo (digite -1 para sair):" << endl;
        std::cin >> numero_procura;
        int indice_encontrado = a.search(numero_procura);
        BTreeNode no_encontrado = a.ObterNo(indice_encontrado);
        no_encontrado.ImprimirInfo();
    }

    return 0;
}
