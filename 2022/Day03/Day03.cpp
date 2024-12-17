#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";

std::map<char, int> priorities =
{
    {'a',  1},
    {'b',  2},
    {'c',  3},
    {'d',  4},
    {'e',  5},
    {'f',  6},
    {'g',  7},
    {'h',  8},
    {'i',  9},
    {'j', 10},
    {'k', 11},
    {'l', 12},
    {'m', 13},
    {'n', 14},
    {'o', 15},
    {'p', 16},
    {'q', 17},
    {'r', 18},
    {'s', 19},
    {'t', 20},
    {'u', 21},
    {'v', 22},
    {'w', 23},
    {'x', 24},
    {'y', 25},
    {'z', 26},

    {'A', 27},
    {'B', 28},
    {'C', 29},
    {'D', 30},
    {'E', 31},
    {'F', 32},
    {'G', 33},
    {'H', 34},
    {'I', 35},
    {'J', 36},
    {'K', 37},
    {'L', 38},
    {'M', 39},
    {'N', 40},
    {'O', 41},
    {'P', 42},
    {'Q', 43},
    {'R', 44},
    {'S', 45},
    {'T', 46},
    {'U', 47},
    {'V', 48},
    {'W', 49},
    {'X', 50},
    {'Y', 51},
    {'Z', 52},
};

class Sack
{
public:
    Sack(std::string sack)
    : sack_(sack)
    {
        size_t len = sack_.length();
        comp1_ = sack_.substr(0, (len / 2));
        comp2_ = sack_.substr(len / 2, len / 2);
    }

    int countPriority()
    {
        for (auto& ch : comp1_)
        {
            if (comp2_.find_first_of(ch) != std::string::npos)
            {
                return priorities[ch];
            }
        }
        return 0;
    }

    int findBadge(Sack& sack1, Sack& sack2)
    {
        for (auto& ch : sack_)
        {
            if ((sack1.sack_.find_first_of(ch) != std::string::npos) && (sack2.sack_.find_first_of(ch) != std::string::npos))
            {
                return priorities[ch];
            }
        }
        return 0;
    }

    std::string sack_;
    std::string comp1_;
    std::string comp2_;
};


typedef std::vector<Sack> Sacks;

void readInputFile(std::string fileName, Sacks& data)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                // Add new elf's data
                data.emplace_back(line);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Sacks data;
    readInputFile(inputFileName, data);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int sum1 = 0;
        for (auto& sack: data)
        {
            sum1 += sack.countPriority();
        }
        std::cout << "Sum is: " << sum1 << std::endl;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        int sum2 = 0;
        for (int i = 0; i < data.size(); i = i + 3)
        {
            sum2 += data[i].findBadge(data[i+1], data[i+2]);
        }
        std::cout << "Sum is: " << sum2 << std::endl;

    }
}
