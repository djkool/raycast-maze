#include "raycaster.h"
#include <math.h>

//Some constant values
#define PI 3.1415926535f
#define TWOPI 6.283185307f
#define FVIEW 0.75f

//The one and only contructor
//Initializes variables and allocates the buffer
//	In:		ScreenX		X Resolution of the display
//			ScreenY		Y Resolution of the display
//			FOV			Feild of View of the Camera
RayCaster::RayCaster(int ScreenX, int ScreenY, float fov)
{
	//Initialize random variables
	m_Map = m_Buffer = NULL;
	m_CameraX = m_CameraY = m_CameraRot = 0;

	SetScreenSize(ScreenX, ScreenY);
	SetFOV(fov);
}

//Destructor just calls some cleanup functions
RayCaster::~RayCaster(void)
{
	DeleteMap();
	DeleteBuffer();
}

/*******************/
/*Specialty Methods*/
/*******************/

//Draw
//Draws all rendering info into our buffer for later display
//Might need to be modified for further capabilities
//
void RayCaster::Draw()
{
	//Error Checking
	if (!m_Buffer || !m_Map)
		return;

	//Wipe the slate
	ClearBuffer();

	//Same handy variables
	int cy = m_ScreenY/2;	//Center of screen
	float ntheta = m_CameraRot + m_FOV/2; // starting ray angle
	float itheta = m_FOV/m_ScreenX;	//Ray angle increment
	float MaxHeight = m_ScreenY/2 - 1; //Max height that can be stored
	float d;
	int h;

	for (int i = 0; i < m_ScreenX; i++)
	{
		//Get distance of ray, cosf is used to correct the Fishbowl effect
		d = (CastRay( m_CameraX, m_CameraY, ntheta ) + FVIEW) * cosf(m_CameraRot - ntheta);

		//calculate height of wall segment, could be where truncation errors occur
		h = static_cast<int>( d > 1 ? MaxHeight/d : MaxHeight);

		//Draw to buffer
		for(int p = 0; p < h; p++)
		{
			m_Buffer[cy-p][i] = '#';
			m_Buffer[cy+p][i] = '#';
		}
		if(h < MaxHeight)
		{
			m_Buffer[cy-h][i] = '-';
			m_Buffer[cy+h][i] = '-';
		}

		//Increment ray angle for next ray
		ntheta -= itheta;
	}
}


//ClearBuffer
//"Spaces out" the buffer essentially clearing it and adding NULL terminators to the end
//
void RayCaster::ClearBuffer()
{
	if(!m_Buffer)
		return;

	//memset fills in memory for a certain number of bytes
	//this fills each line of the buffer with spaces and adds a null terminator to each
	for (int i = 0; i < m_ScreenY; i++)
	{
		memset(m_Buffer[i], 32, m_ScreenX);
		m_Buffer[i][m_ScreenX] = 0;
	}
}

//ShowToScreen
//Copies the buffer directly to the screen for faster redraw rates
//
void RayCaster::ShowToScreen()
{
	if(!m_Buffer)
		return;

	//Force feed all buffer info to the console
	for(int i = 0; i < m_ScreenY; i++)
	{
		con.Goto(i);
		con << m_Buffer[i];
	}

}

