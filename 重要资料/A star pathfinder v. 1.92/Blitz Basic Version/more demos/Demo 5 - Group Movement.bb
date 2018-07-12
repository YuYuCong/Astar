;PATHFINDING DEMO #5 - Group Movement
;===============================================================
;By Patrick Lester (Updated 4/7/04)

;An article describing A* and this code in particular can be found at:
;http://www.policyalmanac.org/games/aStarTutorial.htm

;Features
;----------
;This demo adds a few features that facilitate group movement. These
;new features include:

;- When a group of units is selected and sent to a given location on the 
;  map, different destinations are chosen for each group member.
;- Paths already claimed by nearby units have a small terrain penalty 
;  added to them. This encourages units to find paths that are slightly
;  different from nearby units, which reduces collisions.
;- When a collision occurs, units will always repath, but will not use the 
;  new path if it is too long compared to simply temporarily stopping in 
;  place and waiting for the other unit to move out of the way. This makes 
;  movement a bit smoother than tempstopping most of the time.
;- Areas of the map that are completely walled off from the rest of the map 
;  are identified as separate islands. The CheckRedirect() function is updated to 
;  reflect this information.
;- The FindPath() function has been modified to allow small random movement 
;  when a nonexistent path is found. This prevents units from locking up if they 
;  are approaching from opposite directions down a corridor.

;Instructions
;------------
;- Press enter or right click to activate the sprites, then 
;  right click on any reachable spot on the map. The selected 
;  sprite will use AStar to find the best path to that spot and then
;  go there. You may continue to left or right click to find more
;  paths.
;- Select different units by left clicking on them or box-selecting them.
;- Press the space bar to toggle drawing paths on and off.
;- Draw blue walls by left clicking on the screen. 
;- Erase blue walls by left clicking on them.
;- Press enter again to deactivate the sprites and return		
;  to map drawing mode.
;- Press escape to exit the program.

;===============================================================

;A* set up
Include "includes/aStarLibrary - Demo 5.bb" ;contains A* code.
Include "includes/shared functions.bb"

;Declare globals and types
Global cursor, grid, wallBlock, gMap, gSelectedCircle  ;graphics
Global gDrawing, gErasing, gGameStarted, gDrawMore, gDrawText	
Global gBoxSelecting, gBoxSelectX,gBoxSelectY, gMouseHit1, gMouseHit2
Const userControlled = 1, patrol = 2, random = 3
Type unit
	Field ID, xLoc#, yLoc#, speed# ;speed is in pixels/second
	Field pathAI, pathStatus, pathLength, pathLocation
	Field pathBank, pathBank1, pathBank2
	Field xPath#, yPath#, distanceToNextNode# ;in pixels
	Field targetX#, targetY#, target.unit, unitCollidingWith.unit
	Field startNewPath ;used for delayed-action pathfinding
	Field sprite, red, green, blue
	Field selected
	Field xDistanceFromGroupCenter#, yDistanceFromGroupCenter# 
	Field actualAngleFromGroupCenter, assignedAngleFromGroupCenter 	
End Type

;Full game sequence
LaunchProgram()		
RunProgram()
End
	
;Main program loop	
Function RunProgram()
	While Not KeyHit(1) ;while escape key isn't pressed		
		UpdateGameClock() ;see shared functions.bb include file
		UserInput() ;process mouse and keyboard input	
		MoveUnits() ;move sprites			
		RenderScreen() ;draw stuff on the screen		
	Wend
End Function

