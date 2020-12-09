#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>


template <class T>
class FixedSizeQueue {
public:
    FixedSizeQueue(size_t size) :
    m_size(size) {};

    size_t size() {return m_size;}
    void setSize(size_t size) {m_size = size;}


    void addToQueue(T in) {
        m_queue.push_front(in);
        if (m_queue.size() > m_size) {
            m_queue.pop_back();
        }
    }

    void addAndResize(T in) {
        m_queue.push_front(in);
        if (m_queue.size() > m_size) {
            m_size = m_queue.size();
        }
    }

    std::deque<T> &getQueue() {return m_queue;}

private:
    std::deque<T> m_queue;
    size_t m_size;
};

int main(int argc, char** argv) {
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

    std::cout << "Read " << lines.size() << " lines." << std::endl;

    int windowSize;
    windowSize = std::stoi(lines.front());
    lines.erase(lines.begin());
    std::cout << "Window size is " << windowSize << " . Lines now: " << lines.size() << std::endl;

    size_t sumsSize = ((windowSize * (windowSize+1))/2);
    // Keep this on the stack.
    FixedSizeQueue<long int> nums(windowSize);

    FixedSizeQueue<FixedSizeQueue<long int>> queues(windowSize - 1);

    for (size_t i = queues.size(); i >=1 ; i--) {
        queues.addToQueue(FixedSizeQueue<long int>(i));
        for (size_t j = 0; j < queues.getQueue().front().size(); j++) {
            queues.getQueue().front().addToQueue(0);
        }
        nums.addToQueue(0);
    }

    // When number is inserted
    // push in new queue of size 1.
    // Resize other queues.
    size_t count = 0;

    long int weakness = 0;

    for (auto &itr : lines) {

        auto num = std::stoll(itr);
        // add zero to the numbers queue.
        // Simplifies the adding bit later
        nums.addToQueue(num);
        auto numq = nums.getQueue();

        bool found = false;
        if (count >= windowSize) {
            // start calculating stuff...
            for (auto &outer : queues.getQueue()) {
                if (std::find(outer.getQueue().begin(),
                        outer.getQueue().end(),
                            num) != outer.getQueue().end())
                    {
                            std::cout << "Found " << num << " in sums" << std::endl;
                            found = true;
                            break;
                    }
                }
                if (!found) {
                    std::cout << "END: " << num << std::endl;
                    weakness = num;
                    break;
                }
            }
        
        FixedSizeQueue<long int> newq(1);
        size_t i = queues.size();
        queues.addToQueue(newq);
        //for (auto &qitr : queues.getQueue()) {
        for (auto qitr = queues.getQueue().rbegin(); qitr != queues.getQueue().rend(); qitr++) {

            if (qitr != queues.getQueue().rend())
            {
                qitr->addAndResize(num + numq[i]);
                i--;
            }
            else {
                qitr->addToQueue(num);
            }
        }
        // cheeky stuff
        count++;
    }

    // Part 2 - find a set of contiguous numbers that adds up to the number.
    long int runningTotal = 0;
    long int exploitRange = 0;
    long int rngmin = 0;
    long int rngmax = 0;
    
    int i = 0;
    for (auto &itr : lines) {
        auto num = std::stol(itr);
        // just be dumb and get it finished.
        if (num < weakness) {
            auto index = i;
            auto endIndex = i;
            while(runningTotal < weakness) {
                auto n = std::stol(lines[endIndex]);
                runningTotal += n;
                if (index == endIndex) {
                    rngmin = n;
                    rngmax = n;
                }
                else {
                    if (n > rngmax) {
                        rngmax = n;
                    }
                    else if (n < rngmin) {
                        rngmin = n;
                    }
                }
                endIndex++;
            }
            if (runningTotal == weakness) {
                std::cout  << "Found the weakness:  Range " << index << " " << endIndex << std::endl;
                exploitRange = std::stol(lines[index]) + std::stol(lines[endIndex]);
                std::cout << "MIN " << rngmin << " + MAX " << rngmax << " = " <<  rngmin + rngmax << std::endl;
                break;
            }
            else {
                runningTotal = 0;
            }
        }

        i++;
    }
}
