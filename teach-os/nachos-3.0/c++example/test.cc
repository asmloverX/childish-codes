// test.cc
//	Test routines for the Stack class -- an implementation of a 
//	stack of integers.
//
// 	To compile the stack test, do: "g++ stack.cc test.cc".
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

// The "extern C" is needed to tell the compiler that the stdio functions 
// and the exit function are compiled by a C compiler.  This is necessary
// on many C++ compilers, because the compiler munges procedure names. 

extern "C" {
#include <stdio.h>
extern int exit(int st);
}

#include "stack.h"

//----------------------------------------------------------------------
// main
// 	The test routine will push 10 numbers, starting with 17, onto the 
//	stack, and then print them as it pops them off.
//
//	"argc" -- number of arguments passed to main on the command line
//		(in this case, should be 1 -- the name of the executable)
//	"argv" -- array of text strings containing the arguments passed to 
//		main on the command line
//		(in this case, should be {"a.out"} -- the name of the 
//		executable)  The number of entries in argv should agree
//		with the value in argc.
//----------------------------------------------------------------------

int
main(int argc, char* argv[])
{
    Stack *stack = new Stack(10);   // Constructor with an argument.
    int count = 17;
    
    // Put a bunch of stuff in the stack...
    while (!stack.Full())
	stack.Push(count++);
    
    // ... and take it out again.
    while (!stack.Empty())
	printf("pop %d\n", stack.Pop());

    exit(0);
}
