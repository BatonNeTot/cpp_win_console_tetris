#include "tetris.h"

#include <ctime>
#include <cstdlib>

Tetris::Tetris() :
    currentTetrino(-1),
    nextTetrino(-1) {
    clear();
}

static int inverseHeight(const int height) {
    return TETRIS_HEIGHT - 1 - height;
}

int Tetris::checkTetrino(const int tetrinoIndex, const int x, const int y, const int rotation) {
    auto &tetrino = tetrinos[tetrinoIndex];
    auto &scheme = tetrino.scheme[rotation];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (scheme[j][i]) {
                int pointX = x + i;
                int pointY = inverseHeight(y + j);
                if (pointX < 0 || pointX >= TETRIS_WIDTH || pointY < 0 || pointY >= TETRIS_HEIGHT) {
                    return 1;
                }
                if (field[pointY][pointX] != Block::N) {
                    return 2;
                }
            }
        }
    }
    return 0;
}

bool Tetris::checkLine(const int index) {
    if (index < 0 || index >= TETRIS_HEIGHT)
        return false;

    for (int i = 0; i < TETRIS_WIDTH; i++) {
        if (field[index][i] == Block::N) {
            return false;
        }
    }
    return true;
}

void Tetris::eraseLine(const int index) {
    for (int i = 0; i < TETRIS_WIDTH; i++) {
        for (int j = index; j < TETRIS_HEIGHT - 1; j++) {
            field[j][i] = field[j + 1][i];
        }
        field[TETRIS_HEIGHT - 1][i] = Block::N;
    }
}

int Tetris::chooseTetrino() {
    srand(time(nullptr));
    return rand() % TETRINO_COUNT;
}

void Tetris::clear() {
    for (int i = 0; i < TETRIS_WIDTH; i++) {
        for (int j = 0; j < TETRIS_HEIGHT; j++) {
            field[j][i] = Block::N;
        }
    }
}

int Tetris::nextStep() {
    if (currentTetrino >= 0) {
        if (down()) {

        } else {
            int count = 0;
            auto &tetrino = tetrinos[currentTetrino];
            for (int j = 0; j < 4; j++) {
                int line = inverseHeight(tetrinoY + j);
                for (int i = 0; i < 4; i++) {
                    if (tetrino.scheme[tetrinoRotation][j][i]) {
                        field[line][tetrinoX + i] = tetrino.color;
                    }
                }
                if (checkLine(line)) {
                    eraseLine(line);
                    count++;
                }
            }
            currentTetrino = -1;

            if (count == 0) {
                return nextStep();
            } else {
                return count;
            }
        }
    } else {
        if (nextTetrino < 0) {
            nextTetrino = chooseTetrino();
        }
        currentTetrino = nextTetrino;
        nextTetrino = chooseTetrino();

        tetrinoX = (TETRIS_WIDTH - 4) / 2;
        tetrinoY = 0 + tetrinos[currentTetrino].startOffset;
        tetrinoRotation = 0;

        if (checkTetrino(currentTetrino, tetrinoX, tetrinoY, tetrinoRotation)) {
            currentTetrino = -1;
            return -1;
        }
    }
    return 0;
}

bool Tetris::canDown() {
    if (currentTetrino < 0) {
        return false;
    }
    auto &collision = tetrinos[currentTetrino].collision[tetrinoRotation][1];
    for (int i = 0; i < 4; i++) {
        if (collision[i] >= 0) {
            int checkingCoord = tetrinoY - collision[i] + 4;
            if (checkingCoord >= TETRIS_HEIGHT || field[inverseHeight(checkingCoord)][tetrinoX + i] != Block::N){
                return false;
            }
        }
    }
    return true;
}

bool Tetris::down() {
    if (canDown()) {
        tetrinoY++;
        return true;
    } else {
        return false;
    }
}

