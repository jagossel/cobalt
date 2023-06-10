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
CLS

DIM AS STRING KeyInput = ""
DO
  KeyInput = INKEY()
LOOP WHILE KeyInput = ""

PRINT KeyInput
FOR WriteCode AS BYTE = 1 TO LEN(KeyInput)
  DIM AS STRING CurrentCharacter = MID(KeyInput, WriteCode, 1)
  DIM AS INTEGER CurrentCharacterCode = ASC(CurrentCharacter)

  PRINT TRIM(STR(CurrentCharacterCode))
NEXT WriteCode

