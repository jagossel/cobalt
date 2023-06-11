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
#INCLUDE ONCE "FILE.BI"

#INCLUDE ONCE "GLOBAL.BI"
#INCLUDE ONCE "FUNCS.BI"
#INCLUDE ONCE "KEYDEF.BI"

DECLARE SUB InitializeNewDrawing()
DECLARE SUB SaveDrawing()
DECLARE SUB LoadDrawing()
DECLARE SUB ExportDrawing()
DECLARE SUB ExportDrawingToAnsiText(FileName AS STRING)
DECLARE SUB ExportDrawingToText(FileName AS STRING)
DECLARE SUB ImportDrawing()
DECLARE SUB ImportDrawingAsAnsiText(FileName AS STRING)
DECLARE SUB ImportDrawingAsText(FileName AS STRING)
DECLARE SUB DisplayFileError(FileName AS STRING, ErrorCode AS UBYTE)
DECLARE FUNCTION GetFileName(FileOperation AS STRING) AS STRING
DECLARE FUNCTION GetExportedFileName(FileOperation AS STRING) AS STRING
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
  StatusAtTop = FALSE

  FOR InitializeY AS BYTE = MinimumCursorY TO MaximumCursorY
    FOR InitializeX AS BYTE = MinimumCursorX TO MaximumCursorX
      DrawingCanvas(InitializeX, InitializeY).CharacterCode = 32
      DrawingCanvas(InitializeX, InitializeY).ForegroundColor = 7
      DrawingCanvas(InitializeX, InitializeY).BackgroundColor = 0
    NEXT InitializeX
  NEXT InitializeY
END SUB

