/*
NOTE: This code was written using Microsoft's Visual C++ v 6.0.
I don't know how well it will work with other compilers.

You also will need the Direct X 7.0 SDK, and to install the lib
and include files from the SDK to your C++ lib and include
folders. You can get the DirectX 7.0 SDK (large!) here:
http://download.microsoft.com/download/win98SE/SDK/7.0/W9X/EN-US/dx7sdk-700.1.exe


===========================
A* Pathfinder - Basics Demo
===========================
By Patrick Lester, pwlester@policyalmanac.org

This demo mirrors the tutorial found at 
http://www.policyalmanac.org/games/aStarTutorial.htm

While this demo is fine for demonstration purposes, if you want 
to write/develop your own A* code, Demo 2 is probably a better
starting point. Among other things, it lacks the overhead required
to allow step-by-step pathfinding in this demo.

Instructions
------------
Demo finds the shortest path between the green square and the 
red square.
- Start the pathfinding search by pressing the enter button.
- After the path is found, reset by clicking the mouse. You can 
  draw new walls and then press enter to search again.
- Press 1 to do a step-by-step search. Continue pressing 1 for each 
  new step. Press enter to finish a step-by-step search all at once.
- Draw blue walls by left clicking on the screen. 
- Erase blue walls by left clicking on them.
- Move the red target around by right clicking on the screen.
- Move the green start square around by holding the "g" button
  (g for green) down while left clicking on the screen.


Results
-------
- Path is marked by red dots.
- Squares highlighted in green were added to the "open list" of
  squares to be checked, but were never fully checked.
- Squares highlighted in blue were fully checked and added to the
  "closed list".
- Grey pointers point to a square's parent.
- F, G, and H costs are printed in each cell. The F cost is
  printed in the upper left corner, G in lower left corner, and
  H in lower right corner.

Please email comments to Patrick Lester at pwlester@policyalmanac.org
*/

#define WINDOW_NAME "Program" //Visible when game is minimized
#include "../common/launchWindows.h"
#include "../common/dxLibrary.h"
#include "aStarLibrary - do not use.h"

//-----------------------------------------------------------------------------
// Global variables and constants
//-----------------------------------------------------------------------------
cImage* mousePointer; 
cImage* grid; cImage* greengrid; cImage* bluegrid; cImage* dottedPath;
cImage* greenBlock; cImage* redBlock; cImage* wallBlock;
cImage* parentArrow[9]; cImage* number[10];
int drawing=0, erasing=0, g_showDirections=0; 

//Set pathfinderID and starting locations
int pathfinderID = 1;
int startX = 3, startY = 6; //Set seeker location
int targetX = 12, targetY = 6;//Set initial target location. This can
							  //be changed by right-clicking on the map.


//-----------------------------------------------------------------------------
// Function Prototypes: where necessary
//-----------------------------------------------------------------------------
void CheckUserInput (void);
void LoadGraphics (void);
void ShowDirections (void);


//-----------------------------------------------------------------------------
// Name: GameMain
// Desc: Launch and run game.
//-----------------------------------------------------------------------------
void GameMain (HWND hwnd)
{
	Graphics (800,600,16,hwnd);
	LoadGraphics();

	//Main game loop
    while (!KeyDown(27)) //While escape key not pressed
    {
		CheckUserInput();
		RenderScreen();
		CheckWinMessages();
	}	
	EndGraphics();
	return;
}


