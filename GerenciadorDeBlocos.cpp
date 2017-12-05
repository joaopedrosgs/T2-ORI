//
// Created by pedro on 04/12/2017.
//

#include <cstring>
#include "GerenciadorDeBlocos.h"
#include "BTreeNode.h"

using namespace std;


bool GerenciadorDeBlocos::ehValido() {
    char valido = '*';
    if (leitura.is_open()) {
        leitura.read((char *) &valido, sizeof(char));
        leitura.seekg(-sizeof(char), ios_base::cur);
    }
    return valido != '*';
}


GerenciadorDeBlocos::GerenciadorDeBlocos(int _tamanhoBloco, const char *_nomeArquivo) {
    tamanhoBloco = _tamanhoBloco;
    nomeArquivo = _nomeArquivo;

}


void GerenciadorDeBlocos::CarregarBloco(int indice, BTreeNode *destino) {
    if (!leitura.is_open()) {
        leitura.open(nomeArquivo);
    }
    char bloco[tamanhoBloco];
    leitura.seekg(indice * tamanhoBloco, ios::beg);
    leitura.read(bloco, tamanhoBloco);

    char *tokens = strtok(bloco, "|");// SEPARA O BLOCO POR TOKENS

    int _ordem = atoi(tokens); // Pega a ordem, que é o primeiro elemento

    tokens =  strtok(NULL, "|"); // Avança pro proximo elemento

    int _numero_chaves = atoi(tokens); // Pega o numero de chaves

    int *_chaves = new int[(_ordem * 2) - 1]{0}; // Inicia a variavel que vai conter as chaves

    for (int i=0; i < _ordem * 2-1; i++) { // lê as chaves
        tokens = strtok(NULL, "|");
        _chaves[i] = atoi(tokens);
    }

    int *_filhos = new int[_ordem * 2](); // Inicia a variavel que vai conter os filhos
    memset(_filhos, -1, _ordem * 2*sizeof(int));

    for (int i=0; i < _ordem * 2; i++) { // Pega os filhos
        tokens = strtok(nullptr, "|");
        _filhos[i] = atoi(tokens);
    }

    tokens = strtok(NULL, "|");
    bool _leaf = (atoi(tokens))==1?true:false; // Pega o valor pra leaf

    tokens = strtok(NULL, "|");
    int _indice_no_arquivo = atoi(tokens);

    destino->SetInfo(_ordem, _numero_chaves, _chaves, _filhos, _leaf, _indice_no_arquivo);

    leitura.close();
}


void GerenciadorDeBlocos::SalvarBloco(int indice, BTreeNode *origem) {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    novoBlocoEmIndice(indice);
    escrita.seekp(indice * tamanhoBloco, ios::beg);
    escrita << *origem;
    escrita.flush();
    escrita.close();
}

void GerenciadorDeBlocos::NovoBloco() {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    char zero = 0;
    escrita.seekp(0, ios::end);
    for (int i = 0; i < tamanhoBloco; ++i) {
        escrita.write(&zero, sizeof(char));
    }
    escrita.close();
}
void GerenciadorDeBlocos::novoBlocoEmIndice(int x) {

    char zero = 0;
    escrita.seekp(tamanhoBloco*x, ios::beg);
    for (int i = 0; i < tamanhoBloco; ++i) {
        escrita.write(&zero, sizeof(char));
    }

}


void GerenciadorDeBlocos::DeletarBloco(int indice) {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    char del = '*';
    escrita.seekp(indice * tamanhoBloco, ios::beg);
    escrita.write((char *) &del, sizeof(char));
    escrita.close();
}