SUB SaveDrawing()
  DIM AS STRING FileName = GetFileName("Save")
  IF LEN(FileName) <= 0 THEN EXIT SUB

  FileName = FileName + ".CPD"

  DIM AS BOOLEAN ContinueSave = FALSE
  IF NOT FILEEXISTS(FileName) THEN
    ContinueSave = TRUE
  ELSE
    DIM AS STRING SaveDialogText = FileName + " already exists.  Overwrite?"
    ContinueSave = GetConfirmation(SaveDialogText)
  END IF

  IF ContinueSave THEN
    DIM AS ULONG CurrentFileNumber = FREEFILE
    IF OPEN(FileName FOR BINARY ACCESS WRITE AS #CurrentFileNumber) = 0 THEN
      PUT #CurrentFileNumber, , DrawingCanvas()
      CLOSE #CurrentFileNumber
    ELSE
      DisplayFileError(FileName, ERR())
    END IF
  END IF
END SUB

SUB LoadDrawing()
  DIM AS STRING FileName = GetFileName("Load")
  IF LEN(FileName) <= 0 THEN EXIT SUB

  FileName = FileName + ".CPD"
  DIM AS ULONG CurrentFileNumber = FREEFILE
  IF OPEN(FileName FOR BINARY ACCESS READ AS #CurrentFileNumber) = 0 THEN
    GET #CurrentFileNumber, , DrawingCanvas()
    CLOSE #CurrentFileNumber
  ELSE
    DisplayFileError(FileName, ERR())
  END IF
END SUB

SUB ExportDrawing()
  DIM AS STRING FileName: FileName = GetExportedFileName("Export")
  DIM AS STRING FileExtension: FileExtension = UCASE(RIGHT(FileName, 3))

  IF FileExtension = "TXT" THEN
    ExportDrawingToText(FileName)
  ELSEIF FileExtension = "ANS" THEN
    ExportDrawingToAnsiText(FileName)
  END IF
END SUB

SUB ExportDrawingToAnsiText(FileName AS STRING)
  DIM AS BOOLEAN ContinueSave = FALSE
  IF NOT FILEEXISTS(FileName) THEN
    ContinueSave = TRUE
  ELSE
    DIM AS STRING SaveDialogText = FileName + " already exists.  Overwrite?"
    ContinueSave = GetConfirmation(SaveDialogText)
  END IF

  IF ContinueSave THEN
    DIM AS ULONG CurrentFileNumber = FREEFILE
    DIM AS BYTE CurrentExportForegroundColor = MaximumForegroundColor + 1
    DIM AS BYTE CurrentExportBackgroundColor = MaximumBackgroundColor + 1

    IF OPEN(FileName FOR OUTPUT AS #CurrentFileNumber) = 0 THEN
      FOR WriteCursorY AS BYTE = 1 TO 25
        FOR WriteCursorX AS BYTE = 1 TO 80
          IF DrawingCanvas(WriteCursorX, WriteCursorY).ForegroundColor <> CurrentExportForegroundColor ORELSE DrawingCanvas(WriteCursorX, WriteCursorY).BackgroundColor <> CurrentExportBackgroundColor THEN
            PRINT #CurrentFileNumber, KeyEscape; CHR(91);

            IF DrawingCanvas(WriteCursorX, WriteCursorY).ForegroundColor >= 8 ANDALSO (CurrentExportForegroundColor <= 7 ORELSE CurrentExportForegroundColor > MaximumForegroundColor) THEN
              PRINT #CurrentFileNumber, CHR(49); CHR(59);
            ELSEIF DrawingCanvas(WriteCursorX, WriteCursorY).ForegroundColor <= 7 ANDALSO CurrentExportForegroundColor >= 8 THEN
              PRINT #CurrentFileNumber, CHR(48); CHR(59);
            END IF

            IF DrawingCanvas(WriteCursorX, WriteCursorY).ForegroundColor <> CurrentExportForegroundColor THEN
              PRINT #CurrentFileNumber, CHR(51); TRIM(STR(ConvertColorCodeToAnsiColorCode(DrawingCanvas(WriteCursorX, WriteCursorY).ForegroundColor MOD 8)));
              IF DrawingCanvas(WriteCursorX, WriteCursorY).BackgroundColor <> CurrentExportBackgroundColor THEN
                PRINT #CurrentFileNumber, CHR(59);
              END IF
            END IF

            IF DrawingCanvas(WriteCursorX, WriteCursorY).BackgroundColor <> CurrentExportBackgroundColor THEN
              PRINT #CurrentFileNumber, CHR(52); TRIM(STR(ConvertColorCodeToAnsiColorCode(DrawingCanvas(WriteCursorX, WriteCursorY).BackgroundColor)));
            END IF

            PRINT #CurrentFileNumber, CHR(109);
          END IF

          CurrentExportForegroundColor = DrawingCanvas(WriteCursorX, WriteCursorY).ForegroundColor
          CurrentExportBackgroundColor = DrawingCanvas(WriteCursorX, WriteCursorY).BackgroundColor
          PRINT #CurrentFileNumber, CHR(DrawingCanvas(WriteCursorX, WriteCursorY).CharacterCode);
        NEXT WriteCursorX
      NEXT WriteCursorY

      PRINT #CurrentFileNumber, KeyEscape; CHR(91); CHR(48); CHR(109);

      CLOSE #CurrentFileNumber
    ELSE
      DisplayFileError(FileName, ERR())
    END IF
  END IF
END SUB

SUB ExportDrawingToText(FileName AS STRING)
  DIM AS BOOLEAN ContinueSave = FALSE
  IF NOT FILEEXISTS(FileName) THEN
    ContinueSave = TRUE
  ELSE
    DIM AS STRING SaveDialogText = FileName + " already exists.  Overwrite?"
    ContinueSave = GetConfirmation(SaveDialogText)
  END IF

  IF ContinueSave THEN
    DIM AS STRING CurrentWriteLine
    DIM AS ULONG CurrentFileNumber = FREEFILE

    IF OPEN(FileName FOR OUTPUT AS #CurrentFileNumber) = 0 THEN
      FOR WriteLine AS BYTE = MinimumCursorY TO MaximumCursorY - 1
        FOR WriteCharacter AS BYTE = MinimumCursorX TO MaximumCursorX - 1
          CurrentWriteLine = CurrentWriteLine + CHR(DrawingCanvas(WriteCharacter, WriteLine).CharacterCode)
        NEXT WriteCharacter

        PRINT #CurrentFileNumber, CurrentWriteLine
        CurrentWriteLine = ""
      NEXT WriteLine
      CLOSE #CurrentFileNumber
    ELSE
      DisplayFileError(FileName, ERR())
    END IF
  END IF
END SUB

SUB ImportDrawing()
  DIM AS STRING FileName: FileName = GetExportedFileName("Import")
  DIM AS STRING FileExtension: FileExtension = UCASE(RIGHT(FileName, 3))

  IF FileExtension = "TXT" THEN
    ImportDrawingAsText(FileName)
  ELSEIF FileExtension = "ANS" THEN
    ImportDrawingAsAnsiText(FileName)
  END IF
END SUB

SUB ImportDrawingAsAnsiText(FileName AS STRING)
  DIM AS UBYTE ReadNumberCount = 1
  DIM AS STRING ReadNumbers(ReadNumberCount)
  DIM AS ULONG CurrentFileNumber = FREEFILE
  DIM AS BYTE CurrentReadForegroundColor = 7
  DIM AS BYTE CurrentReadBackgroundColor = 0
  DIM AS BYTE CurrentReadCursorX = MinimumCursorX
  DIM AS BYTE CurrentReadCursorY = MinimumCursorY
  DIM AS BOOLEAN IsEscapeCharacter = FALSE
  DIM AS BOOLEAN IsActionCode = FALSE

  IF OPEN(FileName FOR BINARY ACCESS READ AS #CurrentFileNumber) = 0 THEN
    InitializeNewDrawing()
    DO UNTIL EOF(CurrentFileNumber)
      DIM AS UBYTE ReadCharacter
      GET #CurrentFileNumber, , ReadCharacter

      SELECT CASE AS CONST ReadCharacter
        CASE 10
          CurrentReadCursorX = MinimumCursorX
        CASE 13
          CurrentReadCursorY = CurrentReadCursorY + 1
          IF CurrentReadCursorY > MaximumCursorY THEN EXIT DO
        CASE 26
          EXIT DO
        CASE 27
          IsEscapeCharacter = TRUE
        CASE 48 TO 57
          IF IsActionCode THEN
            ReadNumbers(ReadNumberCount) = ReadNumbers(ReadNumberCount) + CHR(ReadCharacter)
          ELSE
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = ReadCharacter
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
            CurrentReadCursorX = CurrentReadCursorX + 1
            IF CurrentReadCursorX > MaximumCursorX THEN
              CurrentReadCursorY = CurrentReadCursorY + 1
              IF CurrentReadCursorY > MaximumCursorY THEN
                EXIT DO
              ELSE
                CurrentReadCursorX = MinimumCursorX
              END IF
            END IF
          END IF
        CASE 59
          IF IsActionCode THEN
            ReadNumberCount = ReadNumberCount + 1
            REDIM PRESERVE ReadNumbers(ReadNumberCount)
          ELSE
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = 59
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
            CurrentReadCursorX = CurrentReadCursorX + 1
            IF CurrentReadCursorX > MaximumCursorX THEN
              CurrentReadCursorY = CurrentReadCursorY + 1
              IF CurrentReadCursorY > MaximumCursorY THEN
                EXIT DO
              ELSE
                CurrentReadCursorX = MinimumCursorX
              END IF
            END IF
          END IF
        CASE 67
          IF IsActionCode THEN
            IsActionCode = FALSE
            DIM AS INTEGER CursorMovementCount = CINT(ReadNumbers(1))
            FOR MoveCursorIndex AS INTEGER = 1 TO CursorMovementCount
              DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = 32
              DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
              DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
              CurrentReadCursorX = CurrentReadCursorX + 1
              IF CurrentReadCursorX > MaximumCursorX THEN
                CurrentReadCursorY = CurrentReadCursorY + 1
                IF CurrentReadCursorY > MaximumCursorY THEN
                  EXIT DO
                ELSE
                  CurrentReadCursorX = MinimumCursorX
                END IF
              END IF
            NEXT MoveCursorIndex
            ERASE ReadNumbers
            ReadNumberCount = 1
            REDIM ReadNumbers(ReadNumberCount)
          ELSE
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = 67
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
            CurrentReadCursorX = CurrentReadCursorX + 1
            IF CurrentReadCursorX > MaximumCursorX THEN
              CurrentReadCursorY = CurrentReadCursorY + 1
              IF CurrentReadCursorY > MaximumCursorY THEN
                EXIT DO
              ELSE
                CurrentReadCursorX = MinimumCursorX
              END IF
            END IF
          END IF
        CASE 91
          IF IsEscapeCharacter THEN
            IsActionCode = TRUE
            IsEscapeCharacter = FALSE
          ELSE
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = 91
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
            CurrentReadCursorX = CurrentReadCursorX + 1
            IF CurrentReadCursorX > MaximumCursorX THEN
              CurrentReadCursorY = CurrentReadCursorY + 1
              IF CurrentReadCursorY > MaximumCursorY THEN
                EXIT DO
              ELSE
                CurrentReadCursorX = MinimumCursorX
              END IF
            END IF
          END IF
        CASE 109
          IF IsActionCode THEN
            IsActionCode = FALSE
            DIM AS BOOLEAN AddIntensisty = CurrentReadForegroundColor > 7
            FOR ParseCodes AS UBYTE = 1 TO ReadNumberCount
              SELECT CASE ReadNumbers(ParseCodes)
                CASE "0"
                  AddIntensisty = FALSE
                  CurrentReadForegroundColor = 7
                  CurrentReadBackgroundColor = 0
                CASE "1"
                  AddIntensisty = TRUE
                CASE "30"
                  CurrentReadForegroundColor = 0
                CASE "31"
                  CurrentReadForegroundColor = 4
                CASE "32"
                  CurrentReadForegroundColor = 2
                CASE "33"
                  CurrentReadForegroundColor = 6
                CASE "34"
                  CurrentReadForegroundColor = 1
                CASE "35"
                  CurrentReadForegroundColor = 5
                CASE "36"
                  CurrentReadForegroundColor = 3
                CASE "37"
                  CurrentReadForegroundColor = 7
                CASE "40"
                  CurrentReadBackgroundColor = 0
                CASE "41"
                  CurrentReadBackgroundColor = 4
                CASE "42"
                  CurrentReadBackgroundColor = 2
                CASE "43"
                  CurrentReadBackgroundColor = 6
                CASE "44"
                  CurrentReadBackgroundColor = 1
                CASE "45"
                  CurrentReadBackgroundColor = 5
                CASE "46"
                  CurrentReadBackgroundColor = 3
                CASE "47"
                  CurrentReadBackgroundColor = 7
              END SELECT
            NEXT ParseCodes
            IF AddIntensisty THEN
              IF CurrentReadForegroundColor <= 7 THEN CurrentReadForegroundColor = CurrentReadForegroundColor + 8
            ELSEIF CurrentReadForegroundColor > 7 THEN
              CurrentReadForegroundColor = CurrentReadForegroundColor - 8
            END IF
            ERASE ReadNumbers
            ReadNumberCount = 1
            REDIM ReadNumbers(ReadNumberCount)
          ELSE
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = 109
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
            CurrentReadCursorX = CurrentReadCursorX + 1
            IF CurrentReadCursorX > MaximumCursorX THEN
              CurrentReadCursorY = CurrentReadCursorY + 1
              IF CurrentReadCursorY > MaximumCursorY THEN
                EXIT DO
              ELSE
                CurrentReadCursorX = MinimumCursorX
              END IF
            END IF
          END IF
        CASE ELSE
          IF NOT IsActionCode THEN
            IF IsEscapeCharacter THEN
              IsEscapeCharacter = FALSE
              DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = 27
              DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
              DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
              CurrentReadCursorX = CurrentReadCursorX + 1
              IF CurrentReadCursorX > MaximumCursorX THEN
                CurrentReadCursorY = CurrentReadCursorY + 1
                IF CurrentReadCursorY > MaximumCursorY THEN
                  EXIT DO
                ELSE
                  CurrentReadCursorX = MinimumCursorX
                END IF
              END IF
            END IF

            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).CharacterCode = ReadCharacter
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).ForegroundColor = CurrentReadForegroundColor
            DrawingCanvas(CurrentReadCursorX, CurrentReadCursorY).BackgroundColor = CurrentReadBackgroundColor
            CurrentReadCursorX = CurrentReadCursorX + 1
            IF CurrentReadCursorX > MaximumCursorX THEN
              CurrentReadCursorY = CurrentReadCursorY + 1
              IF CurrentReadCursorY > MaximumCursorY THEN
                EXIT DO
              ELSE
                CurrentReadCursorX = MinimumCursorX
              END IF
            END IF
          END IF
      END SELECT
    LOOP
    CLOSE #CurrentFileNumber
  ELSE
    DisplayFileError(FileName, ERR())
  END IF
END SUB

SUB ImportDrawingAsText(FileName AS STRING)
  DIM AS ULONG CurrentFileNumber = FREEFILE
  DIM AS BYTE CurrentReadX = MinimumCursorX
  DIM AS BYTE CurrentReadY = MinimumCursorY
  DIM AS INTEGER CurrentFilePosition = 0

  IF OPEN(FileName FOR BINARY ACCESS READ AS #CurrentFileNumber) = 0 THEN
    InitializeNewDrawing()
    DO UNTIL EOF(CurrentFileNumber)
      DIM AS UBYTE ReadCharacter

      CurrentFilePosition = CurrentFilePosition + 1
      GET #CurrentFileNumber, CurrentFilePosition, ReadCharacter, 1
      IF ReadCharacter = 13 ORELSE ReadCharacter = 10 THEN
        CurrentReadY = CurrentReadY + 1
        IF CurrentReadY > MaximumCursorY THEN
          EXIT DO
        ELSE
          CurrentReadX = MinimumCursorX
        END IF

        CurrentFilePosition = CurrentFilePosition + 1
        GET #CurrentFileNumber, CurrentFilePosition, ReadCharacter, 1
        IF ReadCharacter <> 13 ANDALSO ReadCharacter <> 10 THEN
          DrawingCanvas(CurrentReadX, CurrentReadY).CharacterCode = ReadCharacter

          CurrentReadX = CurrentReadX + 1
          IF CurrentReadX > MaximumCursorX THEN
            CurrentReadY = CurrentReadY + 1
            IF CurrentReadY > MaximumCursorY THEN
              EXIT DO
            ELSE
              CurrentReadX = MinimumCursorX
            END IF
          END IF
        END IF
      ELSE
        DrawingCanvas(CurrentReadX, CurrentReadY).CharacterCode = ReadCharacter

        CurrentReadX = CurrentReadX + 1
        IF CurrentReadX > MaximumCursorX THEN
          CurrentReadY = CurrentReadY + 1
          IF CurrentReadY > MaximumCursorY THEN
            EXIT DO
          ELSE
            CurrentReadX = MinimumCursorX
          END IF
        END IF
      END IF
    LOOP
    CLOSE #1
  ELSE
    DisplayFileError(FileName, ERR())
  END IF
END SUB

SUB DisplayFileError(FileName AS STRING, ErrorCode AS UBYTE)
  DIM AS STRING DialogText
  SELECT CASE ErrorCode
    CASE 2
      DialogText = " :( Cannot find " + FileName
    CASE ELSE
      DialogText = " ?! Unknown error"
  END SELECT

  DIM AS BYTE DialogTextY = 25
  IF StatusAtTop THEN DialogTextY = 1

  LOCATE DialogTextY, 1, 0: COLOR 12, 0: PRINT SPACE(80);
  LOCATE DialogTextY, 1, 0: COLOR 12, 0: PRINT DialogText;
  SLEEP 3000

  COLOR 7, 0
END SUB

FUNCTION GetExportedFileName(FileOperation AS STRING) AS STRING
  DIM AS STRING DialogText: DialogText = "Format (A)NSI or (T)ext: "
  DIM AS BYTE DialogTextY = 25
  IF StatusAtTop THEN DialogTextY = 1

  LOCATE DialogTextY, 1, 0: COLOR 7, 0: PRINT SPACE(80);
  LOCATE DialogTextY, 1, 0: COLOR 7, 0: PRINT DialogText;

  DIM AS BOOLEAN ExitDialog = FALSE
  DIM AS STRING FileExtension = ""

  LOCATE DialogTextY, (LEN(DialogText) + 1), 1

  DO
    DIM AS STRING KeyInput = INKEY()
    IF UCASE(KeyInput) = "A" THEN
      FileExtension = "ANS"
      ExitDialog = TRUE
    ELSEIF UCASE(KeyInput) = "T" THEN
      FileExtension = "TXT"
      ExitDialog = TRUE
    ELSEIF KeyInput = KeyEscape THEN
      ExitDialog = TRUE
    END IF
  LOOP UNTIL ExitDialog

  IF LEN(FileExtension) <= 0 THEN RETURN ""

  DIM AS STRING FileName = GetFileName(FileOperation)
  IF LEN(FileName) <= 0 THEN RETURN ""

  RETURN FileName + "." + FileExtension
END FUNCTION

FUNCTION GetFileName(FileOperation AS STRING) AS STRING
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
    LOCATE DialogTextY, 1: PRINT SPACE(80);
    LOCATE DialogTextY, 1: PRINT FileOperation; "ing "; FileName; "...";
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
