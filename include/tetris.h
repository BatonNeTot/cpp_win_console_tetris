#ifndef TETRIS_H_INCLUDED
#define TETRIS_H_INCLUDED

#include "console.h"

#include <vector>
#include <list>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define FIELD_WSCALE 2
#define FIELD_HSCALE 2

#define FIELD_WBOARD 2
#define FIELD_HBOARD 2

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 22

enum class Block : WORD {
    N = 0,
    I = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    J = BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    L = BACKGROUND_INTENSITY,
    O = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    S = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    T = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
    Z = BACKGROUND_RED | BACKGROUND_INTENSITY
};

class Tetrino{
public:

    const Block color;

    const bool scheme[4][4][4];
    int collision[4][3][4];

    const int startOffset;

    constexpr Tetrino(const Block color,
                      const bool (&scheme)[4][4][4],
                      const int startOffset = 0):
                    color(color),
                    scheme({{
                       {scheme[0][0][0], scheme[0][0][1], scheme[0][0][2], scheme[0][0][3]},
                       {scheme[0][1][0], scheme[0][1][1], scheme[0][1][2], scheme[0][1][3]},
                       {scheme[0][2][0], scheme[0][2][1], scheme[0][2][2], scheme[0][2][3]},
                       {scheme[0][3][0], scheme[0][3][1], scheme[0][3][2], scheme[0][3][3]}
                    },{
                       {scheme[1][0][0], scheme[1][0][1], scheme[1][0][2], scheme[1][0][3]},
                       {scheme[1][1][0], scheme[1][1][1], scheme[1][1][2], scheme[1][1][3]},
                       {scheme[1][2][0], scheme[1][2][1], scheme[1][2][2], scheme[1][2][3]},
                       {scheme[1][3][0], scheme[1][3][1], scheme[1][3][2], scheme[1][3][3]}
                    },{
                       {scheme[2][0][0], scheme[2][0][1], scheme[2][0][2], scheme[2][0][3]},
                       {scheme[2][1][0], scheme[2][1][1], scheme[2][1][2], scheme[2][1][3]},
                       {scheme[2][2][0], scheme[2][2][1], scheme[2][2][2], scheme[2][2][3]},
                       {scheme[2][3][0], scheme[2][3][1], scheme[2][3][2], scheme[2][3][3]}
                    },{
                       {scheme[3][0][0], scheme[3][0][1], scheme[3][0][2], scheme[3][0][3]},
                       {scheme[3][1][0], scheme[3][1][1], scheme[3][1][2], scheme[3][1][3]},
                       {scheme[3][2][0], scheme[3][2][1], scheme[3][2][2], scheme[3][2][3]},
                       {scheme[3][3][0], scheme[3][3][1], scheme[3][3][2], scheme[3][3][3]}
                    }}), collision(),
                    startOffset(startOffset) {
                        for (int r = 0; r < 4; r++) {
                            for (int k = 0; k < 4; k++) {
                                collision[r][0][k] = -1;
                                for (int i = 0; i < 4; i++) {
                                    if (scheme[r][k][i]) {
                                        collision[r][0][k] = i;
                                        break;
                                    }
                                }
                                collision[r][1][k] = -1;
                                for (int i = 0; i < 4; i++) {
                                    if (scheme[r][3 - i][k]) {
                                        collision[r][1][k] = i;
                                        break;
                                    }
                                }
                                collision[r][2][k] = -1;
                                for (int i = 0; i < 4; i++) {
                                    if (scheme[r][k][3 - i]) {
                                        collision[r][2][k] = i;
                                        break;
                                    }
                                }
                            }
                        }
                    }
    ~Tetrino() = default;

};

#define TETRINO_COUNT 7

static const Tetrino tetrinos[] = {
    Tetrino(Block::I, {
                    {
                       {0, 0, 0, 0},
                       {1, 1, 1, 1},
                       {0, 0, 0, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 1, 0},
                       {0, 0, 1, 0},
                       {0, 0, 1, 0},
                       {0, 0, 1, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 0, 0},
                       {1, 1, 1, 1},
                       {0, 0, 0, 0}
                    },{
                       {0, 1, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 0, 0}
                    }
                    }, 1),
    Tetrino(Block::J, {
                    {
                       {0, 0, 0, 0},
                       {1, 0, 0, 0},
                       {1, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {0, 1, 0, 0},
                       {0, 1, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 0, 0},
                       {1, 1, 1, 0},
                       {0, 0, 1, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 0, 0},
                       {1, 1, 0, 0}
                    }
                    }),
    Tetrino(Block::L, {
                    {
                       {0, 0, 0, 0},
                       {0, 0, 1, 0},
                       {1, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 1, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 0, 0},
                       {1, 1, 1, 0},
                       {1, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {1, 1, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 0, 0}
                    }
                    }),
    Tetrino(Block::O, {
                    {
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {0, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {0, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {0, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {0, 1, 1, 0},
                       {0, 0, 0, 0}
                    }
                    }),
    Tetrino(Block::S, {
                    {
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {1, 1, 0, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 1, 0},
                       {0, 0, 1, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 0, 0},
                       {0, 1, 1, 0},
                       {1, 1, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {1, 0, 0, 0},
                       {1, 1, 0, 0},
                       {0, 1, 0, 0}
                    }
                    }),
    Tetrino(Block::T, {
                    {
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {1, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 1, 0},
                       {0, 1, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 0, 0},
                       {1, 1, 1, 0},
                       {0, 1, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {1, 1, 0, 0},
                       {0, 1, 0, 0}
                    }
                    }),
    Tetrino(Block::Z, {
                    {
                       {0, 0, 0, 0},
                       {1, 1, 0, 0},
                       {0, 1, 1, 0},
                       {0, 0, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 1, 0},
                       {0, 1, 1, 0},
                       {0, 1, 0, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 0, 0, 0},
                       {1, 1, 0, 0},
                       {0, 1, 1, 0}
                    },{
                       {0, 0, 0, 0},
                       {0, 1, 0, 0},
                       {1, 1, 0, 0},
                       {1, 0, 0, 0}
                    }
                    })
};

class Tetris{
private:

    Block field[TETRIS_HEIGHT][TETRIS_WIDTH];

    int nextTetrino;
    int currentTetrino;

    int tetrinoX;
    int tetrinoY;
    int tetrinoRotation;

    //0 = fit, 1 = borders, 2 = field
    int checkTetrino(const int tetrinoIndex, const int x, const int y, const int rotation);

    bool checkLine(const int index);
    void eraseLine(const int index);

    int chooseTetrino();

    bool canDown();
    bool canLeft();
    bool canRight();

    void rotate(const int rotation);

public:

    Tetris();

    void clear();
    int nextStep();

    bool down();
    void left();
    void right();

    void rotateClockwise();
    void rotateCounterClockwise();

    void draw(const Console &console) const;
};

#endif // TETRIS_H_INCLUDED
