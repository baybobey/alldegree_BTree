# alldegree_BTree
A flexible B-Tree data structure for any possible given degree. (C++)

### Brief Introduction
A linkedlist data structure is used for both stored data and children data in a single node of a tree. Although not included directly in the structure of the B-Tree, stack and queue are used for some function implementations.

### HOW TO USE
First, you need to include the files in the beginning of your C++ code as: #include "BTree.hpp".
You can define your object in your file as: BTree<type> name_of_obj(degree) (example: BTree<int> my_tree(4)).
  
<b>NOTE:</b> Default degree is 3, which is the minimum degree possible. If you try to enter smaller degree than 3 for your BTree, it throws an error message.

<b>NOTE:</b> By degree of your B-Tree, you are actually determining:
1. The maximum number of elements a node in your tree can hold (= degree).
2. The minimum number of elements a node in your tree can hold (= floor(degree/2)) (no restrictions for root node).
3. The number of children a node can have (max: degree+1, always #of_elements+1 if not a leaf node).

#### Object Functions
Let's say you defined your object as: "BTree<int> my_tree;".

..*void insert(T data)
Use this function to add an element to your B-Tree. (e.g. my_tree.insert(3) adds 3 to your structure).

..*void insert_multiple(const std::vector<T>& list)
As its name tells, you can insert multiple elements to your B-Tree object by using this function. (e.g. my_tree.insert_multiple({10,9,2,-4}) adds 10,9,2 and -4 in given order).