Function LaunchProgram()
	If Not GfxModeExists(800,600,16) RuntimeError "Sorry, this program won't work with your graphics card."	
	Graphics 800,600 : SetBuffer BackBuffer()
	HidePointer : gDrawMore = True
	
	;Load Graphics
	cursor = LoadImage("../../graphics/red_pointer.bmp")
	MaskImage cursor,255,255,255 : MoveMouse 400,225 			
	grid = LoadImage("../../graphics/grid.bmp")		
	MaskImage grid,255,255,255
	wallBlock = LoadImage("../../graphics/wall.bmp")  : ScaleImage wallBlock,.5,.5		
	smiley = LoadImage("../../graphics/smiley.bmp")
	MaskImage smiley,0,0,0 : MidHandle smiley : ScaleImage smiley,.5,.5	
	chaser = LoadImage("../../graphics/ghost.bmp")
	MaskImage chaser,0,0,0 : MidHandle chaser : ScaleImage chaser,.5,.5

	;Create green selection circle
	gSelectedCircle = CreateImage(24,24)
	SetBuffer ImageBuffer(gSelectedCircle)
	Color 0,255,0 : Oval 0,0,24,24,0		
	SetBuffer BackBuffer() : MidHandle gSelectedCircle  

	;Load map walkability data
	LoadMapData("myTerrainData - Demo 5.dat");see common functions.bb include file
	
	;Create and initialize some units
	For ID = 1 To 6
		unit.unit = New unit
		unit\ID = ID : unit\sprite = smiley : unit\speed = 200
	
		If ID = 1 
			unit\xLoc = 112.5 : unit\yLoc = 337.5 : unit\selected = True		
			unit\red = 255 : unit\green = 255 : unit\blue = 0 			
		Else If ID = 2 
			unit\xLoc = 737.5 : unit\yLoc = 337.5 : unit\selected = True	
			unit\red = 255 : unit\green = 0  : unit\blue = 255						
		Else If ID = 3 
			unit\xLoc = 437.5 : unit\yLoc = 237.5 : unit\selected = True	
			unit\red = 0 : unit\green = 0 : unit\blue = 255			
		Else
			Repeat
				x = tileSize*(Rand(775)/tileSize)+tileSize/2
				y = tileSize*(Rand(575)/tileSize)+tileSize/2
			Until walkability(x/tileSize,y/tileSize) = walkable
			unit\xLoc = x : unit\yLoc = y : unit\sprite = chaser
			unit\pathAI = random							
			unit\red = Rand(255) : unit\green = Rand(255) : unit\blue = Rand(255)						
		End If

		unit\pathBank1 = CreateBank(4)			
		unit\pathBank2 = CreateBank(4)
		unit\pathBank = 	unit\pathBank1 
		unit\targetX = unit\xLoc : unit\targetY = unit\yLoc ;required			
		unit\xPath = unit\xLoc : unit\yPath = unit\yLoc ;required			
		ClearNodes(unit.unit)	
	Next		

End Function

;This function handles most user mouse and keyboard input
Function UserInput()

	;Record the mouse hit variables
	gMouseHit1 = MouseHit(1)
	gMouseHit2 = MouseHit(2)	

	;If in map edit mode
	If gGameStarted = False

		If (Not MouseDown(1)) Then gDrawing = False
		If (Not MouseDown(1)) Then gErasing = False
	
		;Edit map by drawing or erasing walls
		If MouseDown(1) 
		
			;Draw walls
			If walkability(MouseX()/tileSize,MouseY()/tileSize) = walkable And gErasing = False
			 	walkability(MouseX()/tileSize,MouseY()/tileSize) = unwalkable
				gDrawing = True
			End If

			;Erase walls	
			If walkability(MouseX()/tileSize,MouseY()/tileSize) = unwalkable And gDrawing = False
			 	walkability(MouseX()/tileSize,MouseY()/tileSize) = walkable
				gErasing = True
			End If	
	
		;Start game if return/enter or right mouse button is hit
		Else If KeyHit(28) Or gMouseHit2 = True
			gGameStarted = True : gMouseHit2 = False
			CopyMapImage()			
			IdentifyIslands() ;update islands() array.			
		End If

	;If in game/pathfinding mode ...
	Else
		;Start box selection by left clicking on the map
		If gMouseHit1
			For unit.unit = Each unit
				unit\selected = False
				If ImageRectCollide(unit\sprite,unit\xLoc,unit\yLoc,0,MouseX(),MouseY(),1,1) = True
					unit\selected = True : oneSelected = True
				End If
			Next
			If oneSelected = False
				gBoxSelecting = True
				gBoxSelectX = MouseX()
				gBoxSelectY = MouseY()
			End If	

		;Box select once the mouse is released.	
		Else If MouseDown(1) = False And gBoxSelecting = True	
			BoxSelectUnits()

		;If right clicking, update target locations for the selected group.	
		Else If gMouseHit2		
			ChooseGroupLocations()	
	
		;Reenter map edit mode by pressing enter/return key
		Else If KeyHit(28) Then 
			gGameStarted = False	
			For unit.unit = Each unit
				unit\pathStatus = notstarted
			Next	
		End If		
				
	End If

	;Save the map by pressing the "s" key
	If KeyHit(31) Then SaveMapData("myTerrainData - Demo 5.dat") ;see common functions.bb include file	

	;Toggle drawing more by pressing space bar
	If KeyHit(57) Then gDrawMore = 1-gDrawMore

	;Toggle drawing text by pressing "t" key
	If KeyHit(20) Then gDrawText = 1-gDrawText	
	
	;ScreenCapture by pressing print screen or F12.
	ScreenCapture() ;see common functions.bb include file		

