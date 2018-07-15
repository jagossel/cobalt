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
TYPE DrawingCharacter
  AS UBYTE CharacterCode
  AS UBYTE ForegroundColor
  AS UBYTE BackgroundColor
END TYPE

DIM SHARED AS DrawingCharacter DrawingCanvas(1 TO 80, 1 TO 25)

DIM AS ULONG CurrentFileNumber = FREEFILE
OPEN "TITLE.CPD" FOR BINARY AS #CurrentFileNumber
  GET #CurrentFileNumber, 1, DrawingCanvas()
CLOSE #CurrentFileNumber

CurrentFileNumber = FREEFILE

OPEN "TMP.TXT" FOR OUTPUT AS #CurrentFileNumber
  FOR WriteLine AS BYTE = 1 TO 25
    DIM AS STRING CurrentLine

    CurrentLine = "DATA "
    FOR WriteData AS BYTE = 1 TO 80
      CurrentLine = CurrentLine + TRIM(STR(DrawingCanvas(WriteData, WriteLine).CharacterCode)) + ", "
      CurrentLine = CurrentLine + TRIM(STR(DrawingCanvas(WriteData, WriteLine).ForegroundColor)) + ", "
      CurrentLine = CurrentLine + TRIM(STR(DrawingCanvas(WriteData, WriteLine).BackgroundColor))
      IF WriteData < 80 THEN CurrentLine = CurrentLine + ", "
    NEXT WriteData

    PRINT #CurrentFileNumber, CurrentLine
  NEXT WriteLine
CLOSE #CurrentFileNumber

