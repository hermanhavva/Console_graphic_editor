#include "colours.h"

const unordered_map<int, WORD> colourEnumToWordMap =
{
    {BLACK, yellowFontBlackText},
    {RED, redText | yellowFontBlackText},
    {CYAN, cyanText | yellowFontBlackText},
    {PURPLE, purpleText | yellowFontBlackText},
    {BLUE, blueText | yellowFontBlackText},
    {GREEN, greenText | yellowFontBlackText}
};

const unordered_map<WORD, int> colourWordToEnumMap =
 {
     {yellowFontBlackText, BLACK},
     {redText | yellowFontBlackText, RED},
     {cyanText | yellowFontBlackText, CYAN},
     {purpleText | yellowFontBlackText, PURPLE},
     {blueText | yellowFontBlackText, BLUE},
     {greenText | yellowFontBlackText, GREEN}
 };