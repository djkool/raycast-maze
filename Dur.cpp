#include <iomanip>
using std::setw;
using std::setfill;

#include "Console.h"
#include ".\dur.h"
#include "Utils.h"

extern Console con;

// Display us in color!
void Dur::show() const
{
	con << *this;
}

// Display us in color!
//
// In:	l	Where to send the output.
//		r	What object to display.
//
// Out:	l	The updated output.
//
// Return:	The updated output.
Console & operator <<(Console &l, const Dur &r)
{
	// Break it down...
	int h = r.minutes / Dur::MinHr, m = r.minutes % Dur::MinHr;

	// Show it!
	l << Red;
	if (h < 10)
		l << '0';
	l << h;
	l << Def;
	l << ':';
	l << Green;
	if (m < 10)
		l << '0';
	l << m;

	// Back to da usual!
	l << Def;

	// We're done - keep goin' if you want to.
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
ostream & operator <<(ostream &l, const Dur &r)
{
	// Break it down...
	int h = r.minutes / Dur::MinHr, m = r.minutes % Dur::MinHr;

	// Show it!
	l << setw(2) << setfill('0') << h << ':' << setw(2) << m << setfill(' ');

	// We're done - keep goin' if you want to.
	return l;
}

// Ask for a duration.
//
// In:	l	Where to read the input from.
//		r	Where to store their answers.
//
// Out:	l	An updated input stream.
//		r	What they typed.
//
// Return:	An updated input stream.
istream & operator >>(istream &l, Dur &r)
{
	int h;
	while (true)
	{
		con << Red << "Hours\t? ";
		if (l >> h)
			break;
		l.clear();
		eatline(l);
	}
	eatline(l);

	int m;
	while (true)
	{
		con << Green << "Minutes\t? ";
		if (l >> m)
			break;
		l.clear();
		eatline(l);
	}
	eatline(l);

	// Back to the usual colors!
	con << Def;

	// Now set the luvin'!
	r.setMinutes(m + h * Dur::MinHr);

	// Keep doin' input if you want.
	return l;
}
