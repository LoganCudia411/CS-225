/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0; 
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(NULL);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
 while(head_ !=NULL){
  ListNode* temp = head_; 
  head_ = head_->next; 
  delete temp; 
 }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(const T & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode* newNode = new ListNode(ndata);
    if(head_!= NULL){
      newNode->next = head_; 
      newNode->prev = NULL; 
      head_->prev = newNode; 
      head_ = newNode; 
    }
    else{
      newNode->next = NULL; 
      newNode->prev = NULL; 
      head_ = newNode;
      tail_ = newNode; 
    }

  length_++;


}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
    ListNode* newNode = new ListNode(ndata);
    if(tail_ == NULL){
      head_ = newNode; 
      tail_ = newNode; 
      newNode ->next = NULL; 
      newNode ->prev = NULL; 
    }
    else{
      newNode->next = NULL; 
      newNode->prev = tail_; 
      tail_->next = newNode; 
      tail_ = newNode; 
    }
    length_++;
}



/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  if(splitPoint < 1 || splitPoint > length_ || start == nullptr){
    return start;
  }

  int i = 0; 
  while (i < splitPoint) {
    start = start->next;
    i++; 
  }

    if(start != nullptr){
      start->prev->next = NULL;
      start->prev = NULL;
      tail_ = start->prev; 
      return start; 
    }

    return nullptr; 


  return start;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in part 1
  ListNode* curr = head_->next; 
  ListNode* last = tail_; 
  ListNode* prev_curr = nullptr; //previous node from before current
  ListNode* next_curr = nullptr;  //next node from current 
  while(curr != tail_){
    prev_curr = curr->prev; 
    next_curr = curr->next;
    tail_ = curr; //tail is pointing to curr since curr will be at the end 
    curr->prev = last; 
    last->next = curr;
    prev_curr->next = next_curr; 
    next_curr->prev = prev_curr;
    last = curr; //since curr was just moved to the tail 
    tail_->next = nullptr; 
    curr = next_curr->next; 


  }
  
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
    //check for if list is empty 
    if(startPoint == endPoint || startPoint == NULL || endPoint == NULL ){
      return; 
    }
        ListNode * curr = startPoint; 
        ListNode * startPrev = startPoint->prev; 
        ListNode * endNext = endPoint->next; 

    while(curr != endNext){
        //swap each nodes next and prev pointer to reverse it! 
        ListNode * temp = curr->prev; 
        curr->prev = curr->next; 
        curr->next = temp; 
        //iterate by going right which we do by using prev now instead of next 
        curr = curr->prev; 
    }

    ListNode* base_start = startPoint; 
    ListNode * base_end = endPoint; 
    //switchng start and end pointers to account for sublist reverses
    base_end->prev = startPrev; 
    base_start->next = endNext; 
   
    //cases for reversing sublist 
    if(endNext != NULL){
      endNext->prev = base_start; // change the node after the end's prev pointer to point to the start which is the new END of the reverse
    }
    if(startPrev != NULL){
      startPrev->next = base_end; //change the node before our start node's next pointer to point to the end which is the START of the reverse
    }
     
    startPoint = base_end; 
    endPoint = base_start;

    if(tail_ == base_end){
      tail_ = base_start; 
    }
     if(head_ == base_start){
      head_ = base_end; 
    }
}
/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in mp_lists part 2
  if(n<=1){
    return; 
  }
  if(n>length_){
    reverse(head_,tail_);
    return;
  }

  int nodes = length_/n;
  ListNode * stopPoint = head_; 
  ListNode * startPoint = head_; 
  for(int i = 0; i<nodes; i++){
    for(int j = 0; j<n-1; j++){
      stopPoint = stopPoint->next; 
    } 
    reverse(startPoint, stopPoint); 
    startPoint = stopPoint->next; 
    stopPoint = stopPoint->next;
  }

  stopPoint = tail_;
  reverse(startPoint, stopPoint);
  
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in mp_lists part 2
  
  //cases for if either or both Lists are empty 
  if(first == NULL){
    return second; 
  }
  if(second == NULL){
    return first; 
  }
  if(second == NULL && first == NULL){
    return NULL; 
  }

  //get the smallest node between both lists and start here 
  ListNode* start = NULL; 
  if(first->data < second->data){ //smaller value is the start 
    start = first; 
    first = first->next; 
  }else{
    start = second; 
    second = second->next; 
  }
  ListNode* curr = start; //curr is the smallest val out of the two lists currently
  while(first !=  NULL && second != NULL){ //go until one list is empty 
    if(first->data < second->data){ 
      //insert in front of first 
      curr->next = first; 
      first->prev = curr;  
      curr = first; //increment curr
      first = first->next; //increment first 
    }else{ //second's val is smaller so we put it after our start
      curr->next = second; 
      second->prev = curr; 
      curr = second; 
      second = second->next; 
    }
  }
  //if first list is empty add the rest to the merge list 
  if(first == NULL && second != NULL ){
      curr->next = second; 
      second->prev = curr; 
  }
  //if second list is empty add the rest to the merge list 
   if(first != NULL && second == NULL ){
      curr->next = first; 
      first->prev = curr; 
  }

  return start;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if(chainLength == 1 || chainLength == 0){
    return start; 
  }
  ListNode * middle = split(start, chainLength/2); //pointer to middile of list 
  ListNode * left = mergesort(start, chainLength/2);
  ListNode * right = mergesort(middle, chainLength - chainLength/2);
  return merge(left, right);


}
