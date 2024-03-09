#pragma once
#include <AEEngine.h>
#include <vector>

struct PrintedCharacter {
    char character;

    AEVec2 position;
    AEVec2 offsetPosition;
};

void PrintTextOverTime(const char* pText, f32 printDelay, f32 xPos, f32 yPos, f32 scale, f32 r, f32 g, f32 b, f32 a, s8 pFont, std::vector<PrintedCharacter>& printedCharVec, f32* printTimer);