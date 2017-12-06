//
// Created by pedro on 04/12/2017.
//

#include <cstring>
#include <cmath>
#include "GerenciadorDeBlocos.h"
#include "BTreeNode.h"

using namespace std;


GerenciadorDeBlocos::GerenciadorDeBlocos(int _tamanhoBloco, const char *_nomeArquivo) {
    tamanhoBloco = _tamanhoBloco;
    nomeArquivo = _nomeArquivo;


}


void GerenciadorDeBlocos::CarregarBloco(int indice, BTreeNode *destino) {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::in);
    }
    char *buffer = new char[tamanhoBloco];
    memset(buffer, '0', tamanhoBloco);
    arquivo.seekg(indice * tamanhoBloco, std::fstream::beg);
    arquivo.read(buffer, tamanhoBloco);
    arquivo.close();

    char *tokens = strtok(buffer, "|");// SEPARA O BLOCO POR TOKENS

    int _ordem = atoi(tokens); // Pega a ordem, que é o primeiro elemento

    tokens = strtok(NULL, "|"); // Avança pro proximo elemento

    int _numero_chaves = atoi(tokens); // Pega o numero de chaves

    int *_chaves = new int[(_ordem * 2) - 1](); // Inicia a variavel que vai conter as chaves

    for (int i = 0; i < _numero_chaves; i++) { // lê as chaves
        tokens = strtok(NULL, "|");
        _chaves[i] = atoi(tokens);
    }

    int *_filhos = new int[_ordem * 2](); // Inicia a variavel que vai conter os filhos
    memset(_filhos, -1, _ordem * 2 * sizeof(int));

    for (int i = 0; i < _ordem * 2; i++) { // Pega os filhos
        tokens = strtok(nullptr, "|");
        _filhos[i] = atoi(tokens);
    }

    tokens = strtok(NULL, "|");
    bool _leaf = (atoi(tokens)) == 1 ? true : false; // Pega o valor pra leaf

    tokens = strtok(NULL, "|");
    int _indice_no_arquivo = atoi(tokens);
    delete[](buffer);
    destino->SetInfo(_ordem, _numero_chaves, _chaves, _filhos, _leaf, _indice_no_arquivo);


}


void GerenciadorDeBlocos::SalvarBloco(int indice, BTreeNode *origem) {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::out | std::fstream::in);
    }
    for (int i = UltimoIndice(); i < indice; i++) {
        NovoBloco();
    }
    zerarBlocoEm(indice);//zerando espaço
    arquivo.seekp(indice * tamanhoBloco, std::fstream::beg);
    if (!arquivo.good()) {
        cout << "Erro de escrita";
        return;
    }
    arquivo << *origem;
    arquivo.flush();
    arquivo.close();

}

void GerenciadorDeBlocos::NovoBloco() {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::out);
    }
    char zero = 0;
    arquivo.seekp(0, std::fstream::end);
    for (int i = 0; i < tamanhoBloco; i++) {
        arquivo.write(&zero, sizeof(char));
    }

}

void GerenciadorDeBlocos::zerarBlocoEm(int x) {

    char zero = 0;
    arquivo.seekp(tamanhoBloco * x, std::fstream::beg);
    for (int i = 0; i < tamanhoBloco; ++i) {
        arquivo.write(&zero, sizeof(char));
    }
    arquivo.flush();

}


void GerenciadorDeBlocos::DeletarBloco(int indice) {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, fstream::in | fstream::out);
    }
    char del = '*';
    arquivo.seekp(indice * tamanhoBloco, std::fstream::beg);
    arquivo.write(&del, sizeof(char));

}

int GerenciadorDeBlocos::UltimoIndice() {
    if (!arquivo.is_open()) {
        arquivo.open(nomeArquivo, std::fstream::in | std::fstream::out);

    }
    arquivo.seekp(0, std::fstream::end);

    int indice = arquivo.tellp() / tamanhoBloco;

    return indice;

}

int GerenciadorDeBlocos::GetTamanhoBloco() {
    return tamanhoBloco;
}




