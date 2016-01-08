#include "gangsta.h"
#include "Console.h"
extern Console con;

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
gangsta::gangsta(const char* w, unsigned int d, const char *nme, short str,
		short dex, 
		short iq, short hlt,
		int chp,
		int mhp,
		double ep, byte f) : npc(nme, str, dex, iq, hlt, chp, mhp, ep, f)
{
		set(w,d);
}

// Fill us in
//
// In:	w	 Our weapon name
//		d	 our extra damage
//		n	 The npc we used to be
gangsta::gangsta(const char* w, unsigned int d, const npc& n) : npc(n)
{
	set(w,d);
}

// Set us up the new stuff
//
// In:	w	Our weapon name
//		d	our extra damage
void gangsta::set(const char* w, unsigned int d)
{
	this->wpn = w;
	dam = d;
}

// Calculate how much we're worth
// 
// Return: our value
double gangsta::Exp(void)
{
	return npc::Exp() + dam;
}

// Show us
void gangsta::show() const
{
	npc::show();
	con << "Weapon\t\t\t: " << wpn << "(+ " << dam << ")\n";
}
// Show us (the smaller version)
void gangsta::minishow() const
{
	npc::minishow();
	con << ", " << wpn << " (+" << dam << ")";
}
// Calculate our new damage
int gangsta::damage(void) const
{
	return npc::damage() + dam;
}
	
// Scream out our battle cry
void gangsta::assault() const
{
	con << FGRed << BGWhite << "caps" << Def << ' ';
}

Console &operator <<(Console& l, const gangsta &g)
{
	g.show();
	return l;
}

// Display what is unique to this class.
void gangsta::data(void) const
{
	con << "Weapon\t\t\t: " << wpn << "(+ " << dam << ")\n";
}

// Display a short form of what is unique to this class.
void gangsta::minidata(void) const
{
	con << ", " << wpn << " (+" << dam << ")";
}