/*
NOTE: This code was written using Microsoft's Visual C++ v 6.0.
I don't know how well it will work with other compilers.

You also will need the Direct X 7.0 SDK, and to install the lib
and include files from the SDK to your C++ lib and include
folders. You can get the DirectX 7.0 SDK (large!) here:
http://download.microsoft.com/download/win98SE/SDK/7.0/W9X/EN-US/dx7sdk-700.1.exe


========================
A* Pathfinder - The Maze
========================
By Patrick Lester, pwlester@policyalmanac.org

A fuller implementation than the Basics demo. This is the 
template you will want to work from if you want to write or
customize your own version of A*.

Instructions
------------
- Press enter to start pathfinding mode
- left or right click anywhere on the map to make the smiley
  go there.
- search times are printed in the upper left hand coner
- map may be edited by pressing enter button to toggle to
  map editing mode and left clicking on the map.

*/

#define WINDOW_NAME "Program" //Visible when game is minimized
#include "../common/launchWindows.h"
#include "../common/dxLibrary.h"
#include "aStarLibrary.h"

//-----------------------------------------------------------------------------
// Global variables and constants
//-----------------------------------------------------------------------------
cImage* wallImage; cImage* mapImage;
cImage* mousePointer; 
cImage* smiley; cImage* chaser;
char smileyActivated = 0;
int xLoc [4]; int yLoc [4]; int speed [4];
int searchTime, g_showDirections=0; 


//-----------------------------------------------------------------------------
// Function Prototypes: where necessary
//-----------------------------------------------------------------------------
void CheckUserInput (void);
void CreateMapImage (void);
void CreateWallImage (void);
void DrawMap (void);
void EditMap (void);
void LoadGraphics (void);
void LoadMapData (void);
void LoadUnitData (void);
void MoveChaser (int ID);
void MoveSmiley (void);
void MoveSprite(int ID);
void RenderScreen (void);
void SaveMapData (void);
void ShowDirections (void);


//-----------------------------------------------------------------------------
// Name: GameMain
// Desc: Launch and run game.
//-----------------------------------------------------------------------------
void GameMain (HWND hwnd)
{
	Graphics (800,600,16,hwnd);
	LoadMapData();
	LoadUnitData();
	LoadGraphics();
	InitializePathfinder();

	//Main game loop
    while (!KeyDown(27)) //While escape key not pressed
    {
		CheckUserInput();

		//Move smiley
		if (smileyActivated == 1) MoveSmiley();
	
		//Move chasers
		if (smileyActivated == 1)  
			for (int ID = 2; ID <= 3; ID++) 
				MoveChaser(ID);

		RenderScreen(); //draw stuff on screen
		CheckWinMessages();
	}	

	SaveMapData();
	EndPathfinder();
	EndGraphics();
	return;
}


//-----------------------------------------------------------------------------
// Name: CheckUserInput
// Desc: Process key and mouse presses.
//-----------------------------------------------------------------------------
void CheckUserInput (void) 
{
	if (KeyHit(13))
	{
		smileyActivated = 1 - smileyActivated;
		if (smileyActivated == 1) CreateMapImage();
	}
	if (smileyActivated == 0) EditMap();

	//Show/hide directions by pressing space bar.
	if (KeyHit(32)) g_showDirections = 1-g_showDirections;
}


//-----------------------------------------------------------------------------
// Name: CreateMapImage
// Desc: Creates the map image
//-----------------------------------------------------------------------------
void CreateMapImage (void) 
{
	FreeImage(mapImage);
	mapImage = CreateImage(800,600); //create a new map image.
	SetBuffer (0,mapImage);
	Color(0,0,255);//set default color to blue
	for (int x = 0; x <= 79; x++){
		for (int y = 0; y <= 59; y++){

		//Draw blue walls
		if (walkability[x][y] == unwalkable) 
			Rect(x*10,y*10,10,10,1);
		
	}}		
	SetBuffer (2);
	Color(255,255,255);//set default color to white
}


//-----------------------------------------------------------------------------
// Name: CreateWallImage
// Desc: Creates a blue 10x10 wall block image
//-----------------------------------------------------------------------------
void CreateWallImage (void) 
{
	wallImage = CreateImage(10,10); //create a new map image.
	SetBuffer (0,wallImage);
	Color(0,0,255);//set default color to blue
	Rect(0,0,10,10,1);
	SetBuffer (2);
	Color(255,255,255);//set default color to white
}


