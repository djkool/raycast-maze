// npc.cpp : Everything we need for a Non - Player - Character.

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <algorithm>
using namespace std;

#include "Console.h"
#include "utils.h"
#include "npc.h"
#include "Pimpassmofo.h"


extern Console con;

// Do we take damage or laugh carelessly?
bool godMode = false;

// The colors to use for our stats.
const conout *AColors[] =
{
	&Red,
	&Green,
	&Yellow,
	&Cyan,
};

// The filename we save and load from.
static const char *FileName = "player.dat";

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
npc::npc(const char *nme, short str, short dex, short iq,
		 short hlt,	int chp, int mhp, double ep, byte f) :
	string()
{
	this->maxHP = 0;
	this->set(nme, str, dex, iq, hlt, chp, mhp, ep, f);
}

// Change our name to "n".
void npc::setName(const char *n)
{
	// Deep copy.
	MY_NAME = n;
}

// Maim or kill a character.
//
// In:	val The amount to hurt for.
void npc::hurt(int val)
{
	if (this->flags & Player && godMode)
		// There can be only one!
		return;

	// Do we have protection?	
	if (this->flags & Mardoc)
		val -= 100;
	else if (this->flags & WetSuit)
		val -= 25;
	else if (this->flags & WetTowel)
		val -= 10;
	if (val < 0)
		val = 0;

	this->currHP -= val;

	if (this->currHP < 0)
		this->currHP = 0;

	// Undeath?
	if (this->currHP == 0 && this->flags & GOODF)
	{
		// Get outta death free!
		this->currHP = this->maxHP;

		con << Green << " killing " << Def << "him,\n";
		con << Cyan << "But he uses a Get-Out-Of-Death-Free Card," << Def;

		// Make sure they lose the card!
		this->flags &= ~GOODF;
	}

}

// Display a character's stats on one line.
void npc::minishow() const
{
	con << White << C_MY_NAME << Def << ", ";

	for (int i = Str; i < NumAttr; i++)
		con << *AColors[i] << this->attr[i] << Def << ", ";

	con << White << this->currHP << Def << '/' 
		<< DarkGray << this->maxHP << Def << ", ";

	if (this->flags & Player)
		con << Cyan << this->xp;
	else
		con << Cyan << this->Exp();

	if (this->flags & WetSuit)
		con << DarkYellow << " Wetsuit";
	if (this->flags & WetTowel)
		con << White << " Wet Towel";
	if (this->flags & Mardoc)
		con << Red << " Mardoc";
	if (this->flags & GOODF)
		con << Cyan << " GOODF Card";

	con << Def;
}

// Display a player's stats.
void npc::show() const
{
	con << *this;
}

// Ask the user for everything.
void npc::ask()
{
	cin >> *this;
}

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
void npc::set(const char *nme, short str, short dex, 
		short iq, short hlt, int chp, int mhp, double ep, byte f)
{
	this->setName(nme);
	this->setAttr(Str, str);
	this->setAttr(Dex, dex);
	this->setAttr(Iq, iq);
	this->setAttr(Hlt, hlt);
	this->setMaxHP(mhp);
	this->setCurrHP(chp);
	this->setXp(ep);
	this->setFlags(f);
}

// Write a player's stats to disk.
void npc::save() const
{
	ofstream ofl(FileName);

	ofl << C_MY_NAME << '\n';

	for (int i = Str; i < NumAttr; i++)
		ofl << this->attr[i] << '\n';

	ofl << this->currHP << '\n';
	ofl << this->maxHP << '\n';
	ofl << this->xp << '\n';
	ofl << (int)this->flags << '\n';

	// Close the file.
	ofl.close();
}

// Get a player's stats from disk.
void npc::load()
{
	ifstream ifl(FileName);

	string temp;
	getline(ifl, temp);

	for (int i = Str; i < NumAttr; i++)
	{
		ifl >> this->attr[i];
		eatline(ifl);
	}

	ifl >> this->currHP;
	eatline(ifl);

	ifl >> this->maxHP;
	eatline(ifl);

	ifl >> this->xp;
	eatline(ifl);

	int f;
	ifl >> f;
	eatline(ifl);

	ifl.close();

	// Set up the player.
	this->set(temp.c_str(), this->attr[Str], this->attr[Dex], this->attr[Iq],
		this->attr[Hlt], this->currHP, this->maxHP, this->xp, f);
}

struct sorty
{
	bool operator()(const auto_ptr<npc> &a, const auto_ptr<npc> &b)
	{
		return a->Exp() > b->Exp();
	}
};

