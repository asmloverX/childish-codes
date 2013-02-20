// stack.cc
//	Routines to implement a LIFO stack of integers.
//	
//	The stack is represented as an array; we simplify return an error
//	if the caller tries to push more things onto the stack than we have
//	room for.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

extern "C" {
#include <stdio.h>
extern int exit(int st);
}

#include "stack.h"


//----------------------------------------------------------------------
// Stack::Stack
// 	The constructor for the Stack class.  Note that it doesn't have a
// 	return type.
//
// 	"sz" -- maximum number of elements on the Stack at any time
//----------------------------------------------------------------------

Stack::Stack(int sz)
{
    if (sz < 1) {
	fprintf(stderr, "Error: Stack: size %d too small\n", sz);
	exit(1);
    }

    // Initialize the data members of the stack object.
    size = sz;
    top = 0;
    stack = new int[size];   // allocate an array of integers.
}

//----------------------------------------------------------------------
// Stack::~Stack
// 	The destructor for the Stack class.  Just get rid of the array we
// 	allocated in the constructor.
//----------------------------------------------------------------------

Stack::~Stack()
{
    // Some compilers and books tell you to write this as:
    //     delete [size] stack;
    // but apparently G++ doesn't like that.

    delete stack;
}

//----------------------------------------------------------------------
// Stack::Push
// 	Put an integer on the top of the stack; exit if the stack overflows.
//
//	"value" -- the value to put on the stack
//----------------------------------------------------------------------

void
Stack::Push(int value)
{
    if (Full()) {
	fprintf(stderr, "Error: Stack overflow\n");
	exit(1);
    }
    
    stack[top++] = value;
}

//----------------------------------------------------------------------
// Stack::Pop
// 	Remove an integer from the top of the stack, returning its value.
//	Exit if the stack has nothing on it.
//----------------------------------------------------------------------

int
Stack::Pop()
{
    if (Empty()) {
	fprintf(stderr, "Error: Stack underflow\n");
	exit(1);
    }
    
    return (stack[--top]);
}

//----------------------------------------------------------------------
// Stack::Full
// 	Return TRUE if the stack has no more room.
//----------------------------------------------------------------------

int
Stack::Full()
{
    return (top == size);
}

//----------------------------------------------------------------------
// Stack::Empty
// 	Return TRUE if the stack has nothing on it.
//----------------------------------------------------------------------

int
Stack::Empty()
{
    return (top == 0);
}