//-----------------------------------------------------------------------------
// Name: DrawMap
// Desc: Edit the map by left clicking while in edit mode.
//-----------------------------------------------------------------------------
void DrawMap(void)
{
	if (smileyActivated == 1) 
		DrawBlock (mapImage,0,0);
	else
	{
		for (int x = 0; x <= 79; x++){
		for (int y = 0; y <= 59; y++){

			//Draw blue walls
			if (walkability[x][y] == unwalkable) 
				DrawBlock (wallImage,x*10,y*10);
		}}
	}
}


//-----------------------------------------------------------------------------
// Name: EditMap
// Desc: Edit the map by left clicking while in edit mode.
//-----------------------------------------------------------------------------
void EditMap (void)
{
	if (MouseHit(1))
	{
		int x = MouseX()/10;
		int y = MouseY()/10;
		walkability[x][y] = 1-walkability[x][y];
	}
}


//-----------------------------------------------------------------------------
// Name: LoadGraphics
// Desc: Loads graphics
//-----------------------------------------------------------------------------
void LoadGraphics (void)
{
	SetFont("Arial",14);
	CreateWallImage();
	CreateMapImage();
	mousePointer = LoadImage("../../Graphics/red_pointer.bmp");
	MaskImage (mousePointer, 255,255,255);

	AutoMidHandle(true);
	smiley = LoadImage("../../Graphics/smiley x 10.bmp");
	MaskImage (smiley, 0,0,0);

	chaser = LoadImage("../../Graphics/ghost x 10.bmp");
	MaskImage (chaser, 0,0,0);
}


//-----------------------------------------------------------------------------
// Name: LoadMapData
// Desc: Load any pre-existing map when launching the program.
//-----------------------------------------------------------------------------
void LoadMapData (void)
{
	ifstream filein;
	filein.open("myTerrainData.dat",ios::nocreate);
	if (filein)
	{
		for (int x = 0; x <= 79; x++){
			for (int y = 0; y <= 59; y++){
			filein >> walkability [x][y];//or filein.read(buffer,length)
			if (walkability [x][y] > 1) walkability [x][y] = 0;
		}}
		filein.close();	
	}
	else //initialize the map to completely walkable
	{
		for (int x = 0; x <= 79; x++){
			for (int y = 0; y <= 59; y++){
				walkability [x][y] = walkable;
		}}
	}
}


//-----------------------------------------------------------------------------
// Name: LoadUnitData
// Desc: Initialize unit-related data
//-----------------------------------------------------------------------------
void LoadUnitData (void) 
{
	xLoc[1] = 125 ; yLoc[1] = 325; //initial smiley location
	xLoc[2] = 725 ; yLoc[2] = 325; //initial chaser location
	xLoc[3] = 465 ; yLoc[3] = 145; //initial chaser location	
	speed[1] = 5;//smiley speed
	speed[2] = 3;//chaser
	speed[3] = 2;//chaser
}


//-----------------------------------------------------------------------------
// Name: MoveChaser
// Desc: This subroutine moves the chasers/ghosts around on the screen.
//	In this case the findPath function is accessed automatically when
//	a chaser reaches the end of his current path. The path info
//	is also updated occasionally.
//-----------------------------------------------------------------------------
void MoveChaser (int ID)  
{
	int targetID = 1; //ID of target (the smiley)

//1. Find Path: If smiley and chaser are not at the same location on the 
//	screen and no path is currently active, find a new path.
	if (xLoc[ID] != xLoc[targetID] || yLoc[ID] != yLoc[targetID]) 
	{
	//If no path has been generated, generate one. Update it when
	//the chaser reaches its fifth step on the current path.	
	if (pathStatus[ID] == notStarted || pathLocation[ID] == 5)
	{
		//Generate a new path. Enter coordinates of smiley sprite (xLoc(1)/
		//yLoc(1)) as the target.
		pathStatus[ID] = FindPath(ID,xLoc[ID],yLoc[ID],
			xLoc[targetID],yLoc[targetID]);
		
	}} 

//2.Move chaser.
	if (pathStatus[ID] == found) MoveSprite(ID);
}


