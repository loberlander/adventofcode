#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <set>

static const char inputFileName[] = "input.txt";
static int maxDeviceJolts = 0;
static long long unsigned int total = 0;
static int ratingLimit = 3;


struct Adapter
{
public:
    Adapter(int jRating, long long unsigned int pWays) :
        m_joltRating(jRating),
        m_pathWays(pWays)
    {}

public:
    int joltRating() const { return m_joltRating; }
    long long unsigned int pathWays() const { return m_pathWays; }

    int m_joltRating;
    mutable long long unsigned int m_pathWays;
};

struct CustomCompare
{
    // this member is required to let container be aware that 
    // comparator is capable of dealing with types other than key
    public: using is_transparent = std::true_type;

//    bool operator()(const int& lhs, const int& rhs) const
//    {
//        return lhs < rhs;
//    }

    bool operator()(const int& lhs, const Adapter& rhs) const
    {
        return lhs < rhs.joltRating();
    }

    bool operator()(const Adapter& lhs, const int& rhs) const
    {
        return lhs.joltRating() < rhs;
    }

    bool operator()(const Adapter& lhs, const Adapter& rhs) const
    {
        return lhs.joltRating() < rhs.joltRating();
    }
};

// 1, 2, 3 jolts lower to produce rated jolts output in input.txt
// device rating is 3 jolts higher than max adapter

std::set<Adapter, CustomCompare> readInputFile(std::string fileName)
{
    std::set<Adapter, CustomCompare> data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            data.emplace(Adapter(std::stoi(line), 0));
        }
        myFile.close();
    }

    return data;
}

std::vector<int> countJoltDiffs(std::set<Adapter, CustomCompare>& data)
{
    std::vector<int> diffs = {0, 0, 0}; // 1 diff, 2 difff, 3 diff

    // The start and end values are really not part of the set...
    int startJolts;
    int endJolts;
    auto it = data.begin();
    startJolts = it->joltRating();
    int currentJolts = startJolts;
    ++it;

    auto endIt = data.rbegin();
    ++endIt;
    endJolts = (endIt)->joltRating();

    while (currentJolts < endJolts)
    {
        switch(it->joltRating() - currentJolts)
        {
        case 1:
            ++diffs[0];
            break;
        case 2:
            ++diffs[1];
            break;
        case 3:
            ++diffs[2];
            break;
        default:
            break;
        }
        currentJolts = it->joltRating();
        ++it;
    }
    ++diffs[2];
    return diffs;
}


long long unsigned int countUniqueWaysBruteForce(std::set<Adapter, CustomCompare>& data, int startJolts, int endJolts, std::string path)
{
    long long unsigned int uniqueWays = 0;

    for(int i = 1; i <= 3; i++)
    {
        auto it = data.find(startJolts + i);
        if (it != data.end())
        {
            if (it->joltRating() == endJolts)
            {
                std::cout << path << ":" << std::to_string(it->joltRating()) << std::endl;
                ++total;
                return 1;
            }
            else
            {
                uniqueWays += countUniqueWaysBruteForce(data, it->joltRating(), endJolts, path + ":" + std::to_string(it->joltRating()));
            }
        }
    }
    return uniqueWays;
}


long long unsigned int countUniqueWays(std::set<Adapter, CustomCompare>& data)
{
    long long unsigned int uniqueWays = 0;

    auto dat = data; // copy the data...

    auto currentIt = dat.begin();

    while (currentIt != dat.end())
    {
        if (currentIt == dat.begin())
        {
            currentIt->m_pathWays = 1; // must start somewhere
        }
        else
        {
            auto itM1 = currentIt;
            --itM1;
            for (auto it = itM1; it != dat.end(); it--)
            {
                if (currentIt->joltRating() - it->joltRating() <= ratingLimit)
                {
                    currentIt->m_pathWays = currentIt->pathWays() + it->pathWays();
                }
                else
                {
                    break;
                }
            }
        }
        ++currentIt;
    }

    auto endIt = dat.rbegin();
    uniqueWays = endIt->pathWays();

    return uniqueWays;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::set<Adapter, CustomCompare> data = readInputFile(inputFileName);

    maxDeviceJolts = data.rbegin()->joltRating() + 3;
    data.emplace(Adapter(0, 0)); // Add the initial outlet
    data.emplace(Adapter(maxDeviceJolts, 0)); // Add the device

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;

        std::vector<int> diffs = countJoltDiffs(data);
        int magicNumber = diffs[0] * diffs[2];
        std::cout << "Today's magic number is " << magicNumber << std::endl;

        std::cout << "Part 2:" << std::endl;
        //long long unsigned int uniqueWays = countUniqueWaysBruteForce(data, 0, maxDeviceJolts, "");
        long long unsigned int uniqueWays = countUniqueWays(data);
        std::cout << "Unique ways : " << uniqueWays << std::endl;
    }
}
