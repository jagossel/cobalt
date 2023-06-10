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
#INCLUDE ONCE "GLOBAL.BI"
#INCLUDE ONCE "KEYDEF.BI"
#INCLUDE ONCE "CHARSETS.BI"
#INCLUDE ONCE "FUNCS.BI"
#INCLUDE ONCE "FILEIO.BI"
#INCLUDE ONCE "HELP.BI"
#INCLUDE ONCE "TITLE.BI"
#INCLUDE ONCE "UI.BI"
#INCLUDE ONCE "DRAWING.BI"

CLS
ShowTitle()

InitializeNewDrawing()
RedrawRegion(MinimumCursorX, MinimumCursorY, MaximumCursorX, MaximumCursorY)
UpdateStatus()

DO
  HandleKeyInput(INKEY())
LOOP WHILE NOT ExitApplication

COLOR 7, 0
CLS
END

