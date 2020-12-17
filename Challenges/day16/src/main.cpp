#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using FieldKey =  std::string;
using FieldVal = std::pair<int, int>;
std::multimap<FieldKey, FieldVal> fields;

std::vector<int> my_ticket;
std::vector<std::vector<int>> nearby_tickets;

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
        if (s.front() != '#' && s.size() != 0)
            lines.push_back(s);
    }

    auto li = lines.begin();
    while (li->find("your ticket:") == std::string::npos) {
        // Split "key: low-high OR low2-high2" into 2 multimap entries
        auto c = li->find(":");
        auto key = li->substr(0, c);
        auto values = li->substr(c+1);
        c = values.find(" or ");
        auto c2 = values.find("-");
        int vlow = std::stoi(values.substr(0, c2));
        int vhigh = std::stoi(values.substr(c2+1,c));
        fields.emplace(key, std::make_pair(vlow, vhigh));
        c2 = values.find_last_of("-");
        vlow = std::stoi(values.substr(c+4,c2));
        vhigh = std::stoi(values.substr(c2+1));
        fields.emplace(key, std::make_pair(vlow, vhigh));
        li++;
    }
    std::advance(li,1);
    auto n = *li;
    while(true) {
        my_ticket.push_back(std::stoi(n));
        if (n.find(',') == std::string::npos) {
            break;
        }
        n = n.substr(n.find(',')+1);
    }
    std::advance(li,2);
    while (li != lines.end()) {
        std::vector<int> ticket;
        auto n = *li;
        while(true) {
            ticket.push_back(std::stoi(n));
            if (n.find(',') == std::string::npos) {
                break;
            }
            n = n.substr(n.find(',')+1);
        }
        nearby_tickets.push_back(ticket);
        li++;
    }

    size_t error_rate = 0;
    std::vector<std::vector<int>> valid_tickets;
    for (auto const &t : nearby_tickets) {
        bool ticket_is_valid = true;
        for (int const &n : t) {
            bool n_is_valid = false;
            for (auto &[k,v] : fields) {
                if ((v.first <= n) && (v.second >= n)) {
                    n_is_valid = true;
                    break;
                }
            }
            if (!n_is_valid) {
                error_rate += n;
                ticket_is_valid = false;
            }
        }
        if (ticket_is_valid) {
            valid_tickets.push_back(t);
        }
    }
    std::cout << "Part 1:" << std::endl;
    std::cout << "Nearby tickets error score: " << error_rate << std::endl;

    std::cout << "Part 2:" << std::endl;
    std::cout << "Using " << valid_tickets.size() << " valid tickets" << std::endl;

    // At the start every field is a possible candidate for each position
    std::vector<std::set<FieldKey>> candidates;
    for (size_t s = 0; s < valid_tickets.front().size(); s++) {
        std::set<FieldKey> c;
        for (auto f : fields) {
            c.insert(f.first);
        }
        candidates.push_back(c);
    }

    // Go through each position in each ticket and eliminate candidates.
    for (auto const &t: valid_tickets) {
        //for (auto const &n : t) {
        for (size_t s = 0; s < t.size(); s++) {
            // Out of the remaining possibilities, which could it be?
            auto &possible = candidates[s];
            std::vector<FieldKey> to_erase;
            //for (size_t p = 0; p < possible.size(); p++) {
            for (auto const &p : possible) {
                //auto range = fields.equal_range(possible[p]);
                auto range = fields.equal_range(p);
                auto n = t[s];
                bool valid = false;
                for (auto it = range.first; it != range.second; it++) {
                    if ((it->second.first <= n) && (it->second.second >= n)) {
                        valid = true;
                    }
                }
                if (!valid) {
                    // Value doesn't fit in that candidate. Remove the candidate from possibilities.
                    to_erase.push_back(p);
                    continue;
                }
            }
            for (auto const &k : to_erase) {
                possible.erase(k);
            }
        }
    }
    // Sort through any candidates that have double possibilities 
    size_t unique_count = 0;
    for (auto const &c : candidates) {
        if (c.size() == 1) {
            unique_count++;
        }
    }
    while (unique_count != candidates.size()) {
        for (size_t c = 0; c < candidates.size(); c++) {
            if (candidates[c].size() > 1) {
                std::vector<FieldKey> to_erase;
                for (size_t _c = 0; _c < candidates.size(); _c++) {
                    // Look through the other candidates to see if it's present.
                    if (_c == c) continue;
                    for (auto &dup : candidates[c]) {
                        if (candidates[_c].count(dup) > 0) {
                            if (candidates[_c].size() == 1) {
                                to_erase.push_back(dup);
                            }
                        }
                    }
                }
                for (auto const &t : to_erase) {
                    candidates[c].erase(t);
                }
            }
        }
        unique_count = 0;
        for (auto const &c : candidates) {
            if (c.size() == 1) {
                unique_count++;
            }
        }
    }
    std::cout << "Got to the end..." << std::endl;
    std::cout << "Fields are: ";
    for (auto const &outer : candidates) {
        std::cout << *outer.begin() << ", ";
    }
    std::cout << std::endl;

    std::vector<int> departure_keys;
    for (size_t c = 0; c < candidates.size(); c++) {
        FieldKey key = *(candidates[c]).begin();
        if (key.substr(0,9) == "departure") {
            departure_keys.push_back(c);
            std::cout << key << std::endl;
        }
    }

    long mul = 1;
    for (int &k : departure_keys) {
        if (k > departure_keys.front()) {
            std::cout << " X ";
        }
        std::cout << my_ticket[k];
        mul *= my_ticket[k];
    }

    std::cout << " = " << mul << std::endl;

}
