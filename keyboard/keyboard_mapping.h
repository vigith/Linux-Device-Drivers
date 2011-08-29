// Please DO-NOT edit this file manually (a hardware specific file).
// This is generated by the Makefile.
// Read License Agreement before using.
// Author: Vigith Maurice <v@vigith.com>


struct keyboard_stat {
  char str[10];
  char hex[4];
  int dec;
  long int count_p;
  long int count_r;
} keyboard_stats[] = {
  { "DUMMY", "0", 0, 0, 0 }, 
  { "Escape", "1", 1, 0, 0 },
  { "one", "2", 2, 0, 0 },
  { "two", "3", 3, 0, 0 },
  { "three", "4", 4, 0, 0 },
  { "four", "5", 5, 0, 0 },
  { "five", "6", 6, 0, 0 },
  { "six", "7", 7, 0, 0 },
  { "seven", "8", 8, 0, 0 },
  { "eight", "9", 9, 0, 0 },
  { "nine", "a", 10, 0, 0 },
  { "zero", "b", 11, 0, 0 },
  { "minus", "c", 12, 0, 0 },
  { "equal", "d", 13, 0, 0 },
  { "Delete", "e", 14, 0, 0 },
  { "Tab", "f", 15, 0, 0 },
  { "+q", "10", 16, 0, 0 },
  { "+w", "11", 17, 0, 0 },
  { "+e", "12", 18, 0, 0 },
  { "+r", "13", 19, 0, 0 },
  { "+t", "14", 20, 0, 0 },
  { "+y", "15", 21, 0, 0 },
  { "+u", "16", 22, 0, 0 },
  { "+i", "17", 23, 0, 0 },
  { "+o", "18", 24, 0, 0 },
  { "+p", "19", 25, 0, 0 },
  { "bracketlef", "1a", 26, 0, 0 },
  { "bracketrig", "1b", 27, 0, 0 },
  { "Return", "1c", 28, 0, 0 },
  { "Control", "1d", 29, 0, 0 },
  { "+a", "1e", 30, 0, 0 },
  { "+s", "1f", 31, 0, 0 },
  { "+d", "20", 32, 0, 0 },
  { "+f", "21", 33, 0, 0 },
  { "+g", "22", 34, 0, 0 },
  { "+h", "23", 35, 0, 0 },
  { "+j", "24", 36, 0, 0 },
  { "+k", "25", 37, 0, 0 },
  { "+l", "26", 38, 0, 0 },
  { "semicolon", "27", 39, 0, 0 },
  { "apostrophe", "28", 40, 0, 0 },
  { "grave", "29", 41, 0, 0 },
  { "Shift", "2a", 42, 0, 0 },
  { "backslash", "2b", 43, 0, 0 },
  { "+z", "2c", 44, 0, 0 },
  { "+x", "2d", 45, 0, 0 },
  { "+c", "2e", 46, 0, 0 },
  { "+v", "2f", 47, 0, 0 },
  { "+b", "30", 48, 0, 0 },
  { "+n", "31", 49, 0, 0 },
  { "+m", "32", 50, 0, 0 },
  { "comma", "33", 51, 0, 0 },
  { "period", "34", 52, 0, 0 },
  { "slash", "35", 53, 0, 0 },
  { "Shift", "36", 54, 0, 0 },
  { "KP_Multipl", "37", 55, 0, 0 },
  { "Alt", "38", 56, 0, 0 },
  { "space", "39", 57, 0, 0 },
  { "CtrlL_Lock", "3a", 58, 0, 0 },
  { "F1", "3b", 59, 0, 0 },
  { "F2", "3c", 60, 0, 0 },
  { "F3", "3d", 61, 0, 0 },
  { "F4", "3e", 62, 0, 0 },
  { "F5", "3f", 63, 0, 0 },
  { "F6", "40", 64, 0, 0 },
  { "F7", "41", 65, 0, 0 },
  { "F8", "42", 66, 0, 0 },
  { "F9", "43", 67, 0, 0 },
  { "F10", "44", 68, 0, 0 },
  { "Num_Lock", "45", 69, 0, 0 },
  { "Scroll_Loc", "46", 70, 0, 0 },
  { "KP_7", "47", 71, 0, 0 },
  { "KP_8", "48", 72, 0, 0 },
  { "KP_9", "49", 73, 0, 0 },
  { "KP_Subtrac", "4a", 74, 0, 0 },
  { "KP_4", "4b", 75, 0, 0 },
  { "KP_5", "4c", 76, 0, 0 },
  { "KP_6", "4d", 77, 0, 0 },
  { "KP_Add", "4e", 78, 0, 0 },
  { "KP_1", "4f", 79, 0, 0 },
  { "KP_2", "50", 80, 0, 0 },
  { "KP_3", "51", 81, 0, 0 },
  { "KP_0", "52", 82, 0, 0 },
  { "KP_Period", "53", 83, 0, 0 },
  { "Last_Conso", "54", 84, 0, 0 },
  { "less", "56", 86, 0, 0 },
  { "F11", "57", 87, 0, 0 },
  { "F12", "58", 88, 0, 0 },
  { "KP_Enter", "60", 96, 0, 0 },
  { "Control", "61", 97, 0, 0 },
  { "KP_Divide", "62", 98, 0, 0 },
  { "Alt", "64", 100, 0, 0 },
  { "Break", "65", 101, 0, 0 },
  { "Find", "66", 102, 0, 0 },
  { "Up", "67", 103, 0, 0 },
  { "Prior", "68", 104, 0, 0 },
  { "Left", "69", 105, 0, 0 },
  { "Right", "6a", 106, 0, 0 },
  { "Select", "6b", 107, 0, 0 },
  { "Down", "6c", 108, 0, 0 },
  { "Next", "6d", 109, 0, 0 },
  { "Insert", "6e", 110, 0, 0 },
  { "Remove", "6f", 111, 0, 0 },
  { "Macro", "70", 112, 0, 0 },
  { "F13", "71", 113, 0, 0 },
  { "F14", "72", 114, 0, 0 },
  { "Help", "73", 115, 0, 0 },
  { "Do", "74", 116, 0, 0 },
  { "F17", "75", 117, 0, 0 },
  { "KP_MinPlus", "76", 118, 0, 0 },
  { "Pause", "77", 119, 0, 0 },
  { "Alt", "7d", 125, 0, 0 },
  { "Alt", "7e", 126, 0, 0 },
  { "nul", "80", 128, 0, 0 },
  { "nul", "81", 129, 0, 0 },
  { "nul", "82", 130, 0, 0 },
  { "nul", "83", 131, 0, 0 },
  { "nul", "84", 132, 0, 0 },
  { "nul", "85", 133, 0, 0 },
  { "nul", "86", 134, 0, 0 },
  { "nul", "87", 135, 0, 0 },
  { "nul", "88", 136, 0, 0 },
  { "nul", "89", 137, 0, 0 },
  { "nul", "8a", 138, 0, 0 },
  { "nul", "8b", 139, 0, 0 },
  { "nul", "8c", 140, 0, 0 },
  { "nul", "8d", 141, 0, 0 },
  { "nul", "8e", 142, 0, 0 },
  { "nul", "8f", 143, 0, 0 },
  { "nul", "90", 144, 0, 0 },
  { "nul", "91", 145, 0, 0 },
  { "nul", "92", 146, 0, 0 },
  { "nul", "93", 147, 0, 0 },
  { "nul", "94", 148, 0, 0 },
  { "nul", "95", 149, 0, 0 },
  { "nul", "96", 150, 0, 0 },
  { "nul", "97", 151, 0, 0 },
  { "nul", "98", 152, 0, 0 },
  { "nul", "99", 153, 0, 0 },
  { "nul", "9a", 154, 0, 0 },
  { "nul", "9b", 155, 0, 0 },
  { "nul", "9c", 156, 0, 0 },
  { "nul", "9d", 157, 0, 0 },
  { "nul", "9e", 158, 0, 0 },
  { "nul", "9f", 159, 0, 0 },
  { "nul", "a0", 160, 0, 0 },
  { "nul", "a1", 161, 0, 0 },
  { "nul", "a2", 162, 0, 0 },
  { "nul", "a3", 163, 0, 0 },
  { "nul", "a4", 164, 0, 0 },
  { "nul", "a5", 165, 0, 0 },
  { "nul", "a6", 166, 0, 0 },
  { "nul", "a7", 167, 0, 0 },
  { "nul", "a8", 168, 0, 0 },
  { "nul", "a9", 169, 0, 0 },
  { "nul", "aa", 170, 0, 0 },
  { "nul", "ab", 171, 0, 0 },
  { "nul", "ac", 172, 0, 0 },
  { "nul", "ad", 173, 0, 0 },
  { "nul", "ae", 174, 0, 0 },
  { "nul", "af", 175, 0, 0 },
  { "nul", "b0", 176, 0, 0 },
  { "nul", "b1", 177, 0, 0 },
  { "nul", "b2", 178, 0, 0 },
  { "nul", "b3", 179, 0, 0 },
  { "nul", "b4", 180, 0, 0 },
  { "nul", "b5", 181, 0, 0 },
  { "nul", "b6", 182, 0, 0 },
  { "nul", "b7", 183, 0, 0 },
  { "nul", "b8", 184, 0, 0 },
  { "nul", "b9", 185, 0, 0 },
  { "nul", "ba", 186, 0, 0 },
  { "nul", "bb", 187, 0, 0 },
  { "nul", "bc", 188, 0, 0 },
  { "nul", "bd", 189, 0, 0 },
  { "nul", "be", 190, 0, 0 },
  { "nul", "bf", 191, 0, 0 },
  { "nul", "c0", 192, 0, 0 },
  { "nul", "c1", 193, 0, 0 },
  { "nul", "c2", 194, 0, 0 },
  { "nul", "c3", 195, 0, 0 },
  { "nul", "c4", 196, 0, 0 },
  { "nul", "c5", 197, 0, 0 },
  { "nul", "c6", 198, 0, 0 },
  { "nul", "c7", 199, 0, 0 },
  { "nul", "c8", 200, 0, 0 },
  { "nul", "c9", 201, 0, 0 },
  { "nul", "ca", 202, 0, 0 },
  { "nul", "cb", 203, 0, 0 },
  { "nul", "cc", 204, 0, 0 },
  { "nul", "cd", 205, 0, 0 },
  { "nul", "ce", 206, 0, 0 },
  { "nul", "cf", 207, 0, 0 },
  { "nul", "d0", 208, 0, 0 },
  { "nul", "d1", 209, 0, 0 },
  { "nul", "d2", 210, 0, 0 },
  { "nul", "d3", 211, 0, 0 },
  { "nul", "d4", 212, 0, 0 },
  { "nul", "d5", 213, 0, 0 },
  { "nul", "d6", 214, 0, 0 },
  { "nul", "d7", 215, 0, 0 },
  { "nul", "d8", 216, 0, 0 },
  { "nul", "d9", 217, 0, 0 },
  { "nul", "da", 218, 0, 0 },
  { "nul", "db", 219, 0, 0 },
  { "nul", "dc", 220, 0, 0 },
  { "nul", "dd", 221, 0, 0 },
  { "nul", "de", 222, 0, 0 },
  { "nul", "df", 223, 0, 0 },
  { "nul", "e0", 224, 0, 0 },
  { "nul", "e1", 225, 0, 0 },
  { "nul", "e2", 226, 0, 0 },
  { "nul", "e3", 227, 0, 0 },
  { "nul", "e4", 228, 0, 0 },
  { "nul", "e5", 229, 0, 0 },
  { "nul", "e6", 230, 0, 0 },
  { "nul", "e7", 231, 0, 0 },
  { "nul", "e8", 232, 0, 0 },
  { "nul", "e9", 233, 0, 0 },
  { "nul", "ea", 234, 0, 0 },
  { "nul", "eb", 235, 0, 0 },
  { "nul", "ec", 236, 0, 0 },
  { "nul", "ed", 237, 0, 0 },
  { "nul", "ee", 238, 0, 0 },
  { "nul", "ef", 239, 0, 0 },
  { "nul", "f0", 240, 0, 0 },
  { "nul", "f1", 241, 0, 0 },
  { "nul", "f2", 242, 0, 0 },
  { "nul", "f3", 243, 0, 0 },
  { "nul", "f4", 244, 0, 0 },
  { "nul", "f5", 245, 0, 0 },
  { "nul", "f6", 246, 0, 0 },
  { "nul", "f7", 247, 0, 0 },
  { "nul", "f8", 248, 0, 0 },
  { "nul", "f9", 249, 0, 0 },
  { "nul", "fa", 250, 0, 0 },
  { "nul", "fb", 251, 0, 0 },
  { "nul", "fc", 252, 0, 0 },
  { "nul", "fd", 253, 0, 0 },
  { "nul", "fe", 254, 0, 0 },
  { "nul", "ff", 255, 0, 0 },
};

int SCAN_CODES = 241;