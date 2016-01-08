// utils.cpp :  A handy grab-bag of functions.

#include <iostream>

#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstring>

#include "Console.h"

#include "utils.h"

extern Console con;

// Seed the random number generator.
void randomize(void)
{
	srand((unsigned int)time(0));
}

// Generate random stat values.
//
// Return: A random number between 3 -> 18 (inclusive).
int Roll3d6(void)
{
	int roll1 = rand() % 6 + 1,
		roll2 = rand() % 6 + 1,
		roll3 = rand() % 6 + 1;

	return roll1 + roll2 + roll3;            
}

// Generate random values.
//
// In:  high    The largest value we want.
//      low     The smallest values we want.
//
// Return: A random number between high and low (inclusive).
int random(int high, int low)
{
	if (high < low)
		Swap(high, low);

	return rand() % (high - low + 1) + low;    
}

// Generate random name values.
//
// Return: The new name.
char *random_name(void)
{
	const int Len = random(7, 3);

	//char *name = new char[Len + 1];

	const int Max = 7;

	static char name[Max + 1];

	const char *Cons  = "bcdfghjklmnpqrstvwxyz";
	const char *Vowls = "aeiouy";

	int cons = random(1, 0);
	for (int i = cons; i < Len; i += 2)
		name[i] = Cons[random((int)strlen(Cons) - 1, 0)];

	for (int j = 1 - cons; j < Len; j += 2)
		name[j] = Vowls[random((int)strlen(Vowls) - 1, 0)];  

	name[Len] = '\0';

	name[0] = toupper(name[0]);

	return name;
}

// Kill all the extra input chode.
//
// In:  l   Where to read the input from.
//
// Out: l   The input input stream.
void eatline(istream &l)
{
	while (l.get() != '\n') { }
}

// Sit and spin here for a while.
void pak(void)
{
	while (!con.get()) { }
}

// Return:  A random number:
//              1   60%
//              2   30%
//              3    7%
//              4    2%
//              5    1%
int getBonus()
{
	int roll = random();
	if (roll <= 60)
		return 1;
	if (roll <= 90)
		return 2;
	if (roll <= 97)
		return 3;
	if (roll <= 99)
		return 4;
	return 5;
}