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
#INCLUDE ONCE "CHARSETS.BI"
#INCLUDE ONCE "KEYDEF.BI"

DECLARE SUB RedrawRegion(StartX AS BYTE, StartY AS BYTE, EndX AS BYTE, EndY AS BYTE)
DECLARE SUB AskCharacterSet()
DECLARE SUB AskColors()
DECLARE SUB DrawColorSwatch(ForegroundColor AS BYTE, BackgroundColor AS BYTE, StartX AS BYTE, StartY AS BYTE, EndX AS BYTE, EndY AS BYTE)

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

SUB AskCharacterSet()
  DIM AS BOOLEAN ExitDialogBox = FALSE
  DIM AS BOOLEAN AcceptChanges = FALSE

  DIM AS BYTE CharacterSetCount = MaximumCharacterSetIndex - MinimumCharacterSetIndex + 1
  DIM AS BYTE StartX = 28
  DIM AS BYTE StartY = ((MaximumCursorY - CharacterSetCount) \ 2) - 1
  DIM AS BYTE EndX = 53
  DIM AS BYTE EndY = StartY + CharacterSetCount + 1
  DIM AS BYTE SelectedIndex = CurrentCharacterSet

  LOCATE StartY, StartX, 0: COLOR 3, 0: PRINT CHR(213); STRING(23, 205); CHR(184);
  FOR PrintCharacterSetLine AS BYTE = 1 TO MaximumCharacterSetIndex
    DIM AS BYTE ItemForegroundColor = 7
    DIM AS BYTE ItemBackgroundColor = 0
    IF PrintCharacterSetLine = SelectedIndex THEN
      ItemForegroundColor = 0
      ItemBackgroundColor = 7
    END IF

    LOCATE StartY + PrintCharacterSetLine, StartX: COLOR 3, 0: PRINT CHR(179);
    COLOR ItemForegroundColor, ItemBackgroundColor: PRINT PadLeft(PrintCharacterSetLine, 2, " "); ":";
    FOR PrintCharacter AS BYTE = 1 TO 10
      COLOR 7, 0: PRINT " ";
      COLOR CurrentForegroundColor, CurrentBackgroundColor: PRINT CharacterSets(PrintCharacterSetLine, PrintCharacter);
    NEXT PrintCharacter
    COLOR 3, 0: PRINT CHR(179);
  NEXT PrintCharacterSetLine
  LOCATE EndY, StartX: COLOR 3, 0: PRINT CHR(212); STRING(23, 205); CHR(190);

  DO
    DIM AS STRING KeyInput = INKEY()
    IF KeyInput = KeyUp THEN
      LOCATE StartY + SelectedIndex, StartX + 1: COLOR 7, 0: PRINT PadLeft(SelectedIndex, 2, " "); ":";
      SelectedIndex = SelectedIndex - 1
      IF SelectedIndex < MinimumCharacterSetIndex THEN SelectedIndex = MinimumCharacterSetIndex
      LOCATE StartY + SelectedIndex, StartX + 1: COLOR 0, 7: PRINT PadLeft(SelectedIndex, 2, " "); ":";
    ELSEIF KeyInput = KeyDown THEN
      LOCATE StartY + SelectedIndex, StartX + 1: COLOR 7, 0: PRINT PadLeft(SelectedIndex, 2, " "); ":";
      SelectedIndex = SelectedIndex + 1
      IF SelectedIndex > MaximumCharacterSetIndex THEN SelectedIndex = MaximumCharacterSetIndex
      LOCATE StartY + SelectedIndex, StartX + 1: COLOR 0, 7: PRINT PadLeft(SelectedIndex, 2, " "); ":";
    ELSEIF KeyInput = KeyEscape THEN
      ExitDialogBox = TRUE
    ELSEIF KeyInput = KeyEnter THEN
      ExitDialogBox = TRUE
      AcceptChanges = TRUE
    END IF
  LOOP UNTIL ExitDialogBox

  IF AcceptChanges THEN
    CurrentCharacterSet = SelectedIndex
  END IF

  RedrawRegion(StartX, StartY, EndX, EndY)
END SUB

