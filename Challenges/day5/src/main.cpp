#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <cstring>


int parseSeatRow(std::string input) {
    // std::cout << input << " -- ";
    int out = (
        ((input[6] == 'B')) |
        ((input[5] == 'B') << 1) |
        ((input[4] == 'B') << 2) |
        ((input[3] == 'B') << 3) |
        ((input[2] == 'B') << 4) |
        ((input[1] == 'B') << 5) |
        ((input[0] == 'B') << 6) 
    );
    return out;
}

int parseSeatColumn(std::string input) {
    // std::cout <<"  " << input << " -- ";
    int out = (
        ((input[2] == 'R')) |
        ((input[1] == 'R') << 1) |
        ((input[0] == 'R') << 2)
    );
    return out;
}

int parseSeat(std::string input) {
    // std::cout << input << " -- ";
    int out = (
        ((input[9] == 'R')) |
        ((input[8] == 'R') << 1) |
        ((input[7] == 'R') << 2) |
        ((input[6] == 'B') << 3) |
        ((input[5] == 'B') << 4) |
        ((input[4] == 'B') << 5) |
        ((input[3] == 'B') << 6) |
        ((input[2] == 'B') << 7) |
        ((input[1] == 'B') << 8) |
        ((input[0] == 'B') << 9) 
    );
    return out;

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
    std::vector<std::string> v;
    std::string s;
    while (std::getline(inFile, s)) {
        v.push_back(s);
    }

    std::cout << "Found " << v.size() << " entries." << std::endl;

    // Each row contains 2 binary representations of numbers. 
    // First 7 digits: F = 0, B = 1
    // Last 3 digits: L = 0, R = 1

    int highSeatId = 0;
    std::list<int> ids;
    for (auto itr : v) {
        // int row = parseSeatRow(itr.substr(0, 7));
        // std::cout << "Row is : " << row;
        // int col = parseSeatColumn(itr.substr(7));
        // std::cout << "  -  Column is : " << col << std::endl;

        //int seatId = (row * 8) + col;
        int seatId = parseSeat(itr);
        //int otherSeatId = itr.c_str() & "BBBBBBBRRR";
        // int otherSeatId = parseSeat(itr);
        // std::cout << "Seat id: " << seatId << " - " << otherSeatId << std::endl;
        if (seatId > highSeatId) {
           // std::cout << "New high seat ID (" << seatId <<" > " << highSeatId <<std::endl; 
            highSeatId = seatId;
        }
        ids.push_back(seatId);

    }
    std::cout << "Highest seat ID at the end was " << highSeatId << std::endl;
    

    // Start part 2
    ids.sort();
    // list is doubly linked so we can check forwards and backwards.
    for (auto it = ids.begin(); it != ids.end(); it++) {
        // Check forward is this + 1;
        if (*std::next(it) != (*it + 1) ) {
           // std::cout << "Possible candidate... " << *it << std::endl;
            // Check that it is in fact +2;
            if (*std::next(it) == (*it + 2)) {
                std::cout << "Found your seat. It's " << (*it + 1) << std::endl;
            }
        }
    }


}