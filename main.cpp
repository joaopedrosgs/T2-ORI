#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"


int main() {
    const char* NomeArquivo = "teste";
    BTree a(3, NomeArquivo);
    for(int i=0; i<25; i++) {
        int random = rand() % 200;
        a.insert(random);
    }
    //a.traverse();


    return 0;
}
