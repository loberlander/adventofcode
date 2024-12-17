#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const std::string delimiter1 = ",";
static const std::string placeholder = "x";

struct Bus
{
    Bus(int id, int frequency, int position) :
        m_id(id),
        m_frequency(frequency),
        m_position(position),
        m_timeTillNext(0)
    {}
    int m_id;
    int m_frequency;
    int m_position;
    int m_timeTillNext;
};

int parseLine1(std::string line)
{
    int departureTime = std::stoi(line);

    return departureTime;
}

struct Schedule
{
    int departureTime;
    std::vector<struct Bus> buses;
};

std::vector<struct Bus> parseLine2(std::string line)
{
    std::vector<struct Bus> buses;

    size_t tokenPosition = 0;
    int position = 0;

    while (!line.empty())
    {
        // Parse the next parameter
        tokenPosition = line.find(delimiter1, 0);
        if (tokenPosition == std::string::npos)
        {
            // must be last element, take that there is...
            tokenPosition = line.size();
        }

        std::string element = line.substr(0, tokenPosition);
        line.erase(0, tokenPosition + delimiter1.length());

        if (element != placeholder)
        {
            int value = std::stoi(element);
            buses.emplace_back(value, value, position);
        }
        ++position;
    }

    return buses;
}

struct Schedule readInputFile(std::string fileName)
{
    struct Schedule data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int i = 1;
        while (getline(myFile, line))
        {
            switch(i)
            {
            case 1:
                data.departureTime = parseLine1(line);
                break;
            case 2:
                data.buses = parseLine2(line);
                break;
            default:
                // ignore if anything
                break;
            }
            ++i;
        }
        myFile.close();
    }

    return data;
}

// https://rosettacode.org/wiki/Chinese_remainder_theorem#C.2B.2B
// returns x where (a * x) % b == 1
long long mul_inv(long long a, long long b)
{
     long long b0 = b, t, q;
     long long x0 = 0, x1 = 1;
    if (b == 1) return 1;
    while (a > 1) {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }
    if (x1 < 0) x1 += b0;
    return x1;
}

long long chinese_remainder(long long* n, long long* a, long long len)
{
    long long p, i, prod = 1, sum = 0;

    for (i = 0; i < len; i++) prod *= n[i];

    for (i = 0; i < len; i++) {
        p = prod / n[i];
        sum += a[i] * mul_inv(p, n[i]) * p;
    }

    return sum % prod;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    struct Schedule data = readInputFile(inputFileName);

    if (data.buses.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        int magicNumber;
        int lowestWait = INT_MAX;
        int lowestBus;

        for (auto & bus : data.buses)
        {
            bus.m_timeTillNext = bus.m_frequency - (data.departureTime % bus.m_frequency);
            if (bus.m_timeTillNext < lowestWait)
            {
                lowestWait = bus.m_timeTillNext;
                lowestBus = bus.m_id;
                magicNumber = lowestWait * lowestBus;
            }
        }

        std::cout << "Part 1:" << std::endl;
        std::cout << "Today's magic number is: " << magicNumber << std::endl;

        // https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/ 
        // https://www.geeksforgeeks.org/chinese-remainder-theorem-set-1-introduction/
        // https://www.geeksforgeeks.org/chinese-remainder-theorem-set-2-implementation/
        // http://mathforum.org/library/drmath/view/64439.html
        //
        // https://rosettacode.org/wiki/Chinese_remainder_theorem#C.2B.2B
        // Chinese Remainder Theorem
        std::cout << "Part 2:" << std::endl;
        // 0  1  2  3  4  5  6   7  8  9  10 11 12  13 14  15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31  32 33 34 35  36 37 38 39 40 41 42 43   44 45 46 47 48 49 50 51 52 53  54 55 56 57 58 59 60  61
        //13, x, x, x, x, x, x, 37, x, x, x, x, x, 449, x, 29, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, x, 19, x, x, x, 23, x, x, x, x, x, x, x, 773, x, x, x, x, x, x, x, x, x, 41, x, x, x, x, x, x, 17
        long long num[] = { 13,   37,    449,    29,    19,    23,    773,    41,    17 };
        long long rem[] = { 0,  37-7, 449-13, 29-15, 19-32, 23-36, 773-44, 41-54, 17-61 };
        //long long num[] = { 1789, 37, 47, 1889 };
        //long long rem[] = { 0, 36, 45, 1886 };
        //unsigned long long num[] = { 67, 7, 59, 61 };
        //unsigned long long rem[] = { 0, 6, 56, 57 };
        //long long num[] = { 7, 13, 59, 31, 19 };
        //long long rem[] = { 0, 12, 55, 25, 12 };
        //unsigned long long num[] = { 17, 13, 19 };
        //unsigned long long rem[] = { 0, 11, 16 };
        //unsigned long long k = sizeof(num) / sizeof(num[0]);
        //unsigned long long earliestTimestamp = findMinX(num, rem, k);
        long long earliestTimestamp = chinese_remainder(num, rem, sizeof(num) / sizeof(num[0]));

        std::cout << "Earliest time stamp: " << earliestTimestamp << std::endl;
    }
}
