#pragma once

#include "npc.h"

class gangsta : public virtual npc
{
private:

	string wpn;				// our new weapon
	unsigned int dam;		// the extra damage it deal

public:
	
	// Fill us in
	//
	// In:	w	 Our weapon name
	//		d		our extra damage
	//		nme     The name to use.
	//      str     The strength.
	//      dex     The dexterity.
	//      iq      The intelligence.
	//      hlt     The health.
	//      chp     The current hit points.
	//      mhp     The max hit points.
	//      ep      The experience points.
	//		f		The flags.
	gangsta(const char* w = "Fist", unsigned int d = 0, const char *nme = random_name(), short str = Roll3d6(),
		short dex = Roll3d6(), 
		short iq = Roll3d6(), short hlt = Roll3d6(),
		int chp = 20,
		int mhp = random(20, 15),
		double ep = 0, byte f = 0);

	// Fill us in
	//
	// In:	w	 Our weapon name
	//		d	 our extra damage
	//		n	 The npc we used to be
	gangsta(const char* w, unsigned int d, const npc& n);

	// Set us up the new stuff
	//
	// In:	w	Our weapon name
	//		d	our extra damage
	void set(const char* w, unsigned int d);

	// Calculate how much we're worth
	// 
	// Return: our value
	double Exp(void);

	// Show us
	virtual void show() const;

	// Show us (the smaller version)
	virtual void minishow() const;

	// Calculate our new damage
	virtual int damage(void) const;

	// Scream out our battle cry
	virtual void assault() const;

	// Console is our friend!
	friend Console &operator <<(Console &l, const gangsta &g);

protected:

	// Display what is unique to this class.
	void data(void) const;

	// Display a short form of what is unique to this class.
	void minidata(void) const;
};