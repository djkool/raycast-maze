// encounter.h : Goodies and battles.

#ifndef _ENCOUNTER_H_
#define _ENCOUNTER_H_

// Forward declaration.
class npc;

class Enc
{
	// Safe for static member functions to manipulate.
	static int crates;

public:

	// All the good stuff.
	//
	// In:  n   The hero.
	//
	// Out: n   The hopefully leveled up hero.
	static void e1(npc &n);

	// The first easy battle.
	//
	// In: n    The hero.
	//
	// Out: n   The hopefully alive hero.
	static void e2(npc &n);

	// A random battle.
	//
	// In:  n   The hero.
	//
	// Out: n   The hopefully alive hero.
	static void e3(npc &n);

	// A tough fight...
	//
	// In:	n	The hero.
	//
	// Out: n   The hopefully alive hero.
	static void e4(npc &n);

	// Our first Alvis encounter...
	//
	// In:	n	The hero.
	//
	// Out: n   The hopefully alive hero.
	static void e5(npc &n);

	// Da pimp battle
	//
	// In:	n	The hero.
	//
	// Out: n   The hopefully alive hero.
	static void e6(npc &n);

	// Who stole some cell samples?
	//
	// In:	n	The hero.
	//
	// Out: n   The hopefully alive hero.
	static void e7(npc &n);

	// The first weak random battle.
	//
	// In: n    The hero.
	//
	// Out: n   The hopefully alive hero.
	static void we1(npc &n);

	// Another miniboss.
	//
	// In:	n	The hero.
	//
	// Out: n   The hopefully alive hero.
	static void we2(npc &n);

	// The pitter-patter of many booted feet!
	//
	// In:  n   The hero.
	//
	// Out: n   The hopefully alive hero.
	static void we3(npc &n);

	// P1mp-m0d3 enabled!
	//
	// In:  n   The hero.
	//
	// Out: n   The hopefully alive hero.
	static void we4(npc &n);

	// Give me a gun
	static npc* promote(npc* n);

	// How many crates have we encountered?
	static int GetCrates(void) { return crates; }
};

#endif