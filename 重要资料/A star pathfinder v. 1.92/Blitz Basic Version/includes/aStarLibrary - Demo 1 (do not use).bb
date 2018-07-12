;This version of the A* Library has been written specifically for the
;Astar Demo 1 program. If you want to use the AStar functions in your
;program, use the other include file, which is used by Demo #2.

;The code in this demo *might* be a little easier to understand for
;people who are not experienced Blitz users. The main library uses
;Blitz "types," which are object-oriented.
;==================================================================

;DECLARE VARIABLES
.declareVariables

	Global mapWidth = 16 ;default
	Global mapHeight = 12 ;default
	
	;Create needed arrays
	Dim walkability(mapWidth+1,mapHeight+1) ;array that holds wall/obstacle information	
	Dim openList(mapWidth*mapHeight+2) ;array holding open list items
	Dim listArray(mapWidth+1,mapHeight+1)  ;array used to record 
		;whether a cell is on the open list or on the closed list.
	Dim openX(mapWidth*mapHeight+2) ;stores the x location of an item on the open list
	Dim openY(mapWidth*mapHeight+2) ;stores the y location of an item on the open list
	Dim parentX(mapWidth+1,mapHeight+1) ;array to store parent of each cell (x)
	Dim parentY(mapWidth+1,mapHeight+1) ;array to store parent of each cell (y)
	Dim Fcost(mapWidth*mapHeight+2)	;array to store F cost for each cell.
	Dim Gcost(mapWidth+1,mapHeight+1) 	;array to store G cost for each cell.
	Dim Hcost(mapWidth*mapHeight+2)   ;array to store H cost for each cell.
	Dim pathLength(2)    ;length of the found path	
	Dim pathLocation(2)  ;current position along the chosen path		

	;Create a memory bank called pathBank to save path data to.
	Dim pathBank(2)
	pathBank(1) = CreateBank(1)
	
	;Declare constants
	Const onOpenList = 1, onClosedList = 2, onNeitherList = 0 ;listArray constants
	Const notfinished = 0, found = 1, nonexistent = 2 ; path-related constants
	Const walkable = 0, unwalkable = 1


;==========================================================
;FIND PATH: This function finds the path and saves it.
Function FindPath(pathfinderID,mapWidth,mapHeight,startX,startY,targetX,targetY,stepBystep=False)

;1.	Reset some variables that need to be cleared
	Dim listArray(mapWidth,mapHeight) ;Reset array indicating if cell is on the open or closed list.
	pathLength(pathfinderID) = notfinished
	pathLocation(pathfinderID) = 1

;2.	Add the starting location to the open list of squares to be checked.
	openListItems = 1
	openList(1) = 1
	openX(1) = startX : openY(1) = startY
	Gcost(startX,startY) = 0 ;reset starting square's G value to 0

;3.	Do the following until a path is found or deemed nonexistent.
	Repeat
	
;4.	If the open list is not empty, take the first cell off of the list.
;	This is the lowest F cost cell on the open list.
	If openListItems <> 0 Then

	;Pop the first item off the list
	parentXval = openX(openList(1)) : parentYVal = openY(openList(1)) ;record cell coordinates
	listArray(parentXval,parentYVal) = onClosedList ;add cell to closed list

	;Open List = Binary Heap: For more on binary heaps/priority queues and 
	;their uses in A*, and how this particular implementation of binary heaps using
	;arrays works, see the following: 
	;http://www.policyalmanac.org/games/binaryHeaps.htm
	openListItems = openListItems - 1 ;reduce number of open list items by 1
	openList(1) = openList(openListItems+1) ;put last item in slot #1
	v = 1
	Repeat
		u = v	
		If 2*u <= openListItems ;if child #1 exists
		 	;Check if the F cost of the parent is greater than child #1	
			If Fcost(openList(u)) >= Fcost(openList(2*u)) Then v = 2*u
		End If
		If 2*u+1 <= openListItems ;if child #2 exists
		 	;Check if the F cost of the parent is greater than child #2	
			If Fcost(openList(v)) >= Fcost(openList(2*u+1)) Then v = 2*u+1		
		End If		
		If u<>v ;if parent's F is > one of its children, swap them
			temp = openList(u)
			openList(u) = openList(v)
			openList(v) = temp				
		Else
			Exit ;if item >= both children, exit loop
		End If	
	Forever
	
