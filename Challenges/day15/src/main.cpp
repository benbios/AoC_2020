#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <map>

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

    std::vector<int> numbers;
    size_t max = 2020;
    numbers.reserve(max);
    std::map<int, size_t> num_reg;
    size_t dl;
    s = lines.back();
    size_t i = 1;
    std::string _s;
    int n, _n;
    while ((dl = s.find(',')) != std::string::npos) {
        if (i>1) {
            num_reg[_n] = i-1;
        }
        _s = s.substr(0, dl);
        n = std::stoi(_s);
        //num_reg[n] = i;
        numbers.push_back(n);
        s = s.substr(dl+1);
        i++;
        _n = n;
    }
    n = std::stoi(s);
    numbers.push_back(n);
    // Start here.
    num_reg[_n] = i-1;
    i++;

    size_t _i, _p;
    int x;
    for (i; i <= max; i++) {   
        size_t _i = i-1;
        // Is last number unique?
        if (num_reg.count(n) == 0){
            x = 0;
        }
        else {
            // last position of the number
            _p = num_reg[n];
            x = _i - _p;
        }
        num_reg[n] = _i;
        numbers.push_back(x);
        _n = n;
        n = x;
        //std::cout << n << std::endl;
    }
    std::cout << "2020: " << numbers[max-1] << std::endl;
    max = 30000000;
    numbers.clear();
    numbers.reserve(max);
    i = 0;
    std::vector<int> num_reg2;
    num_reg2.resize(max);

    s = lines.back();
    while ((dl = s.find(',')) != std::string::npos) {
        if (i>0) {
            //num_reg[_n] = i-1;
            num_reg2[_n] = i;// - 1;
        }
        _s = s.substr(0, dl);
        n = std::stoi(_s);
        numbers.push_back(n);
        s = s.substr(dl+1);
        i++;
        _n = n;
    }
    n = std::stoi(s);
    numbers.push_back(n);
    // Start here.
    num_reg2[_n] = i;
    i++;
    for (i; i <= max; i++) {   
        if (!(i%1000000)) {
            std::cout << i << std::endl;
        }
        // Is last number unique?
        if (num_reg2[n] == 0) {
        //if (num_reg.count(n) == 0){
            x = 0;
        }
        else {
            // last position of the number
            _p = num_reg2[n];
            x = i - _p;
        }
        num_reg2[n] = i;
        numbers.push_back(x);
        _n = n;
        n = x;
    }

    std::cout << max << ": " << numbers[max-1] << std::endl;
}
