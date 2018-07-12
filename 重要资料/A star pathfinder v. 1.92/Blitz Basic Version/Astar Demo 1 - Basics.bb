;PATHFINDING DEMO #1: This demo uses the AStar (A*) algorithm  
;to find the shortest path between two points.
;===============================================================
;By Patrick Lester (Updated 3/5/04)

;A tutorial describing A* and this code in particular can be found at:
;http://www.policyalmanac.org/games/aStarTutorial.htm

;	Instructions
;	------------
;	Demo finds the shortest path between the green square and the 
;	red square.
;	- Draw blue walls by left clicking on the screen. 
;	- Erase blue walls by left clicking on them.
;	- Move the red target around by right clicking on the screen.
;	- Move the green start square around by holding the "g" button
;		(g for green) down while left clicking on the screen.
;	- Start the pathfinding search by pressing the enter button.
;	- After the path is found, reset by clicking the mouse. You can 
;		draw new walls and then press enter to search again.
;	- Press 1 to do a step-by-step search. Continue pressing 1 for each 
;		new step. Press enter to finish a step-by-step search all at once.


;	Results
;	-------
;	- Path is marked by red dots.
;	- Squares highlighted in green were added to the "open list" of
;		squares to be checked, but were never fully checked.
;	- Squares highlighted in blue were fully checked and added to the
;		"closed list".
;	- Grey pointers point to a square's parent.
;	- F, G, and H costs are printed in each cell. The F cost is
;		printed in the upper left corner, G in lower left corner, and
;		H in lower right corner.

;Please email comments to Patrick Lester at pwlester@policyalmanac.org

;===============================================================

;A* set up
Include "includes/aStarLibrary - Demo 1 (do not use).bb"	

;Declare variables
Dim parentArrow(9)
Global cursor, grid, greengrid, bluegrid, dottedPath ;graphics
Global greenBlock, redBlock, wallBlock, numbers ;graphics
Global gTileSize, gStartX, gStartY, gTargetX, gTargetY
Global gDrawing, gErasing, gPath	

;Full program sequence
LaunchProgram()
RunProgram()
End


;Main program loop	
Function RunProgram()
	While Not KeyHit(1) ;while escape key isn't pressed	
		UserInput() ;process mouse and keyboard input			
		Renderscreen(gStartX,gStartY,gTargetX,gTargetY) ;draw stuff on the screen		
	Wend
End Function


Function LaunchProgram()
	If Not GfxModeExists(800,600,16) RuntimeError "Sorry, this program won't work with your graphics card."	
	Graphics 800,600
	SetBuffer BackBuffer()
	HidePointer : MoveMouse 400,225

	;Initialize some global variables
	gTileSize = 50
	gStartX = 3 : gStartY = 6 ;initial seeker location
	gTargetX = 12 : gTargetY = 6  ;initial target location. This can be changed by right-clicking on the map.	

	;Load Graphics
	cursor = LoadImage("../graphics/red_pointer.bmp")
	MaskImage cursor,255,255,255  ;set image transparant color to white			
	grid = LoadImage("../graphics/grid.bmp")		
	MaskImage grid,255,255,255
	greengrid = LoadImage("../graphics/greengrid.bmp")		
	MaskImage greengrid,255,255,255			
	bluegrid = LoadImage("../graphics/bluegrid.bmp")		
	MaskImage bluegrid,255,255,255
	dottedPath = LoadImage("../graphics/path.bmp")		
	MaskImage dottedPath,255,255,255
	greenBlock = LoadImage("../graphics/start.bmp") ;start location
	redBlock = LoadImage("../graphics/end.bmp") ;target location
	wallBlock = LoadImage("../graphics/wall.bmp")  ;wall
	For z = 1 To 8
		parentArrow(z) = LoadImage("../graphics/arrow"+z+".bmp")	
		MaskImage parentArrow(z),255,255,255
	Next
	numbers = LoadAnimImage("../graphics/SmallNumberStrip.bmp",4,5,0,12)
End Function

;This function handles most user mouse and keyboard input
Function UserInput()

	;Edit map by drawing or erasing walls
	If (Not MouseDown(1)) Then gDrawing = False
	If (Not MouseDown(1)) Then gErasing = False	
	If MouseDown(1) And (Not KeyDown(34))
		
		;Draw walls
		If walkability(MouseX()/gTileSize,MouseY()/gTileSize) = walkable And gErasing = False Then
		 	walkability(MouseX()/gTileSize,MouseY()/gTileSize) = unwalkable
			gDrawing = True
		End If

		;Erase walls	
		If walkability(MouseX()/gTileSize,MouseY()/gTileSize) = unwalkable And gDrawing = False Then
		 	walkability(MouseX()/gTileSize,MouseY()/gTileSize) = walkable
			gErasing = True
		End If

	;Move green starting square around
	Else If MouseDown(1) And KeyDown(34) ; if g key is down while left clicking
		gStartX = MouseX()/gTileSize
		gStartY = MouseY()/gTileSize	
			
	;Move red target square around
	Else If MouseHit(2) 
		gTargetX = MouseX()/gTileSize
		gTargetY = MouseY()/gTileSize			
	End If
			
	;Start A* pathfinding search if return/enter key is hit
	If gPath = notfinished ;if path not searched
		If KeyHit(28) Then 
			gPath=FindPath(1,mapWidth,mapHeight,gStartX,gStartY,gTargetX,gTargetY)	
			If gPath = found Then ReadPath(1)	
			listArray(gStartX,gStartY) = 0 ;don't highlight the start square (aesthetics)	

		;If "1" key is hit, start A* pathfinding search step-by-step
		Else If KeyHit(2) Then 
			FlushKeys
			stepBystep = True
			gPath = FindPath(1,mapWidth,mapHeight,gStartX,gStartY,gTargetX,gTargetY,stepBystep)
			If gPath = found Then 
				ReadPath(1)
				listArray(gStartX,gStartY) = 0 ;don't highlight the start square (aesthetics)	
			End If
			;If escape key was hit before finishing path then reset	
			If gPath = notfinished Then Dim listArray(mapWidth+1,mapHeight+1)
			stepBystep = False	
		End If	
	
	;Reset after finishing A* search if pressing mouse button or "1" or enter key 
	Else ;if path is done
		If (MouseDown(1) Or MouseDown(2) Or KeyHit(2) Or KeyHit(28)) Then 
			Dim listArray(17,13) ;clear out list data, which clears screen.
			gPath = notfinished
		End If		
				
	End If
				
