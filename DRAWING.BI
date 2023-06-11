' Cobal - ASCII Art
' Copyright (C) 2018-2023  Jonathon Gosselin

' This program is free software: you can redistribute it and/or modify
' it under the terms of the GNU General Public License as published by
' the Free Software Foundation, either version 3 of the License, or
' (at your option) any later version.

' This program is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
' GNU General Public License for more details.

' You should have received a copy of the GNU General Public License
' along with this program.  If not, see <https://www.gnu.org/licenses/>.
DECLARE SUB DrawCharacter(KeyInput AS STRING)
DECLARE SUB DrawTypedCharacter(KeyInput AS STRING)
DECLARE SUB ShowFullDrawing()
DECLARE SUB ResetCharacter()
DECLARE SUB RedrawRegion(StartX AS BYTE, StartY AS BYTE, EndX AS BYTE, EndY AS BYTE)

SUB DrawCharacter(KeyInput AS STRING)
  IF NOT IsInModifiedKeyRange(KeyInput, 59, 68) THEN EXIT SUB

  DIM AS UBYTE CharacterIndex = ASC(RIGHT(KeyInput, 1)) - 58
  DrawTypedCharacter(CharacterSets(CurrentCharacterSet, CharacterIndex))
END SUB

SUB DrawTypedCharacter(KeyInput AS STRING)
  DrawingCanvas(CurrentCursorX, CurrentCursorY).CharacterCode = ASC(KeyInput)
  DrawingCanvas(CurrentCursorX, CurrentCursorY).ForegroundColor = CurrentForegroundColor
  DrawingCanvas(CurrentCursorX, CurrentCursorY).BackgroundColor = CurrentBackgroundColor

  LOCATE CurrentCursorY, CurrentCursorX, 1
  COLOR CurrentForegroundColor, CurrentBackgroundColor
  PRINT KeyInput;

  PreviousCursorY = CurrentCursorY
  CurrentCursorX = CurrentCursorX + 1
  IF CurrentCursorX > MaximumCursorX THEN CurrentCursorX = MaximumCursorX
END SUB

SUB ShowFullDrawing()
  IF StatusAtTop THEN
    RedrawRegion(1, 1, 80, 1)
  ELSE
    RedrawRegion(1, 25, 80, 25)
  END IF

  LOCATE CurrentCursorY, CurrentCursorX, 0

  DO:LOOP WHILE INKEY() = ""
END SUB

SUB ResetCharacter()
  DrawingCanvas(CurrentCursorX, CurrentCursorY).CharacterCode = 32
  DrawingCanvas(CurrentCursorX, CurrentCursorY).ForegroundColor = 7
  DrawingCanvas(CurrentCursorX, CurrentCursorY).BackgroundColor = 0
  RedrawRegion(CurrentCursorX, CurrentCursorY, CurrentCursorX, CurrentCursorY)
END SUB

SUB RedrawRegion(StartX AS BYTE, StartY AS BYTE, EndX AS BYTE, EndY AS BYTE)
  FOR RedrawY AS BYTE = StartY TO EndY
    FOR RedrawX AS BYTE = StartX TO EndX
      LOCATE RedrawY, RedrawX, 0

      DIM AS DrawingCharacter CurrentDrawingCharacter = DrawingCanvas(RedrawX, RedrawY)
      COLOR CurrentDrawingCharacter.ForegroundColor, CurrentDrawingCharacter.BackgroundColor
      PRINT CHR(CurrentDrawingCharacter.CharacterCode);
    NEXT RedrawX
  NEXT RedrawY
END SUB
