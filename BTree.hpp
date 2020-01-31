#ifndef BTREE_HPP
#define BTREE_HPP

#include <iostream>
#include <vector>
#include "LinkedList.hpp"
#include "StackLinkedList.hpp"
#include "QueueLinkedList.hpp"

template <class T>
class BTree
{
	class Node
	{
	public:
		enum class node_situation{empty, normal, overloaded};

		LinkedList<T> node_data;
		LinkedList<Node*> children;
		node_situation situation;

		Node() {this->situation =node_situation::empty;}
		Node(LinkedList<T> node_data) : node_data(node_data) {this->situation = node_situation::normal;}
		Node(const Node& node) : Node() {*this = node;}

		void insert_to_node(T data, int max_node_data_length);
		T remove_from_node(int index, int min_node_data_length);

		bool is_leaf() const;

		Node& operator=(const Node& rhs);
	};

	Node *root;
	int max_node_data_length;
	int min_node_data_length;
	int max_node_degree;

	Node* search_with_path(T data, int& index, Stack<Node*>& path);
	Node* place_to_insert(T data, int& index, Stack<Node*>& path);
	Node* pre_inorder(Node *start, int& index, Stack<Node*>& path);

	Node* search_with_path_and_index(T data, int& index, Stack<Node*>& path, Stack<Node*>& path_left, Stack<Node*>& path_right, Stack<int>& indices);
	Node* pre_inorder_with_index(Node *start, int& index, Stack<Node*>& path, Stack<Node*>& path_left, Stack<Node*>& path_right, Stack<int>& indices);

	void split(Node *node, Node *parent);//ok1
	bool can_borrow(Node *node_borrower, Node *node_sharer) const; 
	void borrow_from_left(Node *node_borrower, Node *node_sharer, Node *parent, int index);
	void borrow_from_right(Node *node_borrower, Node *node_sharer, Node *parent, int index);
	void merge_left(Node* empty, Node* left_sibling, Node* parent, int index);
	void merge_right(Node* empty, Node* right_sibling, Node* parent, int index);

	void inorder_display(Node *node) const;

	void create_data_list(Node* node, Queue<T>& inorder_data_list);

	void rec_create(Node* to, Node* from);

public:
	BTree(int max_node_degree = 3)
	{
		if (max_node_degree < 3)
			throw("BTree max node degree cannot be less than 3!");

		//Simple btree properties for given degree
		this->root = nullptr;
		this->max_node_data_length = max_node_degree-1;
		this->max_node_degree = max_node_degree;
		this->min_node_data_length = (max_node_data_length)/2;
	}
	BTree(const std::vector<T>& list)
	{
		for (int i=0; i < list.size(); i++)
			this->insert(list[i]);
	}
	BTree(const BTree& btree) : BTree() {*this = btree;}
	virtual ~BTree()
	{
		this->clear();
		this->root = nullptr;
	}

	void insert(T data);//ok1
	void insert_multiple(const std::vector<T>& list);
	void remove(T data);//ok1
	void remove_multiple(const std::vector<T>& list);

	void clear();

	Node* search(T data) const;

	bool is_empty() const;
	bool is_full() const;

	void inorder_display() const;
	void levelorder_display() const;

	BTree& operator=(const BTree& rhs);

	void copy_to(BTree& rhs);
};

//Node functions start
template <class T>
void BTree<T>::Node::insert_to_node(T data, int max_node_data_length)
{
	//Insertion
	if (this->node_data.length() == 0)
	{
		this->node_data.insert_at(data, 0);
		return;
	}
	

	typename LinkedList<T>::Node* temp = this->node_data.get_index(0);
	
	for (int i=0; i < this->node_data.length(); i++)
	{
		if (data < temp->get_data())
		{
			this->node_data.insert_at(data, i);
			break;
		}
		temp = temp->get_next();
	}

	if (temp == nullptr)//biggest element case
		this->node_data.insert_at(data, this->node_data.length());

	//Node Situation Update
	if (this->node_data.length() > max_node_data_length)
		this->situation = node_situation::overloaded;
	else if (this->node_data.length() >= (max_node_data_length)/2)
		this->situation = node_situation::normal;
}

