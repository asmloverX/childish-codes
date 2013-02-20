// stack.h
//	Data structures for a "stack" -- a Last-In-First-Out list of integers.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


// The following defines the Stack class.  The functions are
// implemented in the file stack.cc.
//
// The constructor (initializer) for the Stack is passed the number
// of elements (integers) in the stack.

class Stack {
  public:
    Stack(int sz);    // Constructor:  initialize variables, allocate space.
    ~Stack();         // Destructor:   deallocate space allocated above.
    
    void Push(int value); // Push an integer on the stack, checking for overflow
    int Pop();        // Pop an integer off the stack, checking for underflow.
    
    int Full();       // Returns non-0 if the stack is full, 0 otherwise.
    int Empty();      // Returns non-0 if the stack is empty, 0 otherwise.
    
  private:
    int size;         // The maximum capacity of the stack.
    int top;          // Index of the next position to be used.
    int *stack;       // A pointer to an array that holds the contents.
};
