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
DECLARE SUB ShowHelp()

DIM SHARED AS STRING HelpText(23)
HelpText(1) = "Welcome to Cobalt!"
HelpText(2) = "                 Arrows : Move Cursor"
HelpText(3) = "                   Home : Move to beginning of line"
HelpText(4) = "                    End : Move to end of line"
HelpText(5) = " CTRL+Left / CTRL+Right : Change Background Color"
HelpText(6) = " CTRL+Up   /  CTRL+Down : Change Foreground Color"
HelpText(7) = " ALT+Up    /   ALT+Down : Change Character Set"
HelpText(8) = "                Numbers : Draw with chaeacter"
HelpText(9) = "                  ENTER : Move Down"
HelpText(10) = "              BACKSPACE : Move Left"
HelpText(11) = "                    DEL : Remove Character"
HelpText(12) = "                 CTRL+N : New Drawing"
HelpText(13) = "                 CTRL+S : Save Drawing"
HelpText(14) = "                 CTRL+O : Open Drawing"
HelpText(15) = "                 CTRL+F : Show Full Drawing"
HelpText(16) = "                 CTRL+E : Export Drawing"
HelpText(17) = "                 CTRL+I : Import Drawing"
HelpText(18) = "                    ESC : Exit"
HelpText(19) = ""
HelpText(20) = "    Cobalt Copyright (C) 2018 Jonathon Gosselin"
HelpText(21) = "    This program comes with ABSOLUTELY NO WARRANTY."
HelpText(22) = "    This is free software, and you are welcome to redistribute it"
HelpText(23) = "    under certain conditions.  See LICENSE.TXT for more information."

SUB ShowHelp()
  LOCATE 1, 1, 0
  COLOR 7, 0

  PRINT CHR(201); STRING(78, 205); CHR(187);
  FOR WriteHelpText AS BYTE = 1 TO UBOUND(HelpText)
    LOCATE (WriteHelpText + 1), 1

    DIM AS BYTE PadRight = 77 - LEN(HelpText(WriteHelpText))
    PRINT CHR(186); " "; HelpText(WriteHelpText); STRING(PadRight, 32); CHR(186);
  NEXT WriteHelpText
  PRINT CHR(200); STRING(78, 205); CHR(188);

  DO:LOOP WHILE INKEY() = ""
END SUB

