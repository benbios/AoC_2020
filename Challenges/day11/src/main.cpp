#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

// Forward declaration of Area
class Space;

class Area : public std::enable_shared_from_this<Area> {
    friend class Space;

public:
    Area() : m_x(0), m_y(0) {};
    Area(const Area &area) : m_y(area.m_y), m_x(area.m_x), m_fixed(area.m_fixed), m_spaces(area.m_spaces) {
        std::cout << "Copy constructor called!" << std::endl;
    };
    Area(size_t x, size_t y) : m_x(x), m_y(y), m_fixed(true) {};

    void addSpacesFromLine(std::string line);
    void printArea(void);

    void calculateAdjacents(void);
    void calculateIndirectAdjacents(void);
    void calculateRules(int maxOcc);
    void applyRules(void);

    size_t countOccupied(void);

    std::vector<Space> &operator[](size_t x) {
        return m_spaces[x];
    }

    std::shared_ptr<Space> operator()(signed int x, signed int y) {
        if ((x >= m_x) || (y >= m_y) || (x < 0) || (y < 0)) {
            return std::shared_ptr<Space>{nullptr};
        }
        return (std::make_shared<Space>((*this)[y][x]));
    }

    bool operator==(Area &x); 

    Area& operator= (const Area &area) {
        m_x = area.m_x;
        m_y = area.m_y;
        m_fixed = area.m_fixed;
        m_spaces.clear();
        std::copy(area.m_spaces.begin(), area.m_spaces.end(), std::back_inserter(m_spaces));
        return *this;
    }

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
    Space(unsigned char c, size_t x, size_t y, std::shared_ptr<Area> area) :
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

    void setArea(std::shared_ptr<Area> area) {
        m_area = area;
    }

    void getAdjacents(void) {
        m_adjacents.clear();
        signed int _x = int(m_x);
        signed int _y = int(m_y);

        std::vector<std::pair<signed int, signed int>> coords{
            {m_x - 1, m_y -1},{m_x, m_y - 1},{m_x + 1, m_y - 1},
            {m_x - 1, m_y},                  {m_x + 1, m_y},
            {m_x - 1, m_y +1},{m_x, m_y + 1},{m_x + 1, m_y + 1}
        };

        for (auto const &c : coords) {
            auto adj = m_area->operator()(c.first, c.second);
            if (adj != nullptr) {
                if (adj->m_s != eFLOOR) {
                    m_adjacents.push_back(adj);
                }
            }
        }
    }

    void getIndirectAdjacents(void) {
        m_adjacents.clear();
        signed int _x = int(m_x);
        signed int _y = int(m_y);

        // Find first non-floor seat in all directions.
        
        std::vector<std::pair<signed int, signed int>> directions{
            // Straight up 
            {0, -1},
            // UP right
            {1, -1},
            // Right
            {1, 0},
            // DOWN right
            {1, 1},
            // Straight down
            {0, 1},
            // DOWN left
            {-1, 1},
            // Left
            {-1, 0},
            // UP left
            {-1, -1}
        };

        for (auto d : directions) {
            _x = int(m_x) + d.first;
            _y = int(m_y) + d.second;
            std::shared_ptr<Space> s;
            while ((s = (*m_area)(_x,_y)) != nullptr) {
                if (s->m_s != eFLOOR) {
                    m_adjacents.push_back(s);
                    break;
                }
                _x += d.first;
                _y += d.second;
            }
        }

    }

    void calculateRules(int maxOcc) {
        m_sNext = m_s;
        if (m_s == eFLOOR) {
            return;
        }
        m_adjFree = 0;
        m_adjOccupied = 0;
        if (m_adjacents.size() == 0) {
            getAdjacents();
            if (m_adjacents.size() == 0) {
                std::cerr << "no adjacents" << std::endl;
                return;
            }
        }
        
        for (std::shared_ptr<Space> x : m_adjacents) {
            if (x->m_s == eSEMPTY) {
                m_adjFree++;
            }
            else {
                m_adjOccupied++;
            }
        }

        if (m_s == eSEMPTY && m_adjOccupied  == 0) {
            m_sNext = eSOCC;
        }
        else if (m_s == eSOCC && m_adjOccupied >= maxOcc) {
            m_sNext = eSEMPTY;
        }
    }

