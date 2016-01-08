// pimpassmofo.h - One bad mutha...

#include ".\pimpassmofo.h"
#include "Console.h"
extern Console con;

// Display a character's stats on one line.
void Pimpassmofo::minishow() const
{
	npc::minidata();
	alvis::minidata();
	gangsta::minidata();
}

// Display a player's stats.
void Pimpassmofo::show() const
{
	npc::data();
	alvis::data();
	gangsta::data();
}

// Calculate our new damage
int Pimpassmofo::damage(void) const
{
	return gangsta::damage();
}

// Yell our Battle Cry!
void Pimpassmofo::assault() const
{
	 con << BGRed << FGWhite << "shafts" << Def << ' ';
}

