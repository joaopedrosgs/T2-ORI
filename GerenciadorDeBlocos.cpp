//
// Created by pedro on 04/12/2017.
//

#include "GerenciadorDeBlocos.h"

using namespace std;

template<typename T>
bool GerenciadorDeBlocos<T>::ehValido() {
    char valido = '*';
    if (leitura.is_open()) {
        leitura.read((char *)&valido, sizeof(char));
        leitura.seekg(-sizeof(char), ios_base::cur);
    }
    return valido != '*';
}

template<typename T>
GerenciadorDeBlocos<T>::GerenciadorDeBlocos(int _tamanhoBloco, const char *_nomeArquivo) {
    tamanhoBloco = _tamanhoBloco;
    nomeArquivo = _nomeArquivo;

}

template<typename T>
void GerenciadorDeBlocos<T>::CarregarBloco(int indice, T *destino) {
    if (!leitura.is_open()) {
        leitura.open(nomeArquivo);
    }
    leitura.seekg(indice * tamanhoBloco, ios::beg);
    leitura.read((char *)destino, sizeof(destino));
    leitura.close();
}


template<typename T>
void GerenciadorDeBlocos<T>::SalvarBloco(int indice, T *origem) {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    escrita.seekp(indice * tamanhoBloco, ios::beg);
    escrita << *origem;
    escrita.close();
}

template<typename T>
void GerenciadorDeBlocos<T>::NovoBloco() {
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

template<typename T>
void GerenciadorDeBlocos<T>::DeletarBloco(int indice) {
    if (!escrita.is_open()) {
        escrita.open(nomeArquivo);
    }
    char del = '*';
    escrita.seekp(indice * tamanhoBloco, ios::beg);
    escrita.write((char *) &del, sizeof(char));
    escrita.close();
}

