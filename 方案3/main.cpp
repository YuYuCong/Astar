//#include"function_declare.h"
//#include "AStarLibrary.h"
//
//
//int main(int argc, char * argv[])
//{
//	int array[Height][Width] = {
//		{ 1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     1, 1, 1, 1, 1,   1, 1, 1, 1, 1£¬},
//		{ 1, 0, 0, 0, 1,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 0, 0, 0, 1,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 1, 1, 0, 0,   0, 0, 0, 0, 0,     0, 0, 0, 0, 0,   1, 1, 1, 1, 1, },
//
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },  //5
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 0, 0, 0, 1, },
//
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 1, 0, 0, 1, },  //10
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 0, 0,     0, 1, 0, 0, 0,   0, 1, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 0, 0, 1, 1,     1, 1, 0, 0, 0,   0, 1, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 0, 0, 0,   0, 1, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 0, 0, 0,   0, 1, 0, 0, 1, },
//	
//		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 1, 1, 1,   1, 1, 0, 0, 1, },  //15
//		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 1, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 1, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 0, 0, 0, 0,   0, 1, 0, 0, 0,     0, 0, 0, 0, 0,   0, 0, 0, 0, 1, },
//		{ 1, 1, 1, 1, 1,   1, 1, 1, 1, 1,     1, 1, 1, 1, 1,   1, 1, 1, 1, 1, }
//	};
//	int s_x = 15;
//	int s_y = 3;
//	int e_x =6;
//	int e_y = 16;
//	CAstar *maze = new CAstar(array);
//	CPoint *start = new CPoint(s_x, s_y);
//	CPoint *end = new CPoint(e_x, e_y);
//	CPoint* parent = maze->FindPath(start, end, false);
//
//	vector<vector<int>> path_list;
//	vector<int> temp;
//	path_list.clear();
//	temp.clear();
//
//	while (parent != NULL)
//	{
//		temp.clear();
//		temp.push_back(parent->X);
//		temp.push_back(parent->Y);
//		path_list.push_back(temp);
//		parent = parent->m_pParentPoint;
//	}
//	reverse(path_list.begin(), path_list.end());
//	//¼ì²éÂ·¾¶
//	//for(int i=0;i < path_list.size(); i++){
//	//cout << path_list[i][0] << "," << path_list[i][1] << endl;
//	//}
//
//#ifdef OpenCV_show
//	showmap(array,Point(s_x, s_y),Point(e_x, e_y), path_list);
//#endif
//
//	//while (1);
//	waitKey(0);
//	return 0;
//}
//




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
#include "AStarLibrary.h"

//-----------------------------------------------------------------------------
// Global variables and constants
//-----------------------------------------------------------------------------
cImage* wallImage; cImage* mapImage;
cImage* mousePointer;
cImage* smiley; cImage* chaser;
char smileyActivated = 0;
int xLoc[4]; int yLoc[4]; int speed[4];
int searchTime, g_showDirections = 0;


//-----------------------------------------------------------------------------
// Function Prototypes: where necessary
//-----------------------------------------------------------------------------
void CreateMapImage(void);
void CreateWallImage(void);
void DrawMap(void);
void EditMap(void);
void MoveSprite(int ID);


//-----------------------------------------------------------------------------
// Name: GameMain
// Desc: Launch and run game.
//-----------------------------------------------------------------------------
void main(HWND hwnd)
{
	InitializePathfinder();
	EndPathfinder();
	EndGraphics();
	return;
}

//-----------------------------------------------------------------------------
// Name: CreateMapImage
// Desc: Creates the map image
//-----------------------------------------------------------------------------
void CreateMapImage(void)
{
	FreeImage(mapImage);
	mapImage = CreateImage(800, 600); //create a new map image.
	SetBuffer(0, mapImage);
	Color(0, 0, 255);//set default color to blue
	for (int x = 0; x <= 79; x++) {
		for (int y = 0; y <= 59; y++) {

			//Draw blue walls
			if (walkability[x][y] == unwalkable)
				Rect(x * 10, y * 10, 10, 10, 1);

		}
	}
	SetBuffer(2);
	Color(255, 255, 255);//set default color to white
}

//-----------------------------------------------------------------------------
// Name: CreateWallImage
// Desc: Creates a blue 10x10 wall block image
//-----------------------------------------------------------------------------
void CreateWallImage(void)
{
	wallImage = CreateImage(10, 10); //create a new map image.
	SetBuffer(0, wallImage);
	Color(0, 0, 255);//set default color to blue
	Rect(0, 0, 10, 10, 1);
	SetBuffer(2);
	Color(255, 255, 255);//set default color to white
}


//-----------------------------------------------------------------------------
// Name: DrawMap
// Desc: Edit the map by left clicking while in edit mode.
//-----------------------------------------------------------------------------
void DrawMap(void)
{
	if (smileyActivated == 1)
		DrawBlock(mapImage, 0, 0);
	else
	{
		for (int x = 0; x <= 79; x++) {
			for (int y = 0; y <= 59; y++) {

				//Draw blue walls
				if (walkability[x][y] == unwalkable)
					DrawBlock(wallImage, x * 10, y * 10);
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name: EditMap
// Desc: Edit the map by left clicking while in edit mode.
//-----------------------------------------------------------------------------
void EditMap(void)
{
	if (MouseHit(1))
	{
		int x = MouseX() / 10;
		int y = MouseY() / 10;
		walkability[x][y] = 1 - walkability[x][y];
	}
}


//-----------------------------------------------------------------------------
// Name: MoveSprite
// Desc: Moves the sprites around on the screen.
//-----------------------------------------------------------------------------
void MoveSprite(int ID)
{
	//1.Read path information
	ReadPath(ID, xLoc[ID], yLoc[ID], speed[ID]);

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
