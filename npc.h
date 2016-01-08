// npc.h : Everything we need for a Non - Player - Character.

#ifndef _NPC_H_
#define _NPC_H_

// That's nice, Microsoft.  Get current!
#pragma warning(disable:4290)

#include <iostream>
#include <string>
#include <exception>
#include <memory>
using namespace std;

#include "Console.h"

#include "Utils.h"
#include "Queue.h"
#include "Encounter.h"

// Named constants for our stats.
enum Attributes { Str, Dex, Iq, Hlt, NumAttr, };

// Shortcut for 8 bits.
typedef unsigned char byte;

// Possible values for our flags.						  Get-out-of-death-free
enum Flags { Player = 1, WetTowel = 2, WetSuit = 4, Mardoc = 8, GOODF = 16};

// Sanity-savin' macros!
#define C_NAME(who)	static_cast<const string &>(*who)
#define C_MY_NAME	C_NAME(this)
#define MY_NAME		static_cast<string &>(*this)

// Non - Player - Character.
class npc : private string
{
	// Grant the following class full access to self.  Note:
	//	I have no access to HIS private/protected info.
	//friend class Enc;
	friend void Enc::e1(npc &n);

	// Our attributes.
	short attr[NumAttr];

	// Current and max hit points.
	int currHP, maxHP;

	// Experience points.
	double xp; 

	// Our flags.
	byte flags;

public:
	// What we throw when our brackets go out o' bounds.
	class bad_index : public exception
	{
	public:
		bad_index(const char *s) : exception(s) { }
	};

	// Set up an npc.
	//
	// In:  nme     The name to use.
	//      str     The strength.
	//      dex     The dexterity.
	//      iq      The intelligence.
	//      hlt     The health.
	//      chp     The current hit points.
	//      mhp     The max hit points.
	//      ep      The experience points.
	//		f		The flags.
	npc(const char *nme = random_name(), short str = Roll3d6(),
		short dex = Roll3d6(), 
		short iq = Roll3d6(), short hlt = Roll3d6(),
		int chp = 20,
		int mhp = random(20, 15),
		double ep = 0, byte f = 0);

	// Destructor.
	virtual ~npc(void) { }

	/*************/
	/* Accessors */
	/*************/

	// Return:	Our name.
	const string &getName() const
	{
		return C_MY_NAME;
	}
	
	// Our attributes.
	short getAttr(int which) const
	{
		if (which < Str || which >= NumAttr)
			return 0;
		return this->attr[which];
	}
	
	// Current and max hit points.
	int getCurrHP() const
	{
		return this->currHP;
	}
	int getMaxHP() const
	{
		return this->maxHP;
	}

	// Experience points.
	double getXp() const
	{
		return this->xp; 
	}

	// Our flags.
	byte getFlags() const
	{
		return this->flags;
	}

	/****************************************************************************************************************/
	/* Mutators/Modifiers/Morphers/Doppelgangers/Shape-Shifters/Werewolves/Loup-Garou/Lycanthropes/Xenomorphs/Polys */
	/****************************************************************************************************************/

	// Change our name to "n".
	void setName(const char *n);
	
	// Alter our attributes.
	void setAttr(int which, short val)
	{
		if (which < Str || which >= NumAttr || val < 0)
			return;
		this->attr[which] = val;
	}
	
	// Current and max hit points.
	void setCurrHP(int val)
	{
		if (val < 0)
			val = 0;
		if (val > this->maxHP)
			val = this->maxHP;
		this->currHP = val;
	}
	void setMaxHP(int val)
	{
		if (val < 1)
			val = 1;
		if (val < this->currHP)
			// Merry X-Mas!!!
			val = this->currHP;
		this->maxHP = val;
	}

	// Experience points.
	void setXp(double val)
	{
		if (val < 0)
			val = 0;
		this->xp = val; 
	}

	// Our flags.
	void setFlags(byte val)
	{
		this->flags = val;
	}

	// Calculate the percentage to give some love.
	//
	// Return: The percentage chance for lovin.
	short tohit() const
	{
		return this->attr[Dex] * 5;
	}

	// Heal a character.
	//
	// In:	val The amount to heal for.
	void heal(int val)
	{
		if (val < 0)
			val = 0;

		this->currHP += val;
	   
		if (this->currHP > this->maxHP)
			this->currHP = this->maxHP;
	}

	// Check if someone is alive.
	//
	// Return: true if they live.
	bool alive() const
	{
		return this->currHP > 0;        
	}

	// Calculate how much someone is worth.
	//
	// Return: Their worth.
	double Exp() const
	{
		return 3 * this->attr[Dex] + (this->attr[Str] + this->attr[Iq]) * 2 + 
				this->attr[Hlt] + this->maxHP;
	}

	// What's our area o' effect?
	virtual int getAOE(void) const
	{
		return 1;
	}

	// Maim or kill a character.
	//
	// In:	val The amount to hurt for.
	void hurt(int val);

	// Display a character's stats on one line.
	virtual void minishow() const;

	// Display a player's stats.
	virtual void show() const;

	// Ask the user for everything.
	void ask();

	// Set up an npc.
	//
	// In:  nme     The name to use.
	//      str     The strength.
	//      dex     The dexterity.
	//      iq      The intelligence.
	//      hlt     The health.
	//      chp     The current hit points.
	//      mhp     The max hit points.
	//      ep      The experience points.
	//		f		The flags.
	void set(const char *nme, short str, short dex, 
			short iq, short hlt, int chp, int mhp, double ep, byte f);

	// Write a player's stats to disk.
	void save() const;

	// Get a player's stats from disk.
	void load();

	// Fight an encounter.
	//
	// In:	horde   The list of guys to fight.
	//      num     The number of guys to fight.
	//
	// Out:	horde   The hopefully dead bodies.
	//
	// Return: true if we live.
	bool fight(auto_ptr<npc> horde[], int num);

	// Handle one particular battle.
	//
	// In:	victims     The screaming horde.
	//      num         The number of victims.
	//		bodies		Where to put the dead.
	//
	// Out: victims     The possibly dead pile.
	//		bodies		The filled-in Stack.
	//
	// Return: how many died.
	int attack(auto_ptr<npc> victims[], int num, Queue<npc *> &bodies) const;

	// Calculate the amount of love to give.
	//
	// Return: the amount of damage.
	virtual int damage() const;

	// Display us in color!
	//
	// In:	l	Where to send the output.
	//		r	What object to display.
	//
	// Out:	l	The updated output.
	//
	// Return:	The updated output.
	friend Console & operator <<(Console &l, const npc &r);

	// Display us.
	//
	// In:	l	Where to send the output.
	//		r	What object to display.
	//
	// Out:	l	The updated output.
	//
	// Return:	The updated output.
	friend ostream & operator <<(ostream &l, const npc &r);

	// Ask for a critter.
	//
	// In:	l	Where to read the input from.
	//		r	Where to store their answers.
	//
	// Out:	l	An updated input stream.
	//		r	What they typed.
	//
	// Return:	An updated input stream.
	friend istream & operator >>(istream &l, npc &r);

	virtual void assault() const;

	// Brackets operator.
	//
	// In:	i	The index of the attribute we want.
	//
	// Return:	Our score.
	short &operator [](int i) throw(bad_index, bad_exception);
	const short &operator [](int i) const throw(bad_index, bad_exception);

protected:

	// Display what is unique to this class.
	virtual void data(void) const;

	// Display a short form of what is unique to this class.
	virtual void minidata(void) const;
};

#endif