template <class T>
T BTree<T>::Node::remove_from_node(int index, int min_node_data_length)
{
	if (this->node_data.length() == 0)
		throw("Empty node cannot remove any element!");

	typename LinkedList<T>::Node* temp = this->node_data.extract_node_at(index);
	T data = temp->get_data();

	//Node Situation Update
	if (this->node_data.length() < min_node_data_length)
		this->situation = node_situation::empty;

	delete temp;
	return data;
}

template <class T>
bool BTree<T>::Node::is_leaf() const
{
	return this->children.length() == 0;
}

template <class T>
typename BTree<T>::Node& BTree<T>::Node::operator=(const BTree<T>::Node& rhs)
{
	if (this != &rhs)
	{
		this->node_data = rhs.node_data;
		this->children.clear();
		this->situation = rhs.situation;
	}
	return *this;
}
//Node functions end


//Tree functions start
template <class T>
typename BTree<T>::Node* BTree<T>::search_with_path(T data, int& index, Stack<Node*>& path)
{
	if (this->is_empty())
		return nullptr;

	Node* tracker = this->root;
	typename LinkedList<T>::Node* tracker_data = nullptr;
	typename LinkedList<T>::Node* tracker_children = nullptr;

	bool data_is_greater;
	index = -1;

	while(true)
	{
		path.push(tracker);
		data_is_greater = true;
		tracker_data = tracker->node_data.get_index(0);

		if (!tracker->is_leaf())
		{
			tracker_children = tracker->children.get_index(0);
			for (int i=0; i < tracker->node_data.length(); i++)
			{
				if (data < tracker_data.get_data())
				{
					tracker = tracker_children->get_data();
					data_is_greater = false;
					break;
				}
				tracker_data = tracker_data->get_next();
				tracker_children = tracker_children->get_next();
			}
			if (data_is_greater)
				tracker = tracker_children->get_data();
		}
		else
		{
			for (int i=0; i < tracker->node_data.length(); i++)
			{
				if (data == tracker_data.get_data())
				{
					index = i;
					break;
				}
				tracker_data = tracker_data->get_next();
			}
			if (index == -1)
				return nullptr;
			return tracker;
		}
	}
}

template <class T>
typename BTree<T>::Node* BTree<T>::place_to_insert(T data, int& index, Stack<Node*>& path)
{
	if (this->is_empty())
	{
		this->root = new Node;
		return this->root;
	}

	Node* tracker = this->root;
	typename LinkedList<T>::Node* tracker_data = nullptr;
	typename LinkedList<Node*>::Node* tracker_children = nullptr;

	bool data_is_greater;
	index = -1;

	while(true)
	{
		path.push(tracker);
		data_is_greater = true;
		tracker_data = tracker->node_data.get_index(0);

		if (!tracker->is_leaf())
		{
			tracker_children = tracker->children.get_index(0);
			for (int i=0; i < tracker->node_data.length(); i++)
			{
				if (data < tracker_data->get_data())
				{
					tracker = tracker_children->get_data();
					data_is_greater = false;
					break;
				}
				else if (data == tracker_data->get_data())
					return nullptr;

				tracker_data = tracker_data->get_next();
				tracker_children = tracker_children->get_next();
			}
			if (data_is_greater)
				tracker = tracker_children->get_data();
		}
		else
		{
			for (int i=0; i < tracker->node_data.length(); i++)
			{
				if (data < tracker_data->get_data())
				{
					index = i;
					break;
				}
				else if (data == tracker_data->get_data())
					return nullptr;

				tracker_data = tracker_data->get_next();
			}
			if (tracker_data == nullptr)
				index = tracker->node_data.length();
			return tracker;
		}
	}
}

template <class T>
typename BTree<T>::Node* BTree<T>::pre_inorder(Node *start, int& index, Stack<Node*>& path)
{
	if (start == nullptr)
		throw("nullptr has no presuccessive inorder!");

	Node *temp = start;

	if (!temp->is_leaf())
		temp = temp->children.get_index(index)->get_data();

	while (!temp->is_leaf())
	{
		path.push(temp);
		temp = temp->children.get_index(temp->children.length()-1)->get_data();
	}
	return temp;
}

