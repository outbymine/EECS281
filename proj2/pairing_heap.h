#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H
#include<iostream>  //the the error detection
#include<vector>
using std::vector;

#include "eecs281heap.h"
// Using the method in Weiss's book as a reference for the structure of the pairing heap

//A specialized version of the 'heap' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP = std::less<TYPE> >
class pairing_heap : public eecs281heap<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  pairing_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  pairing_heap(COMP comp = COMP());
  
  //Description: Copy constructor.
  //Runtime: O(n)
  pairing_heap(const pairing_heap& other);
  
  //Description: Copy assignment operator.
  //Runtime: O(n)
  pairing_heap& operator=(const pairing_heap& rhs);
  
  //Description: Destructor
  //Runtime: O(n)
  ~pairing_heap();

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n)
  virtual void make_heap();

  //Description: Add a new element to the heap. This has been provided for you,
  //             in that you should implement push functionality in the
  //             add_node function.
  //Runtime: Amortized O(1)
  virtual void push(const TYPE& val) { add_node(val); }

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap.
  //Runtime: O(1)
  virtual size_type size() const { return heapSize; }

  //Description: Return true if the heap is empty.
  //Runtime: O(1)
  virtual bool empty() const { return (root==NULL); }

  class Node {
    //This node class will allow external users to update the priority of
    //elements that are already inside the heap.
  public:
    //***Add any constructors you need here.
    Node(const TYPE& x)
    {
    	elt=x;
	prev=NULL;
	right=NULL;
	child=NULL;
    }
  public:
    //Description: Allows access to the element at that Node's position.
    //Runtime: O(1) - this has been provided for you.
    const TYPE& operator*() const { return elt; }

    //The following line allows you to access any private data members of this
    //Node class from within the pairing_heap class. (ie: myNode.elt is a legal
    //statement in pairing_heap's add_node() function).
    friend pairing_heap;
  private:
    TYPE elt;
  private:
    //***Add any additional member functions or data you require here.
    Node *prev; // if the node is a leftmost one this denotes its parent, otherwise it 
	       // refers to its left sibling
    Node *right; //the right sibling
    Node *child; //the child should be on its left

  };


  //Description: Updates the priority of an element already in the heap by
  //             replacing the element refered to by the Node with new_value.
  //             Must maintain heap invariants.
  //
  //PRECONDITION: The new priority, given by 'new_value' must be more extreme
  //              (as defined by comp) than the old priority.
  //
  //Runtime: As discussed in reading material.
  void updateElt(Node* node, TYPE new_value);

  //Description: Add a new element to the heap. Returns a Node* corresponding
  //             to the newly added element.
  //Runtime: Amortized O(1)
  Node* add_node(const TYPE& val);

private:
  //***Add any additional member functions or data you require here.
  //***We recommend creating a 'meld' function (see the reading).
  Node *root;
  size_type heapSize; // the size of the pairing heap
  Node* meldChildren(Node *firstSibling)
  {
	if(firstSibling->right==NULL)// if there is only one child heap after delete the root
		return firstSibling;
	static vector<Node*> childTree(10);
	//store the child heap into this buffer;
	unsigned int numSiblings=0;
	while(firstSibling !=NULL)
	{
		if(numSiblings==childTree.size())
			childTree.resize(numSiblings*2);
		childTree[numSiblings]=firstSibling;
		firstSibling->prev->right=NULL;
		firstSibling=firstSibling->right;
		numSiblings++;
	}
	if(numSiblings==childTree.size())
		childTree.resize(numSiblings+1);
	childTree[numSiblings]=NULL;
	unsigned int i=0;
	for(;i+1<numSiblings;i+=2)
		link(childTree[i],childTree[i+1]);
	unsigned int j=i-2;
	if(j==numSiblings-3)
		link(childTree[j], childTree[j+2]);
	while(j>=2)
	{
		link(childTree[j-2], childTree[j]);
		j-=2;
	}
	return childTree[0];
  }
  void link(Node * &first, Node *second);
  void makeEmpty(); 
  Node *replicate(Node *node)
  {
	if(node==NULL)
		return NULL;
	else
	{
		Node *t=new Node(node->elt);
		if((t->child=replicate(node->child))!=NULL)// go on to clone the subtree
			t->child->prev=t;
		if((t->right=replicate(node->right))!=NULL)
			t->right->prev=t;
		return t;	
	}
  }

};



