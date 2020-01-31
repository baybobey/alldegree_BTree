#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>

template <class T>
class LinkedList
{
public:
	class Node
	{
	private:
		T data;
		Node *next;

	protected:
		Node* release_next();

	public:
		Node() : next(nullptr) {}
		Node(T data) : Node() {this->data = data;}
		Node(T data, Node *next) : data(data), next(next) {}
		Node(const Node& node) {*this = node;}

		void set_data(T data);
		const T& get_data();
		Node* get_next();

		void bind_next(Node *given_next);
		void bind_next_plus(Node *given_next);
		Node* unbind_next();

		virtual Node& operator=(const Node& rhs);

		friend class LinkedList;
	};

private:
	Node *head;

protected:
	int list_length;
	
public:
	LinkedList() {this->head = nullptr; this->list_length = 0;}
	LinkedList(const LinkedList& list) : LinkedList() {*this = list;}
	virtual ~LinkedList()
	{
		if (this->head != nullptr)
		{
			this->clear();
			this->head = nullptr;
		}
	}

	void insert_at(T data, int index);
	void insert_after(T data, Node *node = nullptr);
	void insert_node_at(Node *node, int index);
	void insert_node_after(Node *inserted, Node *node2);

	void remove(T data);
	void remove_at(int index);
	void remove_node(Node *node);
	void clear();

	Node* extract_node_at(int index);
	Node* extract_node_after(Node *node = nullptr);

	Node* linear_search(T data) const;
	Node* linear_search_node(Node *node) const;
	Node* get_prev_of_node(Node *node) const;
	Node* get_prev_of_data(T data) const;
	Node* get_index(int index) const;

	bool contains(Node* node) const;
	bool is_empty() const;
	bool is_full() const;

	const int& length() const;

	void display() const;

	LinkedList& operator=(const LinkedList& rhs);
};

//Node class begin
template <class T>
void LinkedList<T>::Node::bind_next(typename LinkedList<T>::Node* node)
{
	if (this->next != nullptr)
		throw("DATA LOSS HAPPENS!");
	this->next = node;
}