//-----------------------------------------------------------------------------
// Name: MoveSmiley
// Desc: This subroutine moves the smiley around on the screen. In
//	this case the findPath function is accessed via mouse clicks.
//-----------------------------------------------------------------------------
void MoveSmiley (void)  
{
	int ID = 1; //ID of smiley sprite

//1.Find Path: If smiley is active, any left or right click 
//	on the map will find a path and make him go there.
	if (MouseHit(1) || MouseHit(2))
	{	
		//Call the findPath function.
		int time1 = timeGetTime();
		pathStatus[ID] = FindPath(ID,xLoc[ID],yLoc[ID],MouseX(),MouseY());
		int time2 = timeGetTime();
		searchTime = time2-time1;
	}

//2.Move smiley.
	if (pathStatus[ID] == found) MoveSprite(ID);	
}


//-----------------------------------------------------------------------------
// Name: MoveSprite
// Desc: Moves the sprites around on the screen.
//-----------------------------------------------------------------------------
void MoveSprite(int ID)
{
//1.Read path information
	ReadPath(ID,xLoc[ID],yLoc[ID],speed[ID]);

//2.Move sprite. xLoc/yLoc = current location of sprite. xPath and
//	yPath = coordinates of next step on the path that were/are
//	read using the readPath function.
	if (xLoc[ID] > xPath[ID]) xLoc[ID] = xLoc[ID] - speed[ID];
	if (xLoc[ID] < xPath[ID]) xLoc[ID] = xLoc[ID] + speed[ID];
	if (yLoc[ID] > yPath[ID]) yLoc[ID] = yLoc[ID] - speed[ID];		
	if (yLoc[ID] < yPath[ID]) yLoc[ID] = yLoc[ID] + speed[ID];
	
//3.When sprite reaches the end location square	(end of its current
//	path) ...		
	if (pathLocation[ID] == pathLength[ID]) 
	{
//		Center the chaser in the square (not really necessary, but 
//		it looks a little better for the chaser, which moves in 3 pixel
//		increments and thus isn't always centered when it reaches its
//		target).
		if (abs(xLoc[ID] - xPath[ID]) < speed[ID]) xLoc[ID] = xPath[ID];
		if (abs(yLoc[ID] - yPath[ID]) < speed[ID]) yLoc[ID] = yPath[ID];
	}
}	


//-----------------------------------------------------------------------------
// Name: RenderScreen
// Desc: Draws stuff on screen
//-----------------------------------------------------------------------------
void RenderScreen (void) 
{
	Cls();
	DrawMap();

	DrawImage (smiley,xLoc[1],yLoc[1]);
	for (char ID = 2; ID <= 3; ID++)
		DrawImage (chaser,xLoc[ID],yLoc[ID]);

	//Show directions
	if (g_showDirections != 0) 
		ShowDirections();
	else
	{
		SetFont("Arial",14);
		Text(0,0,"Press space bar for directions");
	}

	if (smileyActivated == 0) 
	{
		SetFontColor (255,0,0);
		Text (0,20,"Map Edit Mode");
		SetFontColor (255,255,255);
	}
	else
		Text (0,20,combine "Search Time = " + searchTime + " ms");

	DrawImage (mousePointer,MouseX(),MouseY());
	Flip();
}


//-----------------------------------------------------------------------------
// Name: SaveMapData
// Desc: Saves the map when exiting the program.
//-----------------------------------------------------------------------------
void SaveMapData (void) 
{
	ofstream fileout;
	fileout.open("myTerrainData.dat");
	for (int x = 0; x <= 79; x++){
		for (int y = 0; y <= 59; y++){
		if (walkability [x][y] != 1) walkability [x][y] = 0;
		fileout << walkability [x][y];
	}}
	fileout.close();
}


//-----------------------------------------------------------------------------
// Name: ShowDirections
// Desc: Shows directions when the space bar has been hit.
//-----------------------------------------------------------------------------
void ShowDirections(void)
{
	SetBrush(0,0,0);
	SetPen(255,0,0);	
	Rect(100,100,600,401,1);
	SetFont("Arial",16,1,0,1);
	Text(125,130,"Directions");
	SetFont("Arial",16);

	Text(125,150,"- Press the space bar to hide or show these directions.");
	Text(125,170,"- Press enter to start pathfinding mode. Press enter again to toggle back");
	Text(125,190,"  to map-editing mode.");
	Text(125,210,"- When in pathfinding mode, left or right click anywhere on the map to"); 
	Text(125,230,"  make the smiley go there.");
	Text(125,250,"- When in map edit mode, draw blue walls by left clicking on the screen.");
	Text(125,270,"- Erase blue walls by left clicking on them.");

	Text(125,310,"Press escape to exit the program.");
	SetFont("Arial",14);
}

