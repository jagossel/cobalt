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

DECLARE FUNCTION PadLeft(InputValue AS BYTE, TargetLength AS INTEGER, PadCharacter AS STRING) AS STRING
DECLARE FUNCTION IsInKeyRange(KeyInput AS STRING, StartRange AS UBYTE, EndRange AS UBYTE) AS BOOLEAN
DECLARE FUNCTION IsInModifiedKeyRange(KeyInput AS STRING, StartRange AS UBYTE, EndRange AS UBYTE) AS BOOLEAN
DECLARE FUNCTION GetConfirmation(ConfirmationText AS STRING) AS BOOLEAN

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

FUNCTION IsInKeyRange(KeyInput AS STRING, StartRange AS UBYTE, EndRange AS UBYTE) AS BOOLEAN
  IF NOT LEN(KeyInput) = 1 THEN RETURN FALSE

  DIM AS UBYTE KeyInputCode = ASC(KeyInput)
  RETURN KeyInputCode >= StartRange AND KeyInputCode <= EndRange
END FUNCTION

FUNCTION IsInModifiedKeyRange(KeyInput AS STRING, StartRange AS UBYTE, EndRange AS UBYTE) AS BOOLEAN
  IF NOT LEN(KeyInput) = 2 THEN RETURN FALSE
  IF ASC(LEFT(KeyInput, 1)) <> 255 THEN RETURN FALSE

  DIM AS UBYTE KeyInputCode = ASC(RIGHT(KeyInput, 1))
  RETURN KeyInputCode >= StartRange AND KeyInputCode <= EndRange
END FUNCTION

FUNCTION GetConfirmation(DialogText AS STRING) AS BOOLEAN
  DIM AS BYTE DialogLocation = 25
  IF StatusAtTop THEN DialogLocation = 1

  LOCATE DialogLocation, 1, 0: COLOR 7, 0: PRINT SPACE(80);
  LOCATE DialogLocation, 1, 1: COLOR 12, 0: PRINT DialogText; " (Y/N) ";

  DIM AS BOOLEAN DialogResult = FALSE
  DIM AS BOOLEAN ExitDialog = FALSE
  DO
    DIM AS STRING KeyInput = UCASE(INKEY())
    IF KeyInput = "Y" THEN
      ExitDialog = TRUE
      DialogResult = TRUE
    ELSEIF KeyInput = "N" ORELSE KeyInput = KeyEscape THEN
      ExitDialog = TRUE
    END IF
  LOOP UNTIL ExitDialog

  RETURN DialogResult
END FUNCTION