    void applyRules(void)  {
        if (m_s != m_sNext) {
            m_s = m_sNext;
            m_c = ( m_s == eSEMPTY ? sSEMPTY : sSOCC );
        }
    }

    enum State { ERR, eFLOOR, eSEMPTY, eSOCC } ;

    State m_s, m_sNext;
    unsigned char m_c;

public:
    std::vector<std::shared_ptr<Space>> m_adjacents;
private:
    size_t m_x;
    size_t m_y;
    std::shared_ptr<Area> m_area;
    size_t m_adjOccupied;
    size_t m_adjFree;
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
        row.emplace_back(line.c_str()[i], i, ysz, shared_from_this());
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


void Area::calculateAdjacents(void) {
    for (auto &outer : m_spaces) {
        for (auto &inner : outer) {
            inner.setArea(shared_from_this());
            inner.getAdjacents();
        }
    }
}

void Area::calculateIndirectAdjacents(void) {
    for (auto &outer : m_spaces) {
        for (auto &inner : outer) {
            inner.setArea(shared_from_this());
            inner.getIndirectAdjacents();
        }
    }
}

void Area::calculateRules(int maxOcc) {
    for (auto &outer : m_spaces) {
        for (auto &inner : outer) {
            inner.calculateRules(maxOcc);
        }
    }
}

void Area::applyRules(void) {
    for (auto &outer : m_spaces) {
        for (auto &inner : outer) {
            inner.applyRules();
        }
    }
}

size_t Area::countOccupied(void) {
    size_t count = 0;
    for (auto &outer : m_spaces) {
        for (auto &inner : outer) {
            if (inner.m_s == Space::eSOCC) {
                count++;
            }
        }
    }
    return count;
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

    auto pArea = std::make_shared<Area>();

    for (auto &line : lines) {
        pArea->addSpacesFromLine(line);
    }

    // Save a copy for part 2
    auto pAreaCopy = std::make_shared<Area>(*pArea);

    std::cout << "1" << std::endl;
    pArea->printArea();


    std::cout << "Area 1 adjacents..." << std::endl;
    pArea->calculateAdjacents();

    std::cout << "Looking at rules." << std::endl;

    //auto pAreaNext = std::make_shared<Area>(*pArea);
    std::shared_ptr<Area> pAreaNext = std::make_shared<Area>(*pArea);
    pArea->printArea();
    pAreaNext->printArea();

    size_t count = 0;
    do {
        //*pArea = *pAreaNext;
        pArea = std::make_shared<Area>(*pAreaNext);
        std::cout << "PrintArea" << std::endl;
        //pArea->printArea();
        pAreaNext->calculateAdjacents();
        pAreaNext->calculateRules(4);
        pAreaNext->applyRules();
        std::cout << "Print new area" << std::endl;
        pAreaNext->printArea();
        count++;

    } while (!(*pArea == *pAreaNext));

    std::cout << "Settled after " << count << " iterations." << std::endl;
    pAreaNext->printArea();

    size_t c = pAreaNext->countOccupied();
    std::cout << "Found " << c << " occupied seats!" << std::endl;

    std::cout << "==============\nPart 2\n==============" << std::endl;
    pArea = pAreaCopy;

    pArea->calculateAdjacents();
    std::cout << (*pArea)[0][0].m_adjacents.size() << " adjacents" << std::endl;
    pArea->calculateIndirectAdjacents();
    std::cout << (*pArea)[0][0].m_adjacents.size() << " indirect adjacents" << std::endl;
    pAreaNext = std::make_shared<Area>(*pArea);
    count = 0;
    do {
        //*pArea = *pAreaNext;
        pArea = std::make_shared<Area>(*pAreaNext);
        pAreaNext->calculateIndirectAdjacents();
        pAreaNext->calculateRules(5);
        pAreaNext->applyRules();
        std::cout << "Print new area" << std::endl;
        pAreaNext->printArea();
        count++;

    } while (!(*pArea == *pAreaNext));

    c = pAreaNext->countOccupied();

    std::cout << "Found " << c << " occupied seats!" << std::endl;

    return 0;

}
