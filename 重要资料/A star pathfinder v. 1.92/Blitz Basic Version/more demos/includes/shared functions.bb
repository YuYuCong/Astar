;Common Functions
;==================================================================
;This include file contains functions that are used by several demos. It does not contain
;any significant pathfinding-related code.

Global gInt1, gInt2
Const failed = -1, succeeded = 1
Global gScreenCaptureNumber, gLoops, gLoopTime#, gGameTime#

;Returns the lesser of two numbers.
Function Min#(a#,b#)
	If a < b Then Return a
	Return b
End Function

;Returns the greater of two numbers.
Function Max#(a#,b#)
	If a > b Then Return a
	Return b
End Function

;Returns the angle between the first point and the second point.
;Zero degrees is at the 3 o'clock position. Angles proceed in a
;counterclockwise direction. For example, 90 degrees is at
;12 o'clock. 180 degrees is at 9 o'clock, and 270 degrees
;is at 6 o'clock.
;	Also, please note that this function is using screen coordinates,
;where y increases in value as it goes down.
;	Note that the Blitz ATan2() function returns -180 to 180 with
;zero being the 12 o'clock position if y increases as you move up
;the screen, and 6'oclock if y increases as you move down the screen.
;This functions adjusts for that.
Function GetAngle(x1#,y1#,x2#,y2#)
	angle = ATan2(x2-x1,y2-y1)
	If angle >= 90 And angle <= 180
		Return angle-90
	Else
		Return angle+270			
	End If
End Function

;Note: Blitz calculates squares very slowly for some reason,
;so it is much faster to multiply the values rather than using
;the shorthand "^2".
Function GetDistance#(x1#,y1#,x2#,y2#)
	Return Sqr( (x1#-x2#)*(x1#-x2#) + (y1#-y2#)*(y1#-y2#) )	
End Function

;This subroutine copies the map image into one large image for
;faster rendering. This isn't really necessary, and it doesn't effect
;pathfinding at all. It just makes map drawing faster because drawing
;one big image is a lot faster than separately drawing each of the wall
;images and grids. 
Function CopyMapImage()
	FreeImage gMap
	gMap = CreateImage(800,600) ;create a new map image.
	SetBuffer ImageBuffer(gMap)
	For x = 0 To 800/tileSize-1
	For y = 0 To 600/tileSize-1
		If walkability(x,y) = unwalkable Then DrawBlock wallBlock,x*tileSize,y*tileSize
		DrawImage grid,x*tileSize,y*tileSize			
	Next
	Next		
	SetBuffer BackBuffer()
End Function

;This function draws the blue walls and grid.
Function DrawMapImage()
	If gGameStarted = False	
		Cls
		For x = 0 To 800/tileSize-1
		For y = 0 To 600/tileSize-1
			If walkability(x,y) = unwalkable Then DrawBlock wallBlock,x*tileSize,y*tileSize
			DrawImage grid,x*tileSize,y*tileSize			
		Next
		Next
	Else
		DrawBlock gMap,0,0
	End If
End Function	

;This function loads walkability data from a file.
Function LoadMapData(fileName$)
	filein = ReadFile(fileName$)
	If filein
		For x = 0 To 800/tileSize-1
		For y = 0 To 600/tileSize-1
			walkability(x,y) = ReadByte(filein)			
		Next
		Next	 
		CloseFile(filein)	
	End If
End Function	

;This function saves walkability data to a file.
Function SaveMapData(fileName$)
	fileout = WriteFile(fileName$)	
	For x = 0 To 800/tileSize-1
	For y = 0 To 600/tileSize-1
		If walkability(x,y) <> 1Then WriteByte(fileout,0)
		If walkability(x,y) = 1 Then WriteByte(fileout,walkability(x,y))					
	Next
	Next
	CloseFile(fileout)	
End Function

;ScreenCapture by pressing print screen or F12. Successive screen
;captures during the same program run will be saved separately.
Function ScreenCapture()
	If KeyHit(88) Or KeyHit(183)
		SaveBuffer(BackBuffer(),"screenshot"+gScreenCaptureNumber+".bmp")
		gScreenCaptureNumber = gScreenCaptureNumber+1 ;global enables multiple captures
	EndIf	
End Function

;This function draws unit claimed nodes. It is called by the 
;RenderScreen() function.
Function DrawClaimedNodes(drawSelectedOnly=False)
	If gDrawMore = True	
		For x = 0 To 800/tileSize-1
		For y = 0 To 600/tileSize-1
			If claimedNode(x,y) <> Null;claimed nodes
				unit.unit = claimedNode(x,y)
				If drawSelectedOnly=False Or unit\ID <= 3			
					If unit\pathStatus <> stopped
						Color unit\red,unit\green,unit\blue
						Rect x*tileSize+.4*tileSize,y*tileSize+.4*tileSize,.2*tileSize,.2*tileSize,1	
					End If
				End If	
			End If
		Next
		Next	
		For unit.unit = Each unit ;draw square when unit is tempStopped
			If drawSelectedOnly=False Or unit\ID <= 3			
			If unit\pathStatus = tempStopped
				Color unit\red,unit\green,unit\blue
				Rect ReadPathX(unit,unit\pathLocation)-.25*tileSize,ReadPathY(unit,unit\pathLocation)-.25*tileSize,.5*tileSize,.5*tileSize,0
			End If
			End If
		Next		
	End If
End Function


;This function calculates the average amount time that has passed
;per loop over the past 20 game loops. This rolling average
;is combined with speed information (expressed in pixels/second) to
;determine how far to move a unit in a given loop. 
;	We use this time-based approach to ensure consistent unit 
;movement speeds. If units instead moved a fixed distance every 
;loop, the movement speed would be inconsistent from one PC
;to the next because of different chip speeds and monitor refresh
;rates.
;	A rolling average is used because the Millisecs() function does
;not always return a reliably accurate time down to the millisecond. 
;Using an average over the past 20 game loops is more reliable.
Dim savedClockTime#(20)
Global savedClockCount
Function UpdateGameClock()
	savedClockCount = savedClockCount+1
	If savedClockCount = 20 Then savedClockCount = 0
	time# = MilliSecs()
	gLoopTime# = (time#-savedClockTime#(savedClockCount))/20000	
	savedClockTime#(savedClockCount) = time#
	If gLoopTime# > .1 Then gLoopTime# = .0167	
End Function

	