template <class T>
typename BTree<T>::Node* BTree<T>::search_with_path_and_index(T data, int& index, Stack<typename BTree<T>::Node*>& path, Stack<typename BTree<T>::Node*>& path_left, Stack<typename BTree<T>::Node*>& path_right, Stack<int>& indices)
{
	if (this->is_empty())
	{
		this->root = new Node;
		return this->root;
	}

	typename LinkedList<T>::Node* tracker_data = nullptr;
	typename LinkedList<Node*>::Node* tracker_children = nullptr;
	typename LinkedList<Node*>::Node* tracker_children_follower = nullptr;

	Node* follower = nullptr, *tracker = nullptr, *post = nullptr;

	bool data_is_greater;
	index = -1;

	while(true)
	{
		index = 0;
		tracker = path.top();
		tracker_data = tracker->node_data.get_index(0);
		follower = post = nullptr;
		tracker_children_follower = nullptr;

		/*
		if (path_left.top() == nullptr)
			//std::cout << "nullptr, ";
		else
			//std::cout << path_left.top()->node_data.get_index(0)->get_data() << ", ";
		if (path.top() == nullptr)
			//std::cout << "nullptr, ";
		else
			//std::cout << path.top()->node_data.get_index(0)->get_data() << ", ";
		if (path_right.top() == nullptr)
			//std::cout << "nullptr, ";
		else
			//std::cout << path_right.top()->node_data.get_index(0)->get_data() << ", ";
		*/

		if (tracker->is_leaf())
		{
			for (int i=0; i < tracker->node_data.length(); i++, index++)
			{
				if (data == tracker_data->get_data())
					return tracker;
				else if (data < tracker_data->get_data())
					return nullptr;

				tracker_data = tracker_data->get_next();
			}
			return nullptr;
		}
		else
		{
			tracker_children = tracker->children.get_index(0);

			for (int i=0; i < tracker->node_data.length(); i++, index++)
			{
				if (data < tracker_data->get_data())
					break;
				else if (data == tracker_data->get_data())
					return tracker;

				tracker_data = tracker_data->get_next();
				tracker_children_follower = tracker_children;
				tracker_children = tracker_children->get_next();
			}

			if (tracker_children_follower != nullptr)
				follower = tracker_children_follower->get_data();
			else
				follower = nullptr;

			tracker = tracker_children->get_data();

			if (tracker_children->get_next() != nullptr)
				post = tracker_children->get_next()->get_data();
			else
				post = nullptr;
		}

		if (tracker_children_follower != nullptr)
			follower = tracker_children_follower->get_data();
		else
			follower = nullptr;

		tracker = tracker_children->get_data();

		if (tracker_children->get_next() != nullptr)
			post = tracker_children->get_next()->get_data();
		else
			post = nullptr;

		path.push(tracker);
		path_left.push(follower);
		path_right.push(post);
		indices.push(index);

		//std::cout << "Go to index " << index << std::endl;
	}
	return nullptr;
}

template <class T>
typename BTree<T>::Node* BTree<T>::pre_inorder_with_index(Node *start, int& index, Stack<typename BTree<T>::Node*>& path, Stack<typename BTree<T>::Node*>& path_left, Stack<typename BTree<T>::Node*>& path_right, Stack<int>& indices)
{
	if (start == nullptr)
		throw("nullptr has no presuccessive inorder!");

	Node *tracker = start;
	Node *follower = nullptr;
	Node *post = nullptr;

	typename LinkedList<Node*>::Node* temp = nullptr;

	if (!tracker->is_leaf())
	{
		if (index == 0)
		{
			temp = nullptr;
			follower = nullptr;
			post = tracker->children.get_index(1)->get_data();
			tracker = tracker->children.get_index(0)->get_data();
		}
		else
		{
			temp = tracker->children.get_index(index-1);
			follower = temp->get_data();
			tracker = temp->get_next()->get_data();
			post = temp->get_next()->get_next()->get_data();
		}
	}
	else if (index > 0)
	{
		index--;
		return tracker;
	}
	else
		return tracker;

	while (!tracker->is_leaf())
	{
		path.push(tracker);
		path_left.push(follower);
		path_right.push(post);
		indices.push(index);

		index = tracker->children.length()-1;
		temp = tracker->children.get_index(tracker->children.length()-2);
		follower = temp->get_data();
		tracker = temp->get_next()->get_data();
		post = nullptr;
	}

	path.push(tracker);
	path_left.push(follower);
	path_right.push(post);
	indices.push(index);

	index = tracker->node_data.length()-1;
	return tracker;
}

