#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <algorithm>

std::vector<std::string> lines;
std::multimap<int, std::vector<int>> rules;

int A = 256;
int B = 257;

struct Node {
    int key;
    std::vector<struct Node*> leaves;
};


std::vector<std::string> all_valid;
int A_key = 0, B_key = 0;

std::vector<std::vector<char>> valid_by_position;

std::stringstream r0ss;
std::regex rgx, rgx2, rgx3;

std::stringstream r8ss;
std::stringstream r11ss;
std::stringstream r42ss;
std::stringstream r31ss;

void construct(int rule, std::stringstream &rss, bool part2 = false) {

    bool multi = false;
    auto rls = rules.equal_range(rule);
    for (auto it = rls.first; it != rls.second; it++) {
        if ((it->second.front() == A) || (it->second.front() == B)) {
            // Got to a bottom ruuuule
            if (it->second.front() == A) {
                std::cout << "a";
                rss << "a";
            }
            if (it->second.front() == B) {
                std::cout << "b";
                rss << "b";
            }
        }
        else {
            if (std::next(it) != rls.second) {
                rss << "(";
            }
            for (auto r : it->second) {
                if (part2 && r == 11) {
                    // Use magic recursion
                    // STL REGEX NO SUPPORT NAMED CAPTURE ME SAD.

                    rss << "(?P<recurse>" << r42ss.str() << "(?&recurse)?" << r31ss.str() << ")";
                    // Explanation:
                    // Consider r42 is "ab" and r31 is "ba".
                    // :    (?P<recurse>ab(?&recurse)?ba)
                    // (P<recurse>ab creates a named capture group called "recurse"
                    //      that captures "ab".
                    // The questionmark preceding P matches zero or one times.
                    // (?&recurse) recurses the whole pattern, (? matches zero or one
                    //      recursive successes)
                    // Each time the pattern is recursed it ends with checking for "ba"
                    //      so the match is symmetrical.
                }
                else if (part2 && r == 8) {
                    rss << r42ss.str() << "+";
                }
                else {
                    construct(r, rss, part2);
                }
            }
            if (it != rls.first) {
                rss << ")";
            }
        }
        if ((std::next(it) != rls.second) && !multi) {
            rss << "|";
        }
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
    std::string s;
    std::stringstream ss;
    while (std::getline(inFile, s)) {
        if (s.empty()) {
            break;
        }
        //rules.push_back(s);
            std::vector<int> values;
        auto _c = s.find(":");
        auto key = s.substr(0,_c);
        auto value = s.substr(_c+2);
        int n = 0;
        if ((_c = value.find("\"")) != std::string::npos) {
            if (value.find("a") != std::string::npos) {
                values.push_back(A);
                A_key = std::stoi(key);
            }
            else {
                values.push_back(B);
                B_key = std::stoi(key);
            }
            rules.insert(std::make_pair(std::stoi(key), std::move(values)));
        }
        else {

            if ((_c = value.find("|")) != std::string::npos) {
                auto value2 = value.substr(_c+2);
                std::stringstream ss2(value2);
                value = value.substr(0, _c);
                std::vector<int> values2;
                while (ss2 >> n) {
                    values2.push_back(n);
                }
                rules.insert(std::make_pair(std::stoi(key), std::move(values2)));
            }
            std::stringstream ss1(value);
            while (ss1 >> n) {
                values.push_back(n);
            }
            rules.insert(std::make_pair(std::stoi(key), std::move(values)));
        }

    }
    while (std::getline(inFile, s)) {
        lines.push_back(s);
        std::cout << "LINE: " << s << std::endl;
    }

    for (auto [k,v] : rules ) {
        std::cout << "RULE " << k << " : ";
        for (auto _v : v) {
            std::cout << _v << " ";
        }
        std::cout << std::endl;
    }

    std::string str;
    construct(0, r0ss);
    rgx = std::regex(r0ss.str());
    std::cout << std::endl;
    size_t matches = 0;
    for (auto l : lines) {
        if (std::regex_match(l, rgx)) {
            std::cout << "l matches" << std::endl;
            matches++;
        }
        else {
            std::cout << "l does not match" << std::endl;
        }
    }

    std::cout << "Matches: " << matches << std::endl;

    //for (auto l : lines) {
    //    check(l);
    //}
    std::cout << std::endl << r0ss.str() << std::endl;

    std::cout << "Part 2" << std::endl;

    construct(42, r42ss);
    construct(31, r31ss);

    std::cout << "Rule 42 regex: " << std::endl;
    std::cout << r42ss.str() << std::endl;
    std::cout << "Rule 31 regex: " << std::endl;
    std::cout << r31ss.str() << std::endl;


    rules.emplace(11, std::vector<int>{42,31});
    rules.emplace(11, std::vector<int>{42, 11, 31});

    r0ss.str(std::string());
    std::cout << "begin" << std::endl;
    std::cout << std::endl << r0ss.str() << std::endl;
    std::cout << "end" << std::endl;
    construct(0, r0ss, true);

    std::cout << "begin" << std::endl;
    std::cout << std::endl << r0ss.str() << std::endl;
    std::cout << "end" << std::endl;

    std::cout << "DUMPING rgx1:" << std::endl;
    std::cout << r0ss.str() << std::endl << std::endl;


    std::cout << "Now because regex in C++ is total garbage, please copy the expression" << std::endl;
    std::cout << " and head over to https://regex101.com where you can enter it and" << std::endl;
    std::cout << " find how many matches there are." << std::endl;
    std::cout << "==============="<<std::endl<<r0ss.str()<<std::endl<<"=============="<<std::endl;
    std::cout << "Things to match:" << std::endl;
    for (auto l : lines) {
        std::cout << l << std::endl;
    }

}
