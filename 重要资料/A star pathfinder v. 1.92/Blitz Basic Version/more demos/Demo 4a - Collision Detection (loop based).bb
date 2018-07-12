;PATHFINDING DEMO #4a - Collision Avoidance (loop-based)
;===============================================================
;By Patrick Lester (Updated 3/15/04)

;This demo illustrates pathfinding in an environment where units must be able
;to detect collisions and pathfind around one another. I may write an article
;describing these concepts in more detail later, but for now this will have to do:

;Basically units 'claim' nodes as they move along. A unit claims both the square
;it is in and the next one it is moving to. If no other units have claimed the node,
;it moves along normally. If the node has already been claimed, a collision is detected and 
;the unit does one of two things: 

;(1) if the other unit is stopped or following a path that will result in a head-on
;     collision with this unit, this unit generates a new path.

;(2) otherwise, the unit stops and waits for the other unit move out of the way before
;     continuing on its current path.

;When generating new paths, units pay attention to stopped units and any adjacent
;units. They ignore any units that are moving and not adjacent to them.

;A tutorial describing A* in general can be found at:
;http://www.policyalmanac.org/games/aStarTutorial.htm

;Instructions
;------------
;- Press enter or right click to activate the sprites, then left 
;  or right click on any reachable spot on the map. The user-controlled 
;  sprite will use AStar to find the best path to that spot and then
;  go there. You may continue to left or right click to find more
;  paths.
;- Press the space bar to toggle drawing paths on and off.
;- Draw blue walls by left clicking on the screen. 
;- Erase blue walls by left clicking on them.
;- Press enter again to deactivate the sprites and return		
;  to map drawing mode.
;- Press escape to exit the program.

;Please email comments to Patrick Lester at pwlester@policyalmanac.org

;===============================================================

;A* set up
Include "includes/aStarLibrary - Demo 4a (loop based).bb" ;contains A* code.
Include "includes/shared functions.bb"

;Declare globals and types
Global cursor, grid, wallBlock, gMap ;graphics
Global gDrawing, gErasing, gGameStarted, gDrawMore
Const userControlled = 1, patrol = 2, random = 3
Type unit
	Field ID, xLoc#, yLoc#, speed# ;speed is in pixels/second
	Field pathAI, pathStatus, pathLength, pathLocation, pathBank, xPath#, yPath#
	Field targetX, targetY, target.unit, unitCollidingWith.unit
	Field startNewPath ;used for delayed-action pathfinding
	Field sprite, red, green, blue
End Type

;Full game sequence
LaunchProgram()		
RunProgram()
End
	
;Main program loop	
Function RunProgram()
	While Not KeyHit(1) ;while escape key isn't pressed	
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
	wallBlock = LoadImage("../../graphics/wall.bmp") ;wall	
	smiley = LoadImage("../../graphics/smiley.bmp")
	MaskImage smiley,0,0,0 : MidHandle smiley	
	chaser = LoadImage("../../graphics/ghost.bmp")
	MaskImage chaser,0,0,0 : MidHandle chaser

	;Load map walkability data
	LoadMapData("myTerrainData - demo 4.dat");see common functions.bb include file
	
	;Create and initialize some units
	For x = 1 To 4
		unit.unit = New unit
		unit\ID = x
		unit\pathBank = CreateBank(4) ;data bank that unit's path data is stored	
		If unit\ID = 1 Then unit\sprite = smiley
		If unit\ID <> 1 Then unit\sprite = chaser
		unit\speed = 4
		If x = 1 
			unit\xLoc = 425 : unit\yLoc = 225 : unit\pathAI = userControlled			
			unit\red = 255 : unit\green = 255 : unit\blue = 0 			
		Else If x = 2 
			unit\xLoc = 425 : unit\yLoc = 425 : unit\pathAI = random
			unit\red = 0 : unit\green = 255 : unit\blue = 0						
		Else If x = 3 
			unit\xLoc = 125 : unit\yLoc = 325 : unit\pathAI = patrol
			unit\red = 0 : unit\green = 0 : unit\blue = 255				
		Else If x = 4 
			unit\xLoc = 725 : unit\yLoc = 325 : unit\pathAI = patrol
			unit\red = 255 : unit\green = 0 : unit\blue = 255				
		End If		
		ClearNodes(unit.unit)	
	Next	

End Function

