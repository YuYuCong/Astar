;A* Pathfinder (Version 2.0) by Patrick Lester. Used by permission.
;==================================================================
;Last updated 03/22/04

;This version of the aStar library has been modified to handle 
;pathfinding around other units.

;An article describing A* and this code in particular can be found at:
;http://www.policyalmanac.org/games/aStarTutorial.htm

;If you want to use this AStar Library, you may do so free of charge so 
;long as the author byline (above) is retained. Thank you to CaseyC 
;at the Blitz Forums for suggesting the use of binary heaps for the open 
;list. Email comments and questions to Patrick Lester at 
;pwlester@policyalmanac.org.

;==========================================================
;DECLARE VARIABLES
.declareVariables

	;Adjust these variables to match your map dimensions
	Global tileSize = 50, mapWidth = 16, mapHeight = 12

	;Create needed arrays
	Dim walkability(mapWidth+1,mapHeight+1) ;array that holds wall/obstacle information	
	Dim openList(mapWidth*mapHeight+2) ;1 dimensional array holding ID# of open list items
	Dim whichList(mapWidth+1,mapHeight+1)  ;2 dimensional array used to record 
		;whether a cell is on the open list or on the closed list.
	Dim openX(mapWidth*mapHeight+2) ;1d array stores the x location of an item on the open list
	Dim openY(mapWidth*mapHeight+2) ;1d array stores the y location of an item on the open list
	Dim parentX(mapWidth+1,mapHeight+1) ;2d array to store parent of each cell (x)
	Dim parentY(mapWidth+1,mapHeight+1) ;2d array to store parent of each cell (y)
	Dim Fcost(mapWidth*mapHeight+2)	;1d array to store F cost of a cell on the open list
	Dim Gcost(mapWidth+1,mapHeight+1) 	;2d array to store G cost for each cell.
	Dim Hcost(mapWidth*mapHeight+2)	;1d array to store H cost of a cell on the open list		
	
	Dim tempUnwalkability(mapWidth+1,mapHeight+1) ;array that holds info about adjacent units
	Dim claimedNode.unit(mapWidth+1,mapHeight+1); array that stores claimed nodes	
	
	;Declare constants
	Global onClosedList = 10 ;openList variable	
	Const notfinished = 0, notStarted = 0, found = 1 ; pathStatus constants 
	Const nonexistent = 2, tempStopped = 3; pathStatus constants 
	Const walkable = 0, unwalkable = 1 ; walkability array constants	

;==========================================================
;FIND PATH: This function finds the path and saves it. Non-Blitz users please note,
;the first parameter is a pointer to a user-defined object called a unit, which contains all
;relevant info about the unit in question (its current location, speed, etc.). As an
;object-oriented data structure, types are similar to structs in C.
;	Please note that targetX and targetY are pixel-based coordinates relative to the
;upper left corner of the map, which is 0,0.
Function FindPath(unit.unit,targetX,targetY)

;1.	Convert location data (in pixels) to coordinates in the walkability array.
	startX = Floor(unit\xLoc/tileSize) : startY = Floor(unit\yLoc/tileSize)	
	targetX = Floor(targetX/tileSize) : targetY = Floor(targetY/tileSize)

;2.	Check for redirects	
	result = CheckRedirect(unit,targetX,targetY)	
	If result = failed Then Goto noPath ;target is unwalkable and could not find a redirect.
	If result = succeeded Then targetX = gInt1 : targetY = gInt2

	;If starting location and target are in the same location...
	If startX = targetX And startY = targetY 
		unit\pathLength = 0 : unit\pathLocation = 0	
		PokeShort unit\pathBank,0,startX ;store starting x value	
		PokeShort unit\pathBank,2,startY ;store starting y value		
		Return found
	End If

