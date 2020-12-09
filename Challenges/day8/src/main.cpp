#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <deque>

std::vector<std::string> lines = {};
std::vector<std::string> lines2;
//int gPROGRAM_COUNTER = 0;
//int gPROGRAM_LAST = 0;
int gACC = 0;
//std::unordered_set<int> gEXECUTED_INSTRUCTIONS;
//std::unordered_set<int> gCHANGED;
//std::deque<int> gISCACHE;
//std::string *gLINE_LAST;
//bool fixed = false;
//int gLINESWAP = 0;

int gPC =0;
std::unordered_set<int> gEX;
bool gMODIFIED = false;
int gPCsave;
int gACCsave;
std::unordered_set<int> gEXsave;

bool runLineIfUnique(std::string line, bool fix = false) {
    bool stop = false;
    std::string inst = line.substr(0, 3);
    std::string operand = line.substr(4);
    int operandInt = std::stoi(operand);
    if (!gEX.insert(gPC).second) {
        return true;
    }

    if (fix && !gMODIFIED) {
        if (inst == "nop") {
            inst = "jmp";
            gMODIFIED = true;
        }
        else if (inst == "jmp") {
            inst = "nop";
            gMODIFIED = true;
        }

        if (gMODIFIED) {
            gPCsave = gPC;
            gACCsave = gACC;
            for (auto &i : gEX) {
                if (i != gPC) {
                    gEXsave.insert(i);
                }
            }
            std::cout << "Attempted swap of " << gPC << ": " << line << " (now: " << inst << ")" << std::endl;
        }
    }
    if (inst == "nop") {
        gPC++;
    }
    else if (inst == "acc") {
        gACC += operandInt;
        gPC++;
    }
    else if (inst == "jmp") {
        gPC += operandInt;
    }
    else {
        stop = true;
    }
    return stop;
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
    while (std::getline(inFile, s)) {
        lines.push_back(s);
    }

    std::cout << "Read " << lines.size() << " lines." << std::endl;

    for (auto &vit : lines) {
        lines2.push_back(vit);
    }

    while (!runLineIfUnique(lines[gPC]));

    // part 1
    std::cout << "Finished after " << gEX.size() << " instructions" << std::endl;
    std::cout << "Accumulator value: " << gACC << std::endl;

    // part 2
    gPC = 0;
    gACC = 0;
    gEX.clear();

    while (gPC < lines.size()) {
        if (runLineIfUnique(lines[gPC], true)) {
            // Duplicate.
            // Restore to original state
            gPC = gPCsave;
            gACC = gACCsave;
            gEX.clear();
            for (auto i : gEXsave) {
                gEX.insert(i);
            }
            std::cout << "NO good. Restarting at " << gPC << std::endl;
            runLineIfUnique(lines[gPC], false);
            gMODIFIED = false;
        }
    }
    
    std::cout << "Fixed program accumulator is " << gACC << std::endl;
    
}
