#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

const char ACTIVE_CUBE = '#';
const char INACTIVE_CUBE = '.';

using space_t = std::deque<std::deque<std::deque<char>>>;
using space4d_t = std::deque<space_t>;
namespace Space {
    space_t SPACE{{}};    
    size_t XSIZE() { return SPACE.back().back().size();}
    size_t YSIZE() { return SPACE.back().size();}
    size_t ZSIZE() { return SPACE.size();}
    int countActiveCubes();
    int countActiveNeighbours(ssize_t z, ssize_t y, ssize_t x);

    void expandSpace();
    void printSpace();
}

namespace Space4d {
    space4d_t SPACE{{{}}};
    size_t XSIZE() { return SPACE.back().back().back().size();}
    size_t YSIZE() { return SPACE.back().back().size();}
    size_t ZSIZE() { return SPACE.back().size();}
    size_t WSIZE() { return SPACE.size(); }
    int countActiveCubes();
    int countActiveNeighbours(ssize_t w, ssize_t z, ssize_t y, ssize_t x);
    void expandSpace();
    void printSpace();
}


namespace Space {
int countActiveCubes() {
    int count = 0;
    for (auto x : SPACE ) {
        for (auto y : x) {
            for (auto z : y ) {
                if (z == ACTIVE_CUBE) {
                    count++;
                }
            }
        }
    }
    return count;
}
}

