#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#define BOARD_SIZE 8
#define BROOK 2
#define BKNIGHT 3
#define BBISHOP 4
#define BQUEEN 5
#define BKING 6
#define BPAWN 1
#define WPAWN -1
#define WROOK -2
#define WKNIGHT -3
#define WBISHOP -4
#define WQUEEN -5
#define WKING -6

using namespace sf;

int whiteRightRook = 0, whiteLeftRook = 0, whiteKing = 0;
int blackRightRook = 0, blackLeftRook = 0, blackKing = 0;
int move_or_turn = 0; // 0: White's turn, 1: Black's turn
int white_check = 0, black_check = 0;
int transformWhite = 0, transformBlack = 0;

struct poz {
    int x, y;
} oldPos, TransformA, TransformN, Whiteking, Blackking;

// Khoảng cách giữa các quân cờ
int  size = 100; // Kích thước mỗi ô cờ
float pieceSpacing = 10; // Khoảng cách giữa các quân cờ theo chiều ngang/dọc (tính bằng pixel)

int move = 0, x, y;
int board[8][8] = {
    {BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK},
    {BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN},
    {WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK},
};

int wPawn(int ox, int oy, int nx, int ny) {
    if (oy == 6) {
        if ((ny == oy - 1 && nx == ox) && board[ny][nx] == 0 || (ny == oy - 2 && nx == ox) && (board[oy - 2][ox] == 0 && board[oy - 1][ox] == 0)) {
            return 1;
        }
    }
    else if ((ny == oy - 1 && nx == ox) && board[ny][nx] == 0) {
        return 1;
    }
    if (oy - 1 >= 0 && ox - 1 >= 0 && board[oy - 1][ox - 1] > 0) {
        if (ny == oy - 1 && nx == ox - 1) {
            return 1;
        }
    }
    if (oy - 1 >= 0 && ox + 1 < BOARD_SIZE && board[oy - 1][ox + 1] > 0) {
        if (ny == oy - 1 && nx == ox + 1) {
            return 1;
        }
    }
    return 0;
}

int bPawn(int ox, int oy, int nx, int ny) {
    if (oy == 1) {
        if ((ny == oy + 1 && nx == ox) && board[ny][nx] == 0 || (ny == oy + 2 && nx == ox) && (board[oy + 2][ox] == 0 && board[oy + 1][ox] == 0)) {
            return 1;
        }
    }
    else if ((ny == oy + 1 && nx == ox) && board[ny][nx] == 0) {
        return 1;
    }
    if (oy + 1 < BOARD_SIZE && ox - 1 >= 0 && board[oy + 1][ox - 1] < 0) {
        if (ny == oy + 1 && nx == ox - 1) {
            return 1;
        }
    }
    if (oy + 1 < BOARD_SIZE && ox + 1 < BOARD_SIZE && board[oy + 1][ox + 1] < 0) {
        if (ny == oy + 1 && nx == ox + 1) {
            return 1;
        }
    }
    return 0;
}

int wRook(int ox, int oy, int nx, int ny) {
    if (ox == nx) {
        int step = (ny > oy) ? 1 : -1;
        for (int i = oy + step; i != ny; i += step) {
            if (board[i][ox] != 0) return 0;
        }
        if (board[ny][nx] <= 0) return 1;
    }
    else if (oy == ny) {
        int step = (nx > ox) ? 1 : -1;
        for (int i = ox + step; i != nx; i += step) {
            if (board[oy][i] != 0) return 0;
        }
        if (board[ny][nx] <= 0) return 1;
    }
    return 0;
}

int bRook(int ox, int oy, int nx, int ny) {
    if (ox == nx) {
        int step = (ny > oy) ? 1 : -1;
        for (int i = oy + step; i != ny; i += step) {
            if (board[i][ox] != 0) return 0;
        }
        if (board[ny][nx] >= 0) return 1;
    }
    else if (oy == ny) {
        int step = (nx > ox) ? 1 : -1;
        for (int i = ox + step; i != nx; i += step) {
            if (board[oy][i] != 0) return 0;
        }
        if (board[ny][nx] >= 0) return 1;
    }
    return 0;
}

