#include "console.h"

#include <cstdio>
#include <cstring>

Console::Console(const SHORT width, const SHORT height, const HANDLE writer) :
    wHnd(writer),

    windowSize({0, 0, (SHORT)(width - 1), (SHORT)(height - 1)}),
    bufferSize({width, height}),

    characterBuffer(new CHAR_INFO[width * height]),
    characterBufferSize({width, height}),
    characterPosition({0, 0}),
    consoleWriteArea({0, 0, (SHORT)(width - 1), (SHORT)(height - 1)}),

    width(width),
    height(height) {

    SetConsoleTitle("Tetris");

    /* Set the window size */
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

    /* Set the screen's buffer size */
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    int fullSize = width * height;
    for (int i = 0; i < fullSize; i++) {
        characterBuffer[i] = {0};
    }
}

Console::~Console() {
    delete characterBuffer;
}

void Console::draw(const SHORT x, const SHORT y, const CHAR symbol, const WORD colorAttribs) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        CHAR_INFO &character = characterBuffer[x + width * y];
        character.Char.AsciiChar = symbol;
        character.Attributes = colorAttribs & CHAR_ATTRIB_MASK;
    }
}

void Console::drawStr(const SHORT x, const SHORT y, const CHAR* string, const WORD colorAttribs) const {
    if (string) {
        int length = strlen(string);
        for (int i = 0; i < length; i++) {
            draw(x + i, y, string[i], colorAttribs);
        }
    }
}

void Console::update() {
    /* Write our character buffer to the console buffer */
    WriteConsoleOutputA(wHnd, characterBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
}
