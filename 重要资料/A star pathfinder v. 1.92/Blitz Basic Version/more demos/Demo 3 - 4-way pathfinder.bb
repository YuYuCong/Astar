;PATHFINDING DEMO #3 - Smiley & Chaser (4-way)
;===============================================================
;By Patrick Lester (Updated 3/5/04)

;This demo is the same as demo #2, except that it uses a 4-way pathfinder
;rather than an 8-way pathfinder, which may be better for platform games,
;among other uses.

;A tutorial describing A* and this code in particular can be found at:
;http://www.policyalmanac.org/games/aStarTutorial.htm

;This demo uses Blitz "types" to store data about units. Types are 
;an object-oriented data construct. If you are not familiar with
;Blitz types, the code in demo #1 may be a bit easier to understand.

;Instructions
;------------
;- Draw blue walls by left clicking on the screen. 
;- Erase blue walls by left clicking on them.
;- Press enter or right click to activate the smiley sprite, then
;  left or right click on any reachable spot on the map. The smiley 
;  will use AStar to find the best path to that spot and then
;  go there. You may continue to left or right click to find more
;  paths. Press enter again to deactivate the sprite and return		
;  to map drawing mode.
;- Press escape to exit the program.

;Please email comments to Patrick Lester at pwlester@policyalmanac.org

;===============================================================

;A* set up
Include "includes/aStarLibrary - Demo 3 (4 way).bb" ;contains A* code.

;Declare globals and types
Global cursor, grid, wallBlock ;graphics
Global gDrawing, gErasing, gGameStarted
Const userControlled = 1, chase = 2
Type unit
	Field ID, xLoc#, yLoc#, speed#, sprite
	Field pathAI, pathStatus, pathLength, pathLocation, pathBank, xPath, yPath
	Field targetX, targetY, target.unit
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
	Graphics 800,600
	SetBuffer BackBuffer() : HidePointer

	;Load Graphics
	cursor = LoadImage("../../graphics/red_pointer.bmp")
	MaskImage cursor,255,255,255 : MoveMouse 400,225 			
	grid = LoadImage("../../graphics/grid.bmp")		
	MaskImage grid,255,255,255
	dottedPath = LoadImage("../../graphics/path.bmp")		
	MaskImage dottedPath,255,255,255	
	redBlock = LoadImage("../../graphics/end.bmp") ;target location
	wallBlock = LoadImage("../../graphics/wall.bmp") ;wall	
	smiley = LoadImage("../../graphics/smiley.bmp")
	MaskImage smiley,0,0,0 : MidHandle smiley	
	chaser = LoadImage("../../graphics/ghost.bmp")
	MaskImage chaser,0,0,0
	MidHandle chaser
	
	;Create and initialize 2 units
	For x = 1 To 2
		unit.unit = New unit
		unit\ID = x
		unit\pathBank = CreateBank(1) ;data bank that unit's path data is stored	in	
		If x = 1 
			unit\xLoc = 125 : unit\yLoc = 325 : unit\speed = 5.3 ;smiley
			unit\pathAI = userControlled : unit\sprite = smiley
		Else If x = 2 
			unit\xLoc = 725 : unit\yLoc = 325 : unit\speed = 3 ;chaser
			unit\target = First unit
			unit\pathAI = chase : unit\sprite = chaser
		End If	
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

End Function

;This function draws stuff on the screen.
Function RenderScreen()
	Cls		
	For x = 0 To 15 ;Draw the walls and the grid 
	For y = 0 To 11
		If walkability(x,y) = unwalkable Then DrawBlock wallBlock,x*tilesize,y*tilesize
		DrawImage grid,x*tileSize,y*tileSize
	Next
	Next
	
	For unit.unit = Each unit ;Draw units
		DrawImage unit\sprite,unit\xLoc,unit\yLoc
	Next
	
	DrawImage cursor,MouseX(),MouseY() ;Draw the mouse
	Flip
End Function


;This function performs pathfinding and moves the units.
Function MoveUnits()
	If gGameStarted = True 
		For unit.unit = Each unit
			UpdatePath(unit)
			If unit\pathStatus = found Then MoveUnit(unit)	
		Next
	End If		
End Function	

;This function checks for path updates and calls the
;FindPath() function when needed.
Function UpdatePath(unit.unit)
		
	;If the unit is the smiley, trigger new paths using the mouse
	If unit\pathAI = userControlled
		If MouseHit(1) Or MouseHit(2)
			unit\pathStatus = FindPath(unit.unit,MouseX(),MouseY())
		End If
	
	;If the unit is the chaser, update the unit's path every third
	;step until it reaches the smiley.
	Else If unit\pathAI = chase
	
		;If smiley and chaser are not at the same location on the 
		;screen and no path is currently active, find a new path.
		If Not (unit\xLoc = unit\target\xLoc And unit\yLoc = unit\target\yLoc) 

			;If no path has been generated, generate one. Update it when
			;the chaser reaches its third step on the current path.	
			If unit\pathStatus <> found Or unit\pathlocation = 3
				unit\pathStatus = FindPath(unit.unit,unit\target\xLoc,unit\target\yLoc)		
			End If
		End If	
	End If
	
End Function
	

;This function moves the sprites around on the screen.
Function MoveUnit(unit.unit)

	;Check for pathStep advances.
	CheckPathStepAdvance(unit.unit)

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