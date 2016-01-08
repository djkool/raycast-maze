// rpg.cpp : A complete role playing game.

#include <iostream>
#include <new>
#include <cctype>
#include <windows.h>
#include <typeinfo>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <functional>
#include <map>
#include <iomanip> // setfill, setw, setprecision
#include <math.h>
using namespace std;

#include "Console.h"
#pragma comment(lib, "ConsoleD") // Link the Debug Console library.

#include "npc.h"
#include "encounter.h"
#include "utils.h"
#include "maze.h"
#include "Dur.h"
#include "Gangsta.h"
#include "alvis.h"
#include "Stack.h"
#include "Pimpassmofo.h"
#include "RayCaster.h"

/***********/
/* Globals */
/***********/

// Color is good!
Console con(80, 50, true, false);

/**************/
/* Prototypes */
/**************/

void example(void);
float FindTargetAngle(maze & m, int x, int y);

/************/
/* Internal */
/************/
#define PI 3.1415926535f
#define STEP 0.1f
#define ROTSTEP 0.0872664626f
#define FOV 1.04719755f
#define MISC_STEP 0.01f

// Our primary routine.  This is called when the program starts.
//
// Return:	We always return 0.
int main(void)
{
	// Let's see some trailing zero's & 2 decimal places.
    cout << fixed << setprecision(2);

    // Seed the random number generator.    
    randomize();
	
#if 0
	alvis bob("Fireball", 3, "Bob");
	alvis copy(bob);

	bob.show();
	copy.show();
#endif


#if 0
	try
	{
		example();
//		con << "Success!\n";
	}
	catch (...)
	{
		con << "Example() failed in some mysterious way.\n";
	}
#else
    // The map to wander through.
    const char *Map[] =
    {
        "ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл",
        "л1  лa            лaл                aл                 e+л       л           л",
        "ллл лллbллллллл л л л л ллллллллллл ллл ллллллллллллллллллл л ллл л ллллллллл л",
        "лaл     л   л   л   л л   л     лaл     л   л               л л   л л   л   л л",
        "л ллл ллл л л ллллллл ллл л ллл л л ллллл л л ллллллллллллллл л ллллл л л л л л",
        "л л   л   л   л       л+л   л   л л л   лaл   лaл     л    aл л л   л л   л л л",
        "л л л л ллллллл лллллллdл ллл ллл л л л ллллл л л л л л ллллл л л л л ллллл л л",
        "л л лaл  aл   лaл       л  aл     лaл л   л   л   лaл   л     л л лaл   л     л",
        "л л ллл ллл л ллл ллл ллл ллллл ллллл ллл л ллл ллллллл л ллллл л ллллл л ллллл",
        "л л   л   лaл   л  aл c л л   л л   лaл     л   л       л лaл   л     лaл     л",
        "л ллл л л ллл л ллллл л л л л л л л ллл ллллл ллл ллллллл л л ллл ллл ллл ллл л",
        "л   л л л л   л     л л л лaл л л л л   лa    л        aл   л     лaл     лgл л",
        "ллл л л л л ллллллл л л л ллл л л л л ллллллллл л ллллллл ллллллллл л ллллл л л",
        "л     лaл   л     л   л л   л л л л л   л   л   л л   л   л   лa    л   лaл л л",
        "л ллллллллллл ллл ллллл ллл л л л л ллл л л л ллл л л л ллл л ллл ллллл л л л л",
        "л   лa        л   л  aл     л л   л     л л   лa  л лaл     л     л     л л л л",
        "л л ллл ллллллл ллл ллл ллллл ллл ллллллл ллллллл л ллллл ллллллллл ллллл л л л",
        "л л     л   лaл лa    л лa      л      aл лaл   лaл   л   л     л+л     л   л л",
        "л ллллллл л л л ллллл л ллл ллл ллллллллл л л л ллллл л ллл ллл лfллл л л ллл л",
        "л лa      л   л   лaл л   л   л         л л л л     л   л   лa    л   л   л   л",
        "л ллл ллллллл ллл л л л л ллл ллллллллл л л л ллллл ллллл ллллллллл л ллллл ллл",
        "л     лa       aл     лaл     лaaaaaaaa     л    aл                 л    aл  2л",
        "ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл",
    };

    // Calculate the size of the map.
    const int Cols = (const int)strlen(Map[0]);
    const int Rows = sizeof Map / sizeof Map[0];

    maze sealab(Map, Rows, Cols);
	RayCaster spacelab(con.Cols(), con.Rows()-1, FOV);
	spacelab.SetMap(Map, Cols, Rows);

    // The player's location.
    float x, y,theta, targettheta;
	int tx, ty;
	targettheta = theta = 0;

    if (sealab.findentrance(tx, ty))
    {
        char answer;
        do
        {
            con << "What would you like to do (N)pc, (R)andom, (A)sk or (L)oad ? _\b";
            cin >> answer;
            eatline();
            answer = tolower(answer);

        } while (answer != 'n' && answer != 'r' && answer != 'a' && answer != 'l');

        // The starting stat for our players.
        const int Average = 15;

        const int Start_hp = 30;
					  
        auto_ptr<npc> hero(new npc);

        // What did they pick?
        switch (answer)
        {
            case 'n':
                hero->set("Quinn", Average, Average, Average, Average,
                    Start_hp, Start_hp, 0, Player);            
            break;

            case 'r':
				hero->setFlags(hero->getFlags() | Player);
            break;

            case 'a':
			while (true)
				try
				{
					cin >> *hero;
					break;
				}
				catch (...)
				{
					cin.clear();
					eatline();
				}
//                hero->ask();
            break;

            default:
                hero->load();
            break;
        }

		// Did they get the Get-out-of-death-free (GOODF) card?
		if (rand() % 100 == 0)
			hero->setFlags(hero->getFlags() | GOODF);
                
        // Remove the annoying blinking cursor.
        con.CursorOn(false);
        
		// Function pointers to all our fixed encounters.
		//	Function pointers can serve as functors.
		void (*pf[])(npc &) =
		{
			Enc::e1, Enc::e2, Enc::e3, Enc::e4, Enc::e5, Enc::e6,
				Enc::e7,
		};

		maze path;
		char target = 'a';
		sealab.find(path, tx, ty, target, true);

		// How long have we been here?
		Dur tme;
		int counter = 0;

		struct we
		{
			// Wanderin' time!
			int when;

			// Who you gonna call?  Ghostbusters or Jenny (867-5309).
			void (*pf)(npc &);

			// Have we done this?
			bool done;
		} wes[] =
		{
			{ random(     45,      15), Enc::we1, },
			{ random(12 * 60, 10 * 60), Enc::we2, },
			{ random(30 * 60, 25 * 60), Enc::we3, },

		};
		const int NumWEs = sizeof wes / sizeof wes[0];

        // Main game loop.
		bool showPath = false;
		bool autoWalk = false;
		x = tx + 0.5f; y = ty + 0.5f;
		spacelab.SetCameraOrient(x,y,theta);

        while (hero->alive())
        {
				//Show our RayCasted World
				spacelab.Draw();
				spacelab.ShowToScreen();

				if (showPath)
				{
					// Display the map.
					con.Goto(0, 0);
					path.show();
					// Display the player.
					con.Goto(y, x);
					extern bool godMode;
					if (godMode)
						con << BGWhite << FGBlack << char(1) << Def;
					else
						con << Green << char(2) << Def;
				}

                // Display the player's stats.
				con.Goto(con.Rows() - 2);
                hero->minishow();
				con << (180/PI) * targettheta;
				con.Goto(con.Rows() - 1, con.Cols() - 20 - 1);
				con << "Crates: " << Enc::GetCrates() << ' ';
				cout << tme;

				if (Exit == sealab.getSpot(y, x))
                    break;

                // Fixed encounters.
				char symbol = sealab.getSpot(y, x);
				if (isalpha(symbol))
				{
					int which = symbol - 'a';
					(*pf[which])(*hero);
                    sealab.setSpot(y, x, ' ');
                    continue;
				}
				if (sealab.getSpot(y,x) == '+')
				{
					hero.reset(Enc::promote(hero.release()));
					sealab.setSpot(y,x,' ');
					continue;
				}


				// Wanderin' luvin'!
				for (int i = 0; i < NumWEs; i++)
				{
					if (!wes[i].done && tme >= Dur(wes[i].when))
					{
						(*wes[i].pf)(*hero);
						wes[i].done = true;
						continue;
					}
				}

				// Did a wanderin' encounter kill ya?
				if (!hero->alive())
					continue;

            int key;
            do
            {
              key = con.get();

            } while (!autoWalk && key == 0);

            if (key == Esc)
            {
                // Die bizzzzzzzatch!
				hero->setCurrHP(0);
                break;
            }

			if (Tab == key)
			{
				// Switch views!
				showPath = !showPath;
				continue;
			}

			if (F12 == key)
			{
				// Cheaters == Prospers!
				extern bool godMode;
				godMode = !godMode;
				continue;
			}

			if (Enter == key)
			{
				// Walkies!
				autoWalk = !autoWalk;
				if(autoWalk)
					targettheta = FindTargetAngle(path, x, y);
				continue;
			}

            if (key == F5)
            {
                // Save to disk.
                hero->save();
                continue;
            }

			// Cheatin'!
			if (key < Extended && (isalnum(key) || ispunct(key) ))
			{
				// Change targets!
				target = key;
				if (USHRT_MAX == sealab.find(path, x, y, target, true))
					autoWalk = false;
				else
					autoWalk = true;
				targettheta = FindTargetAngle(path, x, y);
				continue;
			}

			spacelab.GetCameraOrient(x,y,theta);

			if (autoWalk)
			{
				if (path.getSpot(y, x) == Searched)
				{
					// Find the next target!
					if (USHRT_MAX ==
						sealab.find(path, x, y, target, true))
					{
						// Leave!
						target = Exit;
						sealab.find(path, x, y, target, true);
					}
					targettheta = FindTargetAngle(path, x, y);
				}

				if (theta > (targettheta - ROTSTEP) && theta < (targettheta + ROTSTEP))
				{
				key = Up;
				theta = targettheta;
				}
				else
				{

					if ( theta + ((theta + PI) >= (2*PI) ? -PI + ROTSTEP/2 : PI - ROTSTEP/2) > targettheta)
						key = Left;
					else
						key = Right;
				}
				//Sleep(10);
			}

            // Store the player's current position.
			float ox, oy;
			ox = x; oy = y;

            if (key == Up)
			{
				x += STEP * cosf(theta);
				y -= STEP * sinf(theta);
				//Very crude collision detection
				if (spacelab.CastRay(x,y,theta) < 0.2f)
				{	
					x = ox;	y = oy;
					if (autoWalk)
					{
						x = int(x) + 0.5f;
						y = int(y) + 0.5f;
						targettheta = FindTargetAngle(path, x, y);
					}

				}
			}
            if (key == Down)
			{
				x -= STEP * cosf(theta);
				y += STEP * sinf(theta);
				//Very crude collision detection
				if (spacelab.CastRay(x,y,theta + PI) < 0.1f)
				{	x = ox;	y = oy; }
			}
            if (key == Right)
                theta -= ROTSTEP;
            if (key == Left)
                theta += ROTSTEP;

			spacelab.SetCameraOrient(x,y,theta);
            
			if(ox != x || oy != y)
			{
				
				// Walkies!
				if(counter++ > 20)
				{
					tme = tme + Dur(1);
					counter = 0;
				}

				// Time to get bettah?
				int when = 30 - hero->getAttr(Hlt);
				if (when < 1)
					// Wolverine!
					when = 1;
				if (!(tme.getMinutes() % when))
					hero->heal(hero->getMaxHP() / 10);
			}
        }
        
        // Wipe the map.
        con.Clear();        

        if (hero->alive())
            con << Green << "Alvislua!\n" << Def;
        else
            con << BGRed << FGWhite << "You loose your HDTV!\n" << Def;

		con << *hero;
//        hero->show();
    }
    else
        con << BGRed << FGWhite << "No Entrance Found Bizzzzzzatch!";
#endif

	// Get ready to return to the operating system.
	con << Def; // Revert to default colors.
	// Center the "Press..." message on last line.
	con.Goto(con.Rows() - 1, (con.Cols() - 25) / 2);
	return 0; // No problemos!
}

