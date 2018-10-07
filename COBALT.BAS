' Cobal - ASCII Art
' Copyright (C) 2018  Jonathon Gosselin

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
#INCLUDE ONCE "GLOBAL.BI"
#INCLUDE ONCE "KEYDEF.BI"
#INCLUDE ONCE "CHARSETS.BI"
#INCLUDE ONCE "FUNCS.BI"
#INCLUDE ONCE "FILEIO.BI"
#INCLUDE ONCE "HELP.BI"
#INCLUDE ONCE "TITLE.BI"
#INCLUDE ONCE "UI.BI"

DECLARE SUB HandleKeyInput(KeyInput AS STRING)
DECLARE SUB UpdateStatus()
DECLARE SUB DrawCharacter(KeyInput AS STRING)
DECLARE SUB DrawTypedCharacter(KeyInput AS STRING)
DECLARE SUB ShowFullDrawing()
DECLARE SUB ResetCharacter()

CLS
ShowTitle()

InitializeNewDrawing()
RedrawRegion(MinimumCursorX, MinimumCursorY, MaximumCursorX, MaximumCursorY)
UpdateStatus()

DIM SHARED AS BOOLEAN ExitApplication = FALSE
DO
  HandleKeyInput(INKEY())
LOOP WHILE NOT ExitApplication

COLOR 7, 0
CLS
END

SUB HandleKeyInput(KeyInput AS STRING)
  IF KeyInput = "" THEN
    EXIT SUB
  ELSEIF KeyInput = KeyUp THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorY = CurrentCursorY - 1
    IF CurrentCursorY < MinimumCursorY THEN CurrentCursorY = MinimumCursorY
  ELSEIF KeyInput = KeyDown THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorY = CurrentCursorY + 1
    IF CurrentCursorY > MaximumCursorY THEN CurrentCursorY = MaximumCursorY
  ELSEIF KeyInput = KeyLeft THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorX = CurrentCursorX - 1
    IF CurrentCursorX < MinimumCursorX THEN CurrentCursorX = MinimumCursorX
  ELSEIF KeyInput = KeyRight THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorX = CurrentCursorX + 1
    IF CurrentCursorX > MaximumCursorX THEN CurrentCursorX = MaximumCursorX
  ELSEIF KeyInput = KeyHome THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorX = MinimumCursorX
  ELSEIF KeyInput = KeyEnd THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorX = MaximumCursorX
  ELSEIF KeyInput = KeyCtrlUp THEN
    CurrentForegroundColor = CurrentForegroundColor - 1
    IF CurrentForegroundColor < MinimumForegroundColor THEN CurrentForegroundColor = MaximumForegroundColor
  ELSEIF KeyInput = KeyCtrlDown THEN
    CurrentForegroundColor = CurrentForegroundColor + 1
    IF CurrentForegroundColor > MaximumForegroundColor THEN CurrentForegroundColor = MinimumForegroundColor
  ELSEIF KeyInput = KeyCtrlLeft THEN
    CurrentBackgroundColor = CurrentBackgroundColor - 1
    IF CurrentBackgroundColor < MinimumBackgroundColor THEN CurrentBackgroundColor = MaximumBackgroundColor
  ELSEIF KeyInput = KeyCtrlRight THEN
    CurrentBackgroundColor = CurrentBackgroundColor + 1
    IF CurrentBackgroundColor > MaximumBackgroundColor THEN CurrentBackgroundColor = MinimumBackgroundColor
  ELSEIF KeyInput = KeyAltUp THEN
    CurrentCharacterSet = CurrentCharacterSet - 1
    IF CurrentCharacterSet < MinimumCharacterSetIndex THEN CurrentCharacterSet = MaximumCharacterSetIndex
  ELSEIF KeyInput = KeyAltDown THEN
    CurrentCharacterSet = CurrentCharacterSet + 1
    IF CurrentCharacterSet > MaximumCharacterSetIndex THEN CurrentCharacterSet = MinimumCharacterSetIndex
  ELSEIF IsInKeyRange(KeyInput, 48, 57) THEN
    DrawCharacter(KeyInput)
  ELSEIF IsInKeyRange(KeyInput, 32, 47) OR IsInKeyRange(KeyInput, 58, 126) THEN
    DrawTypedCharacter(KeyInput)
  ELSEIF KeyInput = KeyBackspace THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorX = CurrentCursorX - 1
    IF CurrentCursorX < MinimumCursorX THEN CurrentCursorX = MinimumCursorX
    ResetCharacter()
  ELSEIF KeyInput = KeyEnter THEN
    PreviousCursorY = CurrentCursorY
    CurrentCursorY = CurrentCursorY + 1
    IF CurrentCursorY > MaximumCursorY THEN
      CurrentCursorY = MaximumCursorY
    ELSE
      CurrentCursorX = MinimumCursorX
    END IF
  ELSEIF KeyInput = KeyCtrlN THEN
    IF GetConfirmation("Start a new drawing?") THEN
      InitializeNewDrawing()
      RedrawRegion(MinimumCursorX, MinimumCursorY, MaximumCursorX, MaximumCursorY)
    END IF
  ELSEIF KeyInput = KeyCtrlO THEN
    IF GetConfirmation("Load a drawing?") THEN
      LoadDrawing()
      RedrawRegion(MinimumCursorX, MinimumCursorY, MaximumCursorX, MaximumCursorY)
    END IF
  ELSEIF KeyInput = KeyCtrlS THEN
    SaveDrawing()
    IF StatusAtTop THEN
      RedrawRegion(1, 1, 80, 1)
    ELSE
      RedrawRegion(1, 25, 80, 25)
    END IF
  ELSEIF KeyInput = KeyCtrlF THEN
    ShowFullDrawing()
  ELSEIF KeyInput = KeyCtrlE THEN
    ExportDrawing()
    IF StatusAtTop THEN
      RedrawRegion(1, 1, 80, 1)
    ELSE
      RedrawRegion(1, 25, 80, 25)
    END IF
  ELSEIF KeyInput = KeyCtrlI THEN
    IF GetConfirmation("Import a drawing?") THEN
      ImportDrawing()
      RedrawRegion(MinimumCursorX, MinimumCursorY, MaximumCursorX, MaximumCursorY)
    END IF
  ELSEIF KeyInput = KeyF1 THEN
    ShowHelp()
    RedrawRegion(MinimumCursorX, MinimumCursorY, MaximumCursorX, MaximumCursorY)
  ELSEIF KeyInput = KeyF2 THEN
    AskCharacterSet()
  ELSEIF KeyInput = KeyF3 THEN
    AskColors()
  ELSEIF KeyInput = KeyDelete THEN
    ResetCharacter()
  ELSEIF KeyInput = KeyEscape THEN
    ExitApplication = GetConfirmation("Exit Cobalt?")
  END IF

  UpdateStatus()