template <class T>
void LinkedList<T>::Node::bind_next_plus(typename LinkedList<T>::Node* node)
{
	if (this->next != nullptr)
		throw("DATA LOSS HAPPENS!");

	node->next = this->next;
	this->next = node;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::Node::unbind_next()
{
	if (this->next != nullptr)
	{
		Node *temp = this->next;
		this->next = temp->next;
		temp->next = nullptr;
		return temp;
	}
	return nullptr;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::Node::release_next()
{
	Node *temp = this->next;
	if (this->next != nullptr)
		this->next = nullptr;

	return temp;
}

template <class T>
void LinkedList<T>::Node::set_data(T data) {this->data = data;}

template <class T>
const T& LinkedList<T>::Node::get_data() {return this->data;}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::Node::get_next() {return this->next;}


template <class T>
typename LinkedList<T>::Node& LinkedList<T>::Node::operator=(const LinkedList<T>::Node& rhs)
{
	if (this != &rhs)
	{
		this->data = rhs.data;
		this->next = rhs.next;
	}
	return *this;
}
//Node class end

//LinkedList class begin
template <class T>
void LinkedList<T>::insert_at(T data, int index)
{
	if (index > this->length() || index < 0)
		throw("Cannot insert element at given index! Index out of range.");
	else if (index == 0)
		this->insert_after(data);
	else
	{
		Node *tracker = this->get_index(index-1);
		Node *temp = new Node(data, tracker->get_next());
		tracker->release_next();
		tracker->bind_next(temp);
		this->list_length++;
	}
}

template <class T>
void LinkedList<T>::insert_after(T data, typename LinkedList<T>::Node *node)
{
	Node *temp = nullptr, *tracker = nullptr;

	if (node == nullptr)
	{
		Node *temp = this->head;
		this->head = new Node(data, temp);
		this->list_length++;
		return;
	}
	else
	{
		tracker = this->linear_search_node(node);
		if (tracker == nullptr)
			throw("Given node doesn't exist!");
		else
		{
			temp = new Node(data, tracker->release_next());
			tracker->bind_next(temp);
			this->list_length++;
			return;
		}
	}
}

template <class T>
void LinkedList<T>::insert_node_at(typename LinkedList<T>::Node *node, int index)
{
	if (index > this->length() || index < 0)
		throw("Cannot insert element!");
	else if (index == 0)
	{
		node->bind_next(this->head);
		this->head = node;
		this->list_length++;
	}
	else
	{
		Node *tracker = this->get_index(index-1);
		node->bind_next(tracker->release_next());
		tracker->bind_next(node);
		this->list_length++;
	}
}

template <class T>
void LinkedList<T>::insert_node_after(typename LinkedList<T>::Node *inserted, typename LinkedList<T>::Node *node)
{
	if (this->contains(node))
	{
		inserted->bind_next(node->release_next());
		node->bind_next(inserted);
		this->list_length++;
	}
	else
		throw("Given node doesn't exist!");
}

template <class T>
void LinkedList<T>::remove(T data)
{
	if (this->is_empty())
		throw("Empty linked list cannot delete an element!");
	else if (this->head->data == data)
	{
		Node *temp = this->head;
		this->head = this->head->get_next();
		delete temp;
		this->list_length--;
		return;
	}

	Node *tracker = this->get_prev_of_data(data);

	if (tracker == nullptr)
		throw("Given element doesn't exist in linked list for deletion!");
	else
	{
		Node *temp = tracker->unbind_next();
		if (temp != nullptr)
		{
			delete temp;
			this->list_length--;
		}
		return;
	}
}

template <class T>
void LinkedList<T>::remove_at(int index)
{
	if (this->is_empty())
		throw("Empty linked list cannot delete an element!");
	else if (index == 0)
	{
		Node *temp = this->head;
		this->head = this->head->get_next();
		delete temp;
		this->list_length--;
	}
	else
	{
		Node *tracker = this->get_index(index-1);
		tracker = tracker->unbind_next();
		if (tracker != nullptr)
		{
			delete tracker;
			this->list_length--;
		}
	}
}

template <class T>
void LinkedList<T>::remove_node(typename LinkedList<T>::Node* node)
{
	if (node == nullptr)
		throw("No specific node given!");
	else if (node == this->head)
		return this->remove_at(0);

	Node *tracker = this->get_prev_of_node(node);

	if (tracker == nullptr)
		throw("Given node does not exist in given linked list!");
	else
	{
		delete tracker->unbind_next();
		this->list_length--;
		return;
	}
}

template <class T>
void LinkedList<T>::clear()
{
	while (!this->is_empty())
		this->remove_at(0);
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::extract_node_at(int index)
{
	if (index >= this->length() || index < 0)
		throw("Index out of scope! No extraction happened!");

	if (this->is_empty())
		throw("Empty list has no element to extract!");
	else if (index == 0)
	{
		Node *temp = this->head;
		this->head = this->head->get_next();
		temp->release_next();
		this->list_length--;
		return temp;
	}

	Node *tracker = this->get_index(index-1);

	if (tracker->next != nullptr)
	{
		this->list_length--;
		return tracker->unbind_next();
	}
	return nullptr;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::extract_node_after(typename LinkedList<T>::Node* node)
{
	if (node == nullptr)
		return this->head;

	if (this->contains(node))
	{
		if (node->get_next() == nullptr)
			return nullptr;

		this->list_length--;
		return node->unbind_next();
	}
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::linear_search(T data) const
{
	if (this->is_empty())
		return nullptr;

	Node *tracker = this->head;
	while (tracker != nullptr)
	{
		if (tracker->data == data)
			return tracker;
		tracker = tracker->get_next();
	}
	return tracker;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::linear_search_node(Node *node) const
{
	if (this->is_empty())
		return nullptr;

	Node *tracker = this->head;
	while (tracker != nullptr)
	{
		if (tracker == node)
			return tracker;
		tracker = tracker->get_next();
	}
	return tracker;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::get_prev_of_data(T data) const
{
	if (this->is_empty())
		return nullptr;

	Node *tracker = this->head, *follower = nullptr;

	while (tracker != nullptr)
	{
		if (tracker->data == data)
			return follower;
		follower = tracker;
		tracker = tracker->get_next();
	}
	return tracker;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::get_prev_of_node(typename LinkedList<T>::Node* node) const
{
	if (this->is_empty())
		throw("Empty linked list doesn't contain prev data!");

	Node *tracker = this->head;
	while (tracker != nullptr)
	{
		if (tracker->get_next() == node)
			return tracker;
		tracker = tracker->get_next();
	}
	throw("Given node is not in linked list!");
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::get_index(int index) const
{
	if (index < 0 || index >= this->length())
		throw("Index out of scope!");
	
	Node *tracker = this->head;
	for (int i=0; i < index; i++)
		tracker = tracker->get_next();
	return tracker;
}

template <class T>
bool LinkedList<T>::contains(typename LinkedList<T>::Node* node) const
{
	if (this->is_empty())
		return false;

	Node *tracker = this->head;
	while (tracker != node)
		tracker = tracker->get_next();

	if (tracker == nullptr)
		return false;
	return true;
}

template <class T>
bool LinkedList<T>::is_empty() const {return this->head == nullptr;}

template <class T>
bool LinkedList<T>::is_full() const
{
	Node *temp = nullptr;
	temp = new Node;
	if (temp == nullptr)
		return true;
	delete temp;
	return false;
}

template <class T>
const int& LinkedList<T>::length() const {return this->list_length;}

template <class T>
void LinkedList<T>::display() const
{
	if (this->is_empty())
		std::cout << std::endl;

	Node *tracker = this->head;
	/*
	for (int i=0; i < this->length(); i++)
	{
		std::cout << tracker->data << " ";
		tracker = tracker->next;
	}
	std::cout << std::endl;*/
	while (tracker != nullptr)
	{
		std::cout << tracker->data << " ";
		tracker = tracker->get_next();
	}
	std::cout << std::endl << std::endl;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs)
{
	if (this != &rhs)
	{
		this->clear();
		Node *tracker = rhs.head;
		Node *creater = nullptr;

		while (tracker != nullptr)
		{
			if (this->head == nullptr)
			{
				this->head = new Node(tracker->get_data());
				creater = this->head;
			}
			else
			{
				creater->next = new Node(tracker->get_data());
				creater = creater->get_next();
			}
			this->list_length++;
			tracker = tracker->get_next();
		}
	}
	return *this;
}
//LinkedList class end

#endif