float FindTargetAngle(maze & m, int x, int y)
{
	float targettheta = 0;

	switch (m.getSpot(y, x))
	{
	case North:
		targettheta = PI/2;
		break;
	case East:
		targettheta = 0;
		break;
	case South:
		targettheta = 3 * PI/2;
		break;
	case West:
		targettheta = PI;
		break;
	default:
		break;
	}

	return targettheta;
}
























//void show_array(const int p[], int num);
//void get_array(int *p, int num);

//void show_array(const int p[], int num)
//{
//    for (int i = 0; i < num; i++)
//        con << p[i] << ' ';
//}
//
//void get_array(int *p, int num)
//{
//    for (int i = 0; i < num; i++)
//    {
//        while (true)
//        {
//            con << "Enter a number ";
//            if (cin >> p[i])
//                break;
//
//            cin.clear();
//            eatline();
//        }
//        eatline();
//    }
//}

//int x = 100;

//int *p = &x;

//con << x << '\n';
//con << *p << '\n';

//con << &x << '\n';
//con << p << '\n';

//*p = 2;

//con << x;

// =====================================

//int a[3] = { 10, 20, 30 };

//con << a[0] << '\n';
//con << *(a + 2) << '\n';

//int *p = &a[1];

//con << *p;

// ================================================

    //int nummorons;
    //con << "How many morons are on the roof ? ";
    //cin >> nummorons;

    //int *morons = new int[nummorons];

    //morons[0] = 10;
    //
    //con << morons[0];

    //delete [] morons;

