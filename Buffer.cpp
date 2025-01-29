#include "Buffer.h"
#include "Index.h"
#include "Arvore.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>

Buffer::Buffer(const string& nomeArquivo, const string& nomeArquivoIndex) 
    : nomeArquivo(nomeArquivo), nomeArquivoIndex(nomeArquivoIndex) {}

void Buffer::escreverDescritor(const vector<Registro>& registros) {
    ofstream arquivoSaida(nomeArquivo, ios::trunc | ios::binary);
    if (!arquivoSaida)
        throw runtime_error("Erro ao abrir o arquivo para escrita.");

    // Vetor índices
    vector<Index> indices;

    for (const Registro& reg : registros) {
        // Empacota o registro
        string buffer = reg.packDescritor();
        // Salva a posição 
        streampos pos = arquivoSaida.tellp();
        // Escreve o registro no arquivo de dados
        arquivoSaida.write(buffer.data(), buffer.size());
        // Cria o objeto Index
        Index index(reg.ID, pos, nomeArquivoIndex);
        Node n = {reg.ID, pos};
        arvoreIndices.Inserir(n);
        // Adiciona o índice no vetor (para posterior escrita no arquivo)
        indices.push_back(index);
    }
    // Mostra a arvore inserida
    // arvoreIndices.Print();
    arquivoSaida.close();
    // Abre o arquivo de índice para escrita
    ofstream arquivoIndex(nomeArquivoIndex, ios::binary | ios::trunc);
    if (!arquivoIndex.is_open())
        throw runtime_error("Erro ao abrir o arquivo de índice.");
    // Escreve todos os índices no arquivo
    for (const Index& index : indices) {
        arquivoIndex.write(reinterpret_cast<const char*>(&index.id), sizeof(index.id));
        arquivoIndex.write(reinterpret_cast<const char*>(&index.endereco), sizeof(index.endereco));
    }
    arquivoIndex.close();
}

Registro Buffer::lerDescritor() {
    ifstream arquivoEntrada(nomeArquivo, ios::binary);
    if (!arquivoEntrada)
        throw runtime_error("Erro ao abrir o arquivo para leitura.");

    int tamanho;
    arquivoEntrada.read(reinterpret_cast<char*>(&tamanho), sizeof(int));
    if (!arquivoEntrada)
        throw runtime_error("Erro ao ler o descritor de tamanho.");

    string buffer(tamanho, '\0');
    arquivoEntrada.read(&buffer[0], tamanho);
    if (!arquivoEntrada)
        throw runtime_error("Erro ao ler o registro com descritor.");

    Registro reg;
    reg.unpackDescritor(buffer);
    return reg;
}

int Buffer::buscarEndereco(int id) {
    // Busca na árvore 
    int address = arvoreIndices.Busca(id);
    if (address != -1) {
        cout << "Índice encontrado: ID = " << id << ", Endereço = " << address << endl;
        return address;
    } else {
        cout << "Índice com ID " << id << " não encontrado." << endl;
        throw runtime_error("Índice não encontrado.");
    }
}

void Buffer::buscarRegistro(int id){
    try {
        int encontra = buscarEndereco(id); // Procura o índice com ID 
        ifstream file(nomeArquivo, ios::binary);
        file.seekg(static_cast<streampos>(encontra));
        string buffer;
        char DelimitadorF;
        // Encontrar o delimitador final '\n'
        while (file.get(DelimitadorF)) {
            buffer += DelimitadorF;
            if (DelimitadorF == '\n') {
                break;
            }
        }
        if (buffer.empty()) {
            cerr << "Erro: nenhum dado lido!" << endl;
            file.close();
            return;
        }

        Registro REG;
        REG.unpackDescritor(buffer);
        // Exibir os dados desserializados
        cout << "===================================================" << endl;
        cout << "ID: " << REG.ID << endl;
        cout << "Title: " << REG.title << endl;
        cout << "Authors: " << REG.authors << endl;
        cout << "Publish Year: " << REG.publishYear << endl;
        cout << "Category: " << REG.category << endl;
        cout << "===================================================" << endl;
        file.close();
    } catch (const runtime_error& e) {
        cerr << "Erro ao buscar registro: " << e.what() << endl;
    }
}

void Buffer::adicionarRegistros(const vector<Registro>& registros) {
    ofstream arquivoSaida(nomeArquivo, ios::app | ios::binary);
    if (!arquivoSaida)
        throw runtime_error("Erro ao abrir o arquivo para escrita.");

    // Vetor de índices
    vector<Index> indices;

    for (const Registro& reg : registros) {
        // Empacota o registro
        string buffer = reg.packDescritor();
        // Salva a posição 
        streampos pos = arquivoSaida.tellp();
        // Escreve o registro no arquivo de dados
        arquivoSaida.write(buffer.data(), buffer.size());
        // Cria o objeto Index
        Index index(reg.ID, pos, nomeArquivoIndex);
        Node n = {reg.ID, pos};
        arvoreIndices.Inserir(n);
        // Adiciona o índice no vetor (para posterior escrita no arquivo)
        indices.push_back(index);
    }
    arquivoSaida.close();
    // escreve no arquivo de índice para escrita
    ofstream arquivoIndex(nomeArquivoIndex, ios::binary | ios::app);
    if (!arquivoIndex.is_open())
        throw runtime_error("Erro ao abrir o arquivo de índice.");
    // Escreve todos os índices no arquivo
    for (const Index& index : indices) {
        arquivoIndex.write(reinterpret_cast<const char*>(&index.id), sizeof(index.id));
        arquivoIndex.write(reinterpret_cast<const char*>(&index.endereco), sizeof(index.endereco));
    }
    arquivoIndex.close();
}

