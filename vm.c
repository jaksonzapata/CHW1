/*
Assignment:
vm.c - Implement a P-machine virtual machine

Authors: Jakson Zapata, Jacob Smith

Language: C (only)

To Compile:
gcc -O2 -Wall -std=c11 -o vm vm.c

To Execute (on Eustis):
./vm input.txt

where:
input.txt is the name of the file containing PM/0 instructions;
each line has three integers (OP L M)

Notes:
- Implements the PM/0 virtual machine described in the homework
  instructions.
- No dynamic memory allocation or pointer arithmetic.
- Does not implement any VM instruction using a separate function.
- Runs on Eustis.

Class: COP 3402 - Systems Software - Fall 2025
Instructor : Dr. Jie Lin
Due Date: Friday , September 12th , 2025
*/

#include <stdio.h>

int main(void)
{
    printf("Hello World!\n");
    return 0;
}