// ===========================================

    //int *p = new int;

    //*p = 90;

    //con << *p;

    //delete p;

    //p = new int;

    //*p = 60;

    //delete p;


// ================================

    //int x = 90;

    //int y = 100;

    //int *p = &x;

    //con << *p;

    //p = &y;

    //con << *p;
// ================================

    //int x = 10;

    //int y;

    //if (x == 20)
    //    y = 100;
    //else
    //    y = 200;

    //y = (x == 20) ? 100 : 200;


    //npc ray;

    //npc &x = ray;

    //set(&x, "Ray", 3, 3, 3, 3, 5, 5, 0);

    //show(&ray);

    //con << '\n';

    //show(&x);

    //delete [] ray.name;


    //int x = 100;

    //int *p = &x;

    //int &y = x;

    //con << "x  = " << x << '\n';
    //con << "*p = " << *p << '\n';
    //con << "y  = " << y;


    //int x = 100;

    //int &y = x;

    //y = 50;


    //con << x;

// BITWISE Business

//Decimal Base 10 = 0 -> 9
//Hexadecimal Base 16 = 0 -> F
//Octal Base 8 = 0 -> 7
//Binary Base 2 = 0 -> 1
//
//What is 1037 in Hex? 0x40D
//
//2^10	2^9	2^8	2^7	2^6	2^5	2^4	2^3	2^2	2^1	2^0
//1024	512	256	128	64	32	16	8	4	2	1
//
//4	2	1	8	4	2	1	8	4	2	1
//1	0	0	0	0	0	0	1	1	0	1


