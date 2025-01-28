/*****************************************************
 * IMPLEMENTA��O DO TAD - Aloca��o din�mica
 *****************************************************
 * TAD FILA IMPLEMENTADO UTILIZANDO ENCADEAMENTO DE N�S
  *****************************************************/
// diretiva de pr�-compila��o para evitar a redefini��o do tipo
#ifndef FILA_H
#define FILA_H

#include <cstdlib> // para usar o apontador NULL
#include <iostream>
#include <cassert>
using namespace std;

// implementa��o de uma classe para representa��o dos n�s da FILA
template <class T>
class NodeFila
{
    public:
        T item;
        NodeFila<T>* prox;
        // construtor default
        NodeFila(){ prox = NULL;}
        // construtor com um parametro
        NodeFila(T it){item =it; prox =NULL;}
};

// Declara��o da classe FILA, seus atributos e m�todos.
template <class TIPO>
class Fila
{
    private:
        NodeFila<TIPO>* head; // apontador para o in�cio da fila
        NodeFila<TIPO>* tail; // apontador para o final da fila
    public:
        Fila(); // construtor do TAD fila
        Fila(const Fila<TIPO>&); // construtor de c�pia
        void Enfileirar(TIPO); // m�todo para enfileirar dados no final da fila
        TIPO  Desenfileirar(); // m�todo para desenfileirar dados do inicio da fila
        bool Vazia(); // m�todo para verificar se um fila est� vazia
        ~Fila(); // destrutor
        void Imprimir();
        Fila<TIPO>& operator=(const Fila<TIPO>&);
};

// construtor do TAD fila
template <class T>
Fila<T>::Fila()
{
    head = NULL;
    tail = NULL;
}

// construtor de c�pia
template <class T>
Fila<T>::Fila(const Fila<T>& f)
{
    NodeFila<T> *aux = f.head;
    head = tail = NULL;
    while ( aux != NULL )
    {
        this->Enfileirar(aux->item);
        aux = aux->prox;
    }
}


// m�todo para enfileirar dados no final da fila
template <class T>
void Fila<T>::Enfileirar(T item)
{
    // criar um novo n� contendo o item e apontando o prox para NULL (veja construtor do Node)
    NodeFila<T>* aux = new NodeFila<T>(item);

    // verifica se a Fila est� vazia (head==NULL) e,
    // caso afirmativo, aponta head para o novo n� (aux)...
    if( head == NULL )
        head = aux;
    else // caso contr�rio, o prox do final da fila aponta para aux.
        tail->prox = aux;
    // faz aux ser o final da fila (tail aponta para aux)
    tail = aux;
}

// m�todo para desenfileirar dados do inicio da fila
template <class T>
T  Fila<T>::Desenfileirar()
{
    assert ( !Vazia() );

    T aux = head->item; // recupera o elemento que est� no in�cio da fila
    NodeFila<T>* tmp = head; // armazena o endere�o de head em tmp
    head = head->prox; // atualiza head para o pr�ximo da fila
    delete tmp;   // remove da fila o n� que estava no in�cio da fila
    return aux;   // retorna o resultado
}

// m�todo para verificar se um fila est� vazia
template <class T>
bool Fila<T>::Vazia()
{
    return head == NULL;
}

template <class T>
void Fila<T>::Imprimir()
{
    NodeFila<T> *aux = head;
    while (aux!=NULL)
    {
        cout<<aux->item<<"->";
        aux = aux->prox;
    }
}

template <class T>
Fila<T>::~Fila()
{
    NodeFila<T> *tmp, *aux = head;
    while (aux != NULL )
    {
        tmp = aux->prox;
        delete aux;
        aux = tmp;
    }
}

template <class T>
Fila<T>& Fila<T>::operator=(const Fila<T>& f)
{
    NodeFila<T> *aux = f.head;
    head = tail = NULL;
    while ( aux != NULL )
    {
        this->Enfileirar(aux->item);
        aux = aux->prox;
    }
    return *this;
}



#endif