template <class T>
void BTree<T>::split(Node* node, Node* parent)
{
	//std::cout << "Split the node that last insertion happened." << std::endl;
	int just_behind_middle = (node->node_data.length()-1)/2;

	Node *creater = new Node;

	typename LinkedList<T>::Node* tracker_data = node->node_data.get_index(0);
	typename LinkedList<Node*>::Node* tracker_children = nullptr;

	T temp;
	Node* temp2;

	Stack<T> data_placement_storage;
	Stack<T> data_placement_storage2;
	Stack<Node*> children_storage;
	Stack<Node*> children_storage2;

	if (parent == nullptr)
	{
		//std::cout << "Create a new root and add last inserted node to its children list." << std::endl;
		parent = new Node;
		parent->children.insert_at(node, 0);
		this->root = parent;
	}

	for (int i=0; i < just_behind_middle; i++)
	{
		temp = tracker_data->get_data();
		data_placement_storage.push(temp);
		tracker_data = tracker_data->get_next();
	}

	temp = tracker_data->get_data();
	parent->insert_to_node(temp, this->max_node_data_length);
	tracker_data = tracker_data->get_next();

	for (int i=just_behind_middle+1; i < node->node_data.length(); i++)
	{
		temp = tracker_data->get_data();
		data_placement_storage2.push(temp);
		tracker_data = tracker_data->get_next();
	}
	if (!node->is_leaf())
	{
		int middle = (node->children.length())/2;
		tracker_children = node->children.get_index(0);

		for (int i=0; i < middle; i++)
		{
			temp2 = tracker_children->get_data();
			children_storage.push(temp2);
			tracker_children = tracker_children->get_next();
		}
		for (int i=middle; i < node->children.length(); i++)
		{
			temp2 = tracker_children->get_data();
			children_storage2.push(temp2);
			tracker_children = tracker_children->get_next();
		}
	}

	//std::cout << "Clear all the elements in the splitted node." << std::endl;
	node->node_data.clear(); node->children.clear();

	while(!data_placement_storage.is_empty())
	{
		temp = data_placement_storage.pop();
		//std::cout << "Add " << temp << " to the beginning of the first node." << std::endl;
		node->insert_to_node(temp, this->max_node_data_length);
	}
	while (!children_storage.is_empty())
	{
		temp2 = children_storage.pop();
		node->children.insert_at(temp2, 0);
	}
	while (!data_placement_storage2.is_empty())
	{
		temp = data_placement_storage2.pop();
		//std::cout << "Add " << temp << " to the beginning of the second node." << std::endl;
		creater->insert_to_node(temp, this->max_node_data_length);
	}
	while (!children_storage2.is_empty())
	{
		temp2 = children_storage2.pop();
		creater->children.insert_at(temp2, 0);
	}


	tracker_children = parent->children.get_index(0);
	for (int i=0; i < parent->children.length(); i++)
	{
		if (tracker_children->get_data() == node)
		{
			parent->children.insert_at(creater, i+1);
			break;
		}
		tracker_children = tracker_children->get_next();
	}
}


template <class T>
bool BTree<T>::can_borrow(Node *borrower, Node *sharer) const
{
	if (sharer == nullptr)
		return false;
	if (borrower == nullptr)
		return false;

	if (borrower->node_data.length() >= this->min_node_data_length)
		throw("Borrow check invoked when borrow not needed!");
	else if (sharer->node_data.length() > this->min_node_data_length)
		return true;
	else
		return false;
}

template <class T>
void BTree<T>::borrow_from_left(typename BTree<T>::Node* borrower, typename BTree<T>::Node* sharer, typename BTree<T>::Node* parent, int index)
{
	
	T temp2 = sharer->remove_from_node(sharer->node_data.length()-1, this->min_node_data_length);
	parent->insert_to_node(temp2, this->max_node_data_length);
	T temp = parent->remove_from_node(index, this->min_node_data_length);
	borrower->insert_to_node(temp, this->max_node_data_length);

	if (!sharer->is_leaf())
	{
		typename LinkedList<Node*>::Node* temp = sharer->children.extract_node_at(sharer->children.length()-1);
		Node* value = temp->get_data();
		delete temp;
		borrower->children.insert_at(value, 0);
	}
}