//unsigned char y = 52 >> 3;
//
//128	64	32	16	8	4	2	1
//
//0	0	1	1	0	1	0	0	// 52
//0	0	0	1	1	0	1	0	// 26
//0	0	0	0	1	1	0	1	// 13
//0	0	0	0	0	1	1	0	//	6

//| OR - Turns bits on.
//
//Bit 1 | Bit 2 | Result
//----------------------
//1	  | 0	  | 1
//0	  | 1	  | 1
//0	  | 0	  | 0
//1	  | 1	  |	1
//
//unsigned char x = 14 | 3;
//
//128	64	32	16	8	4	2	1
//
//0	0	0	0	1	1	1	0 // 14
//0	0	0	0	0	0	1	1 // 3
//0	0	0	0	1	1	1	1 // 14 | 3

//^ XOR - Toggles bits.
//
//Bit 1 | Bit 2 | Result
//----------------------
//1	  | 0	  | 1
//0	  | 1	  | 1
//0	  | 0	  | 0
//1	  | 1	  |	0
//
//unsigned char x = 75 ^ 62;
//
//128	64	32	16	8	4	2	1
//0	1	0	0	1	0	1	1 // 75
//0	0	1	1	1	1	1	0 // 62
//
//0	1	1	1	0	1	0	1 // 117