//CastRay
//My original CastRay function modified ever so slightly to work in the class
//Casts a ray in the theta direction from from position x,y
//May need some tweaking, might be source of some bugs
//	In:		x		x position of camera (or player)
//			y		y position of camera (or player)
//			theta	angle of direction
//			*int value of both x and y are used as the "Cell position",
//				the decimal portion is the excact position within that "Cell"
//
//	Returns:		The distance to the nearest wall in the theta direction
//
float RayCaster::CastRay( float x, float y, float theta)
{
	if(!m_Map)
		return 0.0f;

	//Find "Cell" positon on map, Int value of the position
	int ix = static_cast<int>(x);
	int iy = static_cast<int>(y);

	//Check if cell is not empty, if so.. end recursion
	if ( m_Map[iy][ix] == 'Û' )
		return 0;

	//Calculate the current excact position in each cell, Decimal part
	float fx = x - ix;
	float fy = y - iy;

	//Calculate theta value to each each corner. 12 - topright, 23-topleft, 34-bottomleft, 41-bottomright
	float theta12 = ( 1.0f - fx ? atanf( fy /(1.0f - fx)) : PI/2);
	float theta23 = (	fx  ? PI - atanf( fy / fx )		 : PI/2);
	float theta34 = (	fx	? PI + atanf((1.0f - fy)/ fx ): PI*1.5f);
	float theta41 = ( 1.0f - fx ? TWOPI - atanf((1.0f - fy)/(1.0f - fx)) : PI*1.5f);

	//Check what wall the ray will hit by seeing which range of angles it falls between. 
	//Then using the ratio of theta vs the distance between the 2 angles... you can calculate the position
	//along the wall that the ray strikes.
	float dx, dy;

	//Top wall
	if (theta >= theta12 && theta < theta23)
	{
		dx = 1.0f - (theta - theta12)/(theta23 - theta12) - fx;
		dy = -fy - 0.0001f;//Quick fix for a roundoff bug
	}
	//Left Wall
	else if (theta >= theta23 && theta < theta34)
	{
		dx = -fx - 0.0001f;
		dy = (theta - theta23)/(theta34 - theta23) - fy;
	}
	//Bottom Wall
	else if (theta >= theta34 && theta < theta41)
	{
		dx = (theta - theta34)/(theta41 - theta34) - fx;
		dy = 1.0f - fy;
	}
	//Right Wall, a bit more complicated because theta could either be really high, or really low to fit
	//in this range. 
	else if (theta >= theta41 || theta < theta12)
	{
		dx = 1.0f - fx;
		float tmptheta = (theta < theta12 ? (TWOPI - theta41 + theta) : (theta - theta41) );
		dy = 1.0f - tmptheta/(TWOPI - theta41 + theta12) - fy;
	}

	//Fix any previous quick fixes
	if(fx >= 0.9999f && dx < 0)
		x += .0001f;
	if(fy >= 0.9999f && dy < 0)
		y += .0001f;

	//Find internal distance in cell. 
	float d = sqrtf(dx * dx + dy * dy);

	//Adding dx and dy to the old x and y values sticks the ray on a wall of the NEXT cell.
	//On return you add THAT cells distance to this cells distance, and return that number to the last cell.
	//When the function recurses through, it finds total distance from position to wall
	return d + CastRay(x + dx, y + dy, theta);
}




/**************/
/*Manipulators*/
/**************/

//SetScreenSize
//Sets or Resets the current screen size of the renderer.
//This WILL delete all buffer info.
//	In:		ScreenX		New X Resolution of the screen
//			ScreenY		New Y Resolution of the screen
//
void RayCaster::SetScreenSize(int ScreenX, int ScreenY)
{
	//Error checking
	if (ScreenX <= 0 || ScreenY <= 0)
	{
		//Clear Old info completely
		m_ScreenX = 0;
		m_ScreenY = 0;
		DeleteBuffer();
	}
	else
	{
		m_ScreenX = ScreenX;
		m_ScreenY = ScreenY;

		//Allocate new buffer space
		DeleteBuffer();
		m_Buffer = new char * [m_ScreenY];
		for(int i = 0; i < m_ScreenY; i++)
			m_Buffer[i] = new char[m_ScreenX + 1];
		//Fill in good data;
		ClearBuffer();
	}
}

//SetCameraOrient
//Sets class camera position and direction
//Values will be checked to make sure you can't go off the map
//	In:		x		New x position of camera
//			y		new y position of camera
//			theta	new angle of direction
//
void RayCaster::SetCameraOrient(float x, float y, float theta)
{
	//Error checking
	if(!m_Map)
		return;

	if(x < 0 || x >= m_MapWidth ||
		y < 0 || y >= m_MapHeight)
		return;

	//Everything is okay, set some data
	if (theta < 0) theta += TWOPI;
	if (theta >= TWOPI) theta -= TWOPI;
	m_CameraRot = theta;
	m_CameraX = x;
	m_CameraY = y;
}


//SetMap
//Deletes any current map, and replaces it with the supplied map. Also updates all map info
//	In:		map		2D array of your map, for some reason it's bitchy about what you pass to it
//			width	width of the new map, not including NULL terminator
//			height	height of the new map
//
void RayCaster::SetMap(const char * map[], int width, int height)
{
	//Error checking
	if (!map || width <= 0 || height <= 0)
		return;

	//Delete old map
	DeleteMap();

	//Allocate memory for map, and copy from source
	m_Map = new char *[height];
	for(int i = 0; i < height; i++)
	{
		m_Map[i] = new char[width + 1];
		strcpy(m_Map[i], map[i]);
	}

	//Copy map info
	m_MapWidth = width;
	m_MapHeight = height;
}

//Destroys the buffer memory
void RayCaster::DeleteBuffer()
{
	if (!m_Buffer)
		return;

	for(int i = 0; i < m_ScreenY; i++)
		delete [] m_Buffer[i];
	delete [] m_Buffer;

	m_Buffer = NULL;
}

//Destroys the map memory
void  RayCaster::DeleteMap()
{
	if (!m_Map)
	return;

	for(int i = 0; i < m_MapHeight; i++)
		delete [] m_Map[i];
	delete [] m_Map;

	m_Map = NULL;
}