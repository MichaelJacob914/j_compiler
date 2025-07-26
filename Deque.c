/*
 * Copyright ©2024 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIS 2400 for use solely during Fall Semester 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <stdlib.h>

#include "Deque.h"
///////////////////////////////////////////////////////////////////////////////
// Deque implementation.
// A single node within a deque.
//
// A node contains next and prev pointers as well as a ctomer-supplied
// payload.
/**/

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// "Methods" for our Deque implementation.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Allocates and returns a pointer to a new Deque.
// It is the Caller's responsibility to at some point call Deque_Free to free
// the associated memory.
//
// Arguments: none.
//
// Returns:
// - the newly-allocated linked list, or NULL on error.
Deque* Deque_Allocate(void){
  Deque* node; 
  node = malloc(sizeof(Deque)); 
  if(node == NULL){
    return NULL; 
  }
  node->num_elements = 0; 
  node->front = NULL;
  node->back = NULL; 
  return node; 
}
// Free a Deque that was previously allocated by Deque_Allocate.
//
// Arguments:
// - dq: the deque to free.  It is unsafe to use "dq" after this
//   function returns.
void Deque_Free(Deque *dq){
  DequeNode* now = dq->front;
  DequeNode* temp = NULL;

  while (now != NULL) {
    temp = now->next;
    free(now);
    now = temp;
  }
  free(dq); 
  return; 
}
// Return the number of elements in the deque.
//
// Arguments:
// - dq:  the deque to query.
//
// Returns:
// - deque size.
int Deque_Size(Deque *dq){
  int size = dq->num_elements; 
  return size;  
}

// Adds a new element to the front of the Deque.
//
// Arguments:
// - dq: the Deque to push onto.
// - payload: the payload to push to the front
void Deque_Push_Front(Deque *dq, int payload){
  if(dq->front == NULL){
    dq->num_elements = 1; 
    DequeNode* new = (DequeNode*) malloc(sizeof(DequeNode));
    new->payload = payload; 
    new->next = NULL;
    new->prev = NULL;
    dq->front = new; 
    dq->back = new; 
  }
  else{
    DequeNode* new = (DequeNode*) malloc(sizeof(DequeNode));
    new->payload = payload; 
    new->next = dq->front; 
    new->prev = NULL; 
    dq->front->prev = new; 
    dq->front = new; 
    int size = dq->num_elements; 
    dq->num_elements = size + 1; 

  }
}

// Pop an element from the front of the deque.
//
// Arguments:
// - dq: the Deque to pop from.
// - payload_ptr: a return parameter; on success, the popped node's payload
//   is returned through this parameter.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
bool Deque_Pop_Front(Deque *dq, int *payload_ptr){
  if(dq->front == NULL){
    return false; 
  }
  *payload_ptr = dq->front->payload; 
  if(dq->num_elements == 1){
    DequeNode* temp = dq->front; 
    dq->front = NULL;
    dq->back = NULL; 
    dq->num_elements = 0; 
    free(temp); 
    return true; 
  }
  DequeNode* temp = dq->front; 
  dq->front->next->prev = NULL; 
  dq->front = dq->front->next; 
  int size = dq->num_elements; 
  dq->num_elements = size - 1; 
  free(temp); 
  return true; 
}
// Peeks at the element at the front of the deque.
//
// Arguments:
// - dq: the Deque to peek.
// - payload_ptr: a return parameter; on success, the peeked node's payload
//   is returned through this parameter.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
bool Deque_Peek_Front(Deque *dq, int *payload_ptr){
  if(dq->front == NULL){
    return false; 
  }
  *payload_ptr = dq->front->payload; 
  return true; 
}

// Pushes a new element to the end of the deque.
//
// This is the "end" version of Deque_Push_Front.
//
// Arguments:
// - dq: the Deque to push onto.
// - payload: the payload to push to the end
void Deque_Push_Back(Deque *dq, int payload){
  if(dq->front == NULL){
    dq->num_elements = 1; 
    DequeNode* new = (DequeNode*) malloc(sizeof(DequeNode));
    new->payload = payload; 
    new->next = NULL;
    new->prev = NULL;
    dq->front = new; 
    dq->back = new; 
  }
  else{
    DequeNode* new = (DequeNode*) malloc(sizeof(DequeNode));
    new->payload = payload; 
    new->prev = dq->back; 
    new->next = NULL; 
    dq->back->next = new; 
    dq->back = new; 
    int size = dq->num_elements; 
    dq->num_elements = size + 1; 

  }
}

// Pops an element from the end of the deque.
//
// This is the "end" version of Deque_Pop_Front.
//
// Arguments:
// - dq: the Deque to remove from
// - payload_ptr: a return parameter; on success, the popped node's payload
//   is returned through this parameter.
//
// Returns:
// - false: on failure (eg, the deque is empty).
// - true: on success.
bool Deque_Pop_Back(Deque *dq, int* payload_ptr){
  if(dq->front == NULL){
    return false; 
  }
  *payload_ptr = dq->back->payload; 
  if(dq->num_elements == 1){
    DequeNode* temp = dq->front; 
    dq->front = NULL;
    dq->back = NULL; 
    dq->num_elements = 0; 
    free(temp); 
    return true; 
  }
  DequeNode* temp = dq->back; 
  dq->back->prev->next = NULL; 
  dq->back = dq->back->prev; 
  int size = dq->num_elements; 
  dq->num_elements = size - 1; 
  free(temp); 
  return true; 
}

// Peeks at the element at the back of the deque.
//
// Arguments:
// - dq: the Deque to peek.
// - payload_ptr: a return parameter; on success, the peeked node's payload
//   is returned through this parameter.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
bool Deque_Peek_Back(Deque *dq, int *payload_ptr){
  if(dq->front == NULL){
    return false; 
  }
  *payload_ptr = dq->back->payload; 
  return true; 
}

