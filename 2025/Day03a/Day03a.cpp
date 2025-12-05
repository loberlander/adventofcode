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

        BigNumber position() const
        {
            return position_;
        }

        BigNumber joltage() const
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


struct DescendingBatteryComparator
{
    bool operator()(const Battery& lhs, const Battery& rhs) const
    {
        // rhs is less return true
        // otherwise (lhs is less) return false
        if (lhs.joltage() == rhs.joltage())
        {
            if (lhs.position() < rhs.position())
            {
                return true;
            }
            return false;
        }

        if (lhs.joltage() > rhs.joltage())
        {
            return true;
        }

        return false;
    }
};


// Use a custom comparator that sorts in descending order based on the joltage and the position (when the joltage is the same)
typedef std::multiset<Battery, DescendingBatteryComparator> BatteryBank; // key: Battery(joltage, then position)

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
                            batteryBank.emplace(position, joltage);
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

        for (auto& battery : batteryBank) // use this when the batteries are sorted in descending order
        {
            // Pick the next battery that is located after the last pick AND before the end (i.e. must have enough batteries left to fulfill all batteries)
            if (batteryLast.position() < battery.position() &&
                battery.position() <= (BigNumber)(numBatteries - batteriesLeft))
            {
                // Already found the highest joltage, it is safe to exit the search
                batteryPick = battery;
                break;
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
