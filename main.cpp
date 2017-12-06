
#include <stdlib.h>
#include "BTree.h"


int main() {
    const char* NomeArquivo = "teste";
    BTree a(3, NomeArquivo);
    for (int i = 0; i < 50; i++) {
        int random = rand() % 200;

        cout << "->" << random << endl;
        a.insert(random);
    }
    a.traverse();
    system("PAUSE");

    return 0;
}
