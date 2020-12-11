#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

std::list<int> adaptors;
long count;

std::list<int> copyList(std::list<int> in) {
    std::list<int> out;
    for (auto &itr : in) {
        out.push_back(itr);
    }
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
    std::vector<std::string> lines;
    std::string s;
    while (std::getline(inFile, s)) {
        if (s.front() != '#')
            lines.push_back(s);
    }

    for (auto &i : lines) {
        adaptors.push_back(std::stoi(i));
    }

    adaptors.sort();

    adaptors.push_back(adaptors.back() + 3);
    int ones = 0;
    int twos = 0;
    int threes = 0;
    int last = 0;
    for (auto &i : adaptors) {
        std::cout << i << " ";
        if ((i - last) == 3) {
            threes++;
        }
        else if ((i - last) == 1) {
            ones++;
        }
        last = i;
    }
    std::cout << std::endl;
    std::cout << "Part 1:" << std::endl;
    std::cout << "3s: " << threes << " x 1s: " << ones << " product is: " << threes * ones << std::endl;

    // Part 2

    adaptors.push_front(0);
    unsigned long long count;
    std::map<int, unsigned long> weights;
    weights[0] = 1;
    auto eitr = adaptors.begin();
    std::advance(eitr, 3);
    auto bitr = adaptors.begin();
    while (bitr != adaptors.end()) {
        int n = *bitr;
        weights[n+1] += weights[n];
        weights[n+2] += weights[n];
        weights[n+3] += weights[n];
        advance(bitr,1);
        advance(eitr,1);
    }

    std::cout << "Part 2" << std::endl;
    std::cout << "Routes to wall: " << weights[adaptors.back()] << std::endl;

}