int wBishop(int ox, int oy, int nx, int ny) {
    if (abs(nx - ox) == abs(ny - oy)) {
        int stepX = (nx > ox) ? 1 : -1;
        int stepY = (ny > oy) ? 1 : -1;
        int j = ox + stepX;
        for (int i = oy + stepY; i != ny; i += stepY) {
            if (board[i][j] != 0) return 0;
            j += stepX;
        }
        if (board[ny][nx] <= 0) return 1;
    }
    return 0;
}

int bBishop(int ox, int oy, int nx, int ny) {
    if (abs(nx - ox) == abs(ny - oy)) {
        int stepX = (nx > ox) ? 1 : -1;
        int stepY = (ny > oy) ? 1 : -1;
        int j = ox + stepX;
        for (int i = oy + stepY; i != ny; i += stepY) {
            if (board[i][j] != 0) return 0;
            j += stepX;
        }
        if (board[ny][nx] >= 0) return 1;
    }
    return 0;
}

int wQueen(int ox, int oy, int nx, int ny) {
    return wRook(ox, oy, nx, ny) || wBishop(ox, oy, nx, ny);
}

int bQueen(int ox, int oy, int nx, int ny) {
    return bRook(ox, oy, nx, ny) || bBishop(ox, oy, nx, ny);
}

int wKnight(int ox, int oy, int nx, int ny) {
    if (((abs(nx - ox) == 2 && abs(ny - oy) == 1) || (abs(nx - ox) == 1 && abs(ny - oy) == 2)) && board[ny][nx] <= 0) {
        return 1;
    }
    return 0;
}

int bKnight(int ox, int oy, int nx, int ny) {
    if (((abs(nx - ox) == 2 && abs(ny - oy) == 1) || (abs(nx - ox) == 1 && abs(ny - oy) == 2)) && board[ny][nx] >= 0) {
        return 1;
    }
    return 0;
}

int PawnCheck(int posx, int posy, int regex, int regey) {
    if (move_or_turn == 0) { // Check for Black King
        if (posy + 1 < BOARD_SIZE && posx - 1 >= 0) {
            if (posy + 1 == regey && posx - 1 == regex) {
                return 1;
            }
        }
        if (posy + 1 < BOARD_SIZE && posx + 1 < BOARD_SIZE) {
            if (posy + 1 == regey && posx + 1 == regex) {
                return 1;
            }
        }
    }
    else { // Check for White King
        if (posy - 1 >= 0 && posx - 1 >= 0) {
            if (posy - 1 == regey && posx - 1 == regex) {
                return 1;
            }
        }
        if (posy - 1 >= 0 && posx + 1 < BOARD_SIZE) {
            if (posy - 1 == regey && posx + 1 == regex) {
                return 1;
            }
        }
    }

    return 0;
}

int RookCheck(int ox, int oy, int regex, int regey) {
    if (ox == regex) {
        int step = (regey > oy) ? 1 : -1;
        for (int i = oy + step; i != regey; i += step) {
            if (board[i][ox] != 0) return 0;
        }
        return 1;
    }
    else if (oy == regey) {
        int step = (regex > ox) ? 1 : -1;
        for (int i = ox + step; i != regex; i += step) {
            if (board[oy][i] != 0) return 0;
        }
        return 1;
    }
    return 0;
}

int BishopCheck(int ox, int oy, int regex, int regey) {
    if (abs(regex - ox) == abs(regey - oy)) {
        int stepX = (regex > ox) ? 1 : -1;
        int stepY = (regey > oy) ? 1 : -1;
        int j = ox + stepX;
        for (int i = oy + stepY; i != regey; i += stepY) {
            if (board[i][j] != 0) return 0;
            j += stepX;
        }
        return 1;
    }
    return 0;
}

int QueenCheck(int ox, int oy, int regex, int regey) {
    return RookCheck(ox, oy, regex, regey) || BishopCheck(ox, oy, regex, regey);
}

int KnightCheck(int ox, int oy, int regex, int regey) {
    if ((abs(regex - ox) == 2 && abs(regey - oy) == 1) || (abs(regex - ox) == 1 && abs(regey - oy) == 2)) {
        return 1;
    }
    return 0;
}

