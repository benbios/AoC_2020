
/**************************************
 * --- Day 1: Report Repair ---
 * 
 * After saving Christmas five years in a row, you've decided to take a vacation at a nice resort on a tropical island. Surely, Christmas will go on without you.
 * 
 * The tropical island has its own currency and is entirely cash-only. The gold coins used there have a little picture of a starfish; the locals just call them stars. None of the currency exchanges seem to have heard of them, but somehow, you'll need to find fifty of these coins by the time you arrive so you can pay the deposit on your room.
 * 
 * To save your vacation, you need to get all fifty stars by December 25th.
 * 
 * Collect stars by solving puzzles. Two puzzles will be made available on each day in the Advent calendar; the second puzzle is unlocked when you complete the first. Each puzzle grants one star. Good luck!
 * 
 * Before you leave, the Elves in accounting just need you to fix your expense report (your puzzle input); apparently, something isn't quite adding up.
 * 
 * Specifically, they need you to find the two entries that sum to 2020 and then multiply those two numbers together.
 * 
 * For example, suppose your expense report contained the following:
 * 
 * 1721
 * 979
 * 366
 * 299
 * 675
 * 1456
 * 
 * In this list, the two entries that sum to 2020 are 1721 and 299. Multiplying them together produces 1721 * 299 = 514579, so the correct answer is 514579.
 * 
 * Of course, your expense report is much larger. Find the two entries that sum to 2020; what do you get if you multiply them together?
 * 
***************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char **argv) {

    static const int sTARGET_SUM=2020;

    std::string inFilePath("input.txt");

    if (argc == 2) {
        inFilePath = argv[1];
    }
    else if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [input file]" << std::endl;
        return 1;
    }

    std::cout << "Populating from \"" << inFilePath << "\"" << std::endl;

    std::ifstream inFile(inFilePath);
    std::vector<int> entries;
    std::string s;

    while(inFile >> s) {
        entries.push_back(std::stoi(s));
    }

    std::cout << "Calculating" << std::endl;
    std::cout << entries.size() << std::endl;

    int n1=0, n2=0;

    std::vector<int>::iterator it = entries.begin();
    short unsigned int thingy = 10;
    for(it; it != entries.end(); ++it) {
        auto foundAt = std::find(it, entries.end(), (sTARGET_SUM - *it));
        if (foundAt != entries.end()) {
            std::cout << "Yep! Found it: " << *it << " + " <<
                        *foundAt << " = " << sTARGET_SUM << std::endl;
            n1 = *it;
            n2 = *foundAt;
            break;
        }
    }

    if ((n1 != 0) && (n2 != 0)) {
        int nProduct = n1 * n2;
        std::cout << "product: " << std::endl << nProduct << std::endl;
        return 0;
    }

    else {
        std::cerr << "Something went wrong" << std::endl;
    }

}