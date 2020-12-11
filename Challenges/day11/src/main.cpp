#include <iostream>
#include <vector>
#include <fstream>

// Forward declaration of Area
class Space;

class Area {
    friend class Space;

public:
    Area() : m_x(0), m_y(0) {};
    Area(size_t x, size_t y) : m_x(x), m_y(y), m_fixed(true) {};

    void addSpacesFromLine(std::string line);
    void printArea(void);

    std::vector<Space> &operator[](size_t x) {
        return m_spaces[x];
    }

    bool operator==(Area &x); 

protected:
    bool m_fixed = false;
    size_t m_x, m_y;
    std::vector<std::vector<Space>> m_spaces;


};


class Space {
    friend class Area;

    constexpr static char sFLOOR = '.';
    constexpr static char sSEMPTY = 'L';
    constexpr static char sSOCC = '#';

public:
    Space(unsigned char c, size_t x, size_t y, Area *area) :
        m_c(c), m_x(x), m_y(y), m_area(area) {
        switch(c) {
            case sFLOOR:
                m_s = eFLOOR;
                break;
            case sSEMPTY:
                m_s = eSEMPTY;
                break;
            case sSOCC:
                m_s = eSOCC;
                break;
            default:
                m_s = ERR;
        }
    }

    bool operator==(Space &x) {
        return (m_s == x.m_s);
    }

    bool operator!=(Space &x) {
        return !(operator==(x));
    }

    unsigned char getChar(void) {return m_c;}

    std::vector<Space*> getAdjacents(void) {
        
    }
    enum State { ERR, eFLOOR, eSEMPTY, eSOCC } ;

    State m_s;
    unsigned char m_c;

private:
    size_t m_x;
    size_t m_y;
    Area *m_area;
    std::vector<Space*> m_adjacents;
};

bool Area::operator==(Area &x) {
        signed int ix = m_x - 1;
        signed int iy = m_y - 1;
        if (m_x != x.m_x) {
            return false;
        }
        if  (m_y != x.m_y) {
            return false;
        }

        for (auto iyy = iy; iyy >= 0; --iyy) {
            for (auto ixx = ix; ixx >= 0; --ixx) {
                if ((*this)[iyy][ixx] != x[iyy][ixx]) {
                    return false;
                }

            }

        }

        return true;
    }

void Area::addSpacesFromLine(std::string line) {
    auto ysz = m_spaces.size();
    auto xsz = line.size();
    std::vector<Space> row;
    if (ysz >= m_y) {
        if (m_fixed) {
            std::cerr << "Too many rows for fixed area" << std::endl;
            return;
        }
        else {
            m_y++;
        }
    }
    for (auto i = 0; i < xsz; i++) {
        if (i >= m_x) {
            if (m_fixed) {
                std::cerr << "Too many columns for fixed area" << std::endl;
                break;
            }
            else {
                m_x++;
            }
        }
        row.emplace_back(line.c_str()[i], i, ysz, this);
    }
    m_spaces.push_back(row);
}

void Area::printArea(void) {
    for (auto i = 0; i < m_x; i++) {
        std::cout << "=";
    }
    std::cout << std::endl;

    for (auto &outer : m_spaces) {
        for (auto &inner : outer) {
            std::cout << inner.getChar();
        }
        std::cout << std::endl;
    }
    for (auto i = 0; i < m_x; i++) {
        std::cout << "=";
    }
    std::cout << std::endl << std::endl;
}

int main(int argc, char** argv) {
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
        if (s.front() != '#')
            lines.push_back(s);
    }

    size_t gridWidth = lines.front().size();
    size_t gridHeight = lines.size();

    Area area;
    Area area3(gridWidth, gridHeight);
    Area area4(gridWidth, gridHeight);
    Area area2(gridWidth - 1, gridHeight - 1);

    for (auto &line : lines) {
        area.addSpacesFromLine(line);
        area2.addSpacesFromLine(line);
        area3.addSpacesFromLine(line);
        area4.addSpacesFromLine(line);
        //area.printArea();
    }

    std::cout << "1" << std::endl;
    area.printArea();
    std::cout << "2" << std::endl;
    area2.printArea();
    std::cout << "3" << std::endl;
    area3.printArea();

    area4[0][0] = Space('.', 0, 0, &area);
    std::cout << "4" << std::endl;
    area4.printArea();

    if (area == area2) {
        std::cout << "Equality found" << std::endl;
    }
    else {
        std::cout << "not equal!" << std::endl;
    }

    if (area3 == area) {
        std::cout << "1 and 3 are equal" << std::endl;
    }

    if (!(area4 == area3)) {
        std::cout << "3 and 4 are unequal!" << std::endl;
    }

    return 0;

}