;5.	Check the adjacent cells (Its "children" -- these path children
;	are similar, conceptually, to the binary heap children mentioned
;	above, but don't confuse them. They are different. Path children
;	are portrayed in Demo 1 with grey pointers pointing toward
;	their parents.) Add these adjacent child cells to the open list
;	for later consideration if appropriate (see various if statements
;	below).
	For b = parentYVal-1 To parentYVal+1
	For a = parentXval-1 To parentXval+1

	;If not off the map
	If a <> -1 And b <> -1 And a <> mapWidth And b <> mapHeight

	;If not already on the closed list.			
	If listArray(a,b) <> onClosedList  
	
	;If not a wall/obstacle cell.
	If walkability(a,b) = walkable	

	;Don't cut across corners (optional)
	corner = walkable	
	If a = parentXVal-1 
		If b = parentYVal-1 
			If walkability(parentXval-1,parentYval) = unwalkable Or walkability(parentXval,parentYval-1) = unwalkable Then corner = unwalkable
		Else If b = parentYVal+1 
			If walkability(parentXval,parentYval+1) = unwalkable Or walkability(parentXval-1,parentYval) = unwalkable Then corner = unwalkable 
		End If
	Else If a = parentXVal+1 
		If b = parentYVal-1 
			If walkability(parentXval,parentYval-1) = unwalkable Or walkability(parentXval+1,parentYval) = unwalkable Then corner = unwalkable 
		Else If b = parentYVal+1 
			If walkability(parentXval+1,parentYval) = unwalkable Or walkability(parentXval,parentYval+1) = unwalkable Then corner = unwalkable 
		End If
	End If	
	If corner = walkable

	;If not already on the open list.			
	If listArray(a,b) <> onOpenList ;= onNeitherList 

		;Create a new open list item in the binary heap. Put it at the 
		;bottom end of the heap and let it bubble up to its appropriate
		;place, based on its F value.
		squaresChecked = squaresChecked + 1
		m = openListItems+1 ;m = new item at end of heap
		openList(m) = squaresChecked		
		openX(openList(m)) = a : openY(openList(m)) = b

		;Figure out its G cost
		If Abs(a-parentXval) = 1 And Abs(b-parentYVal) = 1 Then
			addedGCost = 14 ;cost of going to diagonal tiles	
		Else	
			addedGCost = 10 ;cost of going to non-diagonal tiles				
		End If
		Gcost(a,b) = Gcost(parentXval,parentYVal)+addedGCost
			
		;Figure out its H and F costs
		Hcost(openList(m)) = estimateHcost(a,b,targetx,targety)	
		Fcost(openList(m)) = Gcost(a,b) + Hcost(openList(m))			
		parentX(a,b) = parentXval : parentY(a,b) = parentYVal		
		
		;Insert it at the appropriate place in the binary heap open list.
		;Starting at the bottom, successively compare to parent items
		;swapping as needed, until the item finds its place in the heap
		;or bubbles all the way to the top.
		While m <> 1 ;While item hasn't bubbled to the top (m=1)	
			;Check if child is < parent. If so, swap them.	
			If Fcost(openList(m)) <= Fcost(openList(m/2)) Then
				temp = openList(m/2)
				openList(m/2) = openList(m)
				openList(m) = temp
				m = m/2
			Else
				Exit
			End If
		Wend 
		openListItems = openListItems+1 ;add one to number of items in the heap

		;Change listArray to show that the new item is on the open list.
		listArray(a,b) = onOpenList


;6.	If adjacent cell is already on the open list, check to see if this 
;	path to that cell from the starting location is a better one. 
;	If so, change the parent of the cell and its G and F costs.	
	Else; If listArray(a,b) = onOpenList
	
		;Figure out G cost of this path
		If Abs(a-parentXval) = 1 And Abs(b-parentYVal) = 1 Then
			addedGCost = 14;cost of going to diagonal tiles	
		Else	
			addedGCost = 10 ;cost of going to non-diagonal tiles				
		End If
		tempGcost = Gcost(parentXval,parentYVal)+addedGCost
		
		;If this path is shorter (G cost is lower) then change
		;the parent cell, G cost and F cost. 		
		If tempGcost < Gcost(a,b) Then 	;if G cost is less,
			parentX(a,b) = parentXval 	;change the parent
			parentY(a,b) = parentYVal
			Gcost(a,b) = tempGcost 		;change the G cost			

			;Because changing the parent also changes the F cost, if
			;the item is on the open list we need to change the item's
			;recorded F cost and its position on the open list to make
			;sure that we maintain a properly ordered open list.
			If listArray(a,b) = onOpenList
			For x = 1 To openListItems
			If openX(openList(x)) = a And openY(openList(x)) = b Then ;item found
				FCost(openList(x)) = Gcost(a,b) + HCost(openList(x))
				m = x
				
				;See if changing F score bubbles item from it's current location
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
			Next ;For x = 1 To openListItems
			End If ;If listArray(a,b) = onOpenList	

		End If ;If tempGcost < Gcost(a,b) Then

	End If ;If not already on the open list		
	End If ;If corner = walkable
	End If ;If not a wall/obstacle cell.	
	End If ;If not already on the closed list	
	End If ;If not off the map.	
	Next
	Next


