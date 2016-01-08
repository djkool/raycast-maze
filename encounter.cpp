// encounter.cpp : Goodies and battles.

#define _WIN32_WINNT 0x501
#include <windows.h>

#include "Console.h"

#include "utils.h"
#include "encounter.h"
#include "Pimpassmofo.h"

// Initialize that static data member.
int Enc::crates = 0;

extern Console con;

// All the good stuff.
//
// In:  n   The hero.
//
// Out: n   The hopefully leveled up hero.
void Enc::e1(npc &n)
{
	// Found another crate.
	++crates;

	// Kill the map.
	con.Clear();

	const int Chance = n.getAttr(Iq);
	if (random() <= Chance)
	{
		int bonus = getBonus();
		con << "You find some " << Red << "Beebop Cola"
			<< Def << "(" << bonus << "+) Str!\n";
		n.attr[Str] += bonus;
	}
	if (random() <= Chance)
	{
		int bonus = getBonus();
		con << "You find a " << Green << "Dodgeball"
			<< Def << "(" << bonus << "+) Dex!\n";
		n.setAttr(Dex, n.getAttr(Dex) + bonus);
	}
	if (random() <= Chance)
	{
		int bonus = getBonus();
		con << "You find some " << Yellow << "Stimutax"
			<< Def << "(" << bonus << "+) Iq!\n";
		n.setAttr(Iq, n.getAttr(Iq) + bonus);
	}
	if (random() <= Chance)
	{
		int bonus = getBonus();
		con << "You find " << Cyan << "Buckethead Wendy"
			<< Def << "(" << bonus << "+) Hlt!\n";
		n.setAttr(Hlt, n.getAttr(Hlt) + bonus);
	}

	if (random() <= Chance + 5)
	{
		int val = random(5, 3) + n.getAttr(Hlt);
		
		con << "You find a bottle of " << BGRed << FGWhite
			<< "\"Liquid Courage\"" << Def
			<< " and heal for (" << Magenta << val
			<< Def << "+)!\n";

		n.heal(val);
	}

	con << "...and that's all!";

	Sleep(1000);
	pak();

}

// The first easy battle.
//
// In: n    The hero.
//
// Out: n   The hopefully alive hero.
void Enc::e2(npc &n)
{
	// The list of guys to fight.
	auto_ptr<npc> posse[] =
	{
		auto_ptr<npc>(new npc("Beck Bristo", 5, 5, 3, 4, 5, 5)),
		auto_ptr<npc>(new npc("Eric Estrada", 6, 7, 4, 6, 6, 6)),
		auto_ptr<npc>(new npc("Bizzaro Hesh", 7, 8, 2, 10, 10, 10)),       
	};
	// How many do we have?
	const int Num = sizeof posse / sizeof posse[0];

	n.fight(posse, Num);
}

// A random battle.
//
// In:  n   The hero.
//
// Out: n   The hopefully alive hero.
void Enc::e3(npc &n)
{
	// The number of guys to fight.
	const int Num = random(7, 4);

	auto_ptr<npc> *posse = new auto_ptr<npc>[Num];

	for (int i = 0; i < Num; i++)
		posse[i] = auto_ptr<npc>(new npc);

	n.fight(posse, Num);

	delete [] posse;
}

// A tough fight...
//
// In:	n	The hero.
//
// Out: n   The hopefully alive hero.
void Enc::e4(npc &n)
{
	con.Clear();
	con << "Zen Master Wu springs out to protect his 9!\n";
	pak();

	// The number of guys to fight.
	const int Num = random(3, 2);

	auto_ptr<npc>*posse = new auto_ptr<npc>[Num];

	posse[0] = auto_ptr<npc>(new npc("Zen Master Wu", 10, 16, 28, 20, 40, 40, 0, WetTowel));
	for (int i = 1; i < Num; i++)
		posse[i] = auto_ptr<npc>(new npc);

	if (n.fight(posse, Num))
	{
		// Get some gear...
		con.Clear();
		con << "A winna is you! Ha ha ha!";
		n.setFlags(n.getFlags() | WetTowel);
		pak();
	}

	delete [] posse;
}

// Our first Alvis encounter...
//
// In:	n	The hero.
//
// Out: n   The hopefully alive hero.
void Enc::e5(npc &n)
{
	auto_ptr<npc> posse[] =
	{
		auto_ptr<npc>(new alvis("Molotov Cocktail", 10, "Alley Drunk", 20, 10, 20, 20, 200, 200, 0, WetTowel))
	};
	n.fight(posse, 1);
}

// Da pimp battle
//
// In:	n	The hero.
//
// Out: n   The hopefully alive hero.
void Enc::e6(npc &n)
{
	auto_ptr<npc> harem[] =
	{
		auto_ptr<npc>(new Pimpassmofo("Cane", 40, "Cold as ice", 407, "Rick James", 30, 30, 30, 30, 300, 300, 0, Mardoc)),
		auto_ptr<npc>(new npc("Supafreak")),
		auto_ptr<npc>(new npc("Supafreak")),
		auto_ptr<npc>(new npc("Supafreak")),
		auto_ptr<npc>(new npc("Supafreak")),
		auto_ptr<npc>(new npc("Supafreak")),
		auto_ptr<npc>(new npc("Supafreak")),
	};
	const int num = sizeof harem / sizeof harem[0];

	n.fight(harem, num);
	if (n.alive())
	{
		con.Clear();
		con << "You don the Mardoc hide!\n";
		n.setFlags(n.getFlags() | Mardoc);
		pak();
	}
}

