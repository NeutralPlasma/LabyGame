#include "Cell.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>



void Cell::init(const int r, const int c, const int walls, const bool v) {
    setPosition(r, c);
    setWalls(walls);
    setVisited(v);
}
Cell::Cell() { init(0, 0, 0); }
Cell::Cell(const int r, const int c) { init(r, c, 0); }
Cell::Cell(const int r, const int c, const int walls) { init(r, c, walls); }
bool Cell::visited() const { return visit; }
void Cell::setVisited(const bool v) { visit = v; }
int Cell::getRow() const { return row; }
int Cell::getColumn() const { return col; }
void Cell::removeWall(const int w) {
    if (w != WALL_NORTH && w != WALL_EAST && w != WALL_SOUTH && w != WALL_WEST)
        throw std::string("Illegal wall argument");
    walls &= ~w;
}
int Cell::getWalls() const { return walls & WALL_ALL; }
void Cell::setWalls(const int w) { walls = w & WALL_ALL; }
void Cell::setPosition(const int r, const int c) { row = r; col = c; }


std::ostream& operator<<(std::ostream& strm, const Cell& c) {
    if ((c.getWalls() & Cell::WALL_WEST) != 0) strm << '|';
    else strm << ' ';
    if ((c.getWalls() & Cell::WALL_SOUTH) != 0) strm << '_';
    else strm << ' ';

    return strm;
}