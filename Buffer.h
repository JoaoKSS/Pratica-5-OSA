#ifndef BUFFER_H
#define BUFFER_H
#include "Registro.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include "Arvore.h"
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Node
{
    public:
        int id;
        streampos endereco;
        
        bool operator>(const Node& other) const {
            return id > other.id; 
        }

        bool operator<(const Node& other) const {
            return id < other.id; 
        }

        bool operator==(const Node& other) const {
            return id == other.id;
        }
};

class Buffer {
public:
    string nomeArquivo;
    string nomeArquivoIndex;
    ArvoreBinaria<Node> arvoreIndices; 

    Buffer(const string& nomeArquivo, const string& nomeArquivoIndex);
    Buffer() : nomeArquivo(""), nomeArquivoIndex("") {} 

    Registro lerDescritor();
    void escreverDescritor(const vector<Registro>& registros);
    int buscarEndereco(int id);
    void buscarRegistro(int id);
    void adicionarRegistros(const vector<Registro>& registros);
    void criarIndiceInvertido(const vector<Registro>& registros, const string& arquivoStopwords, const string& arquivoPontuacoes, const string& arquivoIndiceInvertido);
    vector<int> buscarPorFrase(const string& frase, const string& arquivoIndiceInvertido, const string& arquivoStopwords, const string& arquivoPontuacoes);

private:
    unordered_map<string, vector<int>> indiceInvertido;
};

#endif