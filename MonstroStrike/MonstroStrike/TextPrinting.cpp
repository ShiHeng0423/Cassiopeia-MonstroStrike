#include "TextPrinting.h"
#include <iostream>

void PrintTextOverTime(const char* pText, f32 printDelay, f32 xPos, f32 yPos, f32 scale, f32 r, f32 g, f32 b, f32 a, s8 pFont, std::vector<PrintedCharacter>& printedCharVec, f32* printTimer)
{
    static int nextRow = 0;
    static f32 newXPosition = 0.f;
    PrintedCharacter printedChar;
    printedChar.position = { xPos, yPos };

    *printTimer += (f32)AEFrameRateControllerGetFrameTime(); // Update the timer
    static int currentIndex = 0;

    if (*printTimer >= printDelay && pText[currentIndex] != '\0')
    {
        *printTimer = 0.f;
        printedChar.character = pText[currentIndex];
        printedCharVec.push_back(printedChar);

        currentIndex++;
    }

    for (size_t i = 0; i < printedCharVec.size(); i++)
    {
        PrintedCharacter& printedC = printedCharVec[i];
        printedChar.position.x = xPos + 0.03f * i - 2 * nextRow;
        AEGfxPrint(pFont, &printedC.character, printedChar.position.x, printedChar.position.y, scale, r, g, b, a);
    }
}
