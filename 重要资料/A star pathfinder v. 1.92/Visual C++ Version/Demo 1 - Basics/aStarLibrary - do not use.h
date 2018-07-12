/*
;===================================================================
;A* Pathfinder - Do Not Use
;===================================================================
;Last updated 06/16/03 -- Visual C++ version

;This version of the A* Library has been written specifically for the
;Astar Demo 1 program. If you want to use the AStar functions in your
;program, use the Demo 2 A* library. The primary difference between 
;them is that this one accomodates stepByStep searches, which are 
;specific to Demo 1. The code included to handle that will
;probably malfunction if used in another program -- so use the other
;version (see Demo 2) instead.
;==================================================================
 */

	const mapWidth = 16, mapHeight = 12, tileSize = 50, numberPeople = 1;
	int onClosedList = 10;
	const notfinished = 0, notStarted = 0;// path-related constants
	const found = 1, nonexistent = 2; 
	const walkable = 0, unwalkable = 1;// walkability array constants

	//Create needed arrays
	char walkability [mapWidth][mapHeight];
	int openList[mapWidth*mapHeight+2]; //1 dimensional array holding ID# of open list items
	int whichList[mapWidth+1][mapHeight+1];  //2 dimensional array used to record 
// 		whether a cell is on the open list or on the closed list.
	int openX[mapWidth*mapHeight+2]; //1d array stores the x location of an item on the open list
	int openY[mapWidth*mapHeight+2]; //1d array stores the y location of an item on the open list
	int parentX[mapWidth+1][mapHeight+1]; //2d array to store parent of each cell (x)
	int parentY[mapWidth+1][mapHeight+1]; //2d array to store parent of each cell (y)
	int Fcost[mapWidth*mapHeight+2];	//1d array to store F cost of a cell on the open list
	int Gcost[mapWidth+1][mapHeight+1]; 	//2d array to store G cost for each cell.
	int Hcost[mapWidth*mapHeight+2];	//1d array to store H cost of a cell on the open list
	int pathLength[numberPeople+1];     //stores length of the found path for critter
	int pathLocation[numberPeople+1];   //stores current position along the chosen path for critter		
	int* pathBank [numberPeople+1];

	//Path reading variables
	int pathStatus[numberPeople+1];
	int xPath[numberPeople+1];
	int yPath[numberPeople+1];


//-----------------------------------------------------------------------------
// Function Prototypes: where needed
//-----------------------------------------------------------------------------
void ReadPath(int pathfinderID,int currentX,int currentY, int pixelsPerFrame);
int ReadPathX(int pathfinderID,int pathLocation);
int ReadPathY(int pathfinderID,int pathLocation);
void RenderScreen (bool stepByStep=false);


//-----------------------------------------------------------------------------
// Name: InitializePathfinder
// Desc: Allocates memory for the pathfinder.
//-----------------------------------------------------------------------------
void InitializePathfinder (void)
{
	for (int x = 0; x < numberPeople+1; x++)
		pathBank [x] = (int*) malloc(4);
}


//-----------------------------------------------------------------------------
// Name: EndPathfinder
// Desc: Frees memory used by the pathfinder.
//-----------------------------------------------------------------------------
void EndPathfinder (void)
{
	for (int x = 0; x < numberPeople+1; x++)
	{
		free (pathBank [x]);
	}
}


