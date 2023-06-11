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
DECLARE SUB ShowHelp()

DIM SHARED AS STRING HelpText(25)
HelpText(1) = "Welcome to Cobalt!"
HelpText(2) = "                 Arrows : Move Cursor"
HelpText(3) = "                   Home : Move to beginning of line"
HelpText(4) = "                    End : Move to end of line"
HelpText(5) = " CTRL+Left / CTRL+Right : Change Background Color"
HelpText(6) = " CTRL+Up   /  CTRL+Down : Change Foreground Color"
HelpText(7) = " ALT+Up    /   ALT+Down : Change Character Set"
HelpText(8) = "                 F1-F10 : Draw with chaeacter"
HelpText(9) = "                  ENTER : Move Down"
HelpText(10) = "              BACKSPACE : Move Left and Remove Character"
HelpText(11) = "                    DEL : Remove Character"
HelpText(12) = "                 CTRL+N : New Drawing"
HelpText(13) = "                 CTRL+S : Save Drawing"
HelpText(14) = "                 CTRL+O : Open Drawing"
HelpText(15) = "                 CTRL+F : Show Full Drawing"
HelpText(16) = "                 CTRL+E : Export Drawing"
HelpText(17) = "                 CTRL+I : Import Drawing"
HelpText(18) = "                    ESC : Exit"
HelpText(19) = "               SHIFT+F2 : Rapid Select Character Set"
HelpText(20) = "               SHIFT+F3 : Rapid Select Color"
HelpText(21) = ""
HelpText(22) = "    Cobalt Copyright (C) 2018-2023 Jonathon Gosselin"
HelpText(23) = "    This program comes with ABSOLUTELY NO WARRANTY."
HelpText(24) = "    This is free software, and you are welcome to redistribute it"
HelpText(25) = "    under certain conditions.  See LICENSE.TXT for more information."

SUB ShowHelp()
  LOCATE 1, 1, 0
  COLOR 7, 0

  FOR WriteHelpText AS BYTE = 1 TO UBOUND(HelpText)
    DIM PadRightLength AS BYTE = 80 - LEN(HelpText(WriteHelpText))
    LOCATE WriteHelpText, 1: PRINT HelpText(WriteHelpText); STRING(PadRightLength, 32);
  NEXT WriteHelpText

  DO:LOOP WHILE INKEY() = ""
END SUB