End Function

;This function selects units when the mouse has been released. It is
;called from the UserInput() function.
Function BoxSelectUnits()
	gBoxSelecting = False
	x1 = Min(gBoxSelectX,MouseX()) : x2 = Max(gBoxSelectX,MouseX())	
	y1 = Min(gBoxSelectY,MouseY()) : y2 = Max(gBoxSelectY,MouseY())	
	For unit.unit = Each unit
		If unit\xLoc > x1 And unit\xLoc < x2 And unit\yLoc > y1 And unit\yLoc < y2
			unit\selected = True
		Else
			unit\selected = False	
		End If
	Next
End Function


;This function draws stuff on the screen.
Function RenderScreen()
	
	;Draw the walls and the grid 	overlay
	DrawMapImage() ;see shared functions.bb include file

	;Draw paths
	If gDrawMore = True
		For unit.unit = Each unit
			If unit\selected = True
				Color unit\red,unit\green,unit\blue			
				For pathLocation = 1 To unit\pathLength
					x1 = ReadPathX(unit.unit,pathLocation-1)
					y1 = ReadPathY(unit.unit,pathLocation-1)
					x2 = ReadPathX(unit.unit,pathLocation)
					y2 = ReadPathY(unit.unit,pathLocation)
					x1=x1+unit\ID*2 : x2=x2+unit\ID*2 : y1=y1+unit\ID : y2=y2+unit\ID									
					Line x1,y1,x2,y2		
				Next	
			End If	
		Next	
	End If
	
	;Draw units
	For unit.unit = Each unit
		DrawImage unit\sprite,unit\xLoc,unit\yLoc	
		If unit\selected = True Then DrawImage gSelectedCircle,unit\xLoc,unit\yLoc					
	Next

	;Draw unit claimed nodes
	DrawClaimedNodes(True);see shared functions.bb include file

	;Draw selection box
	If gBoxSelecting = True
		Color 0,255,0
		Line gBoxSelectX,gBoxSelectY,MouseX(),gBoxSelectY
		Line MouseX(),gBoxSelectY,MouseX(),MouseY()
		Line MouseX(),MouseY(),gBoxSelectX,MouseY()
		Line gBoxSelectX,MouseY(),gBoxSelectX,gBoxSelectY							
	End If
	
	;Draw text on the screen showing some unit statistics.
	If gDrawText = True
		Color 255,255,255 : x = 50 : y = 50
		For unit.unit = Each unit
			Text x,y+0,unit\ID
			Text x,y+15,unit\selected			
			Text x,y+30,unit\pathStatus
			Text x,y+45,unit\xLoc 
			Text x,y+60,unit\yLoc 
			Text x,y+75,unit\xPath
			Text x,y+90,unit\yPath 	
			Text x,y+105,unit\pathLocation+"/"+unit\pathLength			
			x = x + 100	
			If unit\ID Mod 8 = 7 Then y = y+200 : x = 50				
		Next
	End If	
	
	DrawImage cursor,MouseX(),MouseY() ;Draw the mouse
	Flip
End Function

