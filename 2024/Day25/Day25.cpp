#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef std::vector<std::string> Map;
typedef std::string Moves;

typedef long long BigNumber;
typedef std::vector<BigNumber> BigNumbers;

typedef std::map<std::string, BigNumber> ValueMap;


class Key
{
public:
    Key(std::vector<int> pattern)
        : pattern_(pattern)
    { }
    std::vector<int> pattern_;
};

typedef std::vector<Key> Keys;

class Lock
{
public:
    Lock(std::vector<int> pattern)
        : pattern_(pattern)
    {
    }
    std::vector<int> pattern_;
};

typedef std::vector<Lock> Locks;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::vector<std::string> delimiters)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;
    if (delimiters.empty())
    {
        return result;
    }

    int delimIndex = 0;

    while (!line.empty())
    {
        std::string delimiter = delimiters[delimIndex];
        
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());

                delimIndex = delimIndex < delimiters.size() - 1 ? delimIndex + 1 : delimIndex;
                delimiter = delimiters[delimIndex];
                tokenPosition = line.find(delimiter);
            }
        }

        if (tokenPosition != std::string::npos)
        {
            result.emplace_back(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition);
        }
        else
        {
            // Must be last element in the line
            if (!line.empty())
            {
                result.emplace_back(line);
                line.clear();
            }
        }
    }

    return result;
}


void readInputFile(std::string fileName, Locks& locks, Keys& keys)
{
    std::string line;
    std::ifstream myFile(fileName);
    std::vector<std::string> patterns;

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read constants
                    if (!line.empty())
                    {
                        patterns.push_back(line);

                        if (patterns.size() == 7)
                        {
                            bool isLock = false;

                            std::vector<int> pattern;
                            for (int x = 0; x < 5; x++)
                            {
                                int count = 0;
                                count = patterns[1][x] == '#' ? count + 1 : count;
                                count = patterns[2][x] == '#' ? count + 1 : count;
                                count = patterns[3][x] == '#' ? count + 1 : count;
                                count = patterns[4][x] == '#' ? count + 1 : count;
                                count = patterns[5][x] == '#' ? count + 1 : count;
                                pattern.emplace_back(count);
                            }

                            if (patterns[0] == "#####")
                            {
                                locks.emplace_back(pattern);
                            }
                            else
                            {
                                keys.emplace_back(pattern);
                            }

                            patterns.clear();
                        }
                    }
                    else
                    {

                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}

bool isFit(Lock& lock, Key key)
{
    if (lock.pattern_.size() != key.pattern_.size())
    {
        return false;
    }

    for (int i = 0; i < lock.pattern_.size(); i++)
    {
        if (5 - lock.pattern_[i] < key.pattern_[i])
        {
            return false;
        }
    }

    return true;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Locks locks;
    Keys keys;

    readInputFile(inputFileName, locks, keys);

    if (keys.empty() || locks.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        { 
            BigNumber sum = 0;

            for (auto& key : keys)
            {
                for (auto& lock : locks)
                {
                    sum = isFit(lock, key) ? sum + 1 : sum;
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