template <class T>
void BTree<T>::borrow_from_right(typename BTree<T>::Node* borrower, typename BTree<T>::Node* sharer, typename BTree<T>::Node* parent, int index)
{
	T temp2 = sharer->remove_from_node(0, this->min_node_data_length);
	parent->insert_to_node(temp2, this->max_node_data_length);
	T temp = parent->remove_from_node(index, this->min_node_data_length);
	borrower->insert_to_node(temp, this->max_node_data_length);

	if (!sharer->is_leaf())
	{
		typename LinkedList<Node*>::Node* temp = sharer->children.extract_node_at(0);
		Node* value = temp->get_data();
		delete temp;
		borrower->children.insert_at(value, borrower->children.length());
	}
}

template <class T>
void BTree<T>::merge_left(typename BTree<T>::Node* deficient, typename BTree<T>::Node* left_sibling, typename BTree<T>::Node* parent, int index)
{
	Stack<T> data_order;
	Stack<Node*> children_order;

	typename LinkedList<T>::Node* tracker_data = left_sibling->node_data.get_index(0);
	typename LinkedList<Node*>::Node* tracker_children = nullptr;

	//std::cout << "LEFT MERGE VIA PARENT " << parent->node_data.get_index(0)->get_data() << " AND SIBLING " << left_sibling->node_data.get_index(0)->get_data();

	T temp;
	Node* temp2;

	for (int i=0; i < left_sibling->node_data.length(); i++)
	{
		data_order.push(tracker_data->get_data());
		tracker_data = tracker_data->get_next();
	}

	data_order.push(parent->remove_from_node(index-1, this->min_node_data_length));

	if (deficient->node_data.length() != 0)
	{
		tracker_data = deficient->node_data.get_index(0);
		for (int i=0; i < deficient->node_data.length(); i++)
		{
			data_order.push(tracker_data->get_data());
			tracker_data = tracker_data->get_next();
		}
	}

	if (!left_sibling->is_leaf())
	{
		tracker_children = left_sibling->children.get_index(0);
		for (int i=0; i < left_sibling->children.length(); i++)
		{
			children_order.push(tracker_children->get_data());
			tracker_children = tracker_children->get_next();
		}
	}
	if (!deficient->is_leaf())
	{
		tracker_children = deficient->children.get_index(0);
		for (int i=0; i < deficient->children.length(); i++)
		{
			children_order.push(tracker_children->get_data());
			tracker_children = tracker_children->get_next();
		}
	}

	parent->children.remove_at(index);//deficient is deleted
	left_sibling->node_data.clear();
	left_sibling->children.clear();

	while (!data_order.is_empty())
	{
		temp = data_order.pop();
		left_sibling->insert_to_node(temp, this->max_node_data_length);
	}
	while (!children_order.is_empty())
	{
		temp2 = children_order.pop();
		left_sibling->children.insert_at(temp2, 0);
	}
}

