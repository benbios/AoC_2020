#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <iostream>


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
    std::unordered_set<unsigned char> st;
    std::vector<std::unordered_set<unsigned char>> vst;
    size_t total = 0;
    while (std::getline(inFile, s)) {
        v.push_back(s);
        if (!s.empty()) {
            for (char const &c : s) {
                st.insert(c);
            }
        }
        else {
            total += st.size();
            vst.push_back(std::move(st));
            st = std::unordered_set<unsigned char>();
        }
    }
    if (!st.empty()) {
        total += st.size();
        vst.push_back(std::move(st));
    }

    std::cout << "Count: " << total << std::endl;

    // Begin part 2
    st.clear();
    vst.clear();
    total = 0;
    bool start = true;
    for (auto &i : v) {
        if (!i.empty()) {
            if (st.empty() && start) {
                start = false;
                for (char const &c : i) {
                    st.insert(c);
                }
            }
            else {
                std::unordered_set<unsigned char> _st;
                for (char const &c : i) {
                    if (st.count(c)) {
                        _st.insert(c);
                    }
                }
                st = std::move(_st);
            }
        }
        else {
            total += st.size();
            vst.push_back(std::move(st));
            start = true;
            st = std::unordered_set<unsigned char>();
        }
    }
    if (!st.empty()) {
        total += st.size();
        vst.push_back(std::move(st));
    }

    std::cout << "Part 2 Count: " << total << std::endl;

}