;3.	a. Reset some variables that need to be cleared
	If onClosedList > 1000000 ;occasionally redim whichList
		Dim whichList(mapWidth,mapHeight) : onClosedList = 10
	End If	
	onClosedList = onClosedList+5 ;changing the values of onOpenList and onClosed list is faster than redimming whichList() array
	onOpenList = onClosedList-1
	tempUnwalkable = onClosedList-2
	unit\pathLength = notstarted ;i.e, = 0
	unit\pathLocation = notstarted ;i.e, = 0
	Gcost(startX,startY) = 0 ;reset starting square's G value to 0

	;b. Create a footprint for any nearby unit that the pathfinding unit 
	;may be about to collide with. Such nodes are designated as tempUnwalkable.	
	CreateFootPrints(unit.unit)

;4.	Add the starting location to the open list of squares to be checked.
	numberOfOpenListItems = 1
	openList(1) = 1 ;assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	openX(1) = startX : openY(1) = startY

;5.	Do the following until a path is found or deemed nonexistent.
	Repeat
	
;6.	If the open list is not empty, take the first cell off of the list.
	;This is the lowest F cost cell on the open list.
	If numberOfOpenListItems <> 0 Then

	;Pop the first item off the open list.
	parentXval = openX(openList(1)) : parentYVal = openY(openList(1)) ;record cell coordinates of the item
	whichList(parentXval,parentYVal) = onClosedList ;add the item to the closed list

	;Open List = Binary Heap: Delete this item from the open list, which
	;is maintained as a binary heap. For more information on binary heaps, see:
	;http://www.policyalmanac.org/games/binaryHeaps.htm
	numberOfOpenListItems = numberOfOpenListItems - 1 ;reduce number of open list items by 1	
	openList(1) = openList(numberOfOpenListItems+1) ;move the last item in the heap up to slot #1
	v = 1	
	Repeat ;Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
		u = v	
		If 2*u+1 <= numberOfOpenListItems ;if both children exist
		 	;Check if the F cost of the parent is greater than each child.
			;Select the lowest of the two children.	
			If Fcost(openList(u)) >= Fcost(openList(2*u)) Then v = 2*u
			If Fcost(openList(v)) >= Fcost(openList(2*u+1)) Then v = 2*u+1		
		Else
			If 2*u <= numberOfOpenListItems ;if only child #1 exists
			 	;Check if the F cost of the parent is greater than child #1	
				If Fcost(openList(u)) >= Fcost(openList(2*u)) Then v = 2*u
			End If	
		End If
		If u<>v ;if parent's F is > one of its children, swap them
			temp = openList(u)
			openList(u) = openList(v)
			openList(v) = temp				
		Else
			Exit ;otherwise, exit loop
		End If	
	Forever

	