namespace Space4d {
int countActiveCubes() {
    int count = 0;
    for (auto x : SPACE ) {
        for (auto y : x) {
            for (auto z : y ) {
                for (auto w: z) {
                    if (w == ACTIVE_CUBE) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}
}

namespace Space {
void expandSpace() {
    // Add columns
    for (auto &z : SPACE) {
        for (auto &y : z) {
            y.push_back(INACTIVE_CUBE);
            y.push_front(INACTIVE_CUBE);
        }
    }
    // Add rows
    std::deque<char> row;
    for (size_t y = 0; y < SPACE.back().back().size(); y++) {
        row.push_back(INACTIVE_CUBE);
    }
    for (auto &z : SPACE) {
        z.push_back(row);
        z.push_front(row);
    }
    // Add layer
    std::deque<std::deque<char>> layer;
    for (size_t z = 0; z < SPACE.back().size(); z++) {
        layer.push_back(row);
    }
    SPACE.push_back(layer);
    SPACE.push_front(layer);
}
}

namespace Space4d {
void expandSpace() {
    // add min
    for (auto &w : SPACE) {
        for (auto &z : w) {
            for (auto &y : z) {
                y.push_back(INACTIVE_CUBE);
                y.push_front(INACTIVE_CUBE);
            }
        }
    }
    // Add 
    std::deque<char> row;
    for (size_t y = 0; y < SPACE.back().back().back().size(); y++) {
        row.push_back(INACTIVE_CUBE);
    }
    for (auto &w : SPACE) {
        for (auto &z : w) {
            z.push_back(row);
            z.push_front(row);
        }
    }
    std::deque<std::deque<char>> layer;
    for (size_t z = 0; z < SPACE.back().back().size(); z++) {
        layer.push_back(row);
    }
    for (auto &w: SPACE) {
        w.push_back(layer);
        w.push_front(layer);
    }
    space_t cube;
    for (size_t w = 0; w < SPACE.back().size(); w++) {
        cube.push_back(layer);
    }
    SPACE.push_back(cube);
    SPACE.push_front(cube);
}
}


namespace Space {
void printSpace() {
    for (auto z : SPACE) {
        std::cout << "Layer: " << std::endl;
        for (auto y : z) {
            for (auto x : y) {
                std::cout << x;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
}

namespace Space4d {
void printSpace() {
    for (auto w : SPACE) {
        std::cout << "CUBE: " << std::endl;
        for (auto z : w) {
        std::cout << "LAYER: " << std::endl;
            for (auto y : z) {
                for (auto x : y) {
                    std::cout << x;
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

}

namespace Space {
int countActiveNeighbours(ssize_t z, ssize_t y, ssize_t x) {
    int count = 0;
    std::vector<ssize_t> adj = {-1,0,1};
    for (auto _z : adj) {
        for (auto _y : adj) {
            for (auto _x : adj) {
                if (
                    (x+_x < 0) || (x+_x >= XSIZE()) ||
                    (y+_y < 0) || (y+_y >= YSIZE()) ||
                    (z+_z < 0) || (z+_z >= ZSIZE())
                ) {
                    continue;
                }
                if ((_x == 0) && (_y == 0) && (_z == 0)) {
                    continue;
                }
                if (SPACE[z+_z][y+_y][x+_x] == ACTIVE_CUBE) {
                    count++;
                }
            }
        }
    }
    return count;
}
}

namespace Space4d {
int countActiveNeighbours(ssize_t w, ssize_t z, ssize_t y, ssize_t x) {
    int count = 0;
    std::vector<ssize_t> adj = {-1,0,1};
    for (auto _w : adj) {
        for (auto _z : adj) {
            for (auto _y : adj) {
                for (auto _x : adj) {
                    if (
                        (x+_x < 0) || (x+_x >= XSIZE()) ||
                        (y+_y < 0) || (y+_y >= YSIZE()) ||
                        (z+_z < 0) || (z+_z >= ZSIZE()) ||
                        (w+_w < 0) || (w+_w >= WSIZE())
                    ) {
                        continue;
                    }
                    if ((_w == 0) && (_x == 0) && (_y == 0) && (_z == 0)) {
                        continue;
                    }
                    if (SPACE[w+_w][z+_z][y+_y][x+_x] == ACTIVE_CUBE) {
                        count++;
                    }
                }
            }
        }
    }
    return count;

}
}
namespace Space {
enum DIM {
    eX, eY, eZ
};

enum END {
    eFRONT, eBACK
};
}

namespace Space4d {
enum DIM {
    eX, eY, eZ, eW
};
enum END {
    eFRONT, eBACK
};
}

namespace Space {
std::vector<std::vector<char>> getLayer(DIM dim, END end) {
    std::vector<std::vector<char>> layer;
    size_t d0, d1 = 0, d2 = 0;
    size_t l1, l2;
    if (dim == eZ) {
        d0 = ( end == eFRONT ? 0 : SPACE.size()-1);
        l1 = SPACE.back().size();
        l2 = SPACE.back().back().size();
    }
    if (dim == eY) {
        d0 = ( end == eFRONT ? 0 : SPACE.back().size()-1);
        l1 = SPACE.size();
        l2 = SPACE.back().back().size();
    }
    if (dim == eX) {
        d0 = ( end == eFRONT ? 0 : SPACE.back().back().size()-1);
        l1 = SPACE.size();
        l2 = SPACE.back().size();
    }
    for (d1 = 0; d1 < l1; d1++) {
        std::vector<char> row;
        for (d2=0; d2 < l2; d2++) {
            if (dim == eZ) {
                row.push_back(SPACE[d0][d1][d2]);
            }
            if (dim == eY) {
                row.push_back(SPACE[d1][d0][d2]);
            }
            if (dim == eX) {
                row.push_back(SPACE[d1][d2][d0]);
            }
        }
        layer.push_back(row);
    }
    return layer;
}
}

namespace Space4d {

space_t getLayer(DIM dim, END end) {
    space_t cube;
    size_t d0, d1, d2, d3;
    size_t l1, l2, l3;

    if (dim == eW) {
        d0 = ( end == eFRONT ? 0 : SPACE.size() - 1);
        l1 = SPACE.back().size();
        l2 = SPACE.back().back().size();
        l3 = SPACE.back().back().back().size();
    }
    if (dim == eZ) {
        d0 = ( end == eFRONT ? 0 : SPACE.back().size()-1);
        l1 = SPACE.size();
        l2 = SPACE.back().back().size();
        l3 = SPACE.back().back().back().size();
    }
    if (dim == eY) {
        d0 = ( end == eFRONT ? 0 : SPACE.back().back().size()-1);
        l1 = SPACE.size();
        l2 = SPACE.back().size();
        l3 = SPACE.back().back().back().size();
    }
    if (dim == eX) {
        d0 = ( end == eFRONT ? 0 : SPACE.back().back().back().size()-1);
        l1 = SPACE.size();
        l2 = SPACE.back().size();
        l3 = SPACE.back().back().size();
    }
    for (d1 = 0; d1 < l1; d1++) {
        std::deque<std::deque<char>> layer;
        for (d2=0; d2 < l2; d2++) {
        std::deque<char> row;
            for (d3 = 0; d3 < l3; d3++) {
                if (dim == eW) {
                    row.push_back(SPACE[d0][d1][d2][d3]);
                }
                if (dim == eZ) {
                    row.push_back(SPACE[d1][d0][d2][d3]);
                }
                if (dim == eY) {
                    row.push_back(SPACE[d1][d2][d0][d3]);
                }
                if (dim == eX) {
                    row.push_back(SPACE[d1][d2][d3][d0]);
                }
            }
            layer.push_back(row);
        }
        cube.push_back(layer);
    }
    return cube;

}


void contractSpace() {
    space4d_t newspace = SPACE;
    auto xs = XSIZE();
    auto ys = YSIZE();
    auto zs = ZSIZE();
    auto ws = WSIZE();
    bool empty = true;
    std::vector<std::pair<DIM, END>> l = {
        {eX, eBACK},{eX, eFRONT},
        {eY, eBACK},{eY, eFRONT},
        {eZ, eBACK},{eZ, eFRONT},
        {eW, eBACK},{eW, eFRONT}
    };
    do {
        SPACE = newspace;
        for (auto [d,e] : l) {
            bool empty = true;
            auto cube = getLayer(d,e);
            for (auto layer:cube) {
                for (auto outer:layer) {
                    for (auto inner:outer) {
                        if (inner == ACTIVE_CUBE) {
                            empty = false;
                            break;
                        }
                    }
                    if (!empty) {
                        break;
                    }
                }
                if (!empty) {
                    break;
                }
            }
            if (!empty) {
                continue;
            }
            // Remove this cuuube.
            if (d == eW) {
                if (e == eFRONT) {
                    newspace.pop_front();
                }
                else {
                    newspace.pop_back();
                }
            }
            if (d == eZ) {
                for (auto &cube : newspace) {
                    if (e == eFRONT) {
                        cube.pop_front();
                    }
                    else {
                        cube.pop_back();
                    }
                }
            }
            if (d == eY) {
                for (auto &cube : newspace) {
                    for (auto &layer : cube) {
                        if (e == eFRONT) {
                            layer.pop_front();
                        }
                        else {
                            layer.pop_back();
                        }
                    }
                }
            }
            if (d == eX) {
                for (auto &cube : newspace) {
                    for (auto &layer : cube) {
                        for (auto &line : layer) {
                            if (e == eFRONT) {
                                line.pop_front();
                            }
                            else {
                                line.pop_back();
                            }
                        }
                    }
                }
            }
        }
    } while(newspace != SPACE);
}
}

namespace Space {
void contractSpace() {
    space_t newspace = SPACE;
    // find each face.
    //auto zlayer = SPACE.front();
    auto xs = XSIZE();
    auto ys = YSIZE();
    auto zs = ZSIZE();
    bool empty = true;
    std::vector<std::pair<DIM, END>> l = {
        {eX, eBACK},{eX, eFRONT},
        {eY, eBACK},{eY, eFRONT},
        {eZ, eBACK},{eZ, eFRONT}
    };
    do {
        SPACE = newspace;
        for (auto [d,e] : l) {
            bool empty = true;
            auto layer = getLayer(d,e);
            for (auto outer:layer) {
                for (auto inner:outer) {
                    if (inner == ACTIVE_CUBE) {
                        empty = false;
                        break;
                    }
                }
                if (!empty) {
                    break;
                }
            }
            if (!empty) {
                continue;
            }
            // remove this layer
            if (d == eZ) {
                if (e == eFRONT) {
                    newspace.pop_front();
                }
                else {
                    newspace.pop_back();
                }
            }
            if (d == eY) {
                for (auto &layer : newspace) {
                    if (e == eFRONT) {
                        layer.pop_front();
                    }
                    else {
                        layer.pop_back();
                    }
                }
            }
            if (d == eX) {
                for (auto &layer : newspace) {
                    for (auto &thingy : layer) {
                        if (e == eFRONT) {
                            thingy.pop_front();
                        }
                        else {
                            thingy.pop_back();
                        }
                    }
                }
            }
        }
    } while (newspace != SPACE);
}
}

namespace Space4d {
void applyRules() {
    space4d_t newspace = SPACE;

    for (ssize_t w = 0; w < SPACE.size(); w++) {
        for (ssize_t z = 0; z < SPACE.back().size(); z++) {
            for (ssize_t y = 0; y < SPACE.back().back().size(); y++) {
                for (ssize_t x = 0; x < SPACE.back().back().back().size(); x++) {
                    auto cube = SPACE[w][z][y][x];
                    auto nbs = countActiveNeighbours(w,z,y,x);
                    if (cube == ACTIVE_CUBE) {
                        if ((nbs == 2) || (nbs == 3)) {
                            newspace[w][z][y][x] = ACTIVE_CUBE;
                        }
                        else {
                            newspace[w][z][y][x] = INACTIVE_CUBE;
                        }
                    } else {
                        if (nbs == 3) {
                            newspace[w][z][y][x] = ACTIVE_CUBE;
                        }
                        else {
                            newspace[w][z][y][x] = INACTIVE_CUBE;
                        }
                    }
                }
            }
        }
    }
    SPACE = newspace;
}
}

// Apply rules to global space;
namespace Space {
void applyRules() {
    space_t newspace = SPACE;
    for (ssize_t z = 0; z < SPACE.size(); z++) {
        for (ssize_t y = 0; y < SPACE.back().size(); y++) {
            for (ssize_t x = 0; x < SPACE.back().back().size(); x++) {
                auto cube = SPACE[z][y][x];
                // neighbours 
                //std::cout << z << "x" << y << "x" << x << std::endl;
                auto nbs = countActiveNeighbours(z,y,x);
                //std::cout << "nbs: " << nbs << std::endl;
                if (cube == ACTIVE_CUBE) {
                    if ((nbs == 2) || (nbs == 3)) {
                        newspace[z][y][x] = ACTIVE_CUBE;
                    }
                    else {
                        newspace[z][y][x] = INACTIVE_CUBE;
                    }
                } else {
                    if (nbs == 3) {
                        newspace[z][y][x] = ACTIVE_CUBE;
                    }
                    else {
                        newspace[z][y][x] = INACTIVE_CUBE;
                    }
                }
            }
        }
    }
    SPACE = newspace;
}
}

int main(int argc, char** argv) {
    // Hello :)
    std::string inFilePath("input.txt");
    if (argc == 2)
    {
        inFilePath = argv[1];
    }
    else if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " [input file (default ./input.txt)]" << std::endl;
        return 1;
    }

    std::cout << "Populating from \"" << inFilePath << "\"" << std::endl;
    std::ifstream inFile(inFilePath);
    std::vector<std::string> lines;
    std::string s;
    while (std::getline(inFile, s)) {
        lines.push_back(s);
    }

    for (auto &l : lines) {
        std::deque<char> cubes;
        for (auto &c : l) {
            cubes.push_back(c);
        }
        Space::SPACE.back().push_back(cubes);
        Space4d::SPACE.back().back().push_back(cubes);
    }

    Space::printSpace();
    for (int count = 0; count < 6; count++) {
        std::cout << "Iteration " << count << ": " << std::endl; 
        Space::expandSpace();
        Space::applyRules();
        Space::contractSpace();
        Space::printSpace();
    }

    auto c = Space::countActiveCubes();
    std::cout << "COUNT: " << c << std::endl;

    std::cout << "========================================+" << std::endl;
    std::cout << "Part 2:" << std::endl;

    

    for (int count = 0; count < 6; count++) {
        std::cout << "Iteration " << count << ": " << std::endl; 
        Space4d::expandSpace();
        Space4d::applyRules();
        //Space4d::contractSpace();
        Space4d::printSpace();
    }
    c = Space4d::countActiveCubes();
    std::cout << "COUNT: " << c << std::endl;
}
