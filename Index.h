#ifndef Index_H
#define Index_H
#include<string>
#include<cstddef>
#include<stdint.h>
#include <string>
#include <sstream>
using namespace std;

class Index{
    public:
        int id;
        streampos endereco;
        string nomeArquivo;
        
        Index(const string& nomeArquivo); 
        Index(int id, streampos endereco,  const string& nomeArquivo);
        Index(); 
};
#endif