//& AND - When used in conjunction with ~, turns bits off.
//
//Bit 1 | Bit 2 | Result
//----------------------
//1	  | 0	  | 0
//0	  | 1	  | 0
//0	  | 0	  | 0
//1	  | 1	  |	1
//
//unsigned char x = 63 & 17;
//
//128	64	32	16	8	4	2	1
//0	0	1	1	1	1	1	1 //63
//0	0	0	1	0	0	0	1 //17
//0	0	0	1	0	0	0	1 //17
//
//~ NEG - Inverts all the bits.
//
//unsigned char x = ~37;
//
//128	64	32	16	8	4	2	1
//0	0	1	0	0	1	0	1 //37
//1	1	0	1	1	0	1	0 //~37

//& with ~ : Turn them bits off.
//
//unsigned char x = 37 & ~32;
//
//128	64	32	16	8	4	2	1
//0	0	1	0	0	0	0	0 //32
//1	1	0	1	1	1	1	1 // ~32
//
//0	0	1	0	0	1	0	1 // 37
//
//0	0	0	0	0	1	0	1 // 37 & ~32

/*
Call Stack (Abstract Data Type)
OS -> main() -> example() -> a() -> b() -> c()
*/

/*
OS -> main() -> example() -> a(5) -> a(4) -> a(3) -> a(2)
-> a(1) -> a(0)
*/

void a(int count)
{
	if (!count)
		return;
	con << "I'm in a() with a count of " << count << "!\n";
	a(count - 1);
	con << "I'm back in a() with a count of " << count << "!\n";
}

struct friggin_huge
{
	int array[30000];
};

typedef Dur Type;

void print(const Type &d)
{
	con << d << ", ";
}

typedef string First;
typedef double Second;
#define PAIR First, Second
typedef pair<PAIR> his;

void luvin(const his &d)
{
	con << Green << d.first;
	int x, y;
	con.CP(x, y);
	con.Goto(y, 25);
	cout.width(13);
	con << Magenta << d.second;
	con << Def << '\n';
}

