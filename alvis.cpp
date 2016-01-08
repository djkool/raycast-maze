// Alvis.cpp - God of the Western world.

#include ".\alvis.h"
#include "Console.h"
extern Console con;

#define satanic_cast static_cast

// Specialized constructor.
//
// In:  spl		Name of your spell.
//		aoe		Area o' Effect.
//		n		The npc to copy!
alvis::alvis(const char *spl, int aoe, const npc &n) : npc(n), spell()
{
	set(spl, aoe);
}

// Set an alvis.
//
// In:  spl		Name of your spell.
//		aoe		Area o' Effect.
void alvis::set(const char *spl, int aoe)
{
	this->aoe = aoe;

	// Dynamically allocate that spell...
	this->spell = spl;
}

// Display a character's stats on one line.
void alvis::minishow() const
{
	npc::minishow();
	con << ", " << DarkYellow << spell << Red  << '(' << aoe << ')' << Def;
}

// Display a player's stats.
void alvis::show() const
{
	npc::show();
	con << DarkYellow << "Spell\t\t\t: " << spell << '\n';
	con << Red << "AoE\t\t\t\t: " << aoe << '\n' << Def;
}

// Yell our Battle Cry!
void alvis::assault() const
{
	 con << BGRed << FGWhite << "bitch slaps" << Def << ' ';
}

// Display what is unique to this class.
void alvis::data(void) const
{
	con << DarkYellow << "Spell\t\t\t: " << spell << '\n';
	con << Red << "AoE\t\t\t\t: " << aoe << '\n' << Def;
}

// Display a short form of what is unique to this class.
void alvis::minidata(void) const
{
	con << ", " << DarkYellow << spell << Red  << '(' << aoe << ')' << Def;
}