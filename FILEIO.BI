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
DECLARE SUB ExportDrawing()
DECLARE SUB ExportDrawingToAnsiText()
DECLARE SUB ExportDrawingToText()
DECLARE FUNCTION GetFileName() AS STRING
DECLARE FUNCTION IsFileNameCharacter(FileName AS STRING, KeyInput AS STRING) AS BOOLEAN
DECLARE FUNCTION ConvertColorCodeToAnsiColorCode(ColorCode AS BYTE) AS BYTE

CONST AS BYTE WriteNoCodes = 0
CONST AS BYTE WriteForegroundCode = 1
CONST AS BYTE WriteBackgroundCode = 2
CONST AS BYTE WriteBothCodes = 3

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

  FileName = FileName + ".CPD"
  DIM AS ULONG CurrentFileNumber = FREEFILE
  OPEN FileName FOR BINARY AS #CurrentFileNumber
    PUT #CurrentFileNumber, , DrawingCanvas()
  CLOSE #CurrentFileNumber
END SUB

SUB LoadDrawing()
  DIM AS STRING FileName = GetFileName()
  IF LEN(FileName) <= 0 THEN EXIT SUB

  FileName = FileName + ".CPD"
  DIM AS ULONG CurrentFileNumber = FREEFILE
  OPEN FileName FOR BINARY AS #CurrentFileNumber
    GET #CurrentFileNumber, , DrawingCanvas()
  CLOSE #CurrentFileNumber
END SUB

SUB ExportDrawing()
  DIM AS STRING DialogText: DialogText = "Format (A)NSI, (T)ext: "
  DIM AS BYTE DialogTextY = 25
  IF StatusAtTop THEN DialogTextY = 1

  LOCATE DialogTextY, 1, 0: COLOR 7, 0: PRINT SPACE(80);
  LOCATE DialogTextY, 1, 0: COLOR 7, 0: PRINT DialogText;

  DIM AS BOOLEAN ExitDialog = FALSE

  LOCATE DialogTextY, (LEN(DialogText) + 1), 1

  DO
    DIM AS STRING KeyInput = INKEY()
    IF UCASE(KeyInput) = "A" THEN
      ExportDrawingToAnsiText()
      ExitDialog = TRUE
    ELSEIF UCASE(KeyInput) = "T" THEN
      ExportDrawingToText()
      ExitDialog = TRUE
    ELSEIF KeyInput = KeyEscape THEN
      ExitDialog = TRUE
    END IF
  LOOP UNTIL ExitDialog
END SUB

