#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"


int main() {
    const char* NomeArquivo = "teste";
    GerenciadorDeBlocos<BTreeNode> a(1024,  NomeArquivo);
    a.NovoBloco();
    return 0;
}