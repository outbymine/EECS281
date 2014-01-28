#ifndef SORTED_HEAP_H
#define SORTED_HEAP_H

#include "eecs281heap.h"

//A specialized version of the 'heap' ADT that is implemented with an
//underlying sorted array-based container.
//Note: The most extreme element should be found at the end of the
//'data' container, such that traversing the iterators yields the elements in
//sorted order.
template<typename TYPE, typename COMP = std::less<TYPE>>
class sorted_heap : public eecs281heap<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n log n) where n is number of elements in range.
  template<typename InputIterator>
  sorted_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  sorted_heap(COMP comp = COMP());

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n log n)
  virtual void make_heap();

  //Description: Add a new element to the heap.
  //Runtime: O(n)
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(1)
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap. This has been
  //             implemented for you.
  //Runtime: O(1)
  virtual size_type size() const { return data.size(); }

  //Description: Return true if the heap is empty. This has been implemented
  //             for you.
  //Runtime: O(1)
  virtual bool empty() const { return data.empty(); }
private:
  //Note: This vector *must* be used your heap implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
sorted_heap<TYPE, COMP>::sorted_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //Your code.
}

template<typename TYPE, typename COMP>
sorted_heap<TYPE, COMP>::sorted_heap(COMP comp) {
  //Your code.
}

template<typename TYPE, typename COMP>
void sorted_heap<TYPE, COMP>::make_heap() {
  //Your code.
}

template<typename TYPE, typename COMP>
void sorted_heap<TYPE, COMP>::push(const TYPE& val) {
  //Your code.
	int v_size=data.size();
	typename std::vector<TYPE>::iterator itr=data.begin();
	for(int i=0;i<v_size;i++,itr++)
	{
		if(!this->compare(data[i],val))
			break;
	}
	data.insert(itr,val);
}

template<typename TYPE, typename COMP>
void sorted_heap<TYPE, COMP>::pop() {
  //Your code.
  data.erase(data.begin());
}

template<typename TYPE, typename COMP>
const TYPE& sorted_heap<TYPE, COMP>::top() const {
  //Your code.
  return data.front(); //So that this sample implementation compiles.
}

#endif //SORTED_HEAP_H
