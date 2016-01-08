// maze.h : All things map releated.

#ifndef _MAZE_H_
#define _MAZE_H_

#include <string>
using std::string;
#include <vector>
using std::vector;

// Some handy maze symbols.
enum Mazies { Entrance = '1', Exit = '2', Wall = 'Û' };

// Some handy path symbols.
enum Pathies { Searched = 'ù', North = 30, South, East = 16, West, };

class maze
{
public:
	// Set up a maze.
	//
	// In:  pzl     The map to copy from.
	//      r       The number of rows to copy.
	//      c       The number of columns to copy.
	maze(const char * const pzl[], int r, int c);
	// Make a singularity!
	maze(void) : mz()
	{
		this->rows = this->cols = 0;
	}

	// Check if they hit something.
	//
	// In:	ex  The player's horizontal coord.
	//      why The player's vertical coord.
	//
	// Return: true if they hit something.
	bool iswall(int ex, int why) const
	{
		return (ex < 0 || ex >= this->cols || why < 0 ||
			why >= this->rows || Wall == this->mz[why][ex]);
	}

	// Set up a maze.
	//
	// In:  pzl     The map to copy from.
	//      r       The number of rows to copy.
	//      c       The number of columns to copy.
	void set(const char * const pzl[], int r, int c);

	// Display a map.
	void draw(int i, int j) const;

	// Display a map.
	void show() const;

	// Search for an entrance into a map.
	//
	// Out: ex  The horizontal location of the entrance.
	//      why The vertical location of the entrance.
	//
	// Return: true if found.
	bool findentrance(int &ex, int &why) const;

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
	unsigned short find(maze &p, int x, int y, char t, bool ini) const;

	// Look in the maze at a particular location.
	//
	// In:	y	The row to check.
	//		x	The column to check.
	//
	// Return:	The symbol there or a Wall if naughty.
	char getSpot(int y, int x) const
	{
		if (x < 0 || x >= this->cols || y < 0 ||
			y >= this->rows)
			return Wall;
		return this->mz[y][x];
	}

	// Alter the maze at a particular location.
	//
	// In:	y	The row to change.
	//		x	The column to change.
	//		c	The new symbol.
	void setSpot(int y, int x, char c)
	{
		if (x < 0 || x >= this->cols || y < 0 ||
			y >= this->rows)
			return;
		this->mz[y][x] = c;
	}

private:
    // The map.
    vector<string> mz;

    // The size of the map.
    int rows, cols;
};

#endif