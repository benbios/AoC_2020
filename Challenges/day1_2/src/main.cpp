
/**************************************
 * --- Part Two ---
 * 
 * The Elves in accounting are thankful for your help; one of them even offers you a starfish coin they had left over from a past vacation. They offer you a second one if you can find three numbers in your expense report that meet the same criteria.
 * 
 * Using the above example again, the three entries that sum to 2020 are 979, 366, and 675. Multiplying them together produces the answer, 241861950.
 * 
 * In your expense report, what is the product of the three entries that sum to 2020?
 * ***************************************/

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

    int n1=0, n2=0, n3=0;

    for(auto it = entries.begin(); it != entries.end(); ++it) {
        for(auto itt = it+1; itt != entries.end(); ++itt) {
            auto s1= *it + *itt;
            if (s1 >= sTARGET_SUM) {
                continue;
            }

            auto foundAt = std::find(itt, entries.end(), (sTARGET_SUM - s1));
            if (foundAt != entries.end()) {
                std::cout << "Yep! Found it: " << *it << " + " <<
                            *itt << " + " << 
                            *foundAt << " = " << sTARGET_SUM << std::endl;
                n1 = *it;
                n2 = *itt;
                n3 = *foundAt;
                break;
            }

        }
    }

    if ((n1 != 0) && (n2 != 0) && (n3 != 0)) {
        long int nProduct = n1 * n2 * n3;
        std::cout << "product: " << std::endl << nProduct << std::endl;
        return 0;
    }

    else {
        std::cerr << "Something went wrong" << std::endl;
    }

}