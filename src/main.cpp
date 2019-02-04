#include <iostream>
#include <algorithm>

#include "console.h"
#include "input.h"
#include "tetris.h"

#include <time.h>
#include <chrono>


#define MENU_WIDTH 22


#define FORE_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define FORE_GREY_LIGHT FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FORE_GREY_DARK FOREGROUND_INTENSITY
#define FORE_BLACK 0


#define BACK_WHITE BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY


#define BOARD_COLOR FORE_GREY_LIGHT
#define MENU_COLOR FORE_GREY_DARK
#define TEXT_COLOR FORE_WHITE

#define SCORE_WIDTH 14
#define SCORE_NAME_WIDTH (SCORE_WIDTH - 4)
#define INFO_WIDTH 16

void drawScore(Console &console, int score);
void drawHighscore(Console &console, int highscore, const char* author);

void showStartInfo(Console &console);
void showPauseInfo(Console &console);
void showHighscoreInfo(Console &console);
void showGameOverInfo(Console &console);
void hideInfo(Console &console);

using namespace std::chrono_literals;

#define TIME_STEP 800ms

std::chrono::steady_clock::time_point lastTime;

void startGame(Console &console, Tetris &tetris);

int score = 0;
int highscore = 0;

int cursor = 0;

int main(int argc, char** argv) {

    Console console(FIELD_WIDTH * FIELD_WSCALE * 2 + FIELD_WBOARD * 2 + MENU_WIDTH,
                    FIELD_HEIGHT * FIELD_HSCALE + FIELD_HBOARD,
                    GetStdHandle(STD_OUTPUT_HANDLE));
    Input input(console, GetStdHandle(STD_INPUT_HANDLE));
    Tetris tetris;

// ---- DRAWING FIELD ----

    for (int i = 0; i < FIELD_WBOARD; i++) {
        for (int j = 0; j < std::min(FIELD_HEIGHT * FIELD_HSCALE + i + 1,
                                     FIELD_HEIGHT * FIELD_HSCALE + FIELD_HBOARD); j++) {
            CHAR symbol = 1;
            if (j == FIELD_HEIGHT * FIELD_HSCALE + FIELD_HBOARD - 1) {
                symbol = (i == FIELD_WBOARD - 1) ? 0xC8 : 0xCA;
            } else {
                if (j != FIELD_HEIGHT * FIELD_HSCALE + i) {
                    symbol = 0xBA;
                } else {
                    symbol = (i == FIELD_WBOARD - 1) ? 0xCC : 0xC8;
                }
            }
            console.draw(FIELD_WBOARD - i - 1, j, symbol, BOARD_COLOR);
        }

        for (int j = 0; j < std::min(FIELD_HEIGHT * FIELD_HSCALE + i + 1,
                                     FIELD_HEIGHT * FIELD_HSCALE + FIELD_HBOARD); j++) {
            CHAR symbol = 1;
            if (j == FIELD_HEIGHT * FIELD_HSCALE + FIELD_HBOARD - 1) {
                symbol = (i == FIELD_WBOARD - 1) ? 0xBC : 0xCA;
            } else {
                if (j != FIELD_HEIGHT * FIELD_HSCALE + i) {
                    symbol = 0xBA;
                } else {
                    symbol = (i == FIELD_WBOARD - 1) ? 0xB9 : 0xBC;
                }
            }
            console.draw(FIELD_WBOARD + FIELD_WIDTH * FIELD_WSCALE * 2 + i, j, symbol, BOARD_COLOR);
        }
    }

    for (int i = 0; i < FIELD_HBOARD; i++) {
        for (int j = std::max(FIELD_WBOARD - i, 0); j < std::min(FIELD_WBOARD + FIELD_WIDTH * FIELD_WSCALE * 2 + i,
                                                                 FIELD_WBOARD * 2 + FIELD_WIDTH * FIELD_WSCALE * 2); j++) {
            CHAR symbol = 0xCD;

            if (j == 0) {
                symbol = (i == FIELD_HBOARD - 1) ? 0xC8 : 0xCC;
            }

            if (j == FIELD_WBOARD * 2 + FIELD_WIDTH * FIELD_WSCALE * 2 - 1) {
                symbol = (i == FIELD_HBOARD - 1) ? 0xBC : 0xB9;
            }

            console.draw(j, FIELD_HEIGHT * FIELD_HSCALE + i, symbol, BOARD_COLOR);
        }
    }

// ---- DRAWING PREVUE ----

    console.draw(console.width - 18, 1, 0xC9, MENU_COLOR);
    console.draw(console.width - 18, 8, 0xC8, MENU_COLOR);

    console.draw(console.width - 5, 1, 0xBB, MENU_COLOR);
    console.draw(console.width - 5, 8, 0xBC, MENU_COLOR);

    for (int i = 2; i < 8; i++) {
        console.draw(console.width - 18, i, 0xBA, MENU_COLOR);
        console.draw(console.width - 5, i, 0xBA, MENU_COLOR);
    }

    for (int i = console.width - 17; i < console.width - 5; i++) {
        console.draw(i, 1, 0xCD, MENU_COLOR);
        console.draw(i, 8, 0xCD, MENU_COLOR);
    }

// ---- DRAWING SCORE ----

    console.draw(console.width - 20, 10, 0xC9, MENU_COLOR);
    console.draw(console.width - 20, 14, 0xCC, MENU_COLOR);
    console.draw(console.width - 20, 18, 0xC8, MENU_COLOR);

    console.draw(console.width - 3, 10, 0xBB, MENU_COLOR);
    console.draw(console.width - 3, 14, 0xB9, MENU_COLOR);
    console.draw(console.width - 3, 18, 0xBC, MENU_COLOR);

    for (int i = 11; i < 14; i++) {
        console.draw(console.width - 20, i, 0xBA, MENU_COLOR);
        console.draw(console.width - 3, i, 0xBA, MENU_COLOR);

        console.draw(console.width - 20, i + 4, 0xBA, MENU_COLOR);
        console.draw(console.width - 3, i + 4, 0xBA, MENU_COLOR);
    }

    for (int i = console.width - 19; i < console.width - 3; i++) {
        console.draw(i, 10, 0xCD, MENU_COLOR);
        console.draw(i, 14, 0xCD, MENU_COLOR);
        console.draw(i, 18, 0xCD, MENU_COLOR);
    }

    console.drawStr(console.width - 13, 12, "SCORE", TEXT_COLOR);
    drawScore(console, score);

// ---- DRAWING HIGHSCORE ----

    console.draw(console.width - 20, 19, 0xC9, MENU_COLOR);
    console.draw(console.width - 20, 25, 0xCC, MENU_COLOR);
    console.draw(console.width - 20, 29, 0xC8, MENU_COLOR);

    console.draw(console.width - 3, 19, 0xBB, MENU_COLOR);
    console.draw(console.width - 3, 25, 0xB9, MENU_COLOR);
    console.draw(console.width - 3, 29, 0xBC, MENU_COLOR);

    for (int i = 20; i < 25; i++) {
        console.draw(console.width - 20, i, 0xBA, MENU_COLOR);
        console.draw(console.width - 3, i, 0xBA, MENU_COLOR);
    }

    for (int i = 26; i < 29; i++) {
        console.draw(console.width - 20, i, 0xBA, MENU_COLOR);
        console.draw(console.width - 3, i, 0xBA, MENU_COLOR);
    }

    for (int i = console.width - 19; i < console.width - 3; i++) {
        console.draw(i, 19, 0xCD, MENU_COLOR);
        console.draw(i, 25, 0xCD, MENU_COLOR);
        console.draw(i, 29, 0xCD, MENU_COLOR);
    }

    console.drawStr(console.width - 15, 21, "HIGHSCORE", TEXT_COLOR);
    console.drawStr(console.width - 18, 23, "BY: ", TEXT_COLOR);
    drawHighscore(console, highscore, "BOBAN_69");

// ---- ACTUAL GAME

    showStartInfo(console);

#define STATE_BEGINNING 0
#define STATE_GAME 1
#define STATE_PAUSE 2
#define STATE_HIGHSCORE 3
#define STATE_GAME_OVER 4

    int state = STATE_BEGINNING;

    while (true) {
        console.update();
        input.update();

        if (input.isKeyTouched(VK_ESCAPE)) {
            return 0;
        }

        switch (state) {
            case STATE_BEGINNING: {
                if (input.isAnyKeyTouched()) {
                    state = STATE_GAME;
                    startGame(console, tetris);
                }
                break;
            }
            case STATE_GAME: {
                auto currentTime = std::chrono::steady_clock::now();

                if (input.isKeyTouched('P')) {
                    state = STATE_PAUSE;
                    showPauseInfo(console);
                    break;
                }

                if (input.isKeyTyped(VK_DOWN)) {
                    if (tetris.down()) {
                        lastTime = currentTime;
                        tetris.draw(console);
                    }
                }

                if (input.isKeyTyped(VK_LEFT)) {
                    tetris.left();
                    tetris.draw(console);
                }
                if (input.isKeyTyped(VK_RIGHT)) {
                    tetris.right();
                    tetris.draw(console);
                }
                if (input.isKeyTyped(VK_UP)) {
                    tetris.rotateClockwise();
                    tetris.draw(console);
                }

                auto delta = currentTime - lastTime;

                if (delta >= TIME_STEP) {
                    lastTime = currentTime;

                    int result = tetris.nextStep();
                    if (result >= 0) {
                        score += result;
                        drawScore(console, score);
                    } else {
                        if (score > highscore) {
                            state = STATE_HIGHSCORE;
                            showHighscoreInfo(console);
                            highscore = score;
                            drawHighscore(console, highscore, "");
                        } else {
                            state = STATE_GAME_OVER;
                            showGameOverInfo(console);
                        }
                    }

                    tetris.draw(console);
                }
                break;
            }
            case STATE_PAUSE: {
                if (input.isKeyTouched('P')) {
                    state = STATE_GAME;
                    lastTime = std::chrono::steady_clock::now();
                    hideInfo(console);
                }
                break;
            }
            case STATE_HIGHSCORE: {

                if (input.isAnyKeyTyped()) {
                    CHAR symbol = input.lastCharTyped();

                    if (cursor < SCORE_NAME_WIDTH && symbol >= 0x20 && symbol <= 0x7f) {
                        console.draw(console.width - 14 + cursor, 23, symbol, TEXT_COLOR);
                        cursor++;
                    }
                    if (cursor > 0 && input.lastKeyTyped() == VK_BACK) {
                        console.draw(console.width - 14 + cursor, 23, 0, TEXT_COLOR);
                        cursor--;
                    }
                }

                if (cursor < SCORE_NAME_WIDTH) {
                    console.draw(console.width - 14 + cursor, 23, '_', TEXT_COLOR);
                }

                if (input.isKeyTouched(VK_RETURN)) {
                    if (cursor < SCORE_NAME_WIDTH) {
                        console.draw(console.width - 14 + cursor, 23, 0, TEXT_COLOR);
                    }
                    state = STATE_GAME_OVER;
                    showGameOverInfo(console);
                }

                break;
            }
            case STATE_GAME_OVER: {
                if (input.isKeyTouched('Y')) {
                    state = STATE_GAME;
                    startGame(console, tetris);
                } else if (input.isKeyTouched('N')) {
                    return 0;
                }
                break;
            }

        }

    }

    return 0;
}