//-----------------------------------------------------------------------------
// Name: CheckUserInput
// Desc: Process key and mouse presses.
//-----------------------------------------------------------------------------
void CheckUserInput (void) 
{
	static int path=0;

	if (MouseDown(1) == 0 && MouseDown(2) == 0) drawing = 0;
	if (MouseDown(1) == 0 && MouseDown(2) == 0) erasing = 0;
	int mouseXCoordinate = MouseX()/50;
	int mouseYCoordinate = MouseY()/50;	

	//Draw and Erase walls (blue squares)
	if (MouseDown(1) == 1 && (!(KeyDown(71))))//if not "g" key
	{
		//Draw walls
		if (walkability[mouseXCoordinate][mouseYCoordinate]
			== walkable && erasing == 0)
		{
	 	walkability[mouseXCoordinate][mouseYCoordinate] = unwalkable;
		drawing = 1;
		}

		//Erase walls	
		if (walkability[mouseXCoordinate][mouseYCoordinate]
			== unwalkable && drawing == 0)
		{
	 	walkability[mouseXCoordinate][mouseYCoordinate] = walkable;		erasing = 1;
		}
	}

	//Move red target square around
	if (MouseHit(2) == 1) 
	{
		targetX = mouseXCoordinate;
		targetY = mouseYCoordinate;
	}

	//Move green starting square around
	if (KeyDown(71)) {// if g key is down while left clicking	
	if (MouseDown(1) == 1) {
		startX = mouseXCoordinate;
		startY = mouseYCoordinate;
	}}

	//Start A* pathfinding search if return/enter key is hit
	if (path == notfinished) { //if path not searched
	if (KeyHit(13))
	{
		path=FindPath(pathfinderID,startX*50,startY*50,targetX*50,targetY*50);	
		if (path == found) ReadPath(pathfinderID);	
		whichList[startX][startY] = 0;//don't highlight the start square (aesthetics)	
	}}


	//If "1" key is hit, start A* pathfinding search step-by-step
	if (path == notfinished) {//if path not searched
		if (KeyHit(49)){
			path=FindPath(pathfinderID,startX*50,startY*50,targetX*50,targetY*50,true);

			if (path == found)
			{ 
				ReadPath(pathfinderID);
				whichList[startX][startY] = 0;//don't highlight the start square (aesthetics)	
			}

			//If escape key was hit before finishing path then reset	
			if (path == notfinished) 
			{
				for (int x = 0; x < mapWidth; x++)
				{
					for (int y = 0; y < mapHeight;y++)
						whichList [x][y] = 0;
				}
			}
	}}

	//Show/hide directions by pressing space bar.
	if (KeyHit(32)) g_showDirections = 1-g_showDirections;

	//Reset after finishing A* search if pressing mouse button or "1" or enter key 
	if (path != notfinished) {//if path is done
		if (MouseDown(1)==1 || MouseDown(2)==1 || KeyHit(49) || KeyHit(13)) {
			for (int x = 0; x < mapWidth;x++)
			{
				for (int y = 0; y < mapHeight;y++)
					whichList [x][y] = 0;
			}
			path = notfinished;
		}	
	}
}



//-----------------------------------------------------------------------------
// Name: LoadGraphics
// Desc: Loads graphics
//-----------------------------------------------------------------------------
void LoadGraphics (void)
{
	SetFont("Arial",14);

	mousePointer = LoadImage("../../Graphics/red_pointer.bmp");
	MaskImage (mousePointer, 255,255,255);
		
	//Load grids
	grid = LoadImage("../../Graphics/grid.bmp");		
		MaskImage (grid,255,255,255);
	greengrid = LoadImage("../../Graphics/greengrid.bmp");		
		MaskImage (greengrid,255,255,255);
	bluegrid = LoadImage("../../Graphics/bluegrid.bmp");		
		MaskImage (bluegrid,255,255,255);
	dottedPath = LoadImage("../../Graphics/path.bmp");		
		MaskImage (dottedPath,255,255,255);
	
	//Load blocks
	greenBlock = LoadImage("../../Graphics/start.bmp");
	redBlock = LoadImage("../../Graphics/end.bmp");
	wallBlock = LoadImage("../../Graphics/wall.bmp");
	
	//Load parentArrows - these point from a square to its parent in 
	//the pathfinding search.
	for (int z = 1; z <= 8 ; z++)
	{
		CSTR string = combine "../../Graphics/arrow"+z+".bmp";
		parentArrow[z] = LoadImage(string.actualString);
		MaskImage (parentArrow[z],255,255,255);
	}
	
	//Load parentArrows - these point from a square to its parent in 
	//the pathfinding search.
	for (z = 0; z <= 9 ; z++)
	{
		CSTR string = combine "../../Graphics/number"+z+".bmp";
		number[z] = LoadImage(string.actualString);
		MaskImage (number[z],0,0,0);
	}
}



