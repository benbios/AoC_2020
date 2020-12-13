#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <map>


long mod(long a, long b) {
    long ret = a%b;
    if (ret < 0) {
        ret += b;
    }
    return ret;
}

long mul_inv(long a, long b)
{
	long b0 = b, t, q;
	long x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		//t = b, b = mod(a,b) , a = t;
		t = b, b = a%b , a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}
 

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
    int timestamp = std::stoi(lines.front());

    auto buses = lines.back();
    size_t dpos;
    std::vector<int> busIds;
    size_t fullcount = 0;
    while (true) {
        dpos = buses.find(',');
    
        try {
            auto n = std::stoi(buses.substr(0, dpos));
            busIds.push_back(n);
            std::cout << "Added " << n << std::endl;
        }
        catch (std::invalid_argument e) {
            std::cerr << e.what() << std::endl;
        }
        fullcount++;
        if (dpos == std::string::npos)
            break;
        buses = buses.substr(dpos+1);
    }

    std::cout << "timestamp: " << timestamp << std::endl;
    int firstbus = busIds.front();
    int waittime = firstbus - (timestamp % firstbus);
    for (auto b : busIds) {
        auto bd = timestamp % b;
        // looking for smallest difference in bd and b
        std::cout << "b - bd: " << b - bd << std::endl;
        if ((b - bd) < waittime) {
            firstbus = b;
            waittime = b - bd;
        }
    }

    std::cout << "Part 1:" << std::endl;
    std::cout << "First bus is: " << firstbus << std::endl;
    std::cout << "Wait time is: " << waittime << std::endl;

    std::cout << "Product: " << firstbus * waittime << std::endl;

    // part 2
    // Trying the Chinese remainder theorem

    long max = 0;
    for (auto z : busIds) {
        if (z > max)
            max = z;
    }
    std::vector<long> _a, _n, _m, _p;
    buses = lines.back();
    int mpindex = 0;
    while (true) {
        dpos = buses.find(',');
        try {
            auto n = std::stol(buses.substr(0, dpos));

            std::cout << "i: " << mpindex << " | a: " << n-mpindex << " | n:" << n << std::endl;
            //_a.push_back(mod((n-mpindex),n));
            _a.push_back(-mpindex);
            _n.push_back(n);
        }
        catch (std::invalid_argument e) {
            // nop
        }
        if (dpos == std::string::npos)
            break;
        buses = buses.substr(dpos+1);
        mpindex++;
    }

    size_t size = _n.size();
    long M = 1;
    for (auto z = 0; z < size; z++ ) {
        M *= _n[z];
    }

    long Y = 0;

    for (auto z = 0; z < size; z++) {
        _m.push_back(M/_n[z]);
        _p.push_back(mul_inv(_m[z],_n[z]));
        std::cout << z << ": a=>" << _a[z] << " n=>" << _n[z] << " m=>" << _m[z] << " m'=>" << _p[z] << std::endl;
        Y += (_a[z] * _m[z] * _p[z]);
    }

    std::cout << "Part 2: " << std::endl;
    std::cout << "M: " << M << std::endl;

    Y = mod(Y,M);

    std::cout << "Y: " << Y << std::endl;

}
