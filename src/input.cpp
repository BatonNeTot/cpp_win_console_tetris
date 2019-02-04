#include "input.h"

Input::Input(const Console &console, const HANDLE reader) :
    console(console),

    rHnd(reader),

    miceTouched(0),
    micePressed(0),
    miceReleased(0),

    keyTouched(0),
    keyTyped(0),
    keyReleased(0),

    charTouched(0),
    charTyped(0),
    charReleased(0),

    mouseTouched(0),
    mouseReleased(0),

    mouseX(-1),
    mouseY(-1) {}

void Input::update() {
    keysTouched.clear();
    keysTyped.clear();
    keysReleased.clear();

    miceTouched = 0;
    miceReleased = 0;

    /* Variable for holding the number of current events, and a point to it */
    DWORD numEvents = 0;

    /* Variable for holding how many events were read */
    DWORD numEventsRead = 0;

    /* Put the number of console input events into numEvents */
    GetNumberOfConsoleInputEvents(rHnd, &numEvents);

    /* if there's an event */
    if (numEvents) {
        /* Allocate the correct amount of memory to store the events */
        INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

        /* Place the stored events into the eventBuffer pointer */
        ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

        for (DWORD i = 0; i < numEventsRead; i++) {
            switch (eventBuffer[i].EventType) {
                case KEY_EVENT: {
                    KEY_EVENT_RECORD &keyEvent = eventBuffer[i].Event.KeyEvent;

                    if (keyEvent.bKeyDown) {
                        keysTyped.insert(keyEvent.wVirtualKeyCode);

                        keyTyped = keyEvent.wVirtualKeyCode;
                        charTyped = keyEvent.uChar.AsciiChar;

                        if (keysPressed.find(keyEvent.wVirtualKeyCode) == keysPressed.end()) {
                            keysTouched.insert(keyEvent.wVirtualKeyCode);
                            keysPressed.insert(keyEvent.wVirtualKeyCode);

                            keyTouched = keyEvent.wVirtualKeyCode;
                            charTouched = keyEvent.uChar.AsciiChar;
                        }
                    } else {
                        keysPressed.erase(keyEvent.wVirtualKeyCode);
                        keysReleased.insert(keyEvent.wVirtualKeyCode);

                        keyReleased = keyEvent.wVirtualKeyCode;
                        charReleased = keyEvent.uChar.AsciiChar;
                    }
                    break;
                }
                case MOUSE_EVENT: {
                    MOUSE_EVENT_RECORD &mouseEvent = eventBuffer[i].Event.MouseEvent;

                    switch (mouseEvent.dwEventFlags) {
                        case 0: {
                            DWORD mouseCode = micePressed ^ mouseEvent.dwButtonState;
                            micePressed = mouseEvent.dwButtonState;

                            if (micePressed & mouseCode) {
                                miceTouched |= mouseCode;
                                mouseTouched = mouseCode;
                            } else {
                                miceReleased |= mouseCode;
                                mouseReleased = mouseCode;
                            }

                            break;
                        }
                        case MOUSE_MOVED: {
                            SHORT &currentMouseX = mouseEvent.dwMousePosition.X;
                            SHORT &currentMouseY = mouseEvent.dwMousePosition.Y;

                            if (currentMouseX >= 0 && currentMouseX < console.width && currentMouseY >= 0 && currentMouseY < console.height) {
                                mouseX = currentMouseX;
                                mouseY = currentMouseY;
                            } else {
                                mouseX = -1;
                                mouseY = -1;
                            }

                            break;
                        }
                    }
                }
            }
        }

        delete eventBuffer;
    }
}