void example(void)
{
#if 0 // Playing with i/o streams...

	/*int x = 0;
	while (true)
	{
		cout << "Enter an int: ";
		try
		{
			cin >> x;
			break;
		}
		catch (ios_base::failure &x)
		{
			cin.clear();
			while (cin.get() != '\n') { }
		}
	}
	cout << x << '\n';*/


	char temp[100];
	cout << "Enter a string...\n";
	cin.get(temp, 100, '#');

	cout << "Here's what was extracted from the input buffer:\n";
	cout << temp << '\n';

	cout << "Here's how many characters were extracted: ";
	cout << cin.gcount() << '\n';

	cout << "Ignoring 5 characters, or the $, whichever comes first!\n";
	cin.ignore(5, '$');

	cout << "P-ing in the input stream...\n";
	cin.putback('P');

	cout << "This is the first character in the input buffer: ";
	cout << (char)cin.peek() << '\n';

	cout << "\nHere's what was left in the input buffer:\n";
	char x;
	while ((x = cin.get()) != '\n')
		cout << x;

#endif

#if 0 // Manipulator bidness...
	
	// Show them trailing zeros...
	cout << "Using showpoint...\n";
	cout << showpoint << 100.2345F << '\n' << noshowpoint << 1.0f << '\n';

	// Show bool values in English...
	cout << "Using boolalpha...\n";
	bool x = 0;
	cout << boolalpha << x << '\n' << noboolalpha << x << '\n';

	// Show the positive sign...
	cout << "Using showpos...\n";
	cout << showpos << 12.012 << '\n' << -7 << '\n' << noshowpos;

	// Show hex...
	cout << "Using hex...\n";
	hex(cout);
	cout << showbase << uppercase << 255 << '\n';
	cout << noshowbase << nouppercase << dec << 255 << '\n';

	// Show oct...
	cout << "Using oct...\n";
	cout << showbase << oct << int('A') << '\n';
	cout << noshowbase << dec << int('A') << '\n';

	// cin with different number bases
	/*int val;
	cout << "Enter a number in hex...";
	cin >> hex >> val;
	cout << val << '\n';
	cout << "Enter a number in oct...";
	cin >> oct >> val;	
	cout << val << '\n';*/
	// Put it back to normal... >> dec;
	//dec(cin);

	cout << "Using fields...\n";
	// Set a 10-character wide field.
	cout << setw(10) << setfill('$') << left << "Hello";
	cout << setw(10) << setfill(' ') << right << "World!" << '\n';

	// Using internal justification...
	cout.width(10);
	cout << internal << showpos << 1.03f << '\n';

#endif

#if 0
	his p[] =
	{
		his("Slardibartfast", 1e9),
		his("Bob", 79.5),
		his("Toilet", -3.1415926535),
		his("Canada", 2.1),
		his("Canada", 200.1),
	};
	const int Num = sizeof p / sizeof p[0];

	multimap<PAIR, greater<First> > m(p, p + Num);
	//if (1e12 == m["Uber Player"])
	//	con << "Got one!\n";
	//else
	//	con << "Nope...\n";
	//m["Uber Player"] = 1e12;
	for_each(m.begin(), m.end(), luvin);
	con << "Getting a little pickier...\n";
	for_each(m.lower_bound("T"), m.upper_bound("C"), luvin);
#endif

#if 0
	typedef multiset<Type, greater<Type> > mine;

	mine s;
	s.insert(45);
	s.insert(1.5);
	s.insert(15);
	s.insert(2.0);
	s.insert(5);
	for (mine::iterator i = s.begin(); i != s.end(); ++i)
		con << *i << ", ";
	con << '\n';

	Type arr[] =
	{
		Type(15),
		Type(30.9),
		Type(437),
		Type(1),
		Type(1000000),
	};
	const int Num = sizeof arr / sizeof arr[0];
	mine t(arr, arr + Num);
	for_each(t.begin(), t.end(), print);
	con << '\n';

	mine u;
	insert_iterator<mine> ii(u, u.begin());
//	set_union(s.begin(), s.end(), t.begin(), t.end(), ii);
	copy(s.begin(), s.end(), ii);
	copy(t.begin(), t.end(), ii);
//	ostream_iterator<Type> oi(cout, ", ");
	copy(u.begin(), u.end(), ostream_iterator<Type>(cout, ", "));
	con << '\n';
	con << "We found: ";
//	con << *u.find(15);
	for (mine::iterator i = u.lower_bound(16);
		i != u.upper_bound(15); i++)
		con << *i << ", ";
//	for_each(u.begin(), u.end(), print);
#endif

#if 0
	con << "How many players are playin'? ";
	int num;
	cin >> num;
	eatline();

	vector<double> v;
	for (int i = 0; i < num; i++)
		v.push_back(i + 0.5);

	for (int i = 0; i < num; i++)
		con << "#" << v[i] << '\n';
#endif

#if 0
	double value = 3.141592654653589;
	// TODO:  Have Bob debug this over the weekend.
	const unsigned long long *p = reinterpret_cast<unsigned long long *>(&value);
	con << "As a whole number, it's " << *p << "!\n";
#endif

#if 0
	struct guava
	{
		int ten;
		bool fred;
		mutable int count;
	};

	const guava t;
	++t.count;
#endif

#if 0
/*
	volatile int x, y;

	main memory
	cache
	register
*/

	const int Num = 10;
	con << "Num = " << Num << "!\n";
	con << "Num's address is " << &Num << "!\n";
	int *p = const_cast<int *>(&Num);
	*p = 5;
	con << "*p = " << *p << "!\n";
	con << "p is pointin' at " << p << "!\n";
	con << "Num = " << Num << "!\n";
#endif

#if 0
	alvis pa;
	npc &mom = pa;
	try
	{
		Pimpassmofo &me = dynamic_cast<Pimpassmofo &>(mom);
		con << "We did it!\n";
	}
	catch (bad_cast &bc)
	{
		con << bc.what() << '\n';
	}
#endif

#if 0
	int i;
	con << typeid(i).raw_name() << '\n';
#endif

#if 0
	npc *mom = new npc;
	mom->show();

	if (typeid(Pimpassmofo) == typeid(*mom))
		con << "We're a PAM!\n";
	else if (typeid(alvis) == typeid(*mom))
		con << "We're an Alvis!\n";
	else if (typeid(gangsta) == typeid(*mom))
		con << "We're a Gangsta!\n";
	else
		con << "We're an NPC!\n";

	delete mom;
/*
	alvis *me = dynamic_cast<alvis *>(mom);
	if (!me)
		con << "Doh, I wasn't the Alvis!\n";
	else
		con << "Hail to the King, baby!";

	delete mom;
*/
#endif

#if 0
	Stack<friggin_huge> st;
	friggin_huge toilet;
	for (int i = 0; ; i++)
	{
		con << "Trying to push #" << i + 1 << "...\n";
		st.push(toilet);
	}
#endif

#if 0
	struct friggin_huge
	{
		int array[100];
	};
	try
	{
		friggin_huge *what = new friggin_huge[100];
		// Fill in everything and print it.
		con << "Got the mem - how fast is your computer running now?";
		delete [] what;
	}
	catch (std::bad_alloc &ba)
	{
		con << ba.what() << '\n';
	}
#endif

#if 0
	npc test;
	test.show();
	int index = -8;
	while (true)
	{
		try
		{
			con << "Adding to attribute " << (test[index] += 2) << "!\n";
			con << "Done!\n";
			break;
		}
		catch (npc::bad_index &s)
		{
//			con << "The index is " << index << ".\n";
			con << s.what() << '\n';
			++index;
			throw;
		}
		catch (bad_exception &be)
		{
			con << be.what() << "!\n";
			break;
		}
		catch (exception &e)
		{
			con << e.what() << '\n';
		}
		catch (...)
		{
			con << "Toilet!\n";
			break;
		}
		con << "Hi!\n";
	}
#endif

#if 0
	const npc Tutor("Master Varmint");
	Tutor.show();
//	goya[Str] += 5;
//	goya.setAttr(Str, goya.getAttr(Str) + 5);
	con << "Strength = " << Tutor[Str] << "!\n";
#endif

#if 0 // ABC's - abstract base classes!

	// An abstract base class!
	class BasePlayer
	{
	public:
		BasePlayer(void) { }

		// A PURE VIRTUAL function...
		virtual ~BasePlayer(void) = 0;

		virtual void show(void) const = 0;
	};

	class QB : public BasePlayer
	{
	public:
		QB(void) { }
		virtual void show(void) const { cout << "QB show\n"; }
	};

	BasePlayer *p = new QB;

	//p->thing();
	p->show();

#endif

#if 0 // Queue bidness
	
	Queue<int> q(50);
	if (q.isFull())
		cout << "It's full!\n\n";
	if (q.isEmpty())
		cout << "It's empty!\n\n";

	for (int i = 0; i < 100; i++)
		if (!q.Enqueue(i + 1))
			break;

	if (q.isFull())
		cout << "It's full!\n\n";

	cout << "Number o things in the queue: " << q.Size() << '\n';

	int x;
	while (q.Dequeue(x))
		cout << x << '\t';

	q.Destroy();
	if (q.isEmpty())
		cout << "It's empty!\n\n";

	cout << "\nNumber o things in the queue: " << q.Size() << '\n';

#endif


#if 0
	// Find the largest value using a pointer.
	int arr[] = {13, 35, 46, 45, 12};
	const int size = sizeof arr / sizeof arr[0];

	int *biggest = arr;
	for ( int i = 1; i < size; i++)
		if (*biggest < arr[i])
			biggest = &arr[i];

	cout << "The largest value is " << *biggest << '\n';
#endif

#if 0
	Dur first = 1.5, two = 91;
	if (first > two)
		con << "first is bigger";
	else
		con << "two is bigger";
#endif

#if 0
	Dur bomb;
	cin >> bomb;
	cout << "You have " << bomb <<
		" minutes to reach minimum safe distance!\n";
#endif

#if 0 // Stack bidness
	Stack<double> s;
	cout << "Pushing stuff onto the Stack...\n";
	for (int i = 0; i < 100; i++)
	{
		cout << i + 1 << '\t';
		s.push(i + 1);
	}

	cout << "\n\nPopping stuff off the Stack...\n";
	double x;
	while (s.pop(x))
		cout << x << '\t';
#endif

#if 0
	const int Huge = 500000;
	npc *army = new npc[Huge];
	for (int i= 0; i < Huge; i++)
	{
		army[i].minishow();
		con << '\n';
	}
	delete [] army;
#endif

#if 0
	con << "I'm in example()!\n";
	a(5);
	con << "I'm back in example()!\n";
#endif

#if 0
	int (*r)(int, int) = random;
	int roll = random(25, 15);
	con << "Roll = " << roll << "!\n";
	int roll2 = r(100, 50);
	con << "R = " << roll2 << "!\n";
#endif

#if 1
	
#endif
}

// qsort is friend