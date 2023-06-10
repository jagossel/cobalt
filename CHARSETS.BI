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
DATA 176, 177, 178, 219, 220, 221, 222, 223, 254, 32
DATA 218, 191, 196, 194, 193, 195, 180, 179, 192, 217
DATA 201, 187, 205, 203, 202, 204, 185, 186, 200, 188
DATA 214, 183, 196, 210, 208, 199, 182, 186, 211, 189
DATA 213, 184, 205, 209, 207, 198, 181, 179, 212, 190
DATA 197, 206, 215, 216, 240, 244, 245, 174, 175, 254
DATA 46, 249, 250, 248, 42, 15, 240, 241, 242, 243
DATA 244, 245, 246, 247, 248, 249, 250, 251, 252, 253
DATA 49, 50, 51, 52, 53, 54, 55, 56, 57, 48
DATA 30, 31, 16, 17, 18, 29, 24, 25, 26, 27
DATA 1, 2, 3, 4, 5, 6, 11, 12, 15, 32

CONST AS BYTE MinimumCharacterSetIndex = 1
CONST AS BYTE MaximumCharacterSetIndex = 11

DIM SHARED AS STRING CharacterSets(MinimumCharacterSetIndex TO MaximumCharacterSetIndex, 1 TO 10)

FOR ReadCharacterSetY AS BYTE = MinimumCharacterSetIndex TO MaximumCharacterSetIndex
  FOR ReadCharacterSetX AS BYTE = 1 TO 10
    DIM AS UBYTE CharacterCode
    READ CharacterCode

    CharacterSets(ReadCharacterSetY, ReadCharacterSetX) = CHR(CharacterCode)
  NEXT ReadCharacterSetX
NEXT ReadCharacterSetY

