//
// Created by pedro on 04/12/2017.
//

#include "GerenciadorDeBlocos.h"
#include "BTreeNode.h"

using namespace std;




bool GerenciadorDeBlocos::ehValido() {
    char valido = '*';
    if (leitura.is_open()) {
        leitura.read((char *)&valido, sizeof(char));
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

    leitura.close();
}



void GerenciadorDeBlocos::SalvarBloco(int indice, BTreeNode *origem) {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    escrita.seekp(indice * tamanhoBloco, ios::beg);
    escrita << *origem;
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


void GerenciadorDeBlocos::DeletarBloco(int indice) {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    char del = '*';
    escrita.seekp(indice * tamanhoBloco, ios::beg);
    escrita.write((char *) &del, sizeof(char));
    escrita.close();
}




