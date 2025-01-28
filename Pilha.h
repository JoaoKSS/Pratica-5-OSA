/*****************************************************
 * IMPLEMENTAÇÃO DO TAD - Alocação dinâmica
 *****************************************************
 * TAD PILHA IMPLEMENTADO UTILIZANDO ENCADEAMENTO DE NÓS
  *****************************************************/
// diretiva de pré-compilação para evitar a redefinição do tipo
#ifndef PILHA_H
#define PILHA_H

#include <cstdlib> // para usar o apontador NULL
#include <cassert>
using namespace std;

// implementação de uma classe para representação dos nós da PILHA
template <class T>
class NodePilha
{
    public:
        T item;
        NodePilha<T>* prox;
        // construtor default
        NodePilha(){ prox = NULL;}
        // construtor com um parametro
        NodePilha(T it){item =it; prox =NULL;}
};

// Declaração da classe pilha, seus atributos e métodos.
template <class TIPO>
class Pilha
{
    private:
        NodePilha<TIPO>* topo; // apontador para o topo da pilha
    public:
        Pilha(); // construtor do TAD pilha
        void Empilhar(TIPO); // método para empilhar dados no topo da pilha
        TIPO  Desempilhar(); // método para desempilhar dados do topo da pilha
        bool Vazia(); // método para verificar se um pilha está vazia
};

// construtor do TAD pilha
template <class T>
Pilha<T>::Pilha()
{
    topo = NULL;
}

// método para empilhar dados no topo de uma pilha
template <class T>
void Pilha<T>::Empilhar(T item)
{
    // criar um novo nó contendo o item e apontando o prox para NULL (veja construtor do NodePilha)
    NodePilha<T>* aux = new NodePilha<T>(item);
    aux->prox = topo;
    topo = aux;
}

// método para desempilhar dados do topo da pilha
template <class T>
T  Pilha<T>::Desempilhar()
{
    assert ( !Vazia() );

    T aux = topo->item; // recupera o elemento que está no topo da pilha
    NodePilha<T>* tmp = topo; // armazena o endereço do topo em tmp
    topo = topo->prox; // atualiza o topo para o próximo da pilha
    delete tmp;   // remove da pilha o nó que estava no topo da pilha
    return aux;   // retorna o resultado

}

// método para verificar se uma pilha está vazia
template <class T>
bool Pilha<T>::Vazia()
{
    return topo == NULL;
}

#endif
