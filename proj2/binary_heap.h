#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H
#include<iostream> //test output

#include "eecs281heap.h"


//A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class binary_heap : public eecs281heap<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  binary_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  binary_heap(COMP comp = COMP());

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n)
  virtual void make_heap();

  //Description: Add a new element to the heap.
  //Runtime: O(log(n))
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap.
  //Runtime: O(1)
  virtual size_type size() const
    { /*** Fill this in - might be very simple depending on implementation ***/ 
		return currentSize;// notice this is different from data.size();
  }

  //Description: Return true if the heap is empty.
  //Runtime: O(1)
  virtual bool empty() const
    { /*** Fill this in - might be very simple depending on implementation ***/ 
		return (currentSize==0);
  }
private:
  //Note: This vector *must* be used your heap implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
	int currentSize;
	void filterDown(int hole);
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
binary_heap<TYPE, COMP>::binary_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //Your code.
	InputIterator itr;
	data.resize(1);
	for(itr=start;itr<end;itr++)
		data.push_back(*itr);
	currentSize=data.size()+10;
	make_heap();
}

template<typename TYPE, typename COMP>
binary_heap<TYPE, COMP>::binary_heap(COMP comp) {
  //Your code.
	data.resize(10);
	currentSize=0;
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::make_heap() {
  //Your code.
	for(int i=currentSize/2;i>0;i--)
		filterDown(i);
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::push(const TYPE& val) {
  //Your code.
	if(currentSize==int(data.size())-1)
		data.resize(data.size()*2);
	currentSize++;
	int hole=currentSize;
	while(hole>1)
	{
		if(this->compare(val,data[hole/2]))
			data[hole]=data[hole/2];
		else
			break;
		hole=hole/2;
	}
	data[hole]=val;
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::pop() {
  //Your code.
	data[1]=data[currentSize];
	currentSize--;
	filterDown(1);
}

template<typename TYPE, typename COMP>
const TYPE& binary_heap<TYPE, COMP>::top() const {
  //Your code.
  return data[1]; //This line present only so that this provided file compiles.
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::filterDown(int hole)
{
	int child;
	TYPE tmp=data[hole];
	while(hole*2<=currentSize)
	{
		child=hole*2;
		if(child!=currentSize && this->compare(data[child+1],data[child]))
			child++;
		if(this->compare(data[child],tmp))
			data[hole]=data[child];
		else
			break;
		hole=child;
	}
	data[hole]=tmp;
}

#endif //BINARY_HEAP_H