;7.	If open list is empty then there is no path.	
	Else
		path = nonExistent : Exit
	End If

	;If target is added to open list then path has been found.
	If listArray(targetx,targety) = onOpenList Then path = found : Exit	


;8.	If in step-by-step mode draw the screen step by step, waiting
;	for a keypress before the next step is taken.
	If stepByStep = True And path = notfinished Then 
		RenderScreen(startX,startY,targetX,targetY,True)
		WaitKey()	
		;If return is hit, finish finding the rest of the path.		
		If KeyHit(28) Then stepByStep = False
		FlushKeys		
	End If

	Forever ;repeat until path is found or deemed nonexistent


;9.	Save the path if it exists. Copy it to a bank
	If path = found
		
		;a. Working backwards from the target to the starting location by checking
		;each cell's parent, figure out the length of the path.
		pathX = targetX : pathY = targetY	
		Repeat
			tempx = parentX(pathX,pathY)		
			pathY = parentY(pathX,pathY)
			pathX = tempx
			pathLength(pathfinderID) = pathLength(pathfinderID) + 1
		Until pathX = startX And pathY = startY
	
		;b. Resize the data bank to the right size (include a 15 byte buffer
		;on both sides of data to ensure no memory overwrites)	
		ResizeBank pathBank(pathfinderID),pathLength(pathfinderID)*4+30

		;c. Now copy the path information over to the databank. Since we are
		;working backwards from the target to the start location, we copy
		;the information to the data bank in reverse order. The result is
		;a properly ordered set of path data, from the first step to the
		;last.
		pathX = targetX : pathY = targetY				
		cellPosition = pathLength(pathfinderID)*4 ;start at the end	
		While Not (pathX = startX And pathY = startY)			
			PokeShort pathBank(pathfinderID),cellPosition,pathX ;store x value	
			PokeShort pathBank(pathfinderID),cellPosition+2,pathY ;store y value	
			cellPosition = cellPosition - 4 ;work backwards		
			tempx = parentX(pathX,pathY)		
			pathY = parentY(pathX,pathY)
			pathX = tempx
		Wend	
		PokeShort pathBank(pathfinderID),0,startX ;store starting x value	
		PokeShort pathBank(pathfinderID),2,startY ;store starting y value
		
	End If ;If path = found Then  

	Return path; Return information about whether a path has been found.
	
End Function
	
	
;This function estimates the distance from a given square to the
;target square, usually refered to as H or the H cost.	
Function EstimateHcost(a,b,targetx,targety)

		;(1) Manhattan (dx+dy)	
		H = 10*(Abs(a - targetx) + Abs(b - targety))
					
	Return H

		;Other possible H estimate methods (not used here) include:

		;(1) Diagonal Shortcut Estimation Method
		xDistance = Abs(a - targetx)
		yDistance = Abs(b - targety)
		If xDistance > yDistance Then 
			H = 14*yDistance + 10*(xDistance-yDistance)
		Else
			H = 14*xDistance + 10*(yDistance-xDistance)
		End If					
	
		;(2) Max(dx,dy)
		xDistance = Abs(a - targetx)
		yDistance = Abs(b - targety)
		If xDistance > yDistance Then 
			H = 10*xDistance
		Else
			H = 10*yDistance
		End If

End Function	

	
;==========================================================
;READ PATH DATA: These functions read the path data and convert
;it to screen pixel coordinates.
	
Function ReadPath(pathfinderID)
	pathLocation(pathfinderID) = 1 ;set pathLocation to 1st step
	While pathLocation(pathfinderID) < pathLength(pathfinderID)
		a = readPathX(pathfinderID,pathLocation(pathfinderID))
		b = readPathY(pathfinderID,pathLocation(pathfinderID))		
		pathLocation(pathfinderID) = pathLocation(pathfinderID) + 1	
		listArray(a,b) = 3 ;draw dotted path
	Wend 
End Function

Function ReadPathX(pathfinderID,pathLocation)
	x = PeekShort (pathBank(pathfinderID),pathLocation*4)
	Return x
End Function	

Function ReadPathY(pathfinderID,pathLocation)
	y = PeekShort (pathBank(pathfinderID),pathLocation*4+2)
	Return y
End Function

	