bool Tetris::canLeft() {
    if (currentTetrino < 0) {
        return false;
    }
    auto &collision = tetrinos[currentTetrino].collision[tetrinoRotation][0];
    for (int i = 0; i < 4; i++) {
        if (collision[i] >= 0) {
            int checkingCoord = tetrinoX + collision[i] - 1;
            if (checkingCoord < 0 || field[inverseHeight(tetrinoY + i)][checkingCoord] != Block::N){
                return false;
            }
        }
    }
    return true;
}

void Tetris::left() {
    if (canLeft()) {
        tetrinoX--;
    }
}

bool Tetris::canRight() {
    if (currentTetrino < 0) {
        return false;
    }
    auto &collision = tetrinos[currentTetrino].collision[tetrinoRotation][2];
    for (int i = 0; i < 4; i++) {
        if (collision[i] >= 0) {
            int checkingCoord = tetrinoX - collision[i] + 4;
            if (checkingCoord >= TETRIS_WIDTH || field[inverseHeight(tetrinoY + i)][checkingCoord] != Block::N){
                return false;
            }
        }
    }
    return true;
}

void Tetris::right() {
    if (canRight()) {
        tetrinoX++;
    }
}

void Tetris::rotate(const int rotation) {
    if (currentTetrino < 0) {
        return;
    }

    int testRotation = rotation;
    int testX = tetrinoX;
    int testY = tetrinoY;

    auto &tetrino = tetrinos[currentTetrino];

    if (testX < 0) {
        auto &collision = tetrino.collision[testRotation][0];
        for (int i = 0; i < 4; i++) {
            if (collision[i] >= 0) {
                while (testX + collision[i] < 0) {
                    testX++;
                }
            }
        }
    } else if (testX > (TETRIS_WIDTH - 4)) {
        auto &collision = tetrino.collision[testRotation][2];
        for (int i = 0; i < 4; i++) {
            if (collision[i] >= 0) {
                while (testX + 4 - collision[i] > TETRIS_WIDTH) {
                    testX--;
                }
            }
        }
    }

    if (!checkTetrino(currentTetrino, testX, testY, testRotation)) {
        tetrinoRotation = testRotation;
        tetrinoX = testX;
        tetrinoY = testY;
    }
}

void Tetris::rotateClockwise() {
    rotate((tetrinoRotation + 1) % 4);
}

void Tetris::rotateCounterClockwise() {
    rotate((tetrinoRotation - 1) % 4);
}

void static drawPixel(const Console &console, const SHORT x, const SHORT y, const Block color) {
    int pixelX = FIELD_WBOARD + x * FIELD_WSCALE * 2;
    int pixelY = (FIELD_HEIGHT - y - 1) * FIELD_HSCALE;
    for (int i = 0; i < FIELD_WSCALE * 2; i++) {
        for (int j = 0; j < FIELD_HSCALE; j++) {
            console.draw(pixelX + i, pixelY + j, 0, (WORD)color);
        }
    }
}

void Tetris::draw(const Console &console) const {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < TETRIS_WIDTH; x++) {
            drawPixel(console, x, y, field[y][x]);
        }
    }

    if (currentTetrino >= 0) {
        auto &current = tetrinos[currentTetrino];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (current.scheme[tetrinoRotation][j][i]) {
                    drawPixel(console, tetrinoX + i, inverseHeight(tetrinoY + j), current.color);
                }
            }
        }
    }

    if (nextTetrino >= 0) {
        for (int i = console.width - 15; i < console.width - 5; i++) {
            for (int j = 2; j < 7; j++) {
                console.draw(i, j, 0, 0);
            }
        }

        auto &next = tetrinos[nextTetrino];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (next.scheme[0][j][i]) {
                    console.draw(console.width - 15 + i * 2,
                                 3 + j, 0, (WORD)next.color);
                    console.draw(console.width - 15 + i * 2 + 1,
                                 3 + j, 0, (WORD)next.color);
                }
            }
        }
    }
}
