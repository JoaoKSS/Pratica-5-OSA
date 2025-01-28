#include "Index.h"
#include <cstring>
#include<string>
#include<sstream>
#include <fstream>
#include <iostream>
using namespace std;

Index::Index(const string& nomeArquivo) 
    : nomeArquivo(nomeArquivo) {}

Index::Index(int id, streampos endereco,  const string& nomeArquivo)
    : id(id), endereco(endereco), nomeArquivo(nomeArquivo) {}

Index::Index() {}



