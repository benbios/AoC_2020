#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

enum Opr {
    eADD,
    eMULTIPLY
};

struct Operation {
    Opr o;
    long operand;
};


long eval2 (std::string line, size_t * pos = nullptr) {
    size_t n = 0;
    size_t *p;
    long value = 0;
    long next = 0;
    char c[2];
    bool finished = false;
    bool first = true;
    std::vector<long> nums;
    std::vector<Opr> ops;
    if (pos == nullptr) {
        p = &n;
    }
    else {
        p = pos;
    }
    while (!finished) {
        Opr opr;
        if (!first) {
            // Get operator
            if (line[(*p)++] == '*') {
                opr = eMULTIPLY;
            }
            else {
                opr = eADD;
            }
            // skip space.
            ++(*p);
        }
        else {
            // Always add, don't move pointer
            opr = eADD;
            nums.push_back(0);
        }
        if (line[*p] == '(') {
            ++(*p);
            next = eval2(line, p);
        }
        else {
            c[0] = line[(*p)++];
            c[1] = '\0';
            next = std::atol(c);
        }

        // Apply operation
        //if (opr == eMULTIPLY) {
        //    value *= next;
        //}
        //else {
        //    value += next;
        //}
        nums.push_back(next);
        ops.push_back(opr);
        // Check for end?
        if (!first)
        {
            if ((*p) == line.size()) {
                finished = true;
            }
            else if (line[(*p)] == ')') {
                finished = true;
            }
        }
        else {
            first = false;
        }
        // skip space
        (*p)++;
    }
    //std::cout << "Returning: " << value << std::endl;
    auto v = nums[0];
    bool stop = false;
    while (!stop) {
        std::vector<long> _nums{};
        std::vector<Opr> _ops{};
        size_t z = 0;
        //_nums.resize(nums.size());
        //_ops.resize(nums.size());
        for (size_t x = 0; x < ops.size(); x++) {
            if (ops[x] == eADD) {
                if (z >= _nums.size())
                    _nums.resize(z+1);
                _nums[z] += nums[x+1];
                //_nums.push_back(v);
            }
            else {
                _ops.push_back(ops[x]);
                _nums.push_back(nums[x+1]);
                //_ops[z] = (ops[x]);
                //_nums[++z] = nums[x+1];
                z++;
            }
        }
        nums = _nums;
        ops = _ops;
        stop = true;
        
    }
    v = 1;
    for (auto i : nums) {
        v *= i;
    }
    return v;
}



long eval (std::string line, size_t * pos = nullptr) {
    size_t n = 0;
    size_t *p;
    long value = 0;
    long next = 0;
    char c[2];
    bool finished = false;
    bool first = true;
    if (pos == nullptr) {
        p = &n;
    }
    else {
        p = pos;
    }
    while (!finished) {
        Opr opr;
        if (!first) {
            // Get operator
            if (line[(*p)++] == '*') {
                opr = eMULTIPLY;
            }
            else {
                opr = eADD;
            }
            // skip space.
            ++(*p);
        }
        else {
            // Always add, don't move pointer
            opr = eADD;
        }
        if (line[*p] == '(') {
            ++(*p);
            next = eval(line, p);
        }
        else {
            c[0] = line[(*p)++];
            c[1] = '\0';
            next = std::atol(c);
        }

        // Apply operation
        if (opr == eMULTIPLY) {
            value *= next;
        }
        else {
            value += next;
        }
        // Check for end?
        if (!first)
        {
            if ((*p) == line.size()) {
                finished = true;
            }
            else if (line[(*p)] == ')') {
                finished = true;
            }
        }
        else {
            first = false;
        }
        // skip space
        (*p)++;
    }
    //std::cout << "Returning: " << value << std::endl;
    return value;
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

    long total = 0;
    for (auto line : lines) {

        long pls = eval(line);
        std::cout << " + " << pls << std::endl;
        total += pls;
    }
    std::cout << "Total: " << total << std::endl;
    std::cout << "Part 2:" << std::endl;

    total = 0;
    for (auto line : lines) {
        long pls = eval2(line);
        std::cout << " + " << pls << std::endl;
        total += pls;
    }
    std::cout << "Total: " << total << std::endl;
}
