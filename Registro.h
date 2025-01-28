#ifndef REGISTRO_H
#define REGISTRO_H
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

class Registro {
public:
    int ID;
    string title;
    string authors;
    int publishYear;
    string category;

    static const size_t titleTamanho = 100;
    static const size_t authorsTamanho = 100;
    static const size_t categoryTamanho = 50;

    Registro();
    Registro(int ID, const string& title, const string& authors, int publishYear, const string& category);

    static bool carregarDoCSV(const string& arquivoCSV, vector<Registro>& registros);
    
    string packDelimitedbinary() const;
    string packDescritor() const;

    void unpackDelimitedBinary(const string& buffer);
    void unpackDescritor(const string& buffer);
};

// ...existing code...

#endif
