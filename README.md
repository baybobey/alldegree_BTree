# All Degree B-Tree
A flexible B-Tree data structure for any possible given degree. (C++) 

### Brief Introduction
A linkedlist data structure is used for both stored data and children data in a single node of a tree. Although not included directly in the structure of the B-Tree, stack and queue are used for some function implementations.

### HOW TO USE
First, download the hpp files in the same file location with your cpp file you want to use B-Tree structure in. Then you need to include the files in the beginning of your C++ code as:
>**#include "BTree.hpp"**

You can define your object in your file as:
>**BTree\<type\> name_of_obj(degree)**

Example:
```
BTree<int> my_tree(4);
```
**NOTE:** Default degree is 3, which is the minimum degree possible. If you try to enter smaller degree than 3 for your BTree, it throws an error message.

**NOTE:** By degree of your B-Tree, you are actually determining:
1. The maximum number of elements a node in your tree can hold (= degree).
2. The minimum number of elements a node in your tree can hold (= floor(degree/2)) (no restrictions for root node).
3. The number of children a node can have (max: degree+1, always #of_elements+1 if not a leaf node).

## Object Functions
Let's say you defined your object as:
```
BTree<int> my_tree;
```
#### Data Addition-Removal
- ##### void insert(T data)

Use this function to add an element to your B-Tree.
```
my_tree.insert(3); //inserts 3 to my_tree object (if data already exists, insertion does nothing)
```

- ##### void insert_multiple(const std::vector\<T\>& list)

As its name tells, you can insert multiple elements to your B-Tree object by using this function. 
```
my_tree.insert_multiple({10,9,2,-4}); //(adds 10,9,2 and -4 in given order)
```

- ##### void remove(T data)

Use this function to remove an element from your B-Tree. If data does not exist in the structure, then nothing happens.
```
my_tree.remove(3); //removes 3 if exists
```
- ##### void remove_multiple(const std::vector\<T\>& list)

Use this function for removing multiple element from your B-Tree. Removes the elements in given order. Does nothing for elements that does not exist in B-Tree.

```
my_tree.remove_multiple({10,9,2,-4}); //removes 10,9,2 and -4 in given order if they exist
```
  
- ##### void clear()

Clears the whole B-Tree object (removes all the data in B-Tree).
```
my_tree.clear(); //removes whole data in my_tree object
```

#### Data Search
- ##### Node* search(T data)

Searches the tree for given data. If exists, it returns pointer to the Node that the element is stored.

```
my_tree.search(3); //returns pointer to node storing 3 (nullptr if it doesn't exist)
```

#### Capacity Checks
- ##### bool is_empty()

Returns boolean depending on object's emptiness.
```
my_tree.is_empty(); //returns true if my_tree object has no elements
```
- ##### bool is_full()

Returns boolean depending on heap memory used.
```
my_tree.is_full(); //returns true if new node for tree cannot be created
```

##### Tree Displays
- ##### void inorder_display()

Prints the elements in the B-Tree object inorder.
```
my_tree.inorder_display(); //inorder tree display (let's say smallest 4 elements are 1,3,8 and 9)
//OUT: 1 3 8 19 (...)
```

- ##### void levelorder_display()

Prints the elements in the B-Tree object levelorder. One can see node hierarchy by using this function.
```
my_tree.levelorder_display(); //levelorder tree display
```

##### Transfer Operations
- ##### BTree\<T\>& operator=(const BTree\<T\>& rhs)

Lhs object becomes the deep copy of rhs (including degrees).
```
BTree<int> my_tree2(5); //declaration of another B-Tree object with degree 5
my_tree2 = my_tree; //my_tree2 becomes the deep copy of my_tree (meaning its degree also changed)
```

- ##### void copy_to(BTree\<T\>& rhs)

Like assignment operator but degrees remain unchanged.
```
BTree<int> my_tree2(5); //declaration of another B-Tree object with degree 5
my_tree.copy_to(my_tree2); //elements of my_tree are transfered to my_tree2
```
**NOTE:** The object that the datas are transferred to is cleared beforehand whenever copy_to function is called!!!