;This function handles most user mouse and keyboard input
Function UserInput()

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
	
		;Activate smiley sprite if return/enter or right mouse button is hit
		Else If KeyHit(28) Or MouseHit(2)
			gGameStarted = True
			CopyMapImage()
			FlushMouse
		End If

	;If in game/pathfinding mode then reenter map edit mode by pressing 
	;enter/return key. This stops the units.
	Else
		If KeyHit(28) Then 
			gGameStarted = False	
			For unit.unit = Each unit
				unit\pathStatus = notstarted
			Next	
		End If				
	End If

	;Save the map by pressing the "s" key
	If KeyHit(31) Then SaveMapData("myTerrainData - demo 4.dat") ;see common functions.bb include file	

	;Toggle drawing more by pressing space bar
	If KeyHit(57) Then gDrawMore = 1-gDrawMore
	
	;ScreenCapture by pressing print screen or F12.
	ScreenCapture() ;see common functions.bb include file		

End Function


;This function draws stuff on the screen.
Function RenderScreen()
	
	;Draw the walls and the grid 	overlay
	DrawMapImage() ;see shared functions.bb include file

	;Draw paths
	If gDrawMore = True
		For unit.unit = Each unit
			Color unit\red,unit\green,unit\blue			
			For pathLocation = 1 To unit\pathLength
				x1 = ReadPathX(unit.unit,pathLocation-1)
				y1 = ReadPathY(unit.unit,pathLocation-1)
				x2 = ReadPathX(unit.unit,pathLocation)
				y2 = ReadPathY(unit.unit,pathLocation)
				x1=x1+unit\ID*2 : x2=x2+unit\ID*2 : y1=y1+unit\ID : y2=y2+unit\ID									
				Line x1,y1,x2,y2		
			Next		
		Next	
	End If
	
	;Draw units
	For unit.unit = Each unit
		DrawImage unit\sprite,unit\xLoc,unit\yLoc			
	Next

	;Draw unit claimed nodes
	DrawClaimedNodes();see shared functions.bb include file
	
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
		unit\targetX = unit\xLoc-200+Rand(400)
		unit\targetY = unit\yLoc-200+Rand(400)		
		If unit\targetX < 0 Then unit\targetX = 0
		If unit\targetX > 799 Then unit\targetX = 799
		If unit\targetY < 0 Then unit\targetY = 0
		If unit\targetY > 599 Then unit\targetY = 599		
		unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
		
	;If pathAI = userControlled, trigger new paths using the mouse.
	;There is a delay built in so the new path isn't implemented
	;until the next node is reached.
	Else If unit\pathAI = userControlled		
		If MouseHit(1) Or MouseHit(2)			
			unit\targetX = MouseX()
			unit\targetY = MouseY()
			If unit\pathStatus = notstarted Or ReachedNextPathNode(unit) = True
				unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
			Else		
				unit\startNewPath = True ;wait to trigger path 
			End If
		Else If unit\startNewPath = True And ReachedNextPathNode(unit) = True 
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)
			unit\startNewPath = False					
		End If

	;If pathAI = patrol, patrol back and forth between two points.
	Else If unit\pathAI = patrol
		If unit\pathstatus = notstarted
			If unit\xLoc = 725 
				unit\targetX = 150
			Else
				unit\targetX = 725		
			End If
			unit\targetY = 325
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
		End If
		
	;If pathAI = random, choose a random spot on the screen to pathfind to.
	Else If unit\pathAI = random
		If unit\pathstatus = notstarted
			unit\targetX = Rand(0,799)
			unit\targetY = Rand(0,599)
			unit\pathStatus = FindPath(unit.unit,unit\targetX,unit\targetY)	
		End If						
	End If
	
End Function
	

;This function moves the sprites around on the screen.
Function MoveUnit(unit.unit)

	;Check for pathStep advances.
	CheckPathStepAdvance(unit.unit)
	If unit\pathStatus = tempStopped Then Return	

	;Move the unit
	xVector# = unit\xPath-unit\xLoc
	yVector# = unit\yPath-unit\yLoc	
	angle# = ATan2(yVector#,xVector#)
	xSpeed# = Cos(angle)*unit\speed
	ySpeed# = Sin(angle)*unit\speed	
	If Abs(unit\xLoc - unit\xPath) < Abs(xSpeed)
		unit\xLoc = unit\xPath	
	Else	
		unit\xLoc = unit\xLoc + xSpeed 
	End If
	If Abs(unit\yLoc - unit\yPath) < Abs(ySpeed)
		unit\yLoc = unit\yPath
	Else										
		unit\yLoc = unit\yLoc + ySpeed	
	End If

End Function