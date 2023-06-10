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
TYPE DrawingCharacter
  AS UBYTE CharacterCode
  AS BYTE ForegroundColor
  AS BYTE BackgroundColor
END TYPE

CONST AS BYTE MinimumCursorX = 1
CONST AS BYTE MinimumCursorY = 1
CONST AS BYTE MinimumForegroundColor = 0
CONST AS BYTE MinimumBackgroundColor = 0

CONST AS BYTE MaximumCursorX = 80
CONST AS BYTE MaximumCursorY = 25
CONST AS BYTE MaximumForegroundColor = 15
CONST AS BYTE MaximumBackgroundColor = 7

DIM SHARED AS BOOLEAN StatusAtTop = FALSE
DIM SHARED AS BOOLEAN ExitApplication = FALSE
DIM SHARED AS BYTE PreviousCursorY = 0
DIM SHARED AS BYTE CurrentCursorX = 1
DIM SHARED AS BYTE CurrentCursorY = 1
DIM SHARED AS BYTE CurrentForegroundColor = 7
DIM SHARED AS BYTE CurrentBackgroundColor = 0
DIM SHARED AS BYTE CurrentCharacterSet = 1

DIM SHARED AS DrawingCharacter DrawingCanvas(MinimumCursorX TO MaximumCursorX, MinimumCursorY TO MaximumCursorY)

