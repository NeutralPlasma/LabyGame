#ifndef __CELL_H_
#define __CELL_H_

#include <string>
#include <iostream>

class Cell {
private:
    int row;
    int col;
    bool visit;
    int walls;
    void init(const int r, const int c, const int walls, const bool v = false);
public:
    enum WALL {
        WALL_NORTH = 0x0008, WALL_EAST = 0x0004,
        WALL_SOUTH = 0x0002, WALL_WEST = 0x0001,
        WALL_ALL = 0x000f, WALL_NONE = 0x0000
    };
    Cell();
    Cell(const int r, const int c);
    Cell(const int r, const int c, const int stat);
    bool visited() const;
    void setVisited(const bool v = true);
    int getRow() const;
    int getColumn() const;
    void removeWall(const int w);
    int getWalls() const;
    void setWalls(const int w);
    void setPosition(const int r, const int c);
    friend std::ostream& operator<<(std::ostream& strm, const Cell& c);
};

#endif 