SUB AskColors()
  DIM AS BOOLEAN ExitDialogBox = FALSE
  DIM AS BOOLEAN AcceptChanges = FALSE

  DIM AS BYTE StartX = 25
  DIM AS BYTE StartY = 3
  DIM AS BYTE EndX = 55
  DIM AS BYTE EndY = 21
  DIM AS BYTE CurrentForegroundIndex = CurrentForegroundColor
  DIM AS BYTE CurrentBackgroundIndex = CurrentBackgroundColor

  LOCATE StartY, StartX, 0: COLOR 3, 0: PRINT CHR(213); STRING(29, 205); CHR(184);
  FOR DrawDialog AS BYTE = 0 TO 15
    LOCATE DrawDialog + StartY + 1, StartX: COLOR 3, 0: PRINT CHR(179); " ";
    DIM AS BYTE ItemForegroundColor = 7
    DIM AS BYTE ItemBackgroundColor = 0
    IF DrawDialog = CurrentForegroundIndex THEN
      ItemForegroundColor = 0
      ItemBackgroundColor = 7
    END IF

    COLOR ItemForegroundColor, ItemBackgroundColor: PRINT PadLeft(DrawDialog, 2, " ");
    COLOR 3, 0: PRINT STRING(26, 32); CHR(179);
  NEXT DrawDialog

  LOCATE EndY - 1, StartX: COLOR 3, 0: PRINT CHR(179); "    ";
  FOR DrawBackgroundSelection AS BYTE = 0 TO 7
    DIM AS BYTE ItemForegroundColor = 7
    DIM AS BYTE ItemBackgroundcolor = 0

    IF DrawBackgroundSelection = CurrentBackgroundIndex THEN
      ItemForegroundColor = 0
      ItemBackgroundColor = 7
    END IF

    LOCATE EndY - 1, StartX + (DrawBackgroundSelection * 3) + 5
    COLOR ItemForegroundColor, ItemBackgroundColor: PRINT PadLeft(DrawBackgroundSelection, 2, " ");
  NEXT DrawBackgroundSelection
  LOCATE EndY - 1, EndX: COLOR 3, 0: PRINT CHR(179);

  LOCATE EndY, StartX: COLOR 3, 0: PRINT CHR(212); STRING(29, 205); CHR(190);

  DrawColorSwatch(CurrentForegroundIndex, CurrentBackgroundIndex, StartX + 5, StartY + 1, EndX - 1, EndY - 2)

  DO
    DIM AS STRING KeyInput = INKEY()

    IF KeyInput = KeyUp THEN
      LOCATE StartY + CurrentForegroundIndex + 1, StartX + 2: COLOR 7, 0: PRINT PadLeft(CurrentForegroundIndex, 2, " ");
      CurrentForegroundIndex = CurrentForegroundIndex - 1
      IF CurrentForegroundIndex < MinimumForegroundColor THEN CurrentForegroundIndex = MinimumForegroundColor
      LOCATE StartY + CurrentForegroundIndex + 1, StartX + 2: COLOR 0, 7: PRINT PadLeft(CurrentForegroundIndex, 2, " ");
      DrawColorSwatch(CurrentForegroundIndex, CurrentBackgroundIndex, StartX + 5, StartY + 1, EndX - 1, EndY - 2)
    ELSEIF KeyInput = KeyDown THEN
      LOCATE StartY + CurrentForegroundIndex + 1, StartX + 2: COLOR 7, 0: PRINT PadLeft(CurrentForegroundIndex, 2, " ");
      CurrentForegroundIndex = CurrentForegroundIndex + 1
      IF CurrentForegroundIndex > MaximumForegroundColor THEN CurrentForegroundIndex = MaximumForegroundColor
      LOCATE StartY + CurrentForegroundIndex + 1, StartX + 2: COLOR 0, 7: PRINT PadLeft(CurrentForegroundIndex, 2, " ");
      DrawColorSwatch(CurrentForegroundIndex, CurrentBackgroundIndex, StartX + 5, StartY + 1, EndX - 1, EndY - 2)
    ELSEIF KeyInput = KeyLeft THEN
      LOCATE EndY - 1, StartX + (CurrentBackgroundIndex * 3) + 5: COLOR 7, 0: PRINT PadLeft(CurrentBackgroundIndex, 2, " ");
      CurrentBackgroundIndex = CurrentBackgroundIndex - 1
      IF CurrentBackgroundIndex < MinimumBackgroundColor THEN CurrentBackgroundIndex = MinimumBackgroundColor
      LOCATE EndY - 1, StartX + (CurrentBackgroundIndex * 3) + 5: COLOR 0, 7: PRINT PadLeft(CurrentBackgroundIndex, 2, " ");
      DrawColorSwatch(CurrentForegroundIndex, CurrentBackgroundIndex, StartX + 5, StartY + 1, EndX - 1, EndY - 2)
    ELSEIF KeyInput = KeyRight THEN
      LOCATE EndY - 1, StartX + (CurrentBackgroundIndex * 3) + 5: COLOR 7, 0: PRINT PadLeft(CurrentBackgroundIndex, 2, " ");
      CurrentBackgroundIndex = CurrentBackgroundIndex + 1
      IF CurrentBackgroundIndex > MaximumBackgroundColor THEN CurrentBackgroundIndex = MaximumBackgroundColor
      LOCATE EndY - 1, StartX + (CurrentBackgroundIndex * 3) + 5: COLOR 0, 7: PRINT PadLeft(CurrentBackgroundIndex, 2, " ");
      DrawColorSwatch(CurrentForegroundIndex, CurrentBackgroundIndex, StartX + 5, StartY + 1, EndX - 1, EndY - 2)
    ELSEIF KeyInput = KeyEscape THEN
      ExitDialogBox = TRUE
    ELSEIF KeyInput = KeyEnter THEN
      ExitDialogBox = TRUE
      AcceptChanges = TRUE
    END IF
  LOOP UNTIL ExitDialogBox

  IF AcceptChanges THEN
    CurrentForegroundColor = CurrentForegroundIndex
    CurrentBackgroundColor = CurrentBackgroundIndex
  END IF

  RedrawRegion(StartX, StartY, EndX, EndY)
END SUB

SUB DrawColorSwatch(ForegroundColor AS BYTE, BackgroundColor AS BYTE, StartX AS BYTE, StartY AS BYTE, EndX AS BYTE, EndY AS BYTE)
  DIM LineCount AS BYTE = EndY - StartY
  DIM ColumnCount AS BYTE = EndX - StartX

  FOR DrawSwatch AS BYTE = StartY TO EndY
    LOCATE DrawSwatch, StartX: COLOR ForegroundColor, BackgroundColor: PRINT STRING(ColumnCount, 197);
  NEXT DrawSwatch
END SUB

