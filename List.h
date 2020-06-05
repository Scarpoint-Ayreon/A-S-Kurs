#ifndef list_H
#define list_H

using namespace std;

#include <iostream>
#include <exception>

template<typename T>
class List
{
public:
	List();
	~List();

	void pop_front();
	void pop_back();
	void push_back(T data); 
	void push_front(T data); 
	void insert(T value, int index); 
	void clear(); 
	void removeAt(int index); 
	unsigned int GetSize() 
	{
		return Size;
	}
	T& operator[] (const int index); 
	void output_console(); 
	void set(T value, int index); 
	bool isEmpty(); 
	bool contains(T search);


	int find_last(List<T>& search); // Compare 2 lists. Get first element (bigger list, not in ()) index where match starts, or get -1 if there is no match. 
private:


	template<typename T>
	class Node
	{
	public:
		Node* pNext;
		Node* pPrev; 
		T data;

		Node(T data = T(), Node* pNext = nullptr, Node* pPrev = nullptr) // by default, next and previous pointers are nullptr
		{
			this->data = data;
			this->pNext = pNext;
			this->pPrev = pPrev;
		}
	};
	int Size;
	Node<T>* head; 
	Node<T>* tail; 
};


template<typename T>
List<T>::List()// constructor
{
	Size = 0; 
	head = nullptr;
	tail = nullptr;
}


template<typename T>
List<T>::~List() // destructor
{
	clear();
}


template<typename T>
void List<T>::pop_front()
{
	if (Size == 0)
		throw exception("Error, list is empty");
	Node<T>* temp = head;
	head = head->pNext;
	delete temp;
	Size--;
}


template<typename T>
void List<T>::pop_back()
{
	if (Size == 0)
		throw exception("Error, list is empty");
	removeAt(Size - 1);
}

template<typename T>
void List<T>::push_back(T data)
{
	if (head == nullptr)
	{
		head = new Node<T>(data);
		tail = head;
	}
	else
	{
		Node<T>* temp = new Node<T>(data);
		temp->pNext = nullptr;
		temp->pPrev = tail;
		tail->pNext = temp;
		tail = temp;
	}
	Size++;
}


template<typename T>
void List<T>::push_front(T data)
{
	head = new Node<T>(data, head);
	Size++;
	if (Size == 1)
		tail = head;
}


template<typename T>
void List<T>::insert(T data, int index)
{
	if (index == 0)
		push_front(data);
	else
	{
		if (index < 0)
			throw exception("Error, index is negative");
		if (index > Size)
			throw exception("Error, index is bigger than list size + 1");
		Node<T>* previous = this->head;
		Node<T>* nex = this->head;
		for (int i = 0; i < index - 1; i++)
		{
			previous = previous->pNext;
			nex = nex->pNext;
		}
		Node<T>* newNode = new Node<T>(data, previous->pNext);
		previous->pNext = newNode;

		nex = nex->pNext;
		nex->pPrev = previous;
		if (index != Size)
		{
			previous = previous->pNext;
			nex = nex->pNext;
			nex->pPrev = previous;
		}
		else
			tail = nex;

		Size++;
	}
}


template<typename T>
void List<T>::clear()
{
	while (Size)
		pop_front();
}


template<typename T>
void List<T>::removeAt(int index)
{
	if (index == 0)
		pop_front();
	else
	{
		if (index < 0)
			throw exception("Error, index  is negative");
		if (index >= Size)
			throw exception("Error, index is bigger than list size");
		Node<T>* previous = this->head;
		for (int i = 0; i < index - 1; i++)
			previous = previous->pNext;
		Node<T>* toDelete = previous->pNext;
		if (index != Size - 1)
		{
			Node<T>* nex = toDelete->pNext;
			nex->pPrev = previous;
		}
		else
			tail = previous;
		previous->pNext = toDelete->pNext;
		delete toDelete;
		Size--;
	}
}


template<typename T>
T& List<T>::operator[](const int index)
{
	int counter = 0;
	Node<T>* current = this->head;
	while (current != nullptr)
	{
		if (counter == index)
			return current->data;
		current = current->pNext;
		counter++;
	}
}

template<typename T>
void List<T>::output_console()
{
	Node<T>* current = this->head;
	if (Size == 0)
		cout << "Error, ist is empty";
	else
		while (current != nullptr)
		{
			cout << current->data << endl;
			current = current->pNext;
		}
}

template<typename T>
void List<T>::set(T data, int index)
{
	if (index < 0)
		throw exception("Error, index is negative");
	if (index >= Size)
		throw exception("Error, index is bigger than list size");
	int counter = 0;
	Node<T>* current = this->head;
	while (current != nullptr)
	{
		if (counter == index)
			break;
		current = current->pNext;
		counter++;
	}

	current->data = data;

}

template<typename T>
bool List<T>::isEmpty()
{
	if (Size == 0)
		return true;
	else
		return false;
}

template<typename T>
bool List<T>::contains(T search)
{
	Node<T>* temp = head;
	for (unsigned int i = 0; i < GetSize(); ++i)
		if (temp->data == search)
			return true;
	return false;
}

template<typename T>
int List<T>::find_last(List<T>& search)
{
	if (Size == 0)
		throw exception("Error, main list contains 0 items");
	if (search.GetSize() > Size)
		throw exception("Error, included list is bigger than main one");
	if (search.GetSize() == 0)
		throw exception("Error, included list contains 0 items");
	Node<T>* field = this->tail;
	int steps = 0;
	bool match = false;
	for (int i = Size - 1; i >= 0; i--)
	{
		if (field->data == search[search.GetSize() - 1])
		{
			steps = 0;
			for (int j = search.GetSize() - 2; j >= 0; j--)
			{
				steps++;
				field = field->pPrev;
				if (field->data != search[j])
				{
					match = false;
					for (int k = 0; k < steps; k++)
						field = field->pNext;
					steps = 0;
					break;
				}
				if (j == 0)
					match = true;
			}
			if ((match == true) || (search.GetSize() == 1))
			{
				return (i - steps);
			}
		}
		field = field->pPrev;
	}
	return -1;
}

#endif list_H