END SUB

SUB UpdateStatus()
  DIM AS BOOLEAN MoveStatus = FALSE
  IF PreviousCursorY = 12 AND CurrentCursorY = 13 THEN
    StatusAtTop = TRUE
    MoveStatus = TRUE
  ELSEIF PreviousCursorY = 13 AND CurrentCursorY = 12 THEN
    StatusAtTop = FALSE
    MoveStatus = TRUE
  END IF

  DIM AS BYTE StatusY = 25
  IF StatusAtTop THEN StatusY = 1

  IF MoveStatus THEN
    IF StatusAtTop THEN
      RedrawRegion(1, 25, 80, 25)
    ELSE
      RedrawRegion(1, 1, 80, 1)
    END IF
  END IF

  LOCATE StatusY, 1, 0
  COLOR 7, 0
  PRINT "F1 = HELP ";
  PRINT CHR(179);
  PRINT " ";
  PRINT PadLeft(CurrentCursorX, 2, "0");
  PRINT ",";
  PRINT PadLeft(CurrentCursorY, 2, "0");
  PRINT " ";
  PRINT CHR(179);
  PRINT " ";
  PRINT PadLeft(CurrentForegroundColor, 2, "0");
  PRINT ",";
  PRINT PadLeft(CurrentBackgroundColor, 2, "0");
  PRINT " ";
  PRINT CHR(179);
  PRINT " ";

  FOR DisplayCurrentCharacterSet AS BYTE = 1 TO 10
    COLOR 7, 0
    PRINT PadLeft(DisplayCurrentCharacterSet, 1, " ");
    PRINT "=";

    COLOR CurrentForegroundColor, CurrentBackgroundColor
    PRINT CharacterSets(CurrentCharacterSet, DisplayCurrentCharacterSet);

    COLOR 7, 0
    PRINT " ";
  NEXT DisplayCurrentCharacterSet

  LOCATE CurrentCursorY, CurrentCursorX, 1: COLOR CurrentForegroundColor, CurrentBackgroundColor
END SUB

SUB DrawCharacter(KeyInput AS STRING)
  DIM AS BYTE CharacterIndex = VAL(KeyInput)
  IF CharacterIndex = 0 THEN CharacterIndex = 10

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

