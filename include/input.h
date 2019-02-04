#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "console.h"

#include <windows.h>
#include <set>

class Input{
private:

    const Console &console;

    const HANDLE rHnd; /* read (input) handle */

    std::set<WORD> keysTouched;
    std::set<WORD> keysTyped;
    std::set<WORD> keysPressed;
    std::set<WORD> keysReleased;

    DWORD miceTouched;
    DWORD micePressed;
    DWORD miceReleased;

    WORD keyTouched;
    WORD keyTyped;
    WORD keyReleased;

    CHAR charTouched;
    CHAR charTyped;
    CHAR charReleased;

    DWORD mouseTouched;
    DWORD mouseReleased;

    SHORT mouseX;
    SHORT mouseY;

public:

    Input(const Console &console, const HANDLE reader);

    void update();

    inline bool isKeyTouched(const WORD keyCode) { return keysTouched.find(keyCode) != keysTouched.end(); }
    inline bool isKeyTyped(const WORD keyCode) { return keysTyped.find(keyCode) != keysTyped.end(); }
    inline bool isKeyPressed(const WORD keyCode) { return keysPressed.find(keyCode) != keysPressed.end(); }
    inline bool isKeyReleased(const WORD keyCode) { return keysReleased.find(keyCode) != keysReleased.end(); }

    inline bool isMouseTouched(const DWORD mouseCode) { return miceTouched & mouseCode; }
    inline bool isMousePressed(const DWORD mouseCode) { return micePressed & mouseCode; }
    inline bool isMouseReleased(const DWORD mouseCode) { return miceReleased & mouseCode; }

    inline bool isAnyKeyTouched() { return keysTouched.size() != 0; }
    inline bool isAnyKeyTyped() { return keysTyped.size() != 0; }
    inline bool isAnyKeyPressed() { return keysPressed.size() != 0; }
    inline bool isAnyKeyReleased() { return keysReleased.size() != 0; }

    inline bool isAnyMouseTouched() { return miceTouched; }
    inline bool isAnyMousePressed() { return micePressed; }
    inline bool isAnyMouseReleased() { return miceReleased; }

    inline WORD lastKeyTouched() { return keyTouched; }
    inline WORD lastKeyTyped() { return keyTyped; }
    inline WORD lastKeyReleased() { return keyReleased; }

    inline CHAR lastCharTouched() { return charTouched; }
    inline CHAR lastCharTyped() { return charTyped; }
    inline CHAR lastCharReleased() { return charReleased; }

    inline DWORD lastMouseTouched() { return mouseTouched; }
    inline DWORD lastMouseReleased() { return mouseReleased; }

    inline SHORT getMouseX() {return mouseX;}
    inline SHORT getMouseY() {return mouseY;}

};

#endif // INPUT_H_INCLUDED
