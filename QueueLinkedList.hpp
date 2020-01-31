#ifndef QUEUE_LINKEDLIST_HPP
#define QUEUE_LINKEDLIST_HPP

#include <iostream>
#include "LinkedList.hpp"

template <class T>
class Queue : protected LinkedList<T>
{
	typename LinkedList<T>::Node * rear;

public:
	Queue() : LinkedList<T>::LinkedList() {this->rear = nullptr;}
	Queue(const Queue& q) : Queue() {*this = q;}

	void enqueue(T data);
	T dequeue();

	const int& length() const;
	const T& front_element() const;

	void clear();

	bool is_empty() const;
	bool is_full() const;

	Queue& operator=(const Queue& rhs);
};

template <class T>
void Queue<T>::enqueue(T data)
{
	if (this->rear != nullptr)
	{
		typename LinkedList<T>::Node *temp = new typename LinkedList<T>::Node(data);
		this->rear->bind_next(temp);
		this->list_length++;
		this->rear = temp;
	}
	else
	{
		this->LinkedList<T>::insert_at(data, 0);
		this->rear = this->get_index(0);	
	}
}

template <class T>
T Queue<T>::dequeue()
{
	if (this->is_empty())
		throw("Empty queue cannot be dequeued!");

	T temp = this->LinkedList<T>::get_index(0)->get_data();
	this->LinkedList<T>::remove_at(0);

	if (this->length() == 0)
		this->rear = nullptr;
	return temp;
}

template <class T>
const int& Queue<T>::length() const
{
	return this->LinkedList<T>::length();
}

template <class T>
const T& Queue<T>::front_element() const
{
	return this->get_index(0)->get_data();
}

template <class T>
void Queue<T>::clear()
{
	this->LinkedList<T>::clear();
	this->rear = nullptr;
}

template <class T>
bool Queue<T>::is_empty() const
{
	return this->LinkedList<T>::is_empty();
}

template <class T>
bool Queue<T>::is_full() const
{
	return this->LinkedList<T>::is_full();
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue& rhs)
{
	if (this != &rhs)
	{
		this->clear();
		if (rhs.is_empty())
			this->rear = nullptr;
		else
		{
			typename LinkedList<T>::Node *tracker = rhs.get_index(0);
			while (tracker != nullptr)
			{
				this->enqueue(tracker);
				tracker = tracker->get_next();
			}
		}
	}
	return *this;
}

#endif