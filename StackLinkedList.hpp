#ifndef STACK_LINKEDLIST_HPP
#define STACK_LINKEDLIST_HPP

#include <iostream>
#include "LinkedList.hpp"

template <class T>
class Stack : protected LinkedList<T>
{
public:
	Stack() : LinkedList<T>::LinkedList() {}
	Stack(const Stack& stk) : Stack() {*this = stk;}
	~Stack()
	{
		this->LinkedList<T>::~LinkedList();
	}

	void push(T data);
	T pop();
	const T& top() const;

	const int& length() const;

	void clear();

	bool is_empty() const;
	bool is_full() const;

	Stack& operator=(const Stack& rhs);
};

template <class T>
void Stack<T>::push(T data)
{
	this->LinkedList<T>::insert_at(data, 0);
}

template <class T>
T Stack<T>::pop()
{
	if (this->is_empty())
		throw("Empty stack cannot be popped!");

	T temp = this->get_index(0)->get_data();
	this->LinkedList<T>::remove_at(0);
	return temp;
}

template <class T>
const T& Stack<T>::top() const
{
	if (this->is_empty())
		throw("Empty stack has no data on top!");
	return this->get_index(0)->get_data();
}

template <class T>
const int& Stack<T>::length() const
{
	return this->LinkedList<T>::length();
}

template <class T>
void Stack<T>::clear()
{
	this->LinkedList<T>::clear();
}

template <class T>
bool Stack<T>::is_empty() const
{
	return this->LinkedList<T>::is_empty();
}

template <class T>
bool Stack<T>::is_full() const
{
	return this->LinkedList<T>::is_full();
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack& rhs)
{
	return this->LinkedList<T>::operator=(rhs);
}

#endif