//-----------------------------------------------------------------------------
// Name: FindPath
// Desc: Finds a path using A*
//-----------------------------------------------------------------------------
int FindPath (int pathfinderID,int startingX, int startingY,
			  int targetX, int targetY, bool stepByStep = false)
{
	int onOpenList=0, parentXval=0, parentYval=0,
	a=0, b=0, m=0, u=0, v=0, temp=0, corner=0, numberOfOpenListItems=0,
	addedGCost=0, tempGcost = 0, path = 0, x=0, y=0,
	tempx, pathX, pathY, cellPosition,
	newOpenListItemID=0;

//1. Convert location data (in pixels) to coordinates in the walkability array.
	int startX = startingX/tileSize;
	int startY = startingY/tileSize;	
	targetX = targetX/tileSize;
	targetY = targetY/tileSize;

//2.Quick Path Checks: Under the some circumstances no path needs to
//	be generated ...

//	If starting location and target are in the same location...
	if (startX == targetX && startY == targetY && pathLocation[pathfinderID] > 0)
		return found;
	if (startX == targetX && startY == targetY && pathLocation[pathfinderID] == 0)
		return nonexistent;

//	If target square is unwalkable, return that it's a nonexistent path.
	if (walkability[targetX][targetY] == unwalkable)
		goto noPath;

//3.Reset some variables that need to be cleared
	for (x = 0; x < mapWidth; x++)
	{
		for (y = 0; y < mapHeight;y++)
			whichList [x][y] = 0;
	}
	onClosedList = 2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
	onOpenList = 1;
	pathLength [pathfinderID] = notStarted;//i.e, = 0
	pathLocation [pathfinderID] = notStarted;//i.e, = 0
	Gcost[startX][startY] = 0; //reset starting square's G value to 0

//4.Add the starting location to the open list of squares to be checked.
	numberOfOpenListItems = 1;
	openList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	openX[1] = startX ; openY[1] = startY;

//5.Do the following until a path is found or deemed nonexistent.
	do
	{

//6.If the open list is not empty, take the first cell off of the list.
//	This is the lowest F cost cell on the open list.
	if (numberOfOpenListItems != 0)
	{

//7. Pop the first item off the open list.
	parentXval = openX[openList[1]];
	parentYval = openY[openList[1]]; //record cell coordinates of the item
	whichList[parentXval][parentYval] = onClosedList;//add the item to the closed list

//	Open List = Binary Heap: Delete this item from the open list, which
//  is maintained as a binary heap. For more information on binary heaps, see:
//	http://www.policyalmanac.org/games/binaryHeaps.htm
	numberOfOpenListItems = numberOfOpenListItems - 1;//reduce number of open list items by 1	
		
//	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
	openList[1] = openList[numberOfOpenListItems+1];//move the last item in the heap up to slot #1
	v = 1;

//	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
	do
	{
	u = v;		
	if (2*u+1 <= numberOfOpenListItems) //if both children exist
	{
	 	//Check if the F cost of the parent is greater than each child.
		//Select the lowest of the two children.
		if (Fcost[openList[u]] >= Fcost[openList[2*u]]) 
			v = 2*u;
		if (Fcost[openList[v]] >= Fcost[openList[2*u+1]]) 
			v = 2*u+1;		
	}
	else
	{
		if (2*u <= numberOfOpenListItems) //if only child #1 exists
		{
	 	//Check if the F cost of the parent is greater than child #1	
			if (Fcost[openList[u]] >= Fcost[openList[2*u]]) 
				v = 2*u;
		}
	}

	if (u != v) //if parent's F is > one of its children, swap them
	{
		temp = openList[u];
		openList[u] = openList[v];
		openList[v] = temp;			
	}
	else
		break; //otherwise, exit loop
		
	}
	while (!KeyDown(27));//reorder the binary heap


//7.Check the adjacent squares. (Its "children" -- these path children
//	are similar, conceptually, to the binary heap children mentioned
//	above, but don't confuse them. They are different. Path children
//	are portrayed in Demo 1 with grey pointers pointing toward
//	their parents.) Add these adjacent child squares to the open list
//	for later consideration if appropriate (see various if statements
//	below).
	for (b = parentYval-1; b <= parentYval+1; b++){
	for (a = parentXval-1; a <= parentXval+1; a++){

//	If not off the map (do this first to avoid array out-of-bounds errors)
	if (a != -1 && b != -1 && a != mapWidth && b != mapHeight){

//	If not already on the closed list (items on the closed list have
//	already been considered and can now be ignored).			
	if (whichList[a][b] != onClosedList) { 
	
//	If not a wall/obstacle square.
	if (walkability [a][b] != unwalkable) { 
		
//	Don't cut across corners
	corner = walkable;	
	if (a == parentXval-1) 
	{
		if (b == parentYval-1)
		{
			if (walkability[parentXval-1][parentYval] == unwalkable
				|| walkability[parentXval][parentYval-1] == unwalkable) \
				corner = unwalkable;
		}
		else if (b == parentYval+1)
		{
			if (walkability[parentXval][parentYval+1] == unwalkable
				|| walkability[parentXval-1][parentYval] == unwalkable) 
				corner = unwalkable; 
		}
	}
	else if (a == parentXval+1)
	{
		if (b == parentYval-1)
		{
			if (walkability[parentXval][parentYval-1] == unwalkable 
				|| walkability[parentXval+1][parentYval] == unwalkable) 
				corner = unwalkable;
		}
		else if (b == parentYval+1)
		{
			if (walkability[parentXval+1][parentYval] == unwalkable 
				|| walkability[parentXval][parentYval+1] == unwalkable)
				corner = unwalkable; 
		}
	}	
	if (corner == walkable) {
	
//	If not already on the open list, add it to the open list.			
	if (whichList[a][b] != onOpenList) 
	{	

		//Create a new open list item in the binary heap.
		newOpenListItemID = newOpenListItemID + 1; //each new item has a unique ID #
		m = numberOfOpenListItems+1;
		openList[m] = newOpenListItemID;//place the new open list item (actually, its ID#) at the bottom of the heap
		openX[newOpenListItemID] = a;
		openY[newOpenListItemID] = b;//record the x and y coordinates of the new item

		//Figure out its G cost
		if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
			addedGCost = 14;//cost of going to diagonal squares	
		else	
			addedGCost = 10;//cost of going to non-diagonal squares				
		Gcost[a][b] = Gcost[parentXval][parentYval] + addedGCost;

		//Figure out its H and F costs and parent
		Hcost[openList[m]] = 10*(abs(a - targetX) + abs(b - targetY));
		Fcost[openList[m]] = Gcost[a][b] + Hcost[openList[m]];
		parentX[a][b] = parentXval ; parentY[a][b] = parentYval;	

		//Move the new open list item to the proper place in the binary heap.
		//Starting at the bottom, successively compare to parent items,
		//swapping as needed until the item finds its place in the heap
		//or bubbles all the way to the top (if it has the lowest F cost).
		while (m != 1) //While item hasn't bubbled to the top (m=1)	
		{
			//Check if child's F cost is < parent's F cost. If so, swap them.	
			if (Fcost[openList[m]] <= Fcost[openList[m/2]])
			{
				temp = openList[m/2];
				openList[m/2] = openList[m];
				openList[m] = temp;
				m = m/2;
			}
			else
				break;
		}
		numberOfOpenListItems = numberOfOpenListItems+1;//add one to the number of items in the heap

		//Change whichList to show that the new item is on the open list.
		whichList[a][b] = onOpenList;
	}

//8.If adjacent cell is already on the open list, check to see if this 
//	path to that cell from the starting location is a better one. 
//	If so, change the parent of the cell and its G and F costs.	
	else //If whichList(a,b) = onOpenList
	{
	
		//Figure out the G cost of this possible new path
		if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
			addedGCost = 14;//cost of going to diagonal tiles	
		else	
			addedGCost = 10;//cost of going to non-diagonal tiles				
		tempGcost = Gcost[parentXval][parentYval] + addedGCost;
		
		//If this path is shorter (G cost is lower) then change
		//the parent cell, G cost and F cost. 		
		if (tempGcost < Gcost[a][b]) //if G cost is less,
		{
			parentX[a][b] = parentXval; //change the square's parent
			parentY[a][b] = parentYval;
			Gcost[a][b] = tempGcost;//change the G cost			

			//Because changing the G cost also changes the F cost, if
			//the item is on the open list we need to change the item's
			//recorded F cost and its position on the open list to make
			//sure that we maintain a properly ordered open list.
			for (int x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
			{
			if (openX[openList[x]] == a && openY[openList[x]] == b) //item found
			{
				Fcost[openList[x]] = Gcost[a][b] + Hcost[openList[x]];//change the F cost
				
				//See if changing the F score bubbles the item up from it's current location in the heap
				m = x;
				while (m != 1) //While item hasn't bubbled to the top (m=1)	
				{
					//Check if child is < parent. If so, swap them.	
					if (Fcost[openList[m]] < Fcost[openList[m/2]])
					{
						temp = openList[m/2];
						openList[m/2] = openList[m];
						openList[m] = temp;
						m = m/2;
					}
					else
						break;
				} 
				break; //exit for x = loop
			} //If openX(openList(x)) = a
			} //For x = 1 To numberOfOpenListItems
		}//If tempGcost < Gcost(a,b)

	}//else If whichList(a,b) = onOpenList	
	}//If not cutting a corner
	}//If not a wall/obstacle square.
	}//If not already on the closed list 
	}//If not off the map
	}//for (a = parentXval-1; a <= parentXval+1; a++){
	}//for (b = parentYval-1; b <= parentYval+1; b++){

	}//if (numberOfOpenListItems != 0)

//9.If open list is empty then there is no path.	
	else
	{
		path = nonexistent; break;
	}  

	//If target is added to open list then path has been found.
	if (whichList[targetX][targetY] == onOpenList)
	{
		path = found; break;
	}

	//If in step-by-step mode draw the screen step by step, waiting
	//for a keypress before the next step is taken.
	if (path == notfinished && stepByStep == true)
	{
		RenderScreen(stepByStep);
		while (1) //while "1" or "enter" or "escape" key not hit
		{
			if (KeyHit(13) || KeyDown(27)) //if enter or escape hit
			{
				stepByStep = false; break;
			}
			if (KeyHit(49)) break; //if "1" key hit
			CheckWinMessages();
		}
	}

	}
	while (!(KeyDown(27)));//Do until path is found or deemed nonexistent

//10.Save the path if it exists.
	if (path == found)
	{

//a.Working backwards from the target to the starting location by checking
//	each cell's parent, figure out the length of the path.
	pathX = targetX; pathY = targetY;
	do
	{
		//Look up the parent of the current cell.	
		tempx = parentX[pathX][pathY];		
		pathY = parentY[pathX][pathY];
		pathX = tempx;

		//Figure out the path length
		pathLength[pathfinderID] = pathLength[pathfinderID] + 1;
	}
	while (pathX != startX || pathY != startY);

//b.Resize the data bank to the right size in bytes
	pathBank[pathfinderID] = (int*) realloc (pathBank[pathfinderID],
		pathLength[pathfinderID]*8);

//c. Now copy the path information over to the databank. Since we are
//	working backwards from the target to the start location, we copy
//	the information to the data bank in reverse order. The result is
//	a properly ordered set of path data, from the first step to the
//	last.
	pathX = targetX ; pathY = targetY;
	cellPosition = pathLength[pathfinderID]*2;//start at the end	
	do
	{
	cellPosition = cellPosition - 2;//work backwards 2 integers
	pathBank[pathfinderID] [cellPosition] = pathX;
	pathBank[pathfinderID] [cellPosition+1] = pathY;

//d.Look up the parent of the current cell.	
	tempx = parentX[pathX][pathY];		
	pathY = parentY[pathX][pathY];
	pathX = tempx;

//e.If we have reached the starting square, exit the loop.	
	}
	while (pathX != startX || pathY != startY);	

	}
	return path;


//13.If there is no path to the selected target, set the pathfinder's
//	xPath and yPath equal to its current location and return that the
//	path is nonexistent.
noPath:
	xPath[pathfinderID] = startingX;
	yPath[pathfinderID] = startingY;
	return nonexistent;
}



//-----------------------------------------------------------------------------
// Name: ReadPath
// Desc: Reads path data created by FindPath()
//-----------------------------------------------------------------------------
void ReadPath(int pathfinderID) //If a path exists, read the path data
//	from the pathbank.
{
	pathLocation[pathfinderID] = 1; //set pathLocation to 1st step
	while (pathLocation[pathfinderID] < pathLength[pathfinderID])
	{
		int a = pathBank[pathfinderID] [pathLocation[pathfinderID]*2-2];
		int b = pathBank[pathfinderID] [pathLocation[pathfinderID]*2-1];
		pathLocation[pathfinderID] = pathLocation[pathfinderID] + 1;
		whichList[a][b] = 3;//draw dotted path
	} 
}