// Who stole some cell samples?
//
// In:	n	The hero.
//
// Out: n   The hopefully alive hero.
void Enc::e7(npc &n)
{
	con.Clear();

	// The number of guys to fight.
	const int Num = random(150, 50);

	auto_ptr<npc>*posse = new auto_ptr<npc>[Num];

	// Match us!
	if (typeid(Pimpassmofo) == typeid(n))
		posse[0] = auto_ptr<npc>(new Pimpassmofo(dynamic_cast<Pimpassmofo &>(n)));
	else if (typeid(alvis) == typeid(n))
		posse[0] = auto_ptr<npc>(new alvis(dynamic_cast<alvis &>(n)));
	else if (typeid(gangsta) == typeid(n))
		posse[0] = auto_ptr<npc>(new gangsta(dynamic_cast<gangsta &>(n)));
	else
		posse[0] = auto_ptr<npc>(new npc(n));
	posse[0]->setFlags(posse[0]->getFlags() & ~Player);
	posse[0]->setName(("Evil " + posse[0]->getName()).c_str());

	for (int i = 1; i < Num; i++)
		posse[i] = auto_ptr<npc>(new npc("Stormtrooper"));

	n.fight(posse, Num);
}

// The first weak random battle.
//
// In: n    The hero.
//
// Out: n   The hopefully alive hero.
void Enc::we1(npc &n)
{
	// The list of guys to fight.
	auto_ptr<npc> posse[] =
	{
		auto_ptr<npc>(new npc("Dolphin Boy", 6, 4, 3, 7, 10, 10)),
		auto_ptr<npc>(new npc("Ben the Scorpion", 5, 8, 1, 8, 1, 1)),
		auto_ptr<npc>(new npc("Squish Face", 2, 7, 18, 10, 15, 15)),       
		auto_ptr<npc>(new npc("Sharko", 8, 8, 13, 14, 20, 20)),
	};
	// How many do we have?
	const int Num = sizeof posse / sizeof posse[0];

	n.fight(posse, Num);
}

// Another miniboss.
//
// In:	n	The hero.
//
// Out: n   The hopefully alive hero.
void Enc::we2(npc &n)
{
	// The number of guys to fight.
	const int Num = random(30, 25);

	auto_ptr<npc>*posse = new auto_ptr<npc>[Num];

	posse[0] = auto_ptr<npc>(new npc("Ted Nugent", 25, 14, 10, 15, 250, 250, 0, WetSuit));
	for (int i = 1; i < Num; i++)
	{
		char temp[100];
		sprintf(temp, "Fan #%d", i);
		posse[i] = auto_ptr<npc>(new npc(temp, random(15, 13), random(7, 4)));
	}

	if (n.fight(posse, Num))
	{
		// Get some gear...
		con.Clear();
		con << "The used wet suit is yours!";
		n.setFlags(n.getFlags() | WetSuit);
		pak();
	}
	delete [] posse;
}

// The pitter-patter of many booted feet!
//
// In:  n   The hero.
//
// Out: n   The hopefully alive hero.
void Enc::we3(npc &n)
{
	// The number of guys to fight.
	const int Num = random(500, 250);

	auto_ptr<npc> *posse = new auto_ptr<npc>[Num];

	for (int i = 0; i < Num; i++)
	{
		char temp[100];
		sprintf(temp, "Angry Tourist #%d", i);
		posse[i] = auto_ptr<npc>(new npc(temp, random(60, 40)));
	}
	n.fight(posse, Num);

	delete [] posse;
}

// P1mp-m0d3 enabled!
//
// In:  n   The hero.
//
// Out: n   The hopefully alive hero.
void Enc::we4(npc &n)
{
	con.CursorOn(false);

	HWND hwnd = GetConsoleWindow();
	HDC dc = GetDC(hwnd);
	HBITMAP bmp = (HBITMAP)LoadImage(0, "p1mpm0d3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBRUSH br = (HBRUSH)CreatePatternBrush(bmp);
	RECT r;
	GetClientRect(hwnd, &r);
	FillRect(dc, &r, br);
	ValidateRect(hwnd, &r);

	DeleteObject(bmp);
	DeleteObject(br);
	ReleaseDC(hwnd, dc);

	Sleep(5000);
	pak();
	con.CursorOn(true);
	con.Clear();
}

npc* Enc::promote(npc* n)
{
	con.Clear();

	// Time to upgrade
	npc* gnu;

	if (typeid(npc) == typeid(*n))
	{
		con << "You found a nickel plated 9.\n";
		gnu = new gangsta("9mm", 10, *n);
	}
	else if (typeid(gangsta) == typeid(*n))
	{
		con << "You found a bottle of alvian whiskey!\n";
		gnu = new alvis("Molotov Cocktail", 10, *n);
	}
	else if (typeid(alvis) == typeid(*n))
	{
		con << "You find a pure mink coat and diamond-studded cane...\n";
		gnu = new Pimpassmofo("Diamond-studded cane", 50, "Five across the eyes", 20, *n);
	}
	pak();

	delete n;
	return gnu;
}
