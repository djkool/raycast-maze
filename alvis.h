// Alvis.h - God of the Western world.

#pragma once
#include "npc.h"

class alvis : virtual public npc
{
	private:
		// Area o' effect
		int aoe;

		// Name of the spell.
		string spell;

	public:
		// Powerful constructor.
		//
		// In:  spl		Name of your spell.
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
		alvis(const char *spl = "Magic Missile", int aoe = 1,
			const char *nme = random_name(), short str = Roll3d6(),
			short dex = Roll3d6(), 
			short iq = Roll3d6(), short hlt = Roll3d6(),
			int chp = 20,
			int mhp = random(20, 15),
			double ep = 0, byte f = 0) : npc(nme, str, dex, iq, hlt, chp, mhp, ep, f), spell()
		{
			set(spl, aoe);
		}

		// Specialized constructor.
		//
		// In:  spl		Name of your spell.
		//		aoe		Area o' Effect.
		//		n		The npc to copy!
		alvis(const char *spl, int aoe, const npc &n);

		// What's our area o' effect?
		int getAOE(void) const
		{
			return aoe;
		}

		// Set an alvis.
		//
		// In:  spl		Name of your spell.
		//		aoe		Area o' Effect.
		void set(const char *spl, int aoe);

		// Display a character's stats on one line.
		virtual void minishow() const;

		// Display a player's stats.
		virtual void show() const;

		// Yell our Battle Cry!
		void assault() const;

	protected:
		// Display what is unique to this class.
		void data(void) const;

		// Display a short form of what is unique to this class.
		void minidata(void) const;
};
