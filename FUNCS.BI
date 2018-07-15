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
DECLARE FUNCTION PadLeft(InputValue AS BYTE, TargetLength AS INTEGER, PadCharacter AS STRING) AS STRING
DECLARE FUNCTION IsInKeyRange(KeyInput AS STRING, StartRange AS BYTE, EndRange AS BYTE) AS BOOLEAN

FUNCTION PadLeft(InputValue AS BYTE, TargetLength AS INTEGER, PadCharacter AS STRING) AS STRING
  DIM AS STRING InputValueString = STR(InputValue)
  DIM AS INTEGER InputValueStringLength = LEN(InputValueString)
  DIM AS INTEGER PadLength = TargetLength - InputValuestringLength
  PadCharacter = LEFT(PadCharacter, 1)

  DIM AS STRING OutputValue
  IF PadLength <= 0 THEN
    OutputValue = RIGHT(InputValueString, TargetLength)
  ELSE
    OutputValue = STRING(PadLength, PadCharacter) + InputValueString
  END IF

  RETURN OutputValue
END FUNCTION

FUNCTION IsInKeyRange(KeyInput AS STRING, StartRange AS BYTE, EndRange AS BYTE) AS BOOLEAN
  IF NOT LEN(KeyInput) = 1 THEN RETURN FALSE

  DIM AS BYTE KeyInputCode = ASC(KeyInput)
  RETURN KeyInputCode >= StartRange AND KeyInputCode <= EndRange
END FUNCTION