void drawScore(Console &console, int score) {
    char scoreStr[SCORE_WIDTH + 1];

    snprintf(scoreStr, SCORE_WIDTH + 1, "%.*d", SCORE_WIDTH, score);
    console.drawStr(console.width - 18, 16, scoreStr, TEXT_COLOR);
}


void drawHighscore(Console &console, int highscore, const char* author) {
    char scoreStr[SCORE_WIDTH + 1];

    snprintf(scoreStr, SCORE_WIDTH + 1, "%.*d", SCORE_WIDTH, highscore);
    console.drawStr(console.width - 18, 27, scoreStr, TEXT_COLOR);

    snprintf(scoreStr, SCORE_WIDTH + 1, "%*.*s", SCORE_WIDTH - 4, SCORE_WIDTH - 4, author);
    console.drawStr(console.width - 14, 23, scoreStr, TEXT_COLOR);
}

void showStartInfo(Console &console) {
    hideInfo(console);
    console.drawStr(console.width - 14, 33, "WELCOME!", TEXT_COLOR);
    console.drawStr(console.width - 18, 35, "PRESS ANY KEY TO", TEXT_COLOR);
    console.drawStr(console.width - 13, 37, "START", TEXT_COLOR);
}

void showPauseInfo(Console &console) {
    hideInfo(console);
    console.drawStr(console.width - 13, 35, "PAUSE", TEXT_COLOR);
}

