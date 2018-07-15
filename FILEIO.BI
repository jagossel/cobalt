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
#INCLUDE ONCE "FUNCS.BI"
#INCLUDE ONCE "KEYDEF.BI"

DECLARE SUB InitializeNewDrawing()
DECLARE SUB SaveDrawing()
DECLARE SUB LoadDrawing()
DECLARE FUNCTION GetFileName() AS STRING
DECLARE FUNCTION IsFileNameCharacter(FileName AS STRING, KeyInput AS STRING) AS BOOLEAN

SUB InitializeNewDrawing()
  CurrentCursorX = MinimumCursorX
  CurrentCursorY = MinimumCursorY
  CurrentForegroundColor = 7
  CurrentBackgroundColor = 0
  CurrentCharacterSet = MinimumCharacterSetIndex

  FOR InitializeY AS BYTE = MinimumCursorY TO MaximumCursorY
    FOR InitializeX AS BYTE = MinimumCursorX TO MaximumCursorX
      DrawingCanvas(InitializeX, InitializeY).CharacterCode = 32
      DrawingCanvas(InitializeX, InitializeY).ForegroundColor = 7
      DrawingCanvas(InitializeX, InitializeY).BackgroundColor = 0
    NEXT InitializeX
  NEXT InitializeY
END SUB

SUB SaveDrawing()
  DIM AS STRING FileName = GetFileName()
  IF LEN(FileName) <= 0 THEN EXIT SUB

  DIM AS ULONG CurrentFileNumber = FREEFILE
  OPEN FileName FOR BINARY AS #CurrentFileNumber
    PUT #CurrentFileNumber, , DrawingCanvas()
  CLOSE #CurrentFileNumber
END SUB

SUB LoadDrawing()
  DIM AS STRING FileName = GetFileName()
  IF LEN(FileName) <= 0 THEN EXIT SUB

  DIM AS STRING ReadFileHeader

  DIM AS ULONG CurrentFileNumber = FREEFILE
  OPEN FileName FOR BINARY AS #CurrentFileNumber
    GET #CurrentFileNumber, , DrawingCanvas()
  CLOSE #CurrentFileNumber
END SUB

FUNCTION GetFileName() AS STRING
  DIM AS STRING DialogText: DialogText = "Name w/o Extension: "
  DIM AS BYTE DialogTextY = 25
  IF StatusAtTop THEN DialogTextY = 1

  LOCATE DialogTextY, 1, 0: COLOR 7, 0: PRINT SPACE(80);
  LOCATE DialogTextY, 1, 0: COLOR 7, 0: PRINT DialogText;

  DIM AS BOOLEAN ExitDialog = FALSE
  DIM AS BOOLEAN ContinueFileOperation = FALSE
  DIM AS STRING FileName

  LOCATE DialogTextY, (LEN(DialogText) + 1), 1

  DO
    DIM AS STRING KeyInput = INKEY()
    IF IsFileNameCharacter(FileName, KeyInput) THEN
      DIM AS STRING NewCharacter = UCASE(KeyInput)
      FileName = FileName + NewCharacter

      PRINT NewCharacter;
      LOCATE DialogTextY, (LEN(DialogText) + LEN(FileName) + 1), 1
    ELSEIF KeyInput = KeyBackspace THEN
      LOCATE DialogTextY, (LEN(DialogText) + LEN(FileName)), 1:COLOR 7, 0: PRINT " ";
      FileName = LEFT(FileName, LEN(FileName) - 1)
      LOCATE DialogTextY, (LEN(DialogText) + LEN(FileName) + 1), 1
    ELSEIF KeyInput = KeyEscape THEN
      ExitDialog = TRUE
      ContinueFileOperation = FALSE
    ELSEIF KeyInput = KeyEnter THEN
      ExitDialog = TRUE
      ContinueFileOperation = TRUE
    END IF
  LOOP UNTIL ExitDialog

  IF ContinueFileOperation THEN
    RETURN FileName + ".CPD"
  ELSE
    RETURN ""
  END IF
END FUNCTION

FUNCTION IsFileNameCharacter(FileName AS STRING, KeyInput AS STRING) AS BOOLEAN
  IF LEN(KeyInput) <> 1 THEN RETURN FALSE
  IF LEN(FileName) >= 8 THEN RETURN FALSE
  IF IsInKeyRange(KeyInput, 48, 57) THEN RETURN TRUE
  IF IsInKeyRange(KeyInput, 65, 90) THEN RETURN TRUE
  IF IsInKeyRange(KeyInput, 97, 122) THEN RETURN TRUE
  IF KeyInput = "_" THEN RETURN TRUE

  RETURN FALSE
END FUNCTION