//-----------------------------------------------------------------------------
// Name: RenderScreen
// Desc: Draws stuff on screen
//-----------------------------------------------------------------------------
void RenderScreen (bool stepByStep) 
{
	Cls();

	//Draw starting location	
	DrawBlock (greenBlock,startX*50,startY*50);

	//Draw target location
	DrawBlock (redBlock,targetX*50,targetY*50);

	for (int x = 0; x <= 15; x++) //draw each of 16 tiles across the screen
	{
	for (int y = 0; y <= 11; y++) //draw each of 12 tiles down the screen
	{

		//Draw blue walls
		if (walkability[x][y] == unwalkable) DrawBlock(wallBlock,x*50,y*50);

		//Draw cells on open list (green), closed list (blue) and
		//part of the found path, if any (red dots).
		if (whichList[x][y] == 1) DrawImage (greengrid,x*50,y*50);
		if (whichList[x][y] == 2) DrawImage (bluegrid,x*50,y*50);
		if (whichList[x][y] == 3) DrawImage (dottedPath,x*50,y*50);

		//Draw arrows that point to each square's parent.
		if (whichList[x][y] == 1 || whichList[x][y] == 2) 
		{
			if (parentX[x][y] == x && parentY[x][y] == y - 1) 
				DrawImage (parentArrow[1],x*50,y*50);
			else if (parentX[x][y] == x+1 && parentY[x][y] == y - 1) 
				DrawImage (parentArrow[2],x*50,y*50);	
			else if (parentX[x][y] == x+1 && parentY[x][y] == y) 
				DrawImage (parentArrow[3],x*50,y*50);
			else if (parentX[x][y] == x+1 && parentY[x][y] == y + 1) 
				DrawImage (parentArrow[4],x*50,y*50);				
			else if (parentX[x][y] == x && parentY[x][y] == y + 1) 
				DrawImage (parentArrow[5],x*50,y*50);
			else if (parentX[x][y] == x-1 && parentY[x][y] == y + 1) 
				DrawImage (parentArrow[6],x*50,y*50);
			else if (parentX[x][y] == x-1 && parentY[x][y] == y) 
				DrawImage (parentArrow[7],x*50,y*50);
			else if (parentX[x][y] == x-1 && parentY[x][y] == y - 1) 
				DrawImage (parentArrow[8],x*50,y*50);
		}

		//Print F, G and H costs in cells
		if (whichList[x][y] > 0) //if an open, closed, or path cell.
		{
			int G = Gcost[x][y];
			int H = 10*(abs(x - targetX) + abs(y - targetY));
			int F = G + H;
		
			//Print F in upper left corner of cell (F=G+H)
			if (F >= 100) DrawImage (number[F/100],x*50+2,y*50+4);		
			DrawImage (number[(F % 100)/10],x*50+7,y*50+4);
			DrawImage (number[F % 10],x*50+12,y*50+4);
			
			//Print G in bottom left corner of cell (G=distance from start)
			if (G >= 100) DrawImage (number[G/100],x*50+2,y*50+40);	
			DrawImage (number[(G % 100)/10],x*50+7,y*50+40);
			DrawImage (number[G % 10],x*50+12,y*50+40);

			//Print H in bottom right corner of cell (H=distance to target)
			if (H >= 100) DrawImage (number[H/100],x*50+32,y*50+40);
			DrawImage (number[(H % 100)/10],x*50+37,y*50+40);
			DrawImage (number[H % 10],x*50+42,y*50+40);			
		}

		//Draw the grid overlay.
		DrawImage (grid,x*50,y*50);
	}
	}

	//Show directions
	if (g_showDirections != 0) 
		ShowDirections();
	else
	{
		SetFont("Arial",14);
		Text(0,0,"Press space bar for directions");
	}

	if (stepByStep == false) DrawImage (mousePointer,MouseX(),MouseY());
	Flip();
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
	Text(125,150,"Demo finds the shortest path between the green square and the red square.");
	Text(125,170,"- Press the space bar to hide or show these directions.");
	Text(125,190,"- Start the pathfinding search by pressing the enter button.");
	Text(125,210,"- After the path is found, reset by pressing enter or clicking the mouse. You can ");
	Text(125,230,"  draw new walls and then press enter to search again.");
	Text(125,250,"- Press 1 to do a step-by-step search. Continue pressing 1 for each");
	Text(125,270,"  new step.  Press enter to finish a step-by-step search all at once.");
	Text(125,290,"- Draw blue walls by left clicking on the screen.");
	Text(125,310,"- Erase blue walls by left clicking on them.");
	Text(125,330,"- Move the red target around by right clicking on the screen.");
	Text(125,350,"- Move the green start square around by holding the 'g' button");
	Text(125,370,"  (g for green) down while left clicking on the screen.");

	Text(125,410,"Press escape to exit the program.");
}