template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::makeEmpty()
{
	while(!empty())
	{
		pop();
	}
}


template<typename TYPE, typename COMP>
template<typename InputIterator>
pairing_heap<TYPE, COMP>::pairing_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //Your code.
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(COMP comp) {
  //Your code.
  root=NULL;
  heapSize=0;
}

template<typename TYPE, typename COMP> //copy constructor
pairing_heap<TYPE, COMP>::pairing_heap(const pairing_heap& other) {
  //Your code.
  root=NULL;
  *this=other;
}

template<typename TYPE, typename COMP> //assignment operator
pairing_heap<TYPE, COMP>&
pairing_heap<TYPE, COMP>::operator=(const pairing_heap& rhs) {
  //Your code.
  if(this !=&rhs)
  {
	makeEmpty();
	root=replicate(rhs.root);
  }
  return *this;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::~pairing_heap() {
  //Your code.
  makeEmpty();
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::make_heap() {
  //Your code.
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::pop() {
  //Your code.
  if(empty())
   	 std::cerr<<"cannot pop from a empty heap, error";
  Node *oldRoot=root;
  if(root->child==NULL)
    	root=NULL;
  else
    	root=meldChildren(root->child);
  heapSize--;
  delete oldRoot;
}

template<typename TYPE, typename COMP>
const TYPE& pairing_heap<TYPE, COMP>::top() const {
  //Your code.
  return root->elt; //This line present only so that this provided file compiles.
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::add_node(const TYPE& val) {
  //Your code.
  //return to position containing the newly inserted Node
  Node *newNode=new Node(val);
  if(root==NULL)
	root=newNode;
  else
	link(root, newNode);
  heapSize++;
  return newNode;
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::updateElt(Node* node, TYPE new_value) {
  //Your code.
  if(this->compare(node->elt, new_value))
       	std::cerr<<" the new element should be more extreme"; 
  node->elt=new_value;
  if(node!=root)
  {
  	if(node->right!=NULL)
		node->right->prev=node->prev;
	if(node->prev->child==node)
		node->prev->child=node->right;
	else
		node->prev->right=node->right;
	node->right=NULL;
	link(root, node);		
  }
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::link(Node* & first, Node *second)
{

    if (second == NULL)
        return;
    if (this->compare(second->elt,first->elt))
    {
        second->prev = first->prev;
        first->prev = second;
        first->right = second->child;
        if (first->right != NULL)
            first->right->prev = first;
        second->child = first;
        first = second;
    }
    else
    {
        second->prev = first;
        first->right = second->right;
        if (first->right != NULL)
            first->right->prev = first;
        second->right = first->child;
        if (second->right != NULL)
            second->right->prev = second;
        first->child = second;
    }
}


/*template<typename TYPE, typename COMP>
Node* pairing_heap<TYPE, COMP>::meldChildren(Node *firstSibling)
{
	if(firstSibling->right==NULL)// if there is only one child heap after delete the root
		return firstSibling;
	static vector<Node*> childTree(10);
	//store the child heap into this buffer;
	unsigned int numSiblings=0;
	while(firstSibling !=NULL)
	{
		if(numSiblings==childTree.size())
			childTree.resize(numSiblings*2);
		childTree[numSiblings]=firstSibling;
		firstSibling->prev->right=NULL;
		firstSibling=firstSibling->right;
		numSiblings++;
	}
	if(numSiblings==childTree.size())
		childTree.resize(numSiblings+1);
	childTree[numSiblings]=NULL;
	for(unsigned int i=0;i+1<numSiblings;i+=2)
		link(childTree[i],childTree[i+1]);
	unsigned int j=i-2;
	if(j=numSiblings-3)
		link(childTree[j], childTree[j+2]);
	while(j>=2)
	{
		link(childTree[j-2], childTree[j]);
		j-=2;
	}
	return childTree[0];
}
*/


#endif //PAIRING_HEAP_H