;7.	Check the adjacent squares. (Its "children" -- these path children
	;are similar, conceptually, to the binary heap children mentioned
	;above, but don't confuse them. They are different. Path children
	;are portrayed in Demo 1 with grey pointers pointing toward
	;their parents.) Add these adjacent child squares to the open list
	;for later consideration if appropriate (see various if statements
	;below).
	For b = parentYVal-1 To parentYVal+1
	For a = parentXval-1 To parentXval+1

	;If not off the map (do this first to avoid array out-of-bounds errors)
	If a <> -1 And b <> -1 And a <> mapWidth And b <> mapHeight

	;If not already on the closed list (items on the closed list have
	;already been considered and can now be ignored).			
	If whichList(a,b) <> onClosedList 
	
	;If not a wall/obstacle square
	If walkability(a,b) <> unwalkable
	
	;If not an adjacent node that is temporarily unwalkable
	;as defined by CreateFootprints()
	If tempUnwalkability(a,b) <> tempUnwalkable
	
	;If not occupied by a stopped unit
	node = unwalkable
	If claimedNode(a,b) = Null
		node = walkable
	Else If claimedNode(a,b)\pathStatus <> stopped 
		node = walkable	
	End If
	If node = walkable
			
	;Don't cut across corners (this is optional)
	corner = walkable	
	If a = parentXVal-1 
		If b = parentYVal-1 
			If walkability(parentXval-1,parentYval) = unwalkable Then corner = unwalkable
			If walkability(parentXval,parentYval-1) = unwalkable Then corner = unwalkable						
		Else If b = parentYVal+1 
			If walkability(parentXval,parentYval+1) = unwalkable Then corner = unwalkable 
			If walkability(parentXval-1,parentYval) = unwalkable Then corner = unwalkable 							
		End If
	Else If a = parentXVal+1 
		If b = parentYVal-1 
			If walkability(parentXval,parentYval-1) = unwalkable Then corner = unwalkable 
			If walkability(parentXval+1,parentYval) = unwalkable Then corner = unwalkable 							
		Else If b = parentYVal+1 
			If walkability(parentXval+1,parentYval) = unwalkable Then corner = unwalkable 
			If walkability(parentXval,parentYval+1) = unwalkable Then corner = unwalkable 			
		End If
	End If			
	If corner = walkable
	
	;If not already on the open list, add it to the open list.			
	If whichList(a,b) <> onOpenList	

		;Create a new open list item in the binary heap.
		newOpenListItemID = newOpenListItemID + 1; each new item has a unique ID #
		m = numberOfOpenListItems+1
		openList(m) = newOpenListItemID	 ;place the new open list item (actually, its ID#) at the bottom of the heap
		openX(newOpenListItemID) = a : openY(newOpenListItemID) = b ;record the x and y coordinates of the new item

		;Figure out its G cost
		If Abs(a-parentXval) = 1 And Abs(b-parentYVal) = 1 Then
			addedGCost = 14 ;cost of going to diagonal squares	
		Else	
			addedGCost = 10 ;cost of going to non-diagonal squares				
		End If
		Gcost(a,b) = Gcost(parentXval,parentYVal)+addedGCost
			
		;Figure out its H and F costs and parent
		Hcost(openList(m)) = 10*(Abs(a - targetx) + Abs(b - targety)) ; record the H cost of the new square
		Fcost(openList(m)) = Gcost(a,b) + Hcost(openList(m)) ;record the F cost of the new square
		parentX(a,b) = parentXval : parentY(a,b) = parentYVal	;record the parent of the new square	
		
		;Move the new open list item to the proper place in the binary heap.
		;Starting at the bottom, successively compare to parent items,
		;swapping as needed until the item finds its place in the heap
		;or bubbles all the way to the top (if it has the lowest F cost).
		While m <> 1 ;While item hasn't bubbled to the top (m=1)	
			;Check if child's F cost is < parent's F cost. If so, swap them.	
			If Fcost(openList(m)) <= Fcost(openList(m/2)) Then
				temp = openList(m/2)
				openList(m/2) = openList(m)
				openList(m) = temp
				m = m/2
			Else
				Exit
			End If
		Wend 
		numberOfOpenListItems = numberOfOpenListItems+1 ;add one to the number of items in the heap

		;Change whichList to show that the new item is on the open list.
		whichList(a,b) = onOpenList


;8.	If adjacent cell is already on the open list, check to see if this 
	;path to that cell from the starting location is a better one. 
	;If so, change the parent of the cell and its G and F costs.	
	Else; If whichList(a,b) = onOpenList
	
		;Figure out the G cost of this possible new path
		If Abs(a-parentXval) = 1 And Abs(b-parentYVal) = 1 Then
			addedGCost = 14;cost of going to diagonal tiles	
		Else	
			addedGCost = 10 ;cost of going to non-diagonal tiles				
		End If
		tempGcost = Gcost(parentXval,parentYVal)+addedGCost
		
		;If this path is shorter (G cost is lower) then change
		;the parent cell, G cost and F cost. 		
		If tempGcost < Gcost(a,b) Then 	;if G cost is less,
			parentX(a,b) = parentXval 	;change the square's parent
			parentY(a,b) = parentYVal
			Gcost(a,b) = tempGcost 	;change the G cost			

			;Because changing the G cost also changes the F cost, if
			;the item is on the open list we need to change the item's
			;recorded F cost and its position on the open list to make
			;sure that we maintain a properly ordered open list.
			For x = 1 To numberOfOpenListItems ;look for the item in the heap
			If openX(openList(x)) = a And openY(openList(x)) = b Then ;item found
				FCost(openList(x)) = Gcost(a,b) + HCost(openList(x)) ;change the F cost
				
				;See if changing the F score bubbles the item up from it's current location in the heap
				m = x
				While m <> 1 ;While item hasn't bubbled to the top (m=1)	
					;Check if child is < parent. If so, swap them.	
					If Fcost(openList(m)) < Fcost(openList(m/2)) Then
						temp = openList(m/2)
						openList(m/2) = openList(m)
						openList(m) = temp
						m = m/2
					Else
						Exit ;while/wend
					End If
				Wend 
				
				Exit ;for x = loop
			End If ;If openX(openList(x)) = a
			Next ;For x = 1 To numberOfOpenListItems

		End If ;If tempGcost < Gcost(a,b) Then			

	End If ;If not already on the open list				
	End If ;If corner = walkable
	End If ;If not occupied by a stopped unit
	End If ;If not an adjacent, temporarily unwalkable node
	End If ;If not a wall/obstacle cell.	
	End If ;If not already on the closed list	
	End If ;If not off the map.	
	Next
	Next

;9.	If open list is empty then there is no path.	
	Else
		path = nonExistent : Exit
	End If

	;If target is added to open list then path has been found.
	If whichList(targetx,targety) = onOpenList Then path = found : Exit		

	Forever ;repeat until path is found or deemed nonexistent
	
	
;10.	Save the path if it exists. Copy it to a bank. 
	If path = found
		
		;a. Working backwards from the target to the starting location by checking
		;each cell's parent, figure out the length of the path.
		pathX = targetX : pathY = targetY	
		Repeat
			tempx = parentX(pathX,pathY)		
			pathY = parentY(pathX,pathY)
			pathX = tempx
			unit\pathLength = unit\pathLength + 1	
		Until pathX = startX And pathY = startY
	
		;b. Resize the data bank to the right size (leave room to store step 0,
		;which requires storing one more step than the length)
		ResizeBank unit\pathBank,(unit\pathLength+1)*4

		;c. Now copy the path information over to the databank. Since we are
		;working backwards from the target to the start location, we copy
		;the information to the data bank in reverse order. The result is
		;a properly ordered set of path data, from the first step to the
		;last.	
		pathX = targetX : pathY = targetY				
		cellPosition = unit\pathLength*4 ;start at the end	
		While Not (pathX = startX And pathY = startY)			
			PokeShort unit\pathBank,cellPosition,pathX ;store x value	
			PokeShort unit\pathBank,cellPosition+2,pathY ;store y value	
			cellPosition = cellPosition - 4 ;work backwards		
			tempx = parentX(pathX,pathY)		
			pathY = parentY(pathX,pathY)
			pathX = tempx
		Wend	
		PokeShort unit\pathBank,0,startX ;store starting x value	
		PokeShort unit\pathBank,2,startY ;store starting y value

	End If ;If path = found Then 

;11. Return info on whether a path has been found.
	Return path; Returns 1 if a path has been found, 2 if no path exists. 

;12.If there is no path to the selected target, set the pathfinder's
	;xPath and yPath equal to its current location and return that the
	;path is nonexistent.
	.noPath
	unit\xPath = startingX
	unit\yPath = startingY
	Return nonexistent

End Function
	

;==========================================================
;READ PATH DATA: These functions read the path data and convert
;it to screen pixel coordinates.
Function ReadPath(unit.unit)			
	unit\xPath = ReadPathX(unit.unit,unit\pathLocation)
	unit\yPath = ReadPathY(unit.unit,unit\pathLocation)
End Function

Function ReadPathX#(unit.unit,pathLocation)
	If pathLocation <= unit\pathLength
		x = PeekShort (unit\pathBank,pathLocation*4)
		Return tileSize*x + .5*tileSize ;align w/center of square	
	End If
End Function	

Function ReadPathY#(unit.unit,pathLocation)
	If pathLocation <= unit\pathLength
		y = PeekShort (unit\pathBank,pathLocation*4+2)
		Return tileSize*y + .5*tileSize ;align w/center of square		
	End If
End Function


;==========================================================
;COLLISION/NODE CLAIMING FUNCTIONS: These functions handle node claiming
;and collision detection (which occurs when a unit tries to claim a node that
;another unit has already claimed).

;This function checks whether the unit is close enough to the next
;path node to advance to the next one or, if it is the last path step,
;to stop.
Function CheckPathStepAdvance(unit.unit)	
	
	;If starting a new path ...
	If unit\pathLocation = 0			
		If unit\pathLength > 0 
			unit\pathLocation = unit\pathLocation+1
			ClaimNodes(unit.unit)
			ReadPath(unit) ;update xPath and yPath
		Else If unit\pathLength = 0			
			ReadPath(unit) ;update xPath and yPath
			If unit\xLoc = unit\xPath And unit\yLoc = unit\yPath
				unit\pathStatus = notstarted
				ClearNodes(unit.unit)							
			End If
		End If		
	
	;If reaching the next path node.		
	Else If unit\xLoc = unit\xPath And unit\yLoc = unit\yPath
		If unit\pathLocation = unit\pathLength 
			unit\pathStatus = notstarted
			ClearNodes(unit.unit)	
		Else
			unit\pathLocation = unit\pathLocation + 1
			ClaimNodes(unit.unit)
			ReadPath(unit) ;update xPath and yPath		
		End If	
	End If
			
End Function

;This function claims nodes for a unit. It is called by CheckPathStepAdvance().
Function ClaimNodes(unit.unit)
		
	;Clear previously claimed nodes and claim the node the unit is currently occupying.
	ClearNodes(unit.unit)
	
	;Check next path node for a collision.
	unit\unitCollidingWith.unit = DetectCollision(unit.unit)	
	
	;If no collision is detected, claim the node and figure out
	;the distance to the node.
	If unit\unitCollidingWith = Null		
		x2 = PeekShort (unit\pathBank,unit\pathLocation*4)
		y2 = PeekShort (unit\pathBank,unit\pathLocation*4+2)	
		claimedNode(x2,y2) = unit	
		ReadPath(unit.unit) ;update xPath/yPath			
		unit\distanceToNextNode = GetDistance#(unit\xLoc,unit\yLoc,unit\xPath,unit\yPath)	
	
	;Otherwise, if a collision has been detected ...
	Else		
		
		;If node is occupied by a unit not moving normally, repath.
		If unit\unitCollidingWith\pathStatus <> found			
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)					

		;If there is a pending collision between the two units, repath.
		Else If UnitOnOtherUnitPath(unit.unit,unit\unitCollidingWith)
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	

		;Otherwise, just temporarily stop and wait for the other unit to pass by.	
		Else 				
			unit\pathStatus = tempStopped 									
		End If
				
	End If
		
