#include "Buffer.h"
#include "Registro.h"
#include "Index.h"
#include "Arvore.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>

int main() {
    string arquivoCSV = "booksDataset.csv";
    string arquivoBinario = "dadosBinario.bin";
    string arquivoIndex = "Index.bin";
    string arquivoStopwords = "stopwords_en.txt";
    string arquivoPontuacoes = "pontuacoes.txt";
    string arquivoIndiceInvertido = "indiceInvertido.bin";

    vector<Registro> Registros;

    if (!Registro::carregarDoCSV(arquivoCSV, Registros)) {
        cerr << "Erro ao carregar registros do CSV." << endl;
        return 1;
    }

    Buffer buffer(arquivoBinario, arquivoIndex);
    buffer.escreverDescritor(Registros);
    buffer.criarIndiceInvertido(Registros, arquivoStopwords, arquivoPontuacoes, arquivoIndiceInvertido);

    // try {
    //     cout << endl;
    //     cout << "+----------------------------------------------------------------+" << endl;
    //     cout << " Registros buscados e encontrados com sucesso e mostrados abaixo!" << endl;
    //     cout << "+----------------------------------------------------------------+" << endl << endl;
    //     buffer.buscarRegistro(105);
    //     buffer.buscarRegistro(203);
    //     buffer.buscarRegistro(102005);
    // } catch (const runtime_error& e) {
    //     cerr << e.what() << endl;
    // }

    // try {
    //     vector<Registro> novosRegistros = {
    //         Registro(123457, "Zen and the Art of Guitar", "Harper, James",2010, "Music , Instruction & Study , Techniques"),
    //         Registro(123458, "Zero Hour: A Novel", "Smith, Caroline", 2018, "Fiction , Thrillers , Action & Adventure"),
    //         Registro(123459, "Zodiac Signs: An Illustrated Guide", "Moore, Abigail", 2021, "Self-Help , Astrology , Illustrated")
    //     };

    //     buffer.adicionarRegistros(novosRegistros);
    //     cout << endl << endl;
    //     cout << "+-----------------------------------------------------------------+" << endl;
    //     cout << " Registros novos foram adicionados com sucesso e mostrados abaixo!" << endl;
    //     cout << "+-----------------------------------------------------------------+" << endl;
    //     cout << endl;

    //     for(auto reg : novosRegistros) {
    //         buffer.buscarRegistro(reg.ID);
    //     }

    // } catch (const std::exception& e) {
    //     cerr << "Erro ao adicionar registros: " << e.what() << endl;
    // }

    cout << "Digite a palavra ou frase de busca: ";
    string frase;
    getline(cin, frase);

    vector<int> ids = buffer.buscarPorFrase(frase, arquivoIndiceInvertido, arquivoStopwords, arquivoPontuacoes);
    if (!ids.empty()) {
        cout << endl;
        for (int id : ids) {
            buffer.buscarRegistro(id);
        }
    cout << "\nTotal de livros encontrados: " << ids.size() << endl;
    } else {
        cout << "Nenhum resultado encontrado!" << endl;
        cout << "\nTotal de livros encontrados: " << ids.size() << endl;
    }

    cout << endl << "Todas operações concluída com sucesso!\n";
    return 0;
}