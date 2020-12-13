#include <iostream>
#include <fstream>
#include <map>
#include <vector>

// Forward declaration
class Ship {
    enum Heading {
        eEAST,
        eSOUTH,
        eWEST,
        eNORTH
    };
    enum Direction {
        eLEFT,
        eRIGHT
    };

public:
    Ship() = default;
    Ship(int x, int y) : m_x(x), m_y(y), m_isWaypoint(true) {};
    Heading m_heading = eEAST;

    void followInstruction(std::string inst) {
        std::cout << inst << std::endl;
        auto i = inst.substr(0,1);
        auto n = std::stoi(inst.substr(1));

        if (i.front() == 'F') {
            move(n);
        }
        else if (i.front() == 'L' || i.front() == 'R') {
            rotate(directionFromString(i), n);
        }
        else {
            move(headingFromString(i), n);
        }

        std::cout << "Coords: " << m_x << "." << m_y << " | Heading: " << m_heading << std::endl;
    }

    void followInstruction2(std::string inst) {
        std::cout << inst << std::endl;
        auto i = inst.substr(0,1);
        auto n = std::stoi(inst.substr(1));

        if (i.front() == 'F') {
            moveToWaypoint(n);
        }
        else if (i.front() == 'L' || i.front() == 'R') {
            m_waypoint->rotate(directionFromString(i), n);
        }
        else {
            m_waypoint->move(headingFromString(i), n);
        }
        std::cout << "Coords: " << m_x << "." << m_y << " | Heading: " << m_heading << std::endl;
        std::cout << "waypoint coords: " << m_waypoint->m_x << "." << m_waypoint->m_y << " | Heading: " << m_waypoint->m_heading << std::endl;

    }

    void moveToWaypoint(int n) {
        m_x += n * m_waypoint->m_x;
        m_y += n * m_waypoint->m_y;
        //switch(m_waypoint->m_heading) {
        //    case eEAST:
        //        // Starting direction. x is applied +x y is +y
        //        m_x += n * m_waypoint->m_x;
        //        m_y += n * m_waypoint->m_y;
        //        break;
        //    case eSOUTH:
        //        // Rotated 90 degrees clockwise. 
        //        // X is applied -y, y is +x
        //        m_y -= n * m_waypoint->m_x;
        //        m_x += n * m_waypoint->m_y;
        //        break;
        //    case eWEST:
        //        // Rotated 180 degrees from start. 
        //        // x is applied -x, y is applied -y
        //        m_x -= n * m_waypoint->m_x;
        //        m_y -= n * m_waypoint->m_y;
        //        break;
        //    case eNORTH:
        //        // Rotated 90 degrees anti-clockwise. 
        //        // X is applied -y, y is +x
        //        m_y += n * m_waypoint->m_x;
        //        m_x -= n * m_waypoint->m_y;
        //        break;
        //}
    }


    int getManhattanDistance() {
        return (abs(m_x) + abs(m_y));
    }

    void setWaypoint(Ship *waypoint) { m_waypoint = waypoint; }

private:
    bool m_isWaypoint = false;

    Direction directionFromString(std::string inst) {
        if (inst.front() == 'L') {
            return eLEFT;
        }
        else return eRIGHT;
    }

    Heading headingFromString(std::string inst) {
        if (inst.front() == 'N') {
            return eNORTH;
        }
        else if (inst.front() == 'S') {
            return eSOUTH;
        }
        else if (inst.front() == 'E') {
            return eEAST;
        }
        else {
            return eWEST;
        }
    }

    // This overload moves us forwards (current heading)
    void move(int num) {
        move(m_heading, num);
    }
    // This overload move in a direction without changing heading
    void move(Heading h, int num) {
        switch (h) {
            case eEAST:
                m_x += num;
                break;
            case eSOUTH:
                m_y -= num;
                break;
            case eWEST:
                m_x -= num;
                break;
            case eNORTH:
                m_y += num;
                break;
        }
    }

    void rotate(Direction d, int degrees) {
        auto count = degrees / 90;
        Heading next = m_heading;
        while (count > 0) {
            switch(next) {
                case eEAST:
                    next = (d == eLEFT ? eNORTH : eSOUTH);
                    break;
                case eSOUTH:
                    next = (d == eLEFT ? eEAST : eWEST);
                    break;
                case eWEST:
                    next = (d == eLEFT ? eSOUTH : eNORTH);
                    break;
                case eNORTH:
                    next = (d == eLEFT ? eWEST : eEAST);
                    break;
            }
            count--;
            if (m_isWaypoint) {
                if (d == eLEFT) {
                    auto tmp = m_x;
                    m_x = -1 * m_y;
                    m_y = tmp;
                }
                else {
                    auto tmp = m_y;
                    m_y = -1 * m_x;
                    m_x = tmp;
                }
            }
        }
        m_heading = next;
    };

    int m_startX = 0;
    int m_startY = 0;


    Ship *m_waypoint;

protected:
    int m_x = 0;
    int m_y = 0;
};


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

    // Hello :)
    // Part 1
    Ship ship;
    for (auto &line : lines) {
        ship.followInstruction(line);
    }

    std::cout << "Part 1" << std::endl;
    std::cout << "Manhattan distance: " << ship.getManhattanDistance() << std::endl;

    auto ship2 = Ship();
    auto waypoint = Ship(10, 1);
    ship2.setWaypoint(&waypoint);

    // Part 2
    std::cout << "Part 2" << std::endl;
    for (auto &line : lines) {
        ship2.followInstruction2(line);
    }

    std::cout << "Manhattan distance: " << ship2.getManhattanDistance() << std::endl;

}