// Função para carregar stopwords de um arquivo
unordered_set<string> carregarStopwords(const string& arquivoStopwords) {
    unordered_set<string> stopwords;
    ifstream arquivo(arquivoStopwords);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo de stopwords: " << arquivoStopwords << endl;
        return stopwords;
    }
    string palavra;
    while (getline(arquivo, palavra)) {
        stopwords.insert(palavra);
    }
    arquivo.close();
    return stopwords;
}

// Função para carregar pontuações de um arquivo
unordered_set<char> carregarPontuacoes(const string& arquivoPontuacoes) {
    unordered_set<char> pontuacoes;
    ifstream arquivo(arquivoPontuacoes);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo de pontuações: " << arquivoPontuacoes << endl;
        return pontuacoes;
    }
    char pontuacao;
    while (arquivo.get(pontuacao)) {
        pontuacoes.insert(pontuacao);
    }
    arquivo.close();
    return pontuacoes;
}

// Função para processar o título, removendo pontuações e stopwords
vector<string> processarTitulo(const string& titulo, const unordered_set<string>& stopwords, const unordered_set<char>& pontuacoes) {
    vector<string> palavras;
    stringstream ss(titulo);
    string palavra;
    while (ss >> palavra) {
        // Remover pontuações
        palavra.erase(remove_if(palavra.begin(), palavra.end(), [&](char c) {
            return pontuacoes.find(c) != pontuacoes.end();
        }), palavra.end());

        // Converter para minúsculas
        transform(palavra.begin(), palavra.end(), palavra.begin(), ::tolower);

        // Verificar se não é uma stopword
        if (stopwords.find(palavra) == stopwords.end() && !palavra.empty()) {
            palavras.push_back(palavra);
        }
    }
    return palavras;
}

void Buffer::criarIndiceInvertido(const vector<Registro>& registros, const string& arquivoStopwords, const string& arquivoPontuacoes, const string& arquivoIndiceInvertido) {
    unordered_set<string> stopwords = carregarStopwords(arquivoStopwords);
    unordered_set<char> pontuacoes = carregarPontuacoes(arquivoPontuacoes);

    for (const Registro& reg : registros) {
        vector<string> palavras = processarTitulo(reg.title, stopwords, pontuacoes);
        for (const string& palavra : palavras) {
            indiceInvertido[palavra].push_back(reg.ID);
        }
    }

    ofstream arquivoBinario(arquivoIndiceInvertido, ios::binary);
    if (!arquivoBinario) {
        throw runtime_error("Erro ao abrir o arquivo de índice invertido para escrita.");
    }

    for (const auto& [palavra, ids] : indiceInvertido) {
        size_t palavraSize = palavra.size();
        arquivoBinario.write(reinterpret_cast<const char*>(&palavraSize), sizeof(palavraSize));
        arquivoBinario.write(palavra.data(), palavraSize);

        size_t idsSize = ids.size();
        arquivoBinario.write(reinterpret_cast<const char*>(&idsSize), sizeof(idsSize));
        arquivoBinario.write(reinterpret_cast<const char*>(ids.data()), idsSize * sizeof(int));
    }

    arquivoBinario.close();
}

vector<int> Buffer::buscarPorFrase(const string& frase, const string& arquivoIndiceInvertido, const string& arquivoStopwords, const string& arquivoPontuacoes) {
    unordered_set<string> stopwords = carregarStopwords(arquivoStopwords);
    unordered_set<char> pontuacoes = carregarPontuacoes(arquivoPontuacoes);
    // Processa a frase para obter palavras significativas
    vector<string> tokens = processarTitulo(frase, stopwords, pontuacoes);

    ifstream arquivoBinario(arquivoIndiceInvertido, ios::binary);
    if (!arquivoBinario) {
        throw runtime_error("Erro ao abrir o índice invertido.");
    }
    // Para cada token, localiza IDs e faz interseção
    vector<int> resultado;
    bool primeiroToken = true;

    for (const auto& token : tokens) {
        // Reinicia leitura do arquivo
        arquivoBinario.clear();
        arquivoBinario.seekg(0, ios::beg);

        vector<int> idsEncontrados;
        string palavra;
        size_t palavraSize, idsSize;
        // Busca token no índice
        while (arquivoBinario.read(reinterpret_cast<char*>(&palavraSize), sizeof(palavraSize))) {
            palavra.resize(palavraSize);
            arquivoBinario.read(&palavra[0], palavraSize);

            arquivoBinario.read(reinterpret_cast<char*>(&idsSize), sizeof(idsSize));
            vector<int> ids(idsSize);
            arquivoBinario.read(reinterpret_cast<char*>(ids.data()), idsSize * sizeof(int));

            if (palavra == token) {
                idsEncontrados = ids;
                break;
            }
        }
        // Interseção
        if (primeiroToken) {
            resultado = idsEncontrados;
            primeiroToken = false;
        } else {
            vector<int> temp;
            sort(resultado.begin(), resultado.end());
            sort(idsEncontrados.begin(), idsEncontrados.end());
            set_intersection(resultado.begin(), resultado.end(), idsEncontrados.begin(), idsEncontrados.end(), back_inserter(temp));
            resultado = temp;
        }
    }

    arquivoBinario.close();
    return resultado;
}
