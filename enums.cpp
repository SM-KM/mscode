// Enums just take 4 bytes in memory representing the int of the position
// Simple enum, definition ascending from 0 to n
enum weekDay { Monday, Tuesday, Wendsday, Thursday, Friday, Saturday, Sunday };

// The enums value of their positions can also be remapped the keys
enum font { ITALIC = 23, SEMIBOLD = 10, REGULAR = 2 };

/*
You can also implement the use of OR | bit operator by changing
the keys of the enum to the multiple of 2

UNDERLINE = 00000001
BOLD = 00000010
NORMAL = 00000100

int d = BOLD | NORMAL

00000010
00000100
---------
00000110

Allowing to use multiple conditions

*/
enum fontBit { UNDERLINE = 1, BOLD = 2, NORMAL = 4 };
