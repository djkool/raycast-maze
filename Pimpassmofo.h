// pimpassmofo.h - One bad mutha...

#pragma once
#include "alvis.h"
#include "gangsta.h"

class Pimpassmofo : public alvis, public gangsta
{
public:
	// Powerful constructor.
	//
	// In:  w	 Our weapon name
	//		d		our extra damage
	//		spl		Name of your spell.
	//		aoe		Area o' Effect.
	//		nme     The name to use.
	//      str     The strength.
	//      dex     The dexterity.
	//      iq      The intelligence.
	//      hlt     The health.
	//      chp     The current hit points.
	//      mhp     The max hit points.
	//      ep      The experience points.
	//		f		The flags.
	Pimpassmofo(const char* w = "Fist", unsigned int d = 0, 
		const char *spl = "Magic Missile", int aoe = 1,
		const char *nme = random_name(), short str = Roll3d6(),
		short dex = Roll3d6(), 
		short iq = Roll3d6(), short hlt = Roll3d6(),
		int chp = 20,
		int mhp = random(20, 15),
		double ep = 0, byte f = 0)
		: gangsta(w, d),
		  alvis(spl, aoe),
		  npc(nme, str, dex, iq, hlt, chp, mhp, ep, f)
	{ }

	// Specialized Constructor
	//
	// In:  w		Our weapon name
	//		d		our extra damage
	//		spl		Name of your spell.
	//		aoe		Area o' Effect.
	//		n		The dude to copy.
	Pimpassmofo(const char* w, unsigned int d, 
		const char *spl, int aoe, const npc &n)
		: alvis(spl, aoe), gangsta(w, d), npc(n)
	{ }

	// Destructor.
	virtual ~Pimpassmofo(void) { }
	
	// What's our area o' effect?
	int getAOE(void) const
	{
		return alvis::getAOE();
	}

	
	// Display a character's stats on one line.
	virtual void minishow() const;

	// Display a player's stats.
	virtual void show() const;

	// Calculate our new damage
	virtual int damage(void) const;

	// Yell our Battle Cry!
	void assault() const;

protected:
	// Display what is unique to this class.
	void data(void) const { }

	// Display a short form of what is unique to this class.
	void minidata(void) const { }
};
