#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>

class Console{
private:
    static const WORD CHAR_ATTRIB_MASK = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY |
                                         BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;

    const HANDLE wHnd; /* write (output) handle */

    const SMALL_RECT windowSize;
    const COORD bufferSize;

    CHAR_INFO *characterBuffer;

    const COORD characterBufferSize;
    const COORD characterPosition;
    SMALL_RECT consoleWriteArea;

public:
    const SHORT width, height;

    Console(const SHORT width, const SHORT height, const HANDLE writer);
    ~Console();

    void draw(const SHORT x, const SHORT y, const CHAR symbol, const WORD colorAttribs) const;

    void drawStr(const SHORT x, const SHORT y, const CHAR* string, const WORD colorAttribs) const;

    void update();
};

#endif // CONSOLE_H_INCLUDED
