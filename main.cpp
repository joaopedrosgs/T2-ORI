#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"


int main() {
    const char* NomeArquivo = "teste";
    BTree a(2, NomeArquivo);
    a.insert(20);
    a.insert(3);

    a.insert(6);

    a.insert(31);

    a.insert(32);


    a.traverse();


    return 0;
}