End Function	
	
;This function draws stuff on the screen.	
Function RenderScreen(gStartX,gStartY,gTargetX,gTargetY,stepBystep=False)	
	Cls
	
	;Draw green starting location	
	DrawBlock greenBlock,gStartX*gTileSize,gStartY*gTileSize		

	;Draw red target location		
	DrawBlock redBlock,gTargetX*gTileSize,gTargetY*gTileSize
	
	For x = 0 To 15 ;draw each of 16 tiles across the screen
	For y = 0 To 11 ;draw each of 12 tiles down the screen
	
		;Draw blue walls
		If walkability(x,y) = unwalkable Then DrawBlock wallBlock,x*gTileSize,y*gTileSize
				
		;Draw cells on open list (green), closed list (blue) and
		;part of the found path, if any (red dots).
		If listArray(x,y) = 1 Then DrawImage greengrid,x*gTileSize,y*gTileSize
		If listArray(x,y) = 2 Then DrawImage bluegrid,x*gTileSize,y*gTileSize
		If listArray(x,y) = 3 Then DrawImage dottedPath,x*gTileSize,y*gTileSize

		;Draw arrows that point to each square's parent.
		If listArray(x,y) = 1 Or listArray(x,y) = 2 
			If parentX(x,y) = x And parentY(x,y) = y - 1 Then 
				DrawImage parentArrow(1),x*gTileSize,y*gTileSize
			Else If parentX(x,y) = x+1 And parentY(x,y) = y - 1 Then 
				DrawImage parentArrow(2),x*gTileSize,y*gTileSize	
			Else If parentX(x,y) = x+1 And parentY(x,y) = y Then 
				DrawImage parentArrow(3),x*gTileSize,y*gTileSize
			Else If parentX(x,y) = x+1 And parentY(x,y) = y + 1 Then 
				DrawImage parentArrow(4),x*gTileSize,y*gTileSize				
			Else If parentX(x,y) = x And parentY(x,y) = y + 1 Then 
				DrawImage parentArrow(5),x*gTileSize,y*gTileSize
			Else If parentX(x,y) = x-1 And parentY(x,y) = y + 1 Then 
				DrawImage parentArrow(6),x*gTileSize,y*gTileSize
			Else If parentX(x,y) = x-1 And parentY(x,y) = y Then 
				DrawImage parentArrow(7),x*gTileSize,y*gTileSize
			Else If parentX(x,y) = x-1 And parentY(x,y) = y - 1 Then 
				DrawImage parentArrow(8),x*gTileSize,y*gTileSize
			End If	
		End If 

		;Print F, G and H costs in cells
		If listArray(x,y) > 0 ;if an open, closed, or path cell.
			G = Gcost(x,y) 			
			H = estimateHcost(x,y,gTargetX,gTargetY)		
			F = G + H
		
			;Print F in upper left corner of cell (F=G+H)
			If F >= 100 Then DrawImage numbers,x*gTileSize+2,y*gTileSize+4,F/100		
			DrawImage numbers,x*gTileSize+7,y*gTileSize+4,(F Mod 100)/10
			DrawImage numbers,x*gTileSize+12,y*gTileSize+4,F Mod 10				

			;Print G in bottom left corner of cell (G=distance from start)
			If G >= 100 Then DrawImage numbers,x*gTileSize+2,y*gTileSize+40,G/100		
			DrawImage numbers,x*gTileSize+7,y*gTileSize+40,(G Mod 100)/10
			DrawImage numbers,x*gTileSize+12,y*gTileSize+40,G Mod 10

			;Print H in bottom right corner of cell (H=distance to target)
			If H >= 100 Then DrawImage numbers,x*gTileSize+32,y*gTileSize+40,H/100		
			DrawImage numbers,x*gTileSize+37,y*gTileSize+40,(H Mod 100)/10
			DrawImage numbers,x*gTileSize+42,y*gTileSize+40,H Mod 10			
		End If ;If listArray(x,y) > 0	


		;Draw the grid overlay.
		DrawImage grid,x*gTileSize,y*gTileSize

	Next
	Next

	;Draw the mouse (unless in step-by-step mode)
	If stepBystep = False Then DrawImage cursor,MouseX(),MouseY()		
	Flip
End Function
	
	
	
	
		