End Function

;This function clears a unit's claimed nodes. This function is
;called principally by ClaimNodes() before new nodes are
;claimed. It is also called by CheckPathStepAdvance() when the 
;final path node is reached and by LaunchProgram() to initialize
;each unit's initial location.
Function ClearNodes(unit.unit)
	x = Floor(unit\xLoc/tileSize) : y = Floor(unit\yLoc/tileSize)
	For a = x-1 To x+1
	For b = y-1 To y+1
		If a>=0 And a<mapWidth And b>=0 And b<mapHeight
			If claimedNode(a,b) = unit Then claimedNode(a,b) = Null
		End If		
	Next
	Next
	claimedNode(x,y) = unit ;reclaim the one the unit is currently occupying.						
End Function

;This function checks to see if the next path step is free. 
;It is called from ClaimNodes() and by UpdatePath() when the
;unit is tempStopped.
Function DetectCollision.unit(unit.unit)
	x2 = PeekShort (unit\pathBank,unit\pathLocation*4)
	y2 = PeekShort (unit\pathBank,unit\pathLocation*4+2)			
	If claimedNode(x2,y2) = Null	
		x1 = Floor(unit\xLoc/tileSize)
		y1 = Floor(unit\yLoc/tileSize)	
		If x1<>x2 And y1<>y2 ;if next path step is diagonal
			If claimedNode(x1,y2) <> Null			
				If claimedNode(x1,y2) = claimedNode(x2,y1)	
					Return claimedNode(x1,y2)
				End If	
			End If								
		End If
	Else
		Return claimedNode(x2,y2)	
	End If	
