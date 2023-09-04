/***
 * Cobalt (C++ Rewrite) - ASCII Art
 * Copyright (C) 2018-2023  Jonathon Gosselin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ***/
#include <string>
#include <vector>

#ifndef HELP_H
#define HELP_H

std::vector<std::string> getHelpText() {
	std::vector<std::string> helpText;

	helpText.push_back("Welcome to Cobalt 2.0!");
	helpText.push_back(""); // Blank line
	helpText.push_back("                 Arrows : Move Cursor");
	helpText.push_back("                   Home : Move to beginning of line");
	helpText.push_back("                    End : Move to end of line");
	helpText.push_back(" CTRL+Left / CTRL+Right : Change Background Color");
	helpText.push_back(" CTRL+Up   /  CTRL+Down : Change Foreground Color");
	helpText.push_back(" ALT+Up    /   ALT+Down : Change Character Set");
	helpText.push_back("                 F1-F10 : Draw with character");
	helpText.push_back("                  ENTER : Move Down");
	helpText.push_back("              BACKSPACE : Move Left and Remove Character");
	helpText.push_back("                    DEL : Remove Character");
	helpText.push_back("                 CTRL+N : New Drawing");
	helpText.push_back("                 CTRL+S : Save Drawing");
	helpText.push_back("                 CTRL+O : Open Drawing");
	helpText.push_back("                 CTRL+F : Show Full Drawing");
	helpText.push_back("                 CTRL+E : Export Drawing");
	helpText.push_back("                 CTRL+I : Import Drawing");
	helpText.push_back("                    ESC : Exit");
	helpText.push_back("               SHIFT+F2 : Rapid Select Character Set");
	helpText.push_back("               SHIFT+F3 : Rapid Select Color");
	helpText.push_back(""); // Blank line
	helpText.push_back("    Cobalt Copyright (C) 2018-2023 Jonathon Gosselin");
	helpText.push_back("    This program comes with ABSOLUTELY NO WARRANTY.");
	helpText.push_back("    This is free software, and you are welcome to redistribute it");
	helpText.push_back("    under certain conditions.  See LICENSE.TXT for more information.");
	helpText.push_back(""); // Blank line
	helpText.push_back("A note about improting ANSI drawings:");
	helpText.push_back("  Importing ANSI drawings has very limited support.  Currently, it only");
	helpText.push_back("  supports color commands (0m, 1m, 30-37m, and 40-47m) and none of the");
	helpText.push_back("  cursor placement commands.");
	helpText.push_back(""); // Blank line
	helpText.push_back("Notes on Exporting:");
	helpText.push_back("  1) ANSI does not support increasing the color intensity by default;");
	helpText.push_back("     exporting drawing with background color codes of 7 will will be");
	helpText.push_back("     rendered to its non-intensne form (color code - 7)");
	helpText.push_back("  2) Escape and EOF (End of File) characters are repplaced with a");
	helpText.push_back("     space character instead.  Doing so prevents issues rendering the");
	helpText.push_back("     ANSI drawing onto the screen in DOS.");
	helpText.push_back("  3) Since Cobalt only supports 80x25 screens, the ANSI drawing is");
	helpText.push_back("     exported as a single line of text with proper ANSI codes; an");
	helpText.push_back("     80x25 screen should render the ANSI drawing correctly.");
	helpText.push_back("  4) In an attempt to keep the text drawings to aling correctly,");
	helpText.push_back("     the last column and line are removed during the export.");
	helpText.push_back("     Leaving the last line or column will cause mis-alignments.");

	return helpText;
}

#endif
