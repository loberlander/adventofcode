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
#include <ranges> // For std::views::reverse was added in C++20

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";

typedef long long BigNumber;

class Battery
{
    public:
        Battery()
            : position_(0)
            , joltage_(0)
        { }

        Battery(BigNumber position, BigNumber joltage)
            : position_(position)
            , joltage_(joltage)
        { }

        BigNumber position()
        {
            return position_;
        }

        BigNumber joltage()
        {
            return joltage_;
        }

    private:
        BigNumber position_;
        BigNumber joltage_;
};


struct AscendingBigNumberComparator
{
    bool operator()(const BigNumber& lhs, const BigNumber& rhs) const
    {
        // lhs is less return true
        // otherwise (rhs is less) return false
        if (lhs < rhs)
        {
            return true;
        }

        return false;
    }
};


struct DescendingBigNumberComparator
{
    bool operator()(const BigNumber& lhs, const BigNumber& rhs) const
    {
        // rhs is less return true
        // otherwise (lhs is less) return false
        if (lhs > rhs)
        {
            return true;
        }

        return false;
    }
};


// Use a multimap, so that batteries are sorted in ascending order based on their joltage and allow for multiple keys to be present
typedef std::multimap<BigNumber, Battery> BatteryBank; // key: joltage, value: Battery

// Alternatively, use a custom comparator that sorts in descending order to avoid reverse order searching with std::views::reverse
//typedef std::multimap<BigNumber, Battery, DescendingBigNumberComparator> BatteryBank; // key: joltage, value: Battery

typedef std::vector<BatteryBank> BatteryBanks;


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


void readInputFile(std::string fileName, BatteryBanks& batteryBanks)
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
                    // Read each battery
                    if (!line.empty())
                    {
                        BatteryBank batteryBank;
                        BigNumber position = 0;

                        for (auto& ch : line)
                        {
                            BigNumber joltage = std::stoll(std::string(1, ch));
                            Battery battery(position, joltage);
                            batteryBank.emplace(joltage, battery);
                            ++position;
                        }

                        batteryBanks.emplace_back(batteryBank);
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

BigNumber findJoltage(BatteryBank& batteryBank, BigNumber batteriesNeeded)
{
    BigNumber joltage = 0;
    size_t numBatteries = batteryBank.size();
    Battery batteryLast(-1, 0); // Position must be less than the lowest possible value

    for (BigNumber batteriesLeft = batteriesNeeded; batteriesLeft > 0; batteriesLeft--)
    {
        Battery batteryPick(numBatteries, 0); // Set position beyond max value, set joltage to below lowest value

        for (auto& battery : batteryBank | std::views::reverse) // use this when the batteries are sorted in ascending order and C++20 features (std::views::reverse) are available
        //for (auto& battery : batteryBank) // use this when the batteries are sorted in descending order
        //for (auto i t= batteryBank.begin(); it != batteryBank.end(); it++) // // use this when the batteries are sorted in descending order and no c++20 features are available
        //for (auto it = batteryBank.rbegin(); it != batteryBank.rend(); it++) // use this when the batteries are sorted in ascending order and no c++20 features are available
        {
            //auto battery = *it; // use this when iterator based looping is used
            // Pick the next battery that is located after the last pick AND before the end (i.e. must have enough batteries left to fulfill all batteries)
            if (batteryLast.position() < battery.second.position() &&
                battery.second.position() <= (BigNumber)(numBatteries - batteriesLeft))
            {
                if (battery.second.joltage() >= batteryPick.joltage())
                {
                    if (battery.second.position() < batteryPick.position())
                    {
                        batteryPick = battery.second;
                    }
                }
                else
                {
                    // Already found the highest joltage, it is safe to exit the search
                    break;
                }
            }
        }

        joltage = 10 * joltage + batteryPick.joltage();
        batteryLast = batteryPick;
    }

    return joltage;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    BatteryBanks batteryBanks;

    readInputFile(inputFileName, batteryBanks);

    if (batteryBanks.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

            for (auto& batteryBank : batteryBanks)
            {
                BigNumber joltage = 0;

                joltage = findJoltage(batteryBank, 2);

                result += joltage;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            for (auto& batteryBank : batteryBanks)
            {
                BigNumber joltage = 0;

                joltage = findJoltage(batteryBank, 12);

                result += joltage;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
         }

        std::cout << std::endl;
    }
}