End Function

;This function checks to see whether a unit is on another unit's 
;path. It is called by ClaimNodes().
Function UnitOnOtherUnitPath(unit.unit,otherUnit.unit)	
	unitX = Floor(unit\xLoc/tileSize)
	unitY = Floor(unit\yLoc/tileSize)		
	For pathLocation = otherUnit\pathLocation To otherUnit\pathLength		
		If unitX = PeekShort (otherUnit\pathBank,pathLocation*4)
			If unitY = PeekShort (otherUnit\pathBank,pathLocation*4+2)		
				Return True
			End If
		End If
		If pathLocation > otherUnit\pathLocation+1 Then Return
	Next
End Function 

;This function is used by the FindPath() function to 
;check whether the given target location is walkable.
;If not, it finds a new, nearby target location that is
;walkable. The new coordinates are written to the
;gInt1 and gInt2 global variables.
Function CheckRedirect(unit.unit, x,y)
	If NodeOccupied(unit.unit,x,y) = True
		For radius = 1 To 10
			For option = 1 To 4
				If option = 1 
					gInt1 = x : gInt2 = y-radius
				Else If option = 2 
					gInt1 = x : gInt2 = y+radius
				Else If option = 3 
					gInt1 = x-radius : gInt2 = y
				Else If option = 4 
					gInt1 = x+radius : gInt2 = y	
				End If			
				If gInt1 >= 0 And gInt1 < mapWidth And gInt2 >= 0 And gInt2 < mapHeight
					If NodeOccupied(unit.unit,gInt1,gInt2) = False Then Return succeeded ;1	
				End If
			Next
		Next
		Return failed ;unable to find redirect (returns -1).	
	End If
