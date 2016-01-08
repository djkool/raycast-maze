// utils.h :  A handy grab-bag of functions.

#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
using std::istream;

// Exchange two items.
template <typename Any>
void Swap(Any &a, Any &b)
{
	Any temp = a;
	a = b;
	b = temp;
}

// Seed the random number generator.
void randomize(void);

// Generate random stat values.
//
// Return: A random number between 3 -> 18 (inclusive).
int Roll3d6(void);

// Generate random values.
//
// In:  high    The largest value we want.
//      low     The smallest values we want.
//
// Return: A random number between high and low (inclusive).
int random(int high = 100, int low = 1);

// Generate random name values.
//
// Return: The new name.
char *random_name(void);

// Kill all the extra input chode.
//
// In:	l	Where to read the input from.
//
// Out:	l	The input input stream.
void eatline(istream &l = std::cin);

// Sit and spin here for a while.
void pak(void);

// Return:	A random number:
//				1	60%
//				2	30%
//				3	 7%
//				4	 2%
//				5	 1%
int getBonus();

#endif