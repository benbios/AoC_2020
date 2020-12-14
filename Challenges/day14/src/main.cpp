#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <cmath>
#include <map>

//std::bitset<36> bm_not;
//std::bitset<36> bm_or;
//std::vector<std::bitset<36>> the_stack;
//std::vector<unsigned long> *the_stack = new std::vector<unsigned long>();
std::map<unsigned long, unsigned long> *the_stack = new std::map<unsigned long, unsigned long>();
unsigned long m = 0b111111111111111111111111111111111111;
unsigned long bm_not;
unsigned long bm_or;
unsigned long bm_floating;
void setBitmask(std::string inst) {
    bm_not = 0;
    bm_or = 0;
    bm_floating = 0;
    for (signed int i = inst.size()-1; i >= 0; i--) {
        bm_not |= (unsigned long)(inst[i] == '0') << 35-i;
        bm_or |= (unsigned long)(inst[i] == '1') << 35-i;
        bm_floating |= (unsigned long)(inst[i] == 'X') << 35-i;
    }
    //bm_not &= m;
    //bm_or &= m;
    std::cout << "------------" << std::endl;
    std::cout << "msk " << inst << std::endl;
    std::cout << "bor " << std::bitset<36>(bm_or) << std::endl;
    std::cout << "not " << std::bitset<36>(bm_not) << std::endl;
    std::cout << "flt " << std::bitset<36>(bm_floating) << std::endl;
    std::cout << "************" << std::endl;
}

void setMemory(unsigned long loc, unsigned long val) {
    //if (loc >= the_stack->size()) {
        //the_stack->resize(loc + 1);
    //}
    //the_stack[l] = (val|bm_or)&~bm_not;
    std::cout << "+=+=+=+=+" << std::endl;
    std::cout << "bor " << std::bitset<36>(bm_or) << "(" << bm_or << ")" << std::endl;
    std::cout << "not " << std::bitset<36>(bm_not) << "(" << bm_not << ")"  << std::endl;
    std::cout << "val " << std::bitset<36>(val) << "(" << val << ")"  << std::endl;
    std::cout << "res " << std::bitset<36>((val|bm_or) &~bm_not) << "(" << ((val|bm_or) &~bm_not) << ")"  << std::endl;
    the_stack->operator[](loc) = (val|bm_or)&~bm_not;

}

void setMemory2(unsigned long loc, unsigned long val) {
    unsigned long _loc = loc;
    _loc |= bm_or;
    //loc &= ~bm_not;
    _loc &= ~bm_floating;
    // each bit in bm_float can be either 1 or 0.
    std::vector<size_t> floating_bits;
    for (ssize_t i = 35; i >= 0; i--) {
        if ((bm_floating >> i)&(0x1)) {
            floating_bits.push_back(i);
        }
    }

    unsigned long __loc = _loc;
    std::cout << "ooo" << std::endl;
    for (unsigned long x = 0; x < std::pow(2,floating_bits.size()); x++) {
        __loc = _loc;
        for (unsigned long z = 0; z < floating_bits.size(); z++) {
            //std::cout << ((x >> z) & 1) << std::endl;
            //std::cout << floating_bits[z] << std::endl;
            __loc |= (((x >> z) & 1) << floating_bits[z]);
        }
        __loc &= m;
        std::cout << __loc << std::endl;
        //if (__loc >= the_stack->size()) {
        //    the_stack->resize(__loc + 1);
        //}
        (*the_stack)[__loc] = val;
    }

}

void tick(std::string inst, bool part2 = false) {
    std::string delim(" = ");
    auto _i = inst.find(delim);
    std::string i = inst.substr(0, _i);
    std::string o = inst.substr(_i + 3);
    std::string a;
    std::cout << i << std::endl;
    std::cout << o << std::endl;
    auto _a = inst.find("[");
    if (_a != inst.npos) {
        a = inst.substr(_a+1, inst.find("]")-_a-1);
        std::cout << a << std::endl;
    }
    if (i == "mask") {
        setBitmask(o);
    }
    else {
        //setMemory(std::bitset<36>(std::stol(a)), std::bitset<36>(std::stol(o)));
        if (!part2)
            setMemory(std::stoul(a), m&std::stoul(o));
        else
            setMemory2(std::stoul(a), m&std::stoul(o));
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
    std::vector<std::string> lines;
    std::string s;
    while (std::getline(inFile, s)) {
        if (s.front() != '#')
            lines.push_back(s);
    }

    auto itr = lines.begin();
    auto bitmask = *itr;
    std::advance(itr,1);
    // bitmask : X - leave alone, 1 - override 1, 0 - override 0
    // Effectively split it into 2.
    // 0s become NOT 1
    // 1s become OR 1
    for (auto &l : lines) {
        tick(l);
    }

    unsigned long long tot = 0;
    for (auto m : *the_stack) {
        tot += m.second;
    }
    std::cout << "Stack size: " << the_stack->size() << std::endl;
    std::cout << "Total: " << tot << std::endl;;

    std::cout << "part 2:" << std::endl;
    
    the_stack->clear();
    tot = 0;
    for (auto &l : lines) {
        tick(l, true);
    }
    for (auto m : *the_stack) {
        tot += m.second;
    }

    std::cout << "Stack size: " << the_stack->size() << std::endl;
    std::cout << "Total: " << tot << std::endl;;

}