End Function

;This function is used by the CheckRedirect() function to 
;determine whether a given node is walkable for a given unit.
Function NodeOccupied(unit.unit,x,y)
	If walkability(x,y) <> unwalkable
		If claimedNode(x,y) = Null Or claimedNode(x,y) = unit ;node is free
			Return False
		Else ;there is another unit there
			If claimedNode(x,y)\pathStatus = found ;but if it is moving ...
				If claimedNode(x,y)<>unit\unitCollidingWith ;and unit is not colliding with it
				 	Return False
				End If
			End If	
		End If
	End If
	Return True
End Function


;This function is used by the FindPath() function to lay out
;'footprints' for other units within 1 node. FindPath() treats
;these nodes as unwalkable. 
Function CreateFootPrints(unit.unit)
	tempUnwalkable = onClosedList - 2
	unitX = Floor(unit\xLoc/tileSize) : unitY = Floor(unit\yLoc/tileSize)
	For a = unitX-1 To unitX+1
	For b = unitY-1 To unitY+1
		If a >= 0 And a < mapWidth And b>=0 And b < mapHeight
			If claimedNode(a,b) <> Null And claimedNode(a,b) <> unit
				tempUnwalkability(a,b) = tempUnwalkable
			End If			
		End If
	Next
	Next	
End Function	