// Fight an encounter.
//
// In:	horde   The list of guys to fight.
//      num     The number of guys to fight.
//
// Out:	horde   The hopefully dead bodies.
//
// Return: true if we live.
bool npc::fight(auto_ptr<npc> horde[], int num)
{
	if (horde == 0 || num < 1)
		// Die biiizzzzzzatch!
		return false;

	// Wipe the map.
	con.Clear();

/*
	sorty guava;
	auto_ptr<npc> one(new npc), two(new npc);
	one->minishow();
	con << '\n';
	two->minishow();
	if (guava(one, two))
		con << "The first is worth more experience!\n";
	else
		con << "The second is worth more experience!\n";
	pak();
*/
	sort(horde, horde + num, sorty());

/*
	// A variation of the Bubble Sort.
	for (int i = 0; i < num - 1; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (horde[i]->Exp() < horde[j]->Exp())
				Swap(horde[i], horde[j]);
		}
	}
*/

/*
	Bubble Sort	Order n^2
	Quick Sort	Order n * log2n
 
	n = 16
	n * n = 16 * 16 = 256
	n * 4 = 16 *  4	=  64

	n = 65536
	65536 * 65536 = 4,294,967,296
	65536 *	16	  =     1,048,576

	n = 14,000,000
	14,000,000 * 14,000,000 = 196,000,000,000,000
	14,000,000 * 24			=         336,000,000

	2^10	1024
	11		2048
	12		4096
	13		8192
	14		16384
	15		32768
	16		65536
	17		130+
	18		260+
	19		520+
	20		1m+
	21		2m+
	22		4m+
	23		8m+
	24		16m+
*/

/*
	Holy Grails		Order NP-Complete (non-polynomial)
	Travelling Salesman
	Knapsack
	Encryption MIB's!

	.00000000000001%
*/

	con << "You see...\n";
	for (int i = 0; i < num; i++)
	{
		horde[i]->minishow();
		con << '\n';
	}
	pak();

	// Track how many died.
	int killed = 0;

	// Track the round number.
	int round = 0;

	// Where to put da dead
	Queue<npc *> bodies;

	while (this->alive() && killed < num)
	{
		con.Clear();

		con << "Round #" << ++round << '\n';

		// I go first.
		killed += this->attack(horde, num, bodies);

		// Their turn.
		for (int i = 0; i < num; i++)
		{
			if (!horde[i]->alive())
				// Skip the dead people.
				continue;

			auto_ptr<npc> temp[] = { auto_ptr<npc>(this) };

			int howManyDied = horde[i]->attack(temp, 1, bodies);
			temp[0].release();
			if (howManyDied)
				// Bidness, no one else has to go.
				break;
		}
		pak();
	}

	npc *deadGuy;
	if (this->alive())
	{
		char answer = '?';
		while (bodies.Dequeue(deadGuy))
		{
			double ep = deadGuy->Exp();

			con << White << C_NAME(deadGuy) << Def
				<< " was worth " << Magenta << ep
				<< Def << " XP's!\n";

			int old_level = int(this->xp / 100);

			this->xp += ep;

			// Display the player's stats.
			this->minishow();
			con << '\n';

			int new_level = int(this->xp / 100);

			for (int j = 0; j < new_level - old_level; j++)
			{
				while (!strchr("sdihSDIH", answer))
				{
					con << "What would you like to do (S)tr, (D)ex, (I)q, or (H)lt ? _\b";
					cin >> answer;
					eatline();
				}

				// What did they pick?
				switch (tolower(answer))
				{
					case 's':
						++this->attr[Str];
					break;
					
					case 'd':
						++this->attr[Dex];
					break;

					case 'i':
						++this->attr[Iq];
					break;
	
					case 'h':
						++this->attr[Hlt];
					break;
				}

				// Buff up!
				const int Buff = this->attr[Hlt] * 2;
				this->maxHP += Buff;
				this->currHP += Buff;
			}
		}

		// Show our final tally!
		this->minishow();
		pak();
	}
	
	return this->alive();
}

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
int npc::attack(auto_ptr<npc> victims[], int num, Queue<npc *> &bodies) const
{
	if (!victims || num < 1)
		return 0;

	int died = 0;

	int target = 0;

	// Auto target!
	for (int i = 0; i < num; i++)
	{
		if (victims[i]->alive())
		{
			target = i;
			break;
		}
	}

	// The attacker could have multiple attacks...
	int numAttacks = getAOE();
	for (int i = 0; i < numAttacks && i < num; i++, ++target)
	{
		if (target >= num)
			// Ran out of oPPPosition.
			break;

		// Don't target corpses!
		if (!victims[target]->alive())
		{
			// If we ain't countin the dead,
			// don't waste an attack.
			--i;
			continue;
		}

		int dam = this->damage();

		int hit = random();

		// Did we roll good enough to score a critical hit?
		int chance = this->attr[Dex] / 2;
		int ourRoll = hit;
		int multiplier = 1;
		while (ourRoll <= chance)
		{
			++multiplier;
			ourRoll *= 2;

			// Pimpassmofo's get extra luvin'
			if (dynamic_cast<const Pimpassmofo *>(this))
				numAttacks++;
		}
		// How well did we do?
		dam *= multiplier;
		if (multiplier > 1)
			con << Green << 'x' << multiplier << Def;
		else
			con << "  ";

		con << '(' << this->tohit() << "%) " << White
			<< C_MY_NAME << ' ';
		assault();
		con << White << C_NAME(victims[target]) << Def;

		if (this->tohit() >= hit)
		{
			con << " and hits for " << Magenta << dam;

			victims[target]->hurt(dam);

			// Did they live?
			if (!victims[target]->alive())
			{
				con << Green << " killing " << Def << "him!";
				died++;

				// Add the dead guy to the pile.
				bodies.Enqueue(victims[target].get());
				if (!victims[target].get())
					throw "Damn!";
			}
			else
				con << " leaving " << Magenta << victims[target]->currHP
					<< Def << '!';
		}
		else
			con << Yellow << " and whiffs!" << Def;
		con << '\n';

		// Pagination!
		int x, y;
		con.CP(x, y);
		if (y >= con.Rows() - 2)
		{
			// Wait for it!
			pak();
			con.Clear();
		}
	}

	return died;
}

