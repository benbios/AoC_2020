/*********************************
 * 
 * --- Day 2: Password Philosophy ---
 * 
 * Your flight departs in a few days from the coastal airport; the easiest way down to the coast from here is via toboggan.
 * 
 * The shopkeeper at the North Pole Toboggan Rental Shop is having a bad day. "Something's wrong with our computers; we can't log in!" You ask if you can take a look.
 * 
 * Their password database seems to be a little corrupted: some of the passwords wouldn't have been allowed by the Official Toboggan Corporate Policy that was in effect when they were chosen.
 * 
 * To try to debug the problem, they have created a list (your puzzle input) of passwords (according to the corrupted database) and the corporate policy when that password was set.
 * 
 * For example, suppose you have the following list:
 * 
 * 1-3 a: abcde
 * 1-3 b: cdefg
 * 2-9 c: ccccccccc
 * 
 * Each line gives the password policy and then the password. The password policy indicates the lowest and highest number of times a given letter must appear for the password to be valid. For example, 1-3 a means that the password must contain a at least 1 time and at most 3 times.
 * 
 * In the above example, 2 passwords are valid. The middle password, cdefg, is not; it contains no instances of b, but needs at least 1. The first and third passwords are valid: they contain one a or nine c, both within the limits of their respective policies.
 * 
 * How many passwords are valid according to their policies?
 * 
 * --- Part Two ---
 * 
 * While it appears you validated the passwords correctly, they don't seem to be what the Official Toboggan Corporate Authentication System is expecting.
 * 
 * The shopkeeper suddenly realizes that he just accidentally explained the password policy rules from his old job at the sled rental place down the street! The Official Toboggan Corporate Policy actually works a little differently.
 * 
 * Each policy actually describes two positions in the password, where 1 means the first character, 2 means the second character, and so on. (Be careful; Toboggan Corporate Policies have no concept of "index zero"!) Exactly one of these positions must contain the given letter. Other occurrences of the letter are irrelevant for the purposes of policy enforcement.
 * 
 * Given the same example list from above:
 * 
 *     1-3 a: abcde is valid: position 1 contains a and position 3 does not.
 *     1-3 b: cdefg is invalid: neither position 1 nor position 3 contains b.
 *     2-9 c: ccccccccc is invalid: both position 2 and position 9 contain c.
 * 
 * How many passwords are valid according to the new interpretation of the policies?
 * 
 *****************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

class PasswordEntry {

public: 
    PasswordEntry(std::string const &in) : m_raw(in) {

        // Just in case we need to not be case-sensitive
        //transform(m_raw.begin(), m_raw.end(), m_raw.begin(), ::tolower);

        size_t d1pos, d2pos, d3pos;
        d1pos = m_raw.find(m_delim1);
        d2pos = m_raw.find(m_delim2);
        d3pos = m_raw.find(m_delim3);

        std::string part1 = m_raw.substr(0, d1pos);
        std::string part2 = m_raw.substr(d1pos+1, d2pos - d1pos);
        std::string part3 = m_raw.substr(d2pos+1, 1);
        std::string part4 = m_raw.substr(d3pos+2, in.npos - d3pos);

        m_count_min = size_t(std::stoi(part1));
        m_count_max = size_t(std::stoi(part2));
        m_char = std::move(part3);
        m_pass = std::move(part4);
    }

    void print() {
        std::cout << "Character: " << m_char << std::endl <<
                    "Count: " << m_count_min << " to " << m_count_max << std::endl <<
                    "String: " << m_pass << std::endl <<
                    "Raw: " << m_raw << std::endl;
        if (m_count_actual != 0) {
            std::cout << "Actual: " << m_count_actual << std::endl;
        }
        std::cout << std::endl;

        
    }

    bool validate() {
        count();
        return ((m_count_actual >= m_count_min) && (m_count_actual <= m_count_max));
    }

private:
    size_t m_count_min;
    size_t m_count_max;
    std::string m_char;
    std::string m_pass;
    std::string m_raw;
    size_t m_count_actual = 0;

    const std::string m_delim1 = "-";
    const std::string m_delim2 = " ";
    const std::string m_delim3 = ": ";

    size_t count(void) {
        size_t _count=0;
        size_t _pos = 0;
        while (_pos != m_pass.npos) {
            _pos = m_pass.find(m_char, _pos);
            if (_pos != m_pass.npos) {
                _count++;
                _pos++;
            }
        }
        m_count_actual = _count;
        return m_count_actual;
    }
};

class NewPasswordEntry {

public: 
    NewPasswordEntry(std::string const &in) : m_raw(in) {

        // Just in case we need to not be case-sensitive
        //transform(m_raw.begin(), m_raw.end(), m_raw.begin(), ::tolower);

        size_t d1pos, d2pos, d3pos;
        d1pos = m_raw.find(m_delim1);
        d2pos = m_raw.find(m_delim2);
        d3pos = m_raw.find(m_delim3);

        std::string part1 = m_raw.substr(0, d1pos);
        std::string part2 = m_raw.substr(d1pos+1, d2pos - d1pos);
        std::string part3 = m_raw.substr(d2pos+1, 1);
        std::string part4 = m_raw.substr(d3pos+2, in.npos - d3pos);

        m_pos_1 = std::stoi(part1) - 1;
        m_pos_2 = std::stoi(part2) - 1;
        m_char = std::move(part3);
        m_pass = std::move(part4);
    }

    void print() {
        std::cout << "Character: " << m_char << std::endl <<
                    "Positions: " << m_pos_1 << " or " << m_pos_2 << std::endl <<
                    "String: " << m_pass << std::endl <<
                    "Raw: " << m_raw << std::endl;
        std::cout << std::endl;
    }

    bool validate() {
        return m_pass.at(m_pos_1) == *m_char.c_str() ? (
            m_pass.at(m_pos_2) == *m_char.c_str() ? false : true
        ) : (
            m_pass.at(m_pos_2) == *m_char.c_str() ? true : false
        );
    }
private:
    size_t m_pos_1;
    size_t m_pos_2;
    std::string m_char;
    std::string m_pass;
    std::string m_raw;

    const std::string m_delim1 = "-";
    const std::string m_delim2 = " ";
    const std::string m_delim3 = ": ";
};



int main(int argc, char **argv) {

    std::string inFilePath("input.txt");

    if (argc == 2) {
        inFilePath = argv[1];
    }
    else if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [input file (default ./input.txt)]" << std::endl;
        return 1;
    }

    std::cout << "Populating string vector from \"" << inFilePath << "\"" << std::endl;

    std::ifstream inFile(inFilePath);
    std::vector<std::string> entries;
    std::string s;

    while(std::getline(inFile, s)) {
        entries.push_back(s);
    }

    std::cout << "Read " << entries.size() << " lines." << std::endl;

    // Input is XX-YY C: [string]
    // Iterate through the strings and convert them to PasswordEntry

    std::cout << "Validating against old password policy" << std::endl;

    size_t pass_count = 0;
    size_t fail_count = 0;
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        PasswordEntry pe(*it);
        if (pe.validate()) {
            pass_count++;
        }
        else {
            fail_count++;
        }
    }
    std::cout << "Done. Found " << pass_count << " passes and " << fail_count << " failures." << std::endl;

    std::cout << "Validating against new password policy" << std::endl;

    pass_count = 0;
    fail_count = 0;

    for (auto it = entries.begin(); it != entries.end(); ++it) {
        NewPasswordEntry npe(*it);
        if (npe.validate()) {
            pass_count++;
        }
        else {
            fail_count++;
        }
    }
    std::cout << "Done. Found " << pass_count << " passes and " << fail_count << " failures." << std::endl;

}