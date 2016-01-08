#pragma once

#include <iostream>
using std::ostream;
using std::istream;

class Dur
{
private:
	// How many minutes are in an hour.
	static const int MinHr = 60;
//	enum { MinHr = 60 };

	// How long we've been around.
	int minutes;

public:
	// Create a new object/instance.
	//
	// In:	minutes	Our starting minutes.
	//		hours	Our starting hours.
	Dur(int minutes = 0, int hours = 0)
	{
		this->setMinutes(minutes + hours * this->MinHr);
	}
	// Create a new object/instance.
	//
	// In:	h	Our fractional hours.
	Dur(double h)
	{
		this->setMinutes(static_cast<int>(h * this->MinHr));
	}
	// Destructor.
	virtual ~Dur(void) { }

	// Relational operators.
#define DUR_REL_OP(op)						\
	bool operator op(const Dur &r) const	\
	{										\
		return this->minutes op r.minutes;	\
	}

	DUR_REL_OP(>)
	DUR_REL_OP(<)
	DUR_REL_OP(==)
	DUR_REL_OP(<=)
	DUR_REL_OP(!=)
	DUR_REL_OP(>=)

	// Conversion functions.
	operator int() const
	{
		return this->minutes;
	}
	operator double() const
	{
		return static_cast<double>(this->minutes) / Dur::MinHr;
	}

	// Arithmetic operators.
	Dur operator +(const Dur &r) const
	{
		return Dur(this->minutes + r.minutes);
	}
	Dur operator -(const Dur &r) const
	{
		return Dur(this->minutes - r.minutes);
	}
	Dur operator *(double r) const
	{
		return Dur(static_cast<int>(this->minutes * r));
	}

	// Return:	Our minutes.
	int getMinutes() const
	{
		return this->minutes;
	}

	// Alter our time.
	//
	// In:	minutes	Our new value.
	void setMinutes(int minutes)
	{
		this->minutes = minutes;
	}

	// Display us in color!
	void show() const;

	// Friends!
	friend Dur operator *(double l, const Dur &r)
	{
		return Dur(static_cast<int>(r.minutes * l));
	}

	// Display us in color!
	//
	// In:	l	Where to send the output.
	//		r	What object to display.
	//
	// Out:	l	The updated output.
	//
	// Return:	The updated output.
	friend Console & operator <<(Console &l, const Dur &r);

	// Display us.
	//
	// In:	l	Where to send the output.
	//		r	What object to display.
	//
	// Out:	l	The updated output.
	//
	// Return:	The updated output.
	friend ostream & operator <<(ostream &l, const Dur &r);

	// Ask for a duration.
	//
	// In:	l	Where to read the input from.
	//		r	Where to store their answers.
	//
	// Out:	l	An updated input stream.
	//		r	What they typed.
	//
	// Return:	An updated input stream.
	friend istream & operator >>(istream &l, Dur &r);
};