;This function performs pathfinding and moves the units.
Function MoveUnits()
	If gGameStarted = True 
		If KeyDown(16) Then ShowPointer : Stop ;debug-related (stop when pressing 'q')
		For unit.unit = Each unit
			UpdatePath(unit)
			If unit\pathStatus = found Then MoveUnit(unit)				
		Next
	End If		
End Function	

;This function checks for path updates and calls the
;FindPath() function when needed.
Function UpdatePath(unit.unit)
		
	;If the unit is tempStopped, keep checking the
	;blocked path node until it is free and the unit is able continue
	;along its path. If the next step is blocked by a stopped unit
	;then repath.	
	If unit\pathStatus = tempStopped 		
		otherUnit.unit = DetectCollision(unit.unit)
		If otherUnit = Null
			unit\pathStatus = found
			ClaimNodes(unit.unit)	
		Else If otherUnit\pathStatus <> found ;node is blocked by nonmoving unit
			unit\unitCollidingWith = otherUnit
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
		End If						
	
	;If the unit's path is nonexistent, find a path to a random location that
	;is nearby. This will tend to break up units that are locked in place.
	Else If unit\pathStatus = nonexistent		
		unit\pathStatus = FindPath(unit.unit,0,0,randomMove)
		
	;If the unit's pathStatus = notStarted, and the unit is not at its target location, then
	;generate a new path to that location. This can be true if a unit has found a path
	;to a random location after experiencing a nonexistent path (see above).
	Else If unit\pathStatus = stopped
		x1 = Floor(unit\xLoc/tileSize) : y1 = Floor(unit\yLoc/tileSize) 
		x2 = Floor(unit\targetX/tileSize) : y2 = Floor(unit\targetY/tileSize) 		
		If  x1 <> x2 Or y1 <> y2			
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)										
		End If			
	End If			

	;If the unit has been selected, trigger new paths using the mouse.
	;There is a delay built in so the new path isn't implemented
	;until the next node is reached.
	If unit\selected = True		
		If gMouseHit2 = True					 												
			If unit\distanceToNextNode = 0
				unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
			Else		
				unit\startNewPath = True ;wait to trigger path (see below)
			End If
		Else If unit\startNewPath = True And unit\distanceToNextNode = 0 
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)
			unit\startNewPath = False					
		End If
		
	;If pathAI = random, choose a random spot on the screen to pathfind to.
	Else If unit\pathAI = random
		If unit\pathstatus = stopped
			unit\targetX = Rand(0,799)
			unit\targetY = Rand(0,599)
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
		End If						
	End If
	
End Function
	

;This function moves sprites around on the screen. 
Function MoveUnit(unit.unit)

	;Move toward the next path node
	remainingDistance# = MoveTowardNode#(unit.unit,gLoopTime*unit\speed)	
	
	;If there is any remaining distance left after moving toward the node, then
	;check for path step advances and move to the next one. This two step
	;process ensures smooth movement from node to node.
	If remainingDistance <> 0 And unit\startNewPath = False	
		MoveTowardNode#(unit.unit,remainingDistance#)		
	End If		

End Function


;This function checks for path step advances and then moves toward the
;next path node. If the next node is reached, the function returns any
;remaining distance left to be travelled.
Function MoveTowardNode#(unit.unit,distanceTravelled#)
	CheckPathStepAdvance(unit.unit)
	If unit\pathStatus <> found Then Return
	If distanceTravelled# <= unit\distanceToNextNode#
		xVector# = unit\xPath-unit\xLoc
		yVector# = unit\yPath-unit\yLoc	
		angle# = ATan2(yVector#,xVector#)
		unit\xLoc = unit\xLoc + Cos(angle)*distanceTravelled
		unit\yLoc = unit\yLoc + Sin(angle)*distanceTravelled
		unit\distanceToNextNode# = unit\distanceToNextNode#-distanceTravelled# 
	Else ;next path node has been reached
		unit\xLoc = unit\xPath : unit\yLoc = unit\yPath	
		remainingDistance# = distanceTravelled#-unit\distanceToNextNode# 
		unit\distanceToNextNode = 0
		Return remainingDistance#	
	End If	
End Function