// Calculate the amount of love to give.
//
// Return: the amount of damage.
int npc::damage() const
{
	int base = this->attr[Str];

	int mod = base / 2;

	base += random(mod, -mod);

	return base;
}

// Display us in color!
//
// In:	l	Where to send the output.
//		r	What object to display.
//
// Out:	l	The updated output.
//
// Return:	The updated output.
Console & operator <<(Console &l, const npc &r)
{
	l << White << static_cast<const string &>(r) << '\n';

	// English names for our stats.    
	const char *Names[] =
	{
		"Strength\t\t: ", "Dexterity\t\t: ", "Intelligence\t: ",
		"Health\t\t\t: "        
	};

	for (int i = Str; i < NumAttr; i++)
		l << *AColors[i] << Names[i] << r.attr[i] << '\n'; 

	l << White << "HP\t\t\t\t: " << r.currHP << '/' 
		<< r.maxHP << '\n';

	l << "HP %\t\t\t: " 
		<< int(r.currHP / double(r.maxHP) * 100) << '\n';

	l << Magenta << "ToHit%\t\t\t: " << r.tohit() << '\n';

	l << Cyan << "Experience\t\t: " << r.xp << '\n';

	l << LightGray << "Gear\t\t\t: ";

	if (r.flags & WetSuit)
		l << DarkYellow << "Wetsuit";
	if (r.flags & WetTowel)
		l << White << "Wet Towel";
	if (r.flags & Mardoc)
		l << Red << "Mardoc";
	if (r.flags & GOODF)
		l << Cyan << "GOOD Card";

	l << '\n' << Def;

	// Keep goin' if you want.
	return l;
}

// Display us.
//
// In:	l	Where to send the output.
//		r	What object to display.
//
// Out:	l	The updated output.
//
// Return:	The updated output.
ostream & operator <<(ostream &l, const npc &r)
{
	l << static_cast<const string &>(r) << '\n';

	// English names for our stats.    
	const char *Names[] =
	{
		"Strength\t: ", "Dexterity\t: ", "Intelligence\t: ",
		"Health\t\t: "        
	};

	for (int i = Str; i < NumAttr; i++)
		l << Names[i] << r.attr[i] << '\n'; 

	l << "HP\t\t: " << r.currHP << '/' 
		<< r.maxHP << '\n';

	l << "HP %\t\t: " 
		<< int(r.currHP / double(r.maxHP) * 100) << '\n';

	l << "ToHit%\t\t: " << r.tohit() << '\n';

	l << "Experience\t: " << r.xp << '\n';

	l << "Gear\t\t: ";

	if (r.flags & WetSuit)
		l << "Wetsuit";
	if (r.flags & WetTowel)
		l << "Wet Towel";
	if (r.flags & Mardoc)
		l << "Mardoc";
	if (r.flags & GOODF)
		l << "GOOD Card";

	// Keep goin' if you want.
	return l;
}