SUB ExportDrawingToAnsiText()
  DIM AS STRING FileName = GetFileName()
  IF LEN(FileName) <= 0 THEN EXIT SUB

  DIM AS BYTE ExportCurrentForegroundColor = MaximumForegroundColor + 1
  DIM AS BYTE ExportCurrentBackgroundColor = MaximumBackgroundColor + 1
  DIM AS STRING CurrentWriteLine

  FileName = FileName + ".ANS"
  DIM AS ULONG CurrentFileNumber = FREEFILE
  OPEN FileName FOR OUTPUT AS #CurrentFileNumber
  FOR WriteLine AS BYTE = MinimumCursorY TO MaximumCursorY
    FOR WriteCharacter AS BYTE = MinimumCursorX TO MaximumCursorX
      DIM AS BYTE WriteCodeType = WriteNoCodes
      IF DrawingCanvas(WriteCharacter, WriteLine).ForegroundColor <> ExportCurrentForegroundColor THEN
        ExportCurrentForegroundColor = DrawingCanvas(WriteCharacter, WriteLine).ForegroundColor
        WriteCodeType = WriteCodeType + WriteForegroundCode
      END IF

      IF DrawingCanvas(WriteCharacter, WriteLine).BackgroundColor <> ExportCurrentBackgroundColor THEN
        ExportCurrentBackgroundColor = DrawingCanvas(WriteCharacter, WriteLine).BackgroundColor
        WriteCodeType = WriteCodeType + WriteBackgroundCode
      END IF

      IF WriteCodeType > WriteNoCodes THEN
        CurrentWriteLine = CurrentWriteLine + KeyEscape + "["
        IF WriteCodeType = WriteForegroundCode THEN
          IF ExportCurrentForegroundColor > 7 THEN
            CurrentWriteLine = CurrentWriteLine + "1;"
          ELSE
            CurrentWriteLine = CurrentWriteLine + "0;"
          END IF
          CurrentWriteLine = CurrentWriteLine + "3"
          CurrentWriteLine = CurrentWriteLine + TRIM(STR(ConvertColorCodeToAnsiColorCode(ExportCurrentForegroundColor MOD 8)))
        ELSEIF WriteCodeType = WriteBackgroundCode THEN
          CurrentWriteLine = CurrentWriteLine + "4"
          CurrentWriteLine = CurrentWriteLine + TRIM(STR(ConvertColorCodeToAnsiColorCode(ExportCurrentBackgroundColor)))
        ELSEIF WriteCodeType = WriteBothCodes THEN
          IF ExportCurrentForegroundColor > 7 THEN
            CurrentWriteLine = CurrentWriteLine + "1;"
          ELSE
            CurrentWriteLine = CurrentWriteLine + "0;"
          END IF
          CurrentWriteLine = CurrentWriteLine + "3"
          CurrentWriteLine = CurrentWriteLine + TRIM(STR(ConvertColorCodeToAnsiColorCode(ExportCurrentForegroundColor MOD 8))) + ";"
          CurrentWriteLine = CurrentWriteLine + "4"
          CurrentWriteLine = CurrentWriteLine + TRIM(STR(ConvertColorCodeToAnsiColorCode(ExportCurrentBackgroundColor)))
        END IF
        CurrentWriteLine = CurrentWriteLine + "m"
      END IF

      CurrentWriteLine = CurrentWriteLine + CHR(DrawingCanvas(WriteCharacter, WriteLine).CharacterCode)
    NEXT WriteCharacter

    PRINT #CurrentFileNumber, CurrentWriteLine;
    CurrentWriteLine = ""
  NEXT WriteLine

  PRINT #CurrentFileNumber, KeyEscape + "[0;37;40m"

  CLOSE #CurrentFileNumber
END SUB

SUB ExportDrawingToText()
  DIM AS STRING FileName = GetFileName()
  IF LEN(FileName) <= 0 THEN EXIT SUB

  DIM AS STRING CurrentWriteLine

  FileName = FileName + ".TXT"
  DIM AS ULONG CurrentFileNumber = FREEFILE
  OPEN FileName FOR OUTPUT AS #CurrentFileNumber
  FOR WriteLine AS BYTE = MinimumCursorY TO MaximumCursorY - 1
    FOR WriteCharacter AS BYTE = MinimumCursorX TO MaximumCursorX - 1
      CurrentWriteLine = CurrentWriteLine + CHR(DrawingCanvas(WriteCharacter, WriteLine).CharacterCode)
    NEXT WriteCharacter

    PRINT #CurrentFileNumber, CurrentWriteLine
    CurrentWriteLine = ""
  NEXT WriteLine
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
    RETURN FileName
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

FUNCTION ConvertColorCodeToAnsiColorCode(ColorCode AS BYTE) AS BYTE
  DIM AS BYTE AnsiColorCode = 0

  SELECT CASE ColorCode
    CASE 1
      AnsiColorCode = 4
    CASE 2
      AnsiColorCode = 2
    CASE 3
      AnsiColorCode = 6
    CASE 4
      AnsiColorCode = 1
    CASE 5
      AnsiColorCode = 5
    CASE 6
      AnsiColorCode = 3
    CASE 7
      AnsiColorCode = 7
  END SELECT

  RETURN AnsiColorCode
END FUNCTION