void showHighscoreInfo(Console &console) {
    hideInfo(console);
    console.drawStr(console.width - 17, 34, "NEW HIGHSCORE!", TEXT_COLOR);
    console.drawStr(console.width - 18, 36, "ENTER YOUR NAME", TEXT_COLOR);
}

void showGameOverInfo(Console &console) {
    hideInfo(console);
    console.drawStr(console.width - 14, 33, "GAME OVER.", TEXT_COLOR);
    console.drawStr(console.width - 17, 35, "WANT TO REPEAT?", TEXT_COLOR);
    console.drawStr(console.width - 11, 37, "Y  N", TEXT_COLOR);
}

void hideInfo(Console &console) {
    char infoStr[INFO_WIDTH + 1];
    snprintf(infoStr, INFO_WIDTH + 1, "%*s", INFO_WIDTH, "");

    console.drawStr(console.width - 18, 33, infoStr, TEXT_COLOR);
    console.drawStr(console.width - 18, 34, infoStr, TEXT_COLOR);
    console.drawStr(console.width - 18, 35, infoStr, TEXT_COLOR);
    console.drawStr(console.width - 18, 36, infoStr, TEXT_COLOR);
    console.drawStr(console.width - 18, 37, infoStr, TEXT_COLOR);
}

void startGame(Console &console, Tetris &tetris) {
    hideInfo(console);

    lastTime = std::chrono::steady_clock::now();

    score = 0;
    drawScore(console, score);

    tetris.clear();
    tetris.nextStep();
    tetris.draw(console);
}
