#pragma once
#ifndef _RAYCASTER_H_
#define _RAYCASTER_H_

#include "Console.h"
extern Console con;

class RayCaster
{
public:
	//The one and only contructor
	//Initializes variables and allocates the buffer
	//	In:		ScreenX		X Resolution of the display
	//			ScreenY		Y Resolution of the display
	//			FOV			Feild of View of the Camera
	RayCaster(int ScreenX, int ScreenY, float FOV);

	//Destructor just calls some cleanup functions
	~RayCaster(void);



	/*******************/
	/*Specialty Methods*/
	/*******************/

	//Draw
	//Draws all rendering info into our buffer for later display
	//Might need to be modified for further capabilities
	//
	void Draw();

	//ClearBuffer
	//"Spaces out" the buffer essentially clearing it and adding NULL terminators to the end
	//
	void ClearBuffer();

	//ShowToScreen
	//Copies the buffer directly to the screen for faster redraw rates
	//
	void ShowToScreen();

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
	float CastRay( float x, float y, float theta);




	/**************/
	/*Manipulators*/
	/**************/

	//SetScreenSize
	//Sets or Resets the current screen size of the renderer.
	//This WILL delete all buffer info.
	//	In:		ScreenX		New X Resolution of the screen
	//			ScreenY		New Y Resolution of the screen
	//
	void SetScreenSize(int ScreenX, int ScreenY);

	//SetFOV
	//Sets the Feild of View for the class camera
	//	In:		fov		new FOV
	//
	void SetFOV(float fov) { if (fov > 0) m_FOV = fov; }

	//SetCameraOrient
	//Sets class camera position and direction
	//Values will be checked to make sure you can't go off the map
	//	In:		x		New x position of camera
	//			y		new y position of camera
	//			theta	new angle of direction
	//
	void SetCameraOrient(float x, float y, float theta);

	//SetMap
	//Deletes any current map, and replaces it with the supplied map. Also updates all map info
	//	In:		map		2D array of your map, for some reason it's bitchy about what you pass to it
	//			width	width of the new map, not including NULL terminator
	//			height	height of the new map
	//
	void SetMap(const char * map[], int width, int height);



	/***********/
	/*Accessors*/
	/***********/

	//GetScreenSize
	//Sticks the current screen info into the supplied variables
	//	In:		&x		Referenced variable to place X Screen Resolution
	//			&y		"							"Y Screen Resolution
	//
	void GetScreenSize(int & x, int & y) { x = m_ScreenX; y = m_ScreenY;}

	//GetCameraOrient
	//Fills in the supplied variables with the current orientation of the class camera
	//	In:		&x		Referenced variable to place X Camera Position
	//			&y		"							"Y Camera Position
	//			&theta	"							" the Camera's angular direction
	//
	void GetCameraOrient(float & x, float & y, float & theta) {x = m_CameraX; y = m_CameraY; theta = m_CameraRot;}

	//GetFOV
	//	Returns:	Current Feild of View of the class camera
	//
	float GetFOV() { return m_FOV; }


private:

	/*****************/
	/*Private Members*/
	/*****************/

	//Our back buffer where all display data is written to. Since the data is not written directly to the screen,
	//you don't have to visually clear it which cause flickering and tearing. Once the buffer is filled, it is then
	//force feed to the output stream as fast as possible. It's a 2D dynamic array created on construction
	char ** m_Buffer;

	//Current Screen size info
	int m_ScreenX, m_ScreenY;

	//Camera Position and rotation angle. Also the cameras Field of View
	float m_CameraX, m_CameraY, m_CameraRot;
	float m_FOV;

	//Current Map info. Another 2D dynamically allocated array. I originally didn't want the map to be
	//copied into the class. But I had a hard time just storing a pointer to a 2D array.
	//Also the current maps Width and Height variables
	char ** m_Map;
	int m_MapWidth;
	int m_MapHeight;


	/*****************/
	/*Private Methods*/
	/*****************/

	//Some hidden cleanup methods for deleting the memory allocated for both the
	//buffer and the map
	void DeleteBuffer();
	void DeleteMap();

	//Hardware video mode change
	void SetGraphicsMode(int mode);

};

#endif
