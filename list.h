#pragma once

#include <assert.h>


template<class T>
class Node
{
public:
	Node()
	{
		next = prev = nullptr;
	}
	Node(const T& value, Node<T> * n = nullptr, Node<T> * p = nullptr)
	{
		info = value;
		next = n;
		prev = p;
	}
	T info;
	Node<T> *next, *prev;
};


template<class T>
class ListIterator
{
public:
	ListIterator(Node<T> * node)
		: ptr(node)
	{  }
	void operator++()
	{
		assert(ptr != nullptr);

		ptr = ptr->next;
	}
	void operator--()
	{
		assert(ptr != nullptr);

		ptr = ptr->prev;
	}
	T operator*()
	{
		return ptr->info;
	}
	Node<T> * operator->()
	{
		return ptr;
	}
	bool operator !()
	{
		return !ptr;
	}
	bool operator==(const ListIterator &B) const
	{
		return ptr == B.ptr;
	}
	bool operator!=(const ListIterator &B) const
	{
		return ptr != B.ptr;
	}
	bool isValid() const
	{
		return ptr != nullptr;
	}
private:
	Node<T> * ptr;
};


template<class T>
class List
{
public:
	typedef ListIterator<T> iterator;
	List();
	List(const List &B);
	List(const List &&B);
	~List();
	void addToHead(const T& value);
	void addToTail(const T& value);
	T deleteFromHead();
	T deleteFromTail();
	ListIterator<T> begin() const;
	ListIterator<T> end() const;
	ListIterator<T> rbegin() const;
	ListIterator<T> rend() const;
	bool isEmpty() const;
	void clear();

	List<T> & operator=(const List<T> &B);
	List<T> & operator=(const List<T> &&B);
protected:
	Node<T> *head, *tail;
};


template<class T>
List<T>::List()
{
	head = tail = nullptr;
}

template<class T>
List<T>::List(const List &B)
{
	for (Node<T> * tmp = B.head; tmp != nullptr; tmp = tmp->next)
		this->addToTail(tmp->info);
}

template<class T>
List<T>::List(const List &&B)
{
	this->head = B.head;
	this->tail = B.tail;
	B.head = nullptr;
	B.tail = nullptr;
}

template<class T>
List<T>::~List()
{
	clear();
}

template<class T>
void List<T>::addToHead(const T& value)
{
	if (head != nullptr)
	{
		head = new Node<T>(value,head,nullptr);
		head->next->prev = head;
	}
	else
	{
		head = tail = new Node<T>(value);
	}
}

template<class T>
void List<T>::addToTail(const T& value)
{
	if (tail != nullptr)
	{
		tail = new Node<T>(value,nullptr,tail);
		tail->prev->next = tail;
	}
	else
	{
		head = tail = new Node<T>(value);
	}
}

template<class T>
T List<T>::deleteFromHead() {
	T value = head->info;
	if (head == tail)
	{
		delete head;
		head = tail = nullptr;
	}
	else
	{
		head = head->next;
		delete head->prev;
		head->prev = nullptr;
	}

	return value;
}

template<class T>
T List<T>::deleteFromTail()
{
	T value = tail->info;
	if (head == tail)
	{
		delete head;
		head = tail = nullptr;
	}
	else
	{
		tail = tail->prev;
		delete tail->next;
		tail->next = nullptr;
	}

	return value;
}

template<class T>
ListIterator<T> List<T>::begin() const
{
	return ListIterator<T>(head);
}

template<class T>
ListIterator<T> List<T>::end() const
{
	return ListIterator<T>(nullptr);
}

template<class T>
ListIterator<T> List<T>::rbegin() const
{
	return ListIterator<T>(tail);
}

template<class T>
ListIterator<T> List<T>::rend() const
{
	return ListIterator<T>(nullptr);
}

template<class T>
bool List<T>::isEmpty() const
{
	return head == nullptr;
}

template<class T>
void List<T>::clear()
{
	Node<T> * temp;
	while (head != nullptr)
	{
		temp = head->next;
		delete head;
		head = temp;
	}

	tail = nullptr; ///< dont forget to nullify tail
}

template<class T>
List<T> & List<T>::operator=(const List<T> &B)
{
	if (this != &B)
	{
		this->clear();
		for (Node<T> * tmp = B.head; tmp != nullptr; tmp = tmp->next)
			this->addToTail(tmp->info);
	}

	return *this;
}

template<class T>
List<T> & List<T>::operator=(const List<T> &&B)
{
	if (this != &B)
	{
		this->clear();
		this->head = B.head;
		this->tail = B.tail;
		B.head = nullptr;
		B.tail = nullptr;
	}

	return *this;
}