template <class T>
void BTree<T>::merge_right(typename BTree<T>::Node* deficient, typename BTree<T>::Node* right_sibling, typename BTree<T>::Node* parent, int index)
{
	Stack<T> data_order;
	Stack<Node*> children_order;

	typename LinkedList<T>::Node* tracker_data = nullptr;
	typename LinkedList<Node*>::Node* tracker_children = nullptr;

	T temp;
	Node* temp2;

	//std::cout << "RIGHT MERGE VIA PARENT " << parent->node_data.get_index(0)->get_data() << " AND SIBLING " << right_sibling->node_data.get_index(0)->get_data();

	if (deficient->node_data.length() != 0)
	{
		tracker_data = deficient->node_data.get_index(0);
		for (int i=0; i < deficient->node_data.length(); i++)
		{
			data_order.push(tracker_data->get_data());
			tracker_data = tracker_data->get_next();
		}
	}

	data_order.push(parent->remove_from_node(index, this->min_node_data_length));

	tracker_data = right_sibling->node_data.get_index(0);
	for (int i=0; i < right_sibling->node_data.length(); i++)
	{
		data_order.push(tracker_data->get_data());
		tracker_data = tracker_data->get_next();
	}

	if (!deficient->is_leaf())
	{
		tracker_children = deficient->children.get_index(0);
		for (int i=0; i < deficient->children.length(); i++)
		{
			children_order.push(tracker_children->get_data());
			tracker_children = tracker_children->get_next();
		}
	}

	if (!right_sibling->is_leaf())
	{
		tracker_children = right_sibling->children.get_index(0);
		for (int i=0; i < right_sibling->children.length(); i++)
		{
			children_order.push(tracker_children->get_data());
			tracker_children = tracker_children->get_next();
		}
	}

	parent->children.remove_at(index);//deficient is deleted
	right_sibling->node_data.clear();
	right_sibling->children.clear();

	while (!data_order.is_empty())
	{
		temp = data_order.pop();
		right_sibling->insert_to_node(temp, this->max_node_data_length);
	}
	while (!children_order.is_empty())
	{
		temp2 = children_order.pop();
		right_sibling->children.insert_at(temp2, 0);
	}
}


template <class T>
void BTree<T>::insert(T data)
{
	//std::cout << "Inserting " << data << ":" << std::endl;
	if (this->is_empty())
	{
		//std::cout << "Create a root node and put " << data << " in it." << std::endl;
		this->root = new Node;
		this->root->insert_to_node(data, this->max_node_data_length);
		//std::cout << "Done!" << std::endl << std::endl;
		return;
	}

	int index = -1;
	Stack<Node*> path;
	path.push(nullptr);

	Node *tracker = this->place_to_insert(data, index, path);
	Node *temp, *temp2;

	if (tracker == nullptr)
	{
		//std::cout << "Element was already inserted!" << std::endl;
		//std::cout << "Done!" << std::endl << std::endl;
	}

	//std::cout << "It will be inserted to the node with given beginning data: " << tracker->node_data.get_index(0)->get_data() << std::endl;
	tracker->insert_to_node(data, this->max_node_data_length);

	while (path.top() != nullptr)
	{
		temp = path.pop();
		temp2 = path.top();

		if (temp->situation == Node::node_situation::overloaded)
			this->split(temp, temp2);
		else
			break;
	}
	//std::cout << "Done!" << std::endl << std::endl;
}

template <class T>
void BTree<T>::insert_multiple(const std::vector<T>& list)
{
	for (int i=0; i < list.size(); i++)
		this->insert(list[i]);
}

template <class T>
void BTree<T>::remove(T data)
{
	if (this->is_empty())
		return;

	////std::cout << "Removing " << data << " has started!" << std::endl;

	Stack<Node*> path;
	Stack<Node*> path_left;
	Stack<Node*> path_right;
	Stack<int> indices;

	path.push(nullptr);
	path.push(this->root);
	path_left.push(nullptr);
	path_right.push(nullptr);
	indices.push(-1);

	int index = -1;
	int found_index;

	Node *tracker = this->search_with_path_and_index(data, index, path, path_left, path_right, indices);

	if (tracker == nullptr)
		return;

	////std::cout << "Data is on " << tracker->node_data.get_index(0)->get_data() << std::endl;
	Node *temp = tracker, *temp2 = nullptr;

	found_index = index;
	////std::cout << found_index << std::endl;

	temp2 = this->pre_inorder_with_index(temp, index, path, path_left, path_right, indices);
	
	if (temp2 == nullptr)
		return;

	T successor = temp2->node_data.get_index(index)->get_data();
	////std::cout << "Its preorder successor is " << successor << std::endl;

	////std::cout << "Replacing data with preorder successor happens:" << std::endl;
	temp->remove_from_node(found_index, this->min_node_data_length);
	temp->insert_to_node(successor, this->max_node_data_length);
	temp2->remove_from_node(index, this->min_node_data_length);

	Node *temp_left = nullptr, *temp_right = nullptr;

	while (path.top() != nullptr)
	{
		temp = path.pop();
		temp2 = path.top();
		temp_left = path_left.pop();
		temp_right = path_right.pop();
		index = indices.pop();

		/*
		if (temp2 != nullptr)
			//std::cout << "CHEKING NODE WHOSE PARENT IS " << temp2->node_data.get_index(0)->get_data();
		else
			//std::cout << "CHEKING NODE WHOSE PARENT IS NULLPTR";
		//std::cout << ", WHOSE LEFT SIBLING IS ";
		if (temp_left != nullptr)
			//std::cout << temp_left->node_data.get_index(0)->get_data();
		else
			//std::cout << "NULLPTR";
		//std::cout << ", WHOSE RIGHT SIBLING IS ";
		if (temp_right != nullptr)
			//std::cout << temp_right->node_data.get_index(0)->get_data();
		else
			//std::cout << "NULLPTR";
		//std::cout << std::endl;
		*/

		if (temp->situation == Node::node_situation::empty)
		{
			if (can_borrow(temp, temp_right))
			{
				this->borrow_from_right(temp, temp_right, temp2, index);
				break;
			}
			else if (can_borrow(temp, temp_left))
			{
				this->borrow_from_left(temp, temp_left, temp2, index);
				break;
			}
			else if (temp_right != nullptr)
				this->merge_right(temp, temp_right, temp2, index);
			else if (temp_left != nullptr)
				this->merge_left(temp, temp_left, temp2, index);
			else
			{
				temp = this->root;
				this->root = this->root->children.get_index(0)->get_data();
				delete temp;
				return;
			}
		}
		else
			break;
	}
}

