
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <memory>

struct _BagRule : std::enable_shared_from_this<_BagRule> {
    std::set<std::pair<std::string, std::shared_ptr<_BagRule>>> parents;
    std::map<std::pair<std::string, std::shared_ptr<_BagRule>>, size_t> children;

public:
    void addParent(std::pair<std::string, std::shared_ptr<_BagRule>> parent) {
        parents.insert(parent);
    }

    void addChild(std::pair<std::string, std::shared_ptr<_BagRule>> child, size_t count)
    {
        children.insert({child, count});
    }


};


static std::map<std::string, std::shared_ptr<_BagRule>> sRULES_REGISTRY = {};



void parse(std::string line) {
    constexpr const char *delimChars = " bags contain ";
    static const std::string sSubjectDelim(delimChars);
    static const size_t sSubjectDelimLength = sSubjectDelim.size();

    static const std::string sNumberDelim = " ";
    static const std::string sNotLastDelim = ",";
    static const std::string sLastDelim = ".";
    static const std::string sOtherDelim = " bag";

    bool finished = false;

    size_t colorEnd = line.find(sSubjectDelim);
    std::string subjectColor = line.substr(0, colorEnd);

    // Insert the thingy
    size_t oldSize = sRULES_REGISTRY.size();
    auto parentRegEntry = *(sRULES_REGISTRY.insert({subjectColor, std::make_shared<_BagRule>()})).first;
    if (sRULES_REGISTRY.size() != oldSize) {
        std::cout << "PARENT added a new bag type" << std::endl;
    }


    std::string remainder = line.substr(colorEnd + sSubjectDelimLength);
    std::string _targetColor;
    std::string targetColor;
    std::string targetNumberStr;
    size_t _next = 0;
    size_t next = 0;
    int targetNumber;
    while (!finished) {
        if (remainder.find("no other bags") != std::string::npos) {
            targetNumber = 0;
            finished = true;
            continue;
        }
        _next = remainder.find(sNumberDelim); 
        next = remainder.find_first_of("" + sNotLastDelim + sLastDelim);
        targetNumberStr = remainder.substr(0, _next);
        _targetColor = remainder.substr(_next + 1, next - _next);
        targetColor = remainder.substr(_next+1, remainder.find(sOtherDelim) - _next - 1);
        oldSize = sRULES_REGISTRY.size();
        auto childRegEntry = *(sRULES_REGISTRY.insert({targetColor, std::make_shared<_BagRule>()})).first;
        if (sRULES_REGISTRY.size() != oldSize) {
            std::cout << "CHILD added a new bag type" << std::endl;
        }

        childRegEntry.second->addParent(parentRegEntry);
        targetNumber = std::stoi(targetNumberStr);
        parentRegEntry.second->addChild(childRegEntry, targetNumber);

        std::cout << ":" << subjectColor << ":" << targetColor << "::" << targetNumberStr << ":" << std::endl;
        if (remainder.at(next) != ',') {
            finished = true;
            continue;
        }
        remainder = remainder.substr(next+2);
    }

}

size_t addParents(std::shared_ptr<_BagRule> c, int level = 0)
{
    static std::set<std::shared_ptr<_BagRule>> theset = {};
    for (auto it = c->parents.begin(); it != c->parents.end(); it++) {
        theset.insert(it->second);
        addParents(it->second, level+1);
    }
    return theset.size();
}


size_t countChildren(std::string name, std::shared_ptr<_BagRule> p, int multiplier = 1, int level = 0);
size_t countChildren(std::shared_ptr<_BagRule> p, int multiplier = 1, int level = 0)
{
    size_t count = 0;
    size_t _internalCount = 0;
    count = multiplier;
    if (p->children.size() != 0)
    {
        for (auto it = p->children.begin(); it != p->children.end(); it++) {
            count += countChildren(it->first.first, it->first.second, it->second * multiplier, level + 1);
        }
    }
    std::cout << std::endl;
    return count;
}

size_t countChildren(std::string name, std::shared_ptr<_BagRule> p, int multiplier, int level) {
    std::cout << "\t " << name << std::endl;
    return countChildren(p, multiplier, level);
}


int main(int argc, char** argv) {
    // After parse
    // Check bag rule name.
    // eg. gRULES_REGISTRY.find(bagname);
    // Create blank if doesn't exist
    // Or as it's a set just blindly insert.?
    // gRULES_REGSITRY.insert({bagname, new _BagRule());
    // Find children
    // For each child
    // Add in registry if doesn't exist.
    // Insert this as parent into child's parent set
    // Insert child into children.
    //  

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

        parse(s);
        //break;

    };

    std::cout << "bye" << std::endl;
    size_t bagCount = 0;

    auto shinyGold = sRULES_REGISTRY["shiny gold"];
    std::cout << "boing" << std::endl;

    size_t goldCount = addParents(shinyGold);
    std::cout << "THE GOLD COUNT IS: " << goldCount << std::endl;

    // minus one because we're not counting the root bag (shiny gold)
    size_t childCount = countChildren(shinyGold) - 1;
    std::cout << "THE CHILD COUNT IS: " << childCount << std::endl;


    // Hello :)
}

