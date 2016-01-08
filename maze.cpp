// maze.cpp : All things map releated.

#include <cstring>
#include <map>
using std::pair;
using std::map;

#include "Console.h"

#include "maze.h"

extern Console con;

// Set up a maze.
//
// In:  pzl     The map to copy from.
//      r       The number of rows to copy.
//      c       The number of columns to copy.
maze::maze(const char * const pzl[], int r, int c) : mz()
{
	this->set(pzl, r, c);
}

// Set up a maze.
//
// In:  pzl     The map to copy from.
//      r       The number of rows to copy.
//      c       The number of columns to copy.
void maze::set(const char * const pzl[], int r, int c)
{
	// Out with the old!
	this->mz.clear();

    this->rows = r;
    this->cols = c;

	if (0 == r || 0 == c || 0 == pzl)
		// No maze?  Adios!
		return;

	// Deep copy!
	this->mz.insert(this->mz.begin(), pzl, pzl + this->rows);
  //  for (int i = 0; i < this->rows; i++)
		//this->mz.push_back(pzl[i]);
}

// Display a map.
void maze::draw(int i, int j) const
{
	switch (this->mz[i][j])
	{
	case Entrance:
        con << Green << 'ï' << Def;
		break;
	case Exit:
        con << BGRed << FGWhite << 'ï' << Def;
		break;
	case 'a':
        con << Yellow << 'ð' << Def;
		break;
	case 'b':
        con << Cyan << 'ë' << Def;
		break;
	case 'c':
        con << Red << 'ä' << Def;
		break;
	case 'd':
	case 'e':  
	case 'f':            
		con << ' ' << Def;
		break;
	
	case 'g':            
		con << Red << char(2) << Def;
		break;

	case '+':
		con << Green << '$' << Def;
		break;
	case Searched:
		con << DarkGray << static_cast<char>(Searched) << Def;
		break;
	case North:
	case East:
	case South:
	case West:
		con << Green << this->mz[i][j] << Def;
		break;
	default:
        con << this->mz[i][j];
		break;
	}
}

// Display a map.
void maze::show() const
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
			this->draw(i, j);
        con << '\n';
    }
}

// Search for an entrance into a map.
//
// Out: ex  The horizontal location of the entrance.
//      why The vertical location of the entrance.
//
// Return: true if found.
bool maze::findentrance(int &ex, int &why) const
{
    for (why = 0; why < this->rows; why++)
    {
		string::size_type where = this->mz[why].find('1');
		if (string::npos != where)
		{
			// Gotcha!
            ex = static_cast<int>(where);
            return true;
        }
    }
    return false;
}

// Locate a particular spot within a maze.
//
// In:	p	Where to place the guiding arrows.
//		x	Our current column.
//		y	Our current row.
//		t	The symbol to look for.
//		ini	true if this is the first time we've checked for
//				this target.
//
// Out:	p	Some arrows to follow.
//
// Return:	The number of steps to take to get to your
//				destination (0 = you're on it, otherwise
//				1+).  USHRT_MAX means we couldn't find it.
unsigned short maze::find(maze &p, int x, int y, char t, bool ini) const
{
	if (ini)
		// Initialization time!
		p = *this;

	if (this->iswall(x, y) || Searched == p.mz[y][x])
		// Dead end!
		return USHRT_MAX;

/* Let's see!
	con.Goto(y, x);
	con << BGWhite << FGBlack << ' ';
	for (int i = 0; i < 5000000; i++) { }
*/

	if (t == this->mz[y][x])
		// Found it!
		return 0;

	// Leave a little "present".
	p.mz[y][x] = Searched;

	// Recursive-fu!
#define PAIR unsigned short, char
	typedef pair<PAIR> mine;
	mine dirs[] =
	{
		mine(this->find(p, x, y - 1, t, false), North),
		mine(this->find(p, x + 1, y, t, false), East),
		mine(this->find(p, x, y + 1, t, false), South),
		mine(this->find(p, x - 1, y, t, false), West),
	};
	const int NumDirs = sizeof dirs / sizeof dirs[0];

	map<PAIR> result(dirs, dirs + NumDirs);

	// Which is the shortest one?
	unsigned short shortest = result.begin()->first;
	char arrow = result.begin()->second;
	if (USHRT_MAX == shortest)
		// Couldn't find it anywhere!
		return shortest;

	// Show 'em the way!
	p.mz[y][x] = arrow;

	// Found a winner!
	unsigned short fudge = 1;
	if ('a' != this->mz[y][x] && isalpha(this->mz[y][x]))
		// Take a trip 'round the world!
		fudge = this->rows * this->cols;
	return shortest + fudge;
}