template <class T>
void BTree<T>::remove_multiple(const std::vector<T>& list)
{
	for (int i=0; i < list.size(); i++)
		this->remove(list[i]);
}

template <class T>
void BTree<T>::clear()
{
	if (this->is_empty())
		return;

	Queue<Node*> remover;
	Node* temp = nullptr;
	typename LinkedList<Node*>::Node* children_tracker = nullptr;
	remover.enqueue(this->root);

	while (!remover.is_empty())
	{
		temp = remover.dequeue();
		if (!temp->is_leaf())
		{
			children_tracker = temp->children.get_index(0);
			for (int i=0; i < temp->children.length(); i++)
			{
				remover.enqueue(children_tracker->get_data());
				children_tracker = children_tracker->get_next();
			}
		}
		delete temp;
	}
	this->root = nullptr;
}

template <class T>
typename BTree<T>::Node* BTree<T>::search(T data) const
{
	if (this->is_empty())
		return nullptr;

	Node* tracker = this->root;
	typename LinkedList<T>::Node* tracker_node_data = nullptr;

	while (tracker != nullptr)
	{
		tracker_node_data = tracker->node_data.get_index(0);
		if (!tracker->is_leaf())
		{
			for (int i=0; i < tracker->node_data.length(); i++)
			{
				if (data < tracker_node_data->get_data())
				{
					tracker = tracker->children.get_index(i)->get_data();
					break;
				}
				else if (data == tracker_node_data->get_data())
					return tracker;

				tracker_node_data = tracker_node_data->get_next();
			}
			tracker = tracker->children.get_index(tracker->children.length()-1)->get_data();
		}
		else
		{
			for (int i=0; i < tracker->node_data.length(); i++)
			{
				if (data > tracker_node_data->get_data())
					return nullptr;
				else if (data == tracker_node_data->get_data())
					return tracker;

				tracker_node_data = tracker_node_data->get_next();
			}
			return nullptr;
		}
	}
	return nullptr;
}

template <class T>
void BTree<T>::inorder_display() const
{
	inorder_display(this->root);
	std::cout << std::endl;
}

