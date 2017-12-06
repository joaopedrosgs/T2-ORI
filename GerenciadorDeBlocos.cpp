//
// Created by pedro on 04/12/2017.
//

#include <cstring>
#include "GerenciadorDeBlocos.h"
#include "BTreeNode.h"

using namespace std;


GerenciadorDeBlocos::GerenciadorDeBlocos(int _tamanhoBloco, const char *_nomeArquivo) {
    tamanhoBloco = _tamanhoBloco;
    nomeArquivo = _nomeArquivo;
}

//Lê um bloco de um determinado indice e coloca-o no destino
void GerenciadorDeBlocos::CarregarBloco(int indice, BTreeNode *destino) {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::in);
    }
    char *buffer = new char[tamanhoBloco];
    for(int i=0; i<tamanhoBloco; i++) {
        buffer[i]=0;
    }
    arquivo.seekg(indice * tamanhoBloco, std::fstream::beg);
    arquivo.read(buffer, tamanhoBloco);
    arquivo.close();
    Deserializar(destino, buffer); // Deserializa o buffer para o destino
    delete[](buffer);



}
// Recebe um BTreeNode e salva no indice correto (zera o indice antes para evitar escrever menos que um bloco)
void GerenciadorDeBlocos::SalvarBloco(int indice, BTreeNode *origem) {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::out | std::fstream::in);
    }
    for (int i = UltimoIndice(); i < indice; i++) {
        novoBloco(); // Caso ocorra de precisarmos escrever no indice 7 e o indice 6 já existe mas ainda não foi salvo
    }
    zerarBlocoEm(indice);//zerando espaço para evitar escrever menos que um bloco
    arquivo.seekp(indice * tamanhoBloco, std::fstream::beg);
    if (!arquivo.good()) {
        cout << "Erro de escrita";
        return;
    }
    arquivo << *origem;
    arquivo.flush();
    arquivo.close();

}
//Cria um novo bloco (uso externo porque abre e fecha o arquivo)
void GerenciadorDeBlocos::NovoBloco() {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::out);
    }
    char zero = 0;
    arquivo.seekp(0, std::fstream::end);
    for (int i = 0; i < tamanhoBloco; i++) {
        arquivo.write(&zero, sizeof(char));
    }
    arquivo.flush();
    arquivo.close();

}
//Cria um novo bloc (uso interno)
void GerenciadorDeBlocos::novoBloco() {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::out);
    }
    char zero = 0;
    arquivo.seekp(0, std::fstream::end);
    for (int i = 0; i < tamanhoBloco; i++) {
        arquivo.write(&zero, sizeof(char));
    }

}
//Escreve zeros no bloco
void GerenciadorDeBlocos::zerarBlocoEm(int indice) {

    char zero = 0;
    arquivo.seekp(tamanhoBloco * indice, std::fstream::beg);
    for (int i = 0; i < tamanhoBloco; ++i) {
        arquivo.write(&zero, sizeof(char));
    }
    arquivo.flush();

}

//Pega o tamanho do arquivo e divide pelo tamanho do bloco, obtendo assim o indice
int GerenciadorDeBlocos::UltimoIndice() {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::in | std::fstream::out);

    }
    arquivo.seekp(0, std::fstream::end);

    int indice = static_cast<int>(arquivo.tellp() / tamanhoBloco);

    return indice;

}


//Recebe um buffer e converte ele para um BTreeNode
void GerenciadorDeBlocos::Deserializar(BTreeNode *destino, char *buffer) {
    char *tokens = strtok(buffer, "|");// SEPARA O BLOCO POR TOKENS

    int _ordem = atoi(tokens); // Pega a ordem, que é o primeiro elemento

    tokens = strtok(NULL, "|"); // Avança pro proximo elemento

    int _numero_chaves = atoi(tokens); // Pega o numero de chaves

    int _chaves[(_ordem * 2) - 1]; // Inicia a variavel que vai conter as chaves
    for(int i=0; i<_ordem*2-1; i++)
        _chaves[i]=0;

    for (int i = 0; i < _numero_chaves; i++) { // lê as chaves
        tokens = strtok(NULL, "|");
        _chaves[i] = atoi(tokens);
    }

    int _filhos[_ordem * 2]; // Inicia a variavel que vai conter os filhos
    for(int i=0; i<_ordem*2; i++)
        _filhos[i]=-1;

    for (int i = 0; i < _ordem * 2; i++) { // Pega os filhos
        tokens = strtok(nullptr, "|");
        _filhos[i] = atoi(tokens);
    }

    tokens = strtok(NULL, "|");
    bool _leaf = (atoi(tokens)) == 1; // Pega o valor pra leaf

    tokens = strtok(NULL, "|");
    int _indice_no_arquivo = atoi(tokens);
    destino->SetInfo(_ordem, _numero_chaves, _chaves, _filhos, _leaf, _indice_no_arquivo);
}