int KingCheck(int ox, int oy, int regex, int regey) {
    if (abs(ox - regex) <= 1 && abs(oy - regey) <= 1) return 1;
    return 0;
}

int BlackKingCheck(int posRegex, int posRegey) {
    int ok = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] < 0) {
                if (board[i][j] == WPAWN) {
                    ok = PawnCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == WROOK) {
                    ok = RookCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == WKNIGHT) {
                    ok = KnightCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == WBISHOP) {
                    ok = BishopCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == WQUEEN) {
                    ok = QueenCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == WKING) {
                    ok = KingCheck(j, i, posRegex, posRegey);
                }
                if (ok == 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int BlackKing(int ox, int oy, int nx, int ny) {
    if (abs(ox - nx) <= 1 && abs(oy - ny) <= 1 && board[ny][nx] >= 0)
    {
        int temp = board[ny][nx];
        board[ny][nx] = BKING;
        board[oy][ox] = 0;
        int ok = BlackKingCheck(nx, ny);
        board[ny][nx] = temp;
        board[oy][ox] = BKING;
        if (ok == 1) return 1;
        else return 0;
    }

    if (blackRightRook == 0 && blackKing == 0 && board[0][5] == 0 && board[0][6] == 0 && ny == 0 && nx == 6) {
        int ok = BlackKingCheck(4, 0);
        if (ok == 1) {
            ok = BlackKingCheck(5, 0);
            if (ok == 1) {
                ok = BlackKingCheck(6, 0);
                if (ok == 1) {
                    blackKing = 1;
                    blackRightRook = 1;
                    board[0][7] = 0;
                    board[0][5] = BROOK;
                    return 1;
                }
            }
        }
    }
    if (blackLeftRook == 0 && blackKing == 0 && board[0][3] == 0 && board[0][2] == 0 && board[0][1] == 0 && ny == 0 && nx == 2) {
        int ok = BlackKingCheck(4, 0);
        if (ok == 1) {
            ok = BlackKingCheck(3, 0);
            if (ok == 1) {
                ok = BlackKingCheck(2, 0);
                if (ok == 1) {
                    ok = BlackKingCheck(1, 0);
                    if (ok == 1) {
                        blackKing = 1;
                        blackLeftRook = 1;
                        board[0][0] = 0;
                        board[0][3] = BROOK;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int WhiteKingCheck(int posRegex, int posRegey) {
    int ok = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] > 0) {
                if (board[i][j] == BPAWN) {
                    ok = PawnCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == BROOK) {
                    ok = RookCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == BKNIGHT) {
                    ok = KnightCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == BBISHOP) {
                    ok = BishopCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == BQUEEN) {
                    ok = QueenCheck(j, i, posRegex, posRegey);
                }
                if (board[i][j] == BKING) {
                    ok = KingCheck(j, i, posRegex, posRegey);
                }
                if (ok == 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int WhiteKing(int ox, int oy, int nx, int ny) {
    if (abs(ox - nx) <= 1 && abs(oy - ny) <= 1 && board[ny][nx] <= 0)
    {
        int temp = board[ny][nx];
        board[ny][nx] = WKING;
        board[oy][ox] = 0;

        int ok = WhiteKingCheck(nx, ny);

        board[ny][nx] = temp;
        board[oy][ox] = WKING;

        if (ok == 1) return 1;
        else return 0;
    }
    // rocada in dreapta
    if (whiteKing == 0 && whiteRightRook == 0 && board[7][5] == 0 && board[7][6] == 0 && ny == 7 && nx == 6) {
        int ok = 1;
        ok = WhiteKingCheck(4, 7);
        if (ok == 1) {
            ok = WhiteKingCheck(5, 7);
            if (ok == 1) {
                ok = WhiteKingCheck(6, 7);
                if (ok == 1) {
                    board[7][7] = 0;
                    board[7][5] = WROOK;
                    whiteKing = 1;
                    whiteRightRook = 1;
                    return 1;
                }
            }
        }
    }
    // rocada in stanga
    if (whiteKing == 0 && whiteLeftRook == 0 && board[7][3] == 0 && board[7][2] == 0 && board[7][1] == 0 && ny == 7 && nx == 2) {
        int ok = 1;
        ok = WhiteKingCheck(4, 7);
        if (ok == 1) {
            ok = WhiteKingCheck(3, 7);
            if (ok == 1) {
                ok = WhiteKingCheck(2, 7);
                if (ok == 1) {
                    ok = WhiteKingCheck(1, 7);
                    if (ok == 1) {
                        board[7][0] = 0;
                        board[7][3] = WROOK;
                        whiteKing = 1;
                        whiteLeftRook = 1;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void pozwhiteKing() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == WKING) {
                Whiteking.x = j;
                Whiteking.y = i;
                break;
            }
        }
    }
}

void pozblackKing() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == BKING) {
                Blackking.y = i;
                Blackking.x = j;
                break;
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800 + BOARD_SIZE * pieceSpacing, 800 + BOARD_SIZE * pieceSpacing), "Chesssssssss!!!!");
    sf::Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;

    t1.loadFromFile("Pictures/8x8_brown_sand.png");
    t2.loadFromFile("Pictures/bP.png");
    t3.loadFromFile("Pictures/wP.png");
    t4.loadFromFile("Pictures/bR.png");
    t5.loadFromFile("Pictures/wR.png");
    t6.loadFromFile("Pictures/wN.png");
    t7.loadFromFile("Pictures/bN.png");
    t8.loadFromFile("Pictures/bB.png");
    t9.loadFromFile("Pictures/wB.png");
    t10.loadFromFile("Pictures/wQ.png");
    t11.loadFromFile("Pictures/bQ.png");
    t12.loadFromFile("Pictures/bK.png");
    t13.loadFromFile("Pictures/wK.png");
    t14.loadFromFile("Pictures/4quan.png");
    t15.loadFromFile("Pictures/4quanden.png");

    // Kích thước hình ảnh quân cờ (giả sử tất cả hình ảnh có cùng kích thước)
    sf::Vector2u textureSize = t2.getSize();
    float pieceScaleX = (size - pieceSpacing) / textureSize.x;
    float pieceScaleY = (size - pieceSpacing) / textureSize.y;

    Sprite boardSprite(t1);
    // Điều chỉnh kích thước bàn cờ để có khoảng cách giữa các ô
    boardSprite.setScale((size * BOARD_SIZE + pieceSpacing * (BOARD_SIZE - 1)) / t1.getSize().x, (size * BOARD_SIZE + pieceSpacing * (BOARD_SIZE - 1)) / t1.getSize().y);

    Sprite blackPawnSprite(t2);
    blackPawnSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite whitePawnSprite(t3);
    whitePawnSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite blackRookSprite(t4);
    blackRookSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite whiteRookSprite(t5);
    whiteRookSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite whiteKnightSprite(t6);
    whiteKnightSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite blackKnightSprite(t7);
    blackKnightSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite blackBishopSprite(t8);
    blackBishopSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite whiteBishopSprite(t9);
    whiteBishopSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite whiteQueenSprite(t10);
    whiteQueenSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite blackQueenSprite(t11);
    blackQueenSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite blackKingSprite(t12);
    blackKingSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite whiteKingSprite(t13);
    whiteKingSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite movementSprite;
    Sprite whiteTransformSprite(t14);
    whiteTransformSprite.setScale(pieceScaleX, pieceScaleY);
    Sprite blackTransformSprite(t15);
    blackTransformSprite.setScale(pieceScaleX, pieceScaleY);

    float dx = 0, dy = 0;
    int movedPieceNumber = 0;
    while (window.isOpen())
    {
        // Tính toán vị trí chuột dựa trên kích thước cửa sổ và khoảng cách
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        x = pos.x / (size + pieceSpacing);
        y = pos.y / (size + pieceSpacing);
        sf::Event e;

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            window.clear();

            if (e.type == sf::Event::MouseButtonPressed)
            {
                if (e.key.code == sf::Mouse::Left)
                {
                    if (transformWhite == 1)
                    {
                        if (pos.y >= TransformA.y * (size + pieceSpacing) && pos.y <= (TransformA.y + 1) * (size + pieceSpacing) &&
                            pos.x >= TransformA.x * (size + pieceSpacing) && pos.x <= (TransformA.x + 1) * (size + pieceSpacing))
                        {
                            int xx = (int)(pos.x - TransformA.x * (size + pieceSpacing)) % (int)(size + pieceSpacing);
                            int yy = (int)(pos.y - TransformA.y * (size + pieceSpacing)) % (int)(size + pieceSpacing);

                            if (xx < size && yy < size && xx > 0 && yy > 0) {
                                if (xx < size / 2 && yy < size / 2)
                                {
                                    board[TransformA.y][TransformA.x] = WROOK;
                                    transformWhite = 0;
                                }
                                if (xx > size / 2 && yy < size / 2)
                                {
                                    board[TransformA.y][TransformA.x] = WQUEEN;
                                    transformWhite = 0;
                                }
                                if (xx > size / 2 && yy > size / 2)
                                {
                                    board[TransformA.y][TransformA.x] = WKNIGHT;
                                    transformWhite = 0;
                                }
                                if (xx < size / 2 && yy > size / 2)
                                {
                                    board[TransformA.y][TransformA.x] = WBISHOP;
                                    transformWhite = 0;
                                }
                            }

                            if (transformWhite == 0)
                            {
                                pozwhiteKing();
                                int checkStatus = BlackKingCheck(Blackking.x, Blackking.y);
                                if (checkStatus == 0)
                                {
                                    black_check = 1;
                                }
                                move_or_turn = 1;
                            }
                        }
                    }
                    if (transformBlack == 1)
                    {
                        if (pos.y >= TransformN.y * (size + pieceSpacing) && pos.y <= (TransformN.y + 1) * (size + pieceSpacing) &&
                            pos.x >= TransformN.x * (size + pieceSpacing) && pos.x <= (TransformN.x + 1) * (size + pieceSpacing))
                        {
                            int xx = (int)(pos.x - TransformN.x * (size + pieceSpacing)) % (int)(size + pieceSpacing);
                            int yy = (int)(pos.y - TransformN.y * (size + pieceSpacing)) % (int)(size + pieceSpacing);
                            if (xx < size && yy < size && xx > 0 && yy > 0) {
                                if (xx < size / 2 && yy < size / 2)
                                {
                                    board[TransformN.y][TransformN.x] = BROOK;
                                    transformBlack = 0;
                                }
                                if (xx > size / 2 && yy < size / 2)
                                {
                                    board[TransformN.y][TransformN.x] = BQUEEN;
                                    transformBlack = 0;
                                }
                                if (xx > size / 2 && yy > size / 2)
                                {
                                    board[TransformN.y][TransformN.x] = BKNIGHT;
                                    transformBlack = 0;
                                }
                                if (xx < size / 2 && yy > size / 2)
                                {
                                    board[TransformN.y][TransformN.x] = BBISHOP;
                                    transformBlack = 0;
                                }
                            }

                            if (transformBlack == 0)
                            {
                                pozblackKing();
                                int checkStatus = WhiteKingCheck(Whiteking.x, Whiteking.y);
                                if (checkStatus == 0)
                                {
                                    white_check = 1;
                                }
                                move_or_turn = 0;
                            }
                        }
                    }
                    // Kiểm tra click vào quân cờ
                    if (board[y][x] != 0)
                    {
                        // Tính toán offset để di chuyển quân cờ
                        dx = pos.x - (x * (size + pieceSpacing) + pieceSpacing / 2);
                        dy = pos.y - (y * (size + pieceSpacing) + pieceSpacing / 2);


                        if (board[y][x] == BPAWN && move_or_turn == 1)
                        {
                            movedPieceNumber = BPAWN;
                            movementSprite = blackPawnSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == WPAWN && move_or_turn == 0)
                        {
                            movedPieceNumber = WPAWN;
                            movementSprite = whitePawnSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == BROOK && move_or_turn == 1)
                        {
                            movedPieceNumber = BROOK;
                            movementSprite = blackRookSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == WROOK && move_or_turn == 0)
                        {
                            movedPieceNumber = WROOK;
                            movementSprite = whiteRookSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == WKNIGHT && move_or_turn == 0)
                        {
                            movedPieceNumber = WKNIGHT;
                            movementSprite = whiteKnightSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == BKNIGHT && move_or_turn == 1)
                        {
                            movedPieceNumber = BKNIGHT;
                            movementSprite = blackKnightSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == BBISHOP && move_or_turn == 1)
                        {
                            movedPieceNumber = BBISHOP;
                            movementSprite = blackBishopSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == WBISHOP && move_or_turn == 0)
                        {
                            movedPieceNumber = WBISHOP;
                            movementSprite = whiteBishopSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == WQUEEN && move_or_turn == 0)
                        {
                            movedPieceNumber = WQUEEN;
                            movementSprite = whiteQueenSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == BQUEEN && move_or_turn == 1)
                        {
                            movedPieceNumber = BQUEEN;
                            movementSprite = blackQueenSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == BKING && move_or_turn == 1)
                        {
                            movedPieceNumber = BKING;
                            movementSprite = blackKingSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] == WKING && move_or_turn == 0)
                        {
                            movedPieceNumber = WKING;
                            movementSprite = whiteKingSprite;
                            // board[y][x] = 0;
                        }
                        if (board[y][x] != 0)
                        {
                            move = 1;
                            oldPos.x = x;
                            oldPos.y = y;
                        }
                    }
                }
            }
            if (e.type == Event::MouseButtonReleased)
            {
                if (e.key.code == Mouse::Left)
                {
                    int ok = 2;
                    if (movedPieceNumber == WPAWN && move == 1)
                    {
                        ok = wPawn(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == BPAWN && move == 1)
                    {
                        ok = bPawn(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == WROOK && move == 1)
                    {
                        ok = wRook(oldPos.x, oldPos.y, x, y);
                        if (ok == 1 && whiteLeftRook == 0 && oldPos.y == 7 && oldPos.x == 0)
                        {
                            whiteLeftRook = 1;
                        }
                        if (ok == 1 && whiteRightRook == 0 && oldPos.y == 7 && oldPos.x == 7)
                        {
                            whiteRightRook = 1;
                        }
                    }
                    if (movedPieceNumber == BROOK && move == 1)
                    {
                        ok = bRook(oldPos.x, oldPos.y, x, y);
                        if (ok == 1 && blackRightRook == 0 && oldPos.y == 0 && oldPos.x == 7)
                        {
                            blackRightRook = 1;
                        }
                        if (ok == 1 && blackLeftRook == 0 && oldPos.y == 0 && oldPos.x == 0)
                        {
                            blackLeftRook = 1;
                        }
                    }
                    if (movedPieceNumber == WBISHOP && move == 1)
                    {
                        ok = wBishop(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == BBISHOP && move == 1)
                    {
                        ok = bBishop(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == WQUEEN && move == 1)
                    {
                        ok = wQueen(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == BQUEEN && move == 1)
                    {
                        ok = bQueen(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == WKNIGHT && move == 1)
                    {
                        ok = wKnight(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == BKNIGHT && move == 1)
                    {
                        ok = bKnight(oldPos.x, oldPos.y, x, y);
                    }
                    if (movedPieceNumber == BKING && move == 1)
                    {
                        ok = BlackKing(oldPos.x, oldPos.y, x, y);
                        if (ok == 1 && blackKing == 0)
                        {
                            blackKing = 1;
                        }
                    }
                    if (movedPieceNumber == WKING && move == 1)
                    {
                        ok = WhiteKing(oldPos.x, oldPos.y, x, y);
                        if (ok == 1 && whiteKing == 0)
                        {
                            whiteKing = 1;
                        }
                    }

                    if (ok == 1)
                    {

                        board[y][x] = movedPieceNumber;
                        board[oldPos.y][oldPos.x] = 0;

                        if (y == 0 && movedPieceNumber == WPAWN)
                        {
                            transformWhite = 1;
                            TransformA.x = x;
                            TransformA.y = y;
                            board[y][x] = 0;
                        }
                        if (y == 7 && movedPieceNumber == BPAWN)
                        {
                            transformBlack = 1;
                            TransformN.x = x;
                            TransformN.y = y;
                            board[y][x] = 0;
                        }

                        if (move_or_turn == 0) // White moved, Black's turn
                        {

                            pozblackKing();
                            int blackCheckStatus = BlackKingCheck(Blackking.x, Blackking.y);
                            if (blackCheckStatus == 0)
                            {
                                black_check = 1;
                            }
                            else black_check = 0;



                        }
                        else // Black moved, White's turn
                        {

                            pozwhiteKing();
                            int whiteCheckStatus = WhiteKingCheck(Whiteking.x, Whiteking.y);
                            if (whiteCheckStatus == 0)
                            {
                                white_check = 1;
                            }
                            else white_check = 0;



                        }
                        if (transformBlack == 0 && transformWhite == 0)
                        {
                            if (move_or_turn == 0) move_or_turn = 1;
                            else move_or_turn = 0;
                        }

                    }

                    move = 0;
                }
            }
        }
        // Display
        window.clear();
        window.draw(boardSprite);

        if (transformWhite == 1)
        {
            whiteTransformSprite.setPosition(TransformA.x * (size + pieceSpacing) + pieceSpacing / 2, TransformA.y * (size + pieceSpacing) + pieceSpacing / 2);
            window.draw(whiteTransformSprite);
        }

        if (transformBlack == 1)
        {
            blackTransformSprite.setPosition(TransformN.x * (size + pieceSpacing) + pieceSpacing / 2, TransformN.y * (size + pieceSpacing) + pieceSpacing / 2);
            window.draw(blackTransformSprite);
        }

        if (move == 1)
        {
            // Di chuyển quân cờ đến vị trí chuột (có tính khoảng cách)
            movementSprite.setPosition(pos.x - dx, pos.y - dy);
            window.draw(movementSprite);
        }

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] != 0)
                {
                    // Tính toán vị trí vẽ quân cờ có tính khoảng cách
                    float pieceX = j * (size + pieceSpacing) + pieceSpacing / 2;
                    float pieceY = i * (size + pieceSpacing) + pieceSpacing / 2;

                    if (board[i][j] == BPAWN)
                    {
                        blackPawnSprite.setPosition(pieceX, pieceY);
                        window.draw(blackPawnSprite);
                    }
                    if (board[i][j] == WPAWN)
                    {
                        whitePawnSprite.setPosition(pieceX, pieceY);
                        window.draw(whitePawnSprite);
                    }
                    if (board[i][j] == BROOK)
                    {
                        blackRookSprite.setPosition(pieceX, pieceY);
                        window.draw(blackRookSprite);
                    }
                    if (board[i][j] == WROOK)
                    {
                        whiteRookSprite.setPosition(pieceX, pieceY);
                        window.draw(whiteRookSprite);
                    }
                    if (board[i][j] == WKNIGHT)
                    {
                        whiteKnightSprite.setPosition(pieceX, pieceY);
                        window.draw(whiteKnightSprite);
                    }
                    if (board[i][j] == BKNIGHT)
                    {
                        blackKnightSprite.setPosition(pieceX, pieceY);
                        window.draw(blackKnightSprite);
                    }
                    if (board[i][j] == BBISHOP)
                    {
                        blackBishopSprite.setPosition(pieceX, pieceY);
                        window.draw(blackBishopSprite);
                    }
                    if (board[i][j] == WBISHOP)
                    {
                        whiteBishopSprite.setPosition(pieceX, pieceY);
                        window.draw(whiteBishopSprite);
                    }
                    if (board[i][j] == WQUEEN)
                    {
                        whiteQueenSprite.setPosition(pieceX, pieceY);
                        window.draw(whiteQueenSprite);
                    }
                    if (board[i][j] == BQUEEN)
                    {
                        blackQueenSprite.setPosition(pieceX, pieceY);
                        window.draw(blackQueenSprite);
                    }
                    if (board[i][j] == BKING)
                    {
                        blackKingSprite.setPosition(pieceX, pieceY);
                        window.draw(blackKingSprite);
                    }
                    if (board[i][j] == WKING)
                    {
                        whiteKingSprite.setPosition(pieceX, pieceY);
                        window.draw(whiteKingSprite);
                    }
                }
            }
        }
        window.display();
    }
    return 0;
}