#include "Registro.h"
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

Registro::Registro() : ID(0), title(""), authors(""), publishYear(0), category("") {}

Registro::Registro(int ID, const string& title, const string& authors, int publishYear, const string& category)
    : ID(ID), title(title), authors(authors), publishYear(publishYear), category(category) {}

// Função auxiliar para contar aspas
int CountA(const string& str) {
    int count = 0;
    for (char campo : str) {
        if (campo == '"') {
            count++;
        }
    }
    return count;
}

// Função auxiliar para leitura de campos com aspas
bool read(stringstream& ss, string& campo) {
    getline(ss, campo, ';');
    
    // Verifica se há aspas e elas estão em número ímpar
    while (CountA(campo) % 2 != 0) {
        string temp;
        getline(ss, temp, ';');
        campo += ";" + temp;
    }
    return true;
}

// Método para carregar registros de um arquivo CSV
bool Registro::carregarDoCSV(const string& arquivoCSV, vector<Registro>& registros) {
    // Se a lista de registros já estiver populada não ler novamente
    if (!registros.empty()) {
        return true;
    }
    
    ifstream arquivoIn(arquivoCSV);
    if (!arquivoIn) {
        cerr << "Erro ao abrir o arquivo CSV: " << arquivoCSV << endl;
        return false;
    }

    string linha;
    getline(arquivoIn, linha); // Ignora o cabeçalho
    while (getline(arquivoIn, linha)) {
        stringstream ss(linha);
        string ID_str, title, authors, publishYear_str, category;

        getline(ss, ID_str, ';');
        if (!read(ss, title)) {
            cerr << "Erro ao ler o título" << endl;
            continue;
        }
        getline(ss, authors, ';');
        getline(ss, publishYear_str, ';');
        getline(ss, category);

        try {
            int ID = stoi(ID_str);
            int publishYear = stoi(publishYear_str);

            Registro reg(ID, title, authors, publishYear, category);
            registros.push_back(reg);
        } catch (const invalid_argument& e) {
            cerr << "Erro ao converter valores: " << e.what() << endl;
            continue;
        }
    }
    arquivoIn.close();
    return true;
}

// Empacota os dados do registro em um formato com descritor de tamanho.
string Registro::packDescritor() const {
    string linha = to_string(ID) + "|" + title + "|" + authors + "|" + to_string(publishYear) + "|" + category + "\n";

    int tamanho = linha.size();
    string buffer(sizeof(int), '\0');
    memcpy(&buffer[0], &tamanho, sizeof(int));
    buffer += linha;
    return buffer;
}

void Registro::unpackDelimitedBinary(const string& buffer) {
    stringstream stream(buffer);
    string ID_str;
    getline(stream, ID_str, '|');
    ID = stoi(ID_str);
    getline(stream, title, '|');
    getline(stream, authors, '|');
    string publishYear_str;
    getline(stream, publishYear_str, '|');
    publishYear = stoi(publishYear_str);
    getline(stream, category, '\n');
}

void Registro::unpackDescritor(const string& buffer) {
    if (buffer.size() < sizeof(int)) {
        throw runtime_error("Buffer size is smaller than expected.");
    }

    int tamanho;
    memcpy(&tamanho, buffer.data(), sizeof(int));

    if (buffer.size() < sizeof(int) + tamanho) {
        throw runtime_error("Buffer size is smaller than expected.");
    }

    unpackDelimitedBinary(buffer.substr(sizeof(int), tamanho));
}