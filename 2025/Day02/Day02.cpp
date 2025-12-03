#include <iostream>
#include <iomanip>
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
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef long long BigNumber;

class Range
{
    public:
        Range()
            : low_(0)
            , high_(0)
            , range_(high_-low_)
        { }

        Range(BigNumber low, BigNumber high)
            : low_(low)
            , high_(high)
            , range_(high_ - low_)
        { }

        BigNumber low()
        {
            return low_;
        }

        BigNumber high()
        {
            return high_;
        }

        BigNumber range()
        {
            return range_;
        }

        BigNumber low_;
        BigNumber high_;
        BigNumber range_;
};

typedef std::vector<Range> Ranges;


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


void readInputFile(std::string fileName, Ranges& ranges)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch (state)
            {
                case 1:
                {
                    // Read rotation
                    if (!line.empty())
                    {
                        auto strRanges = split(line, { "," });
                        for (auto& strRange : strRanges)
                        {
                            auto ids = split(strRange, { "-" });
                            if (ids.size() == 2)
                            {
                                Range range(std::stoll(ids[0]), std::stoll(ids[1]));
                                ranges.emplace_back(range);
                            }
                        }
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {

                    break;
                }
            }
        }
        myFile.close();
    }
}


bool isSilly1(BigNumber val)
{
    auto valAsString = std::to_string(val);
    
    auto strLen = valAsString.size();

    // Must have even number of digits
    if (strLen % 2 != 0)
    {
        return false;
    }

    auto str1 = valAsString.substr(0, strLen / 2);
    auto str2 = valAsString.substr(strLen / 2);

    if (str1 != str2)
    {
        return false;
    }

    return true;
}


bool isSilly2(BigNumber val)
{
    auto valAsString = std::to_string(val);
    auto strLen = valAsString.size();

    // search for substrings, no need to go beyond the half size of the original string
    for (int i = 1; i <= strLen / 2; i++)
    {
        // Only interested in situations where the original string length is an exact multiple of the substring length
        // I.e. the original string length is divisable by the substring length
        if (strLen % i == 0)
        {
            // take the first i character(s)
            auto str1 = valAsString.substr(0, i);

            bool found = true;

            // chop up the original string into substring sizes and check if we have a match with ALL substrings
            for (int j = i; j < strLen; j += i)
            {
                auto str2 = valAsString.substr(j, i);
                if (str1 != str2)
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                return true;
            }
        }
    }

    return false;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Ranges ranges;

    readInputFile(inputFileName, ranges);

    if (ranges.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            // Calculate the sum of all silly numbers
            BigNumber result = 0;

            for (auto& range : ranges)
            {
                for (BigNumber i = range.low(); i <= range.high(); i++)
                {
                    if (isSilly1(i))
                    {
                        result += i;
                    }
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Calculate the sum of all silly numbers
            BigNumber result = 0;

            for (auto& range : ranges)
            {
                for (BigNumber i = range.low(); i <= range.high(); i++)
                {
                    if (isSilly2(i))
                    {
                        result += i;
                    }
                }
            }
            std::cout << "Total: " << result;
            std::cout << std::endl;
         }

        std::cout << std::endl;
    }
}