void myLower(char &thing)
{
	thing = tolower(thing);
}

// Ask for a critter.
//
// In:	l	Where to read the input from.
//		r	Where to store their answers.
//
// Out:	l	An updated input stream.
//		r	What they typed.
//
// Return:	An updated input stream.
istream & operator >>(istream &l, npc &r)
{
	cin.exceptions(ios_base::failbit);

	con << "What is your name : ?";
	string temp;
	getline(l, temp);

	string mine(temp);
	//for (string::size_type i = 0; i < mine.length(); i++)
	//	mine[i] = tolower(mine[i]);
	//for (string::iterator i = mine.begin(); i != mine.end(); i++)
	//	*i = tolower(*i);
	//for_each(mine.begin(), mine.end(), myLower);
	transform(mine.begin(), mine.end(), mine.begin(), tolower);

/*
	functors (function objects)
		function pointers
		normal function names
		class/struct that overloads the parenthesis operator

	generator			function that takes no args
	unary functor		function that takes 1 arg
	binary functor		function that takes 2 args

	predicate			unary functor that returns a bool
	binary predicate	binary functor that returns a bool
*/

/*
	i
	god
	^  ^
	|  |
	b  e
*/

/*
	iterator
	--------
	input			output
	forward
	bidirectional
	random access
*/

	// Cheat time.
	if ("god" == mine)
	{
		r.set(temp.c_str(), 1000, 1000, 1000, 1000, 10000, 10000, 100000, Player | Mardoc);
		return l;
	}

	
	const char *Names[] =
	{
		"\"strength\"", "\"dexterity\"",  "\"intelligence\"",
		"\"health\""
	};

	for (int i = Str; i < NumAttr; i++)
	{
		while (true)
		{
			con << "What is your " << *AColors[i] << Names[i] 
				<< Def << " : (3 -> 18) ? __\b\b";
			if (l >> r.attr[i] && r.attr[i] > 2 && r.attr[i] < 19)
				break;
		}
		eatline(l);
	}

	while (true)
	{
		con << "What is your max hit points (1+) ? ";
		if (l >> r.maxHP && r.maxHP > 0 && r.maxHP < r.attr[Hlt] * 6)
			break;
	}
	eatline(l);

	while (true)
	{
		con << "What is your current hit points (1+) ? ";
		if (l >> r.currHP && r.currHP > 0 && r.currHP <= r.maxHP)
			break;
	}
	eatline(l);

	while (true)
	{
		con << "What is your experience points (0+) ? ";
		if (l >> r.xp && r.xp >= 0)
			break;
	}
	eatline(l);

	r.set(temp.c_str(), r.attr[Str], r.attr[Dex], r.attr[Iq],
		r.attr[Hlt], r.currHP, r.maxHP, r.xp, Player);

	// Don't throw any more exceptions, cin...
	cin.exceptions(0);

	// Keep goin' if you want.
	return l;
}

void npc::assault() const
{
	con << BGRed << FGWhite << "Sp00n's" << Def << ' ';
}

// Brackets operator.
//
// In:	i	The index of the attribute we want.
//
// Return:	Our score.

/*
set_unexpected(handler);

unexpected()
	terminate()
		abort()

set_terminate(handler);
*/

short & npc::operator [](int i) throw(bad_index, bad_exception)
{
#if _DEBUG
	if (i < 0 || i >= NumAttr)
	{
		char s[1000];
		sprintf(s, "Bad index %d at line #%d in %s (function %s)!",
			i, __LINE__, __FILE__, __FUNCSIG__);
		throw bad_index(s);
	}
#endif
	return this->attr[i];
}
const short & npc::operator [](int i) const	throw(bad_index, bad_exception)
{
#if _DEBUG
	if (i < 0 || i >= NumAttr)
	{
		char s[1000];
		sprintf(s, "Bad index %d at line #%d in %s (function %s)!",
			i, __LINE__, __FILE__, __FUNCSIG__);
		throw bad_index(s);
	}
#endif
	return this->attr[i];
}

// Display what is unique to this class.
void npc::data(void) const
{
	npc::show();
}

// Display a short form of what is unique to this class.
void npc::minidata(void) const
{
	npc::minishow();
}