template <class T>
void BTree<T>::levelorder_display() const
{
	if (this->root == nullptr)
	{
		//std::cout << "Empty" << std::endl;
		std::cout << std::endl;
		return;
	}

	Queue<Node*> level;
	Queue<Node*> parent;
	Node* temp = nullptr, *temp2 = nullptr;

	typename LinkedList<T>::Node* data_tracker = nullptr;
	typename LinkedList<Node*>::Node* children_tracker = nullptr;

	level.enqueue(this->root);
	parent.enqueue(nullptr);

	int level_length;
	std::cout << std::endl << "Root: ";

	while (!level.is_empty())
	{
		level_length = level.length();

		for (int i=0; i < level_length; i++)
		{
			temp = level.dequeue();
			temp2 = parent.dequeue();

			if (temp2 != nullptr)
				std::cout << temp2->node_data.get_index(0)->get_data() << ": ";

			data_tracker = temp->node_data.get_index(0);
			std::cout << "(";

			for (int i=0; i < temp->node_data.length()-1; i++)
			{
				std::cout << data_tracker->get_data() << ",";
				data_tracker = data_tracker->get_next();
			}
			std::cout << data_tracker->get_data() << ")    ";

			if (!temp->is_leaf())
			{
				children_tracker = temp->children.get_index(0);
				for (int i=0; i < temp->children.length(); i++)
				{
					level.enqueue(children_tracker->get_data());
					children_tracker = children_tracker->get_next();

					if (i == 0)
						parent.enqueue(temp);
					else
						parent.enqueue(nullptr);
				}
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}

template <class T>
void BTree<T>::inorder_display(Node* node) const
{
	if (node == nullptr)
	{
		std::cout << std::endl;
		return;
	}

	typename LinkedList<T>::Node* tracker_data = node->node_data.get_index(0);
	typename LinkedList<Node*>::Node* tracker_children = nullptr;

	if (node->is_leaf())
	{
		for (int i=0; i < node->node_data.length(); i++)
		{
			std::cout << tracker_data->get_data() << " ";
			tracker_data = tracker_data->get_next();
		}
	}
	else
	{
		tracker_children = node->children.get_index(0);
		for (int i=0; i < node->node_data.length(); i++)
		{
			inorder_display(tracker_children->get_data());
			std::cout << tracker_data->get_data() << " ";
			tracker_data = tracker_data->get_next();
			tracker_children = tracker_children->get_next();
		}
		inorder_display(tracker_children->get_data());
	}
}

template <class T>
BTree<T>& BTree<T>::operator=(const BTree& rhs)
{
	this->clear();
	if (rhs.root == nullptr)
		return *this;

	*this = BTree<T>(rhs.max_node_degree);
	if (rhs.root != nullptr)
	{
		this->root = new Node;
		rec_create(this->root, rhs.root);
	}

	return *this;
}

template <class T>
void BTree<T>::copy_to(BTree& rhs)
{
	Queue<T> data_list;
	create_data_list(this->root, data_list);

	rhs.clear();

	while (! data_list.is_empty())
	{
		T temp = data_list.dequeue();
		rhs.insert(temp);
	}
}

template <class T>
void BTree<T>::create_data_list(Node* node, Queue<T>& data_list)
{
	if (node == nullptr || node->node_data.length() == 0)
		return;

	typename LinkedList<T>::Node* tracker = node->node_data.get_index(0);

	int len = node->node_data.length();
	for (int i=0; i < len; i++)
	{
		data_list.enqueue(tracker->get_data());
		tracker = tracker->get_next();
	}

	if (node->children.length() == 0)
		return;

	typename LinkedList<Node*>::Node* tracker_children = node->children.get_index(0);
	len = node->children.length();

	for (int i=0; i < len; i++)
	{
		create_data_list(tracker_children->get_data(), data_list);
		tracker_children = tracker_children->get_next();
	}
}

template <class T>
bool BTree<T>::is_empty() const
{
	return this->root == nullptr;
}

template <class T>
void BTree<T>::rec_create(Node* to, Node* from)
{
	if (from != nullptr)
	{
		to->node_data = from->node_data;

		LinkedList<Node*> & from_child_ref = from->children;
		int len = from_child_ref.length();

		if (len == 0)
			return;

		Stack<Node*> nodes_endtobegin;
		typename LinkedList<Node*>::Node* tracker = from_child_ref.get_index(0);

		for (int i=0; i < len; i++)
		{
			nodes_endtobegin.push(tracker->get_data());
			tracker = tracker->get_next();
		}

		for (int i=0; i < len; i++)
		{
			Node* temp = nodes_endtobegin.pop();
			Node* created = new Node;
			to->children.insert_at(created, 0);
			rec_create(created, temp);
		}
	}
}
//Tree functions end


#endif