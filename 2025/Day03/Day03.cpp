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

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "input_test.txt";
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

    BigNumber position_;
    BigNumber joltage_;
};

typedef std::multimap<BigNumber, Battery> BatteryBank; // key: joltage, value: Battery
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
            // Calculate the sum of all silly numbers
            BigNumber result = 0;

            for (auto& batteryBank : batteryBanks)
            {
                BigNumber joltage = 0;
                size_t numBatteries = batteryBank.size();
                Battery battery1;
                Battery battery2;

                // pick the first battery with the higest joltage as long as it is not the last battery in the bank
                // walk through the sorted map elements
                for (auto& battery : batteryBank)
                {
                    if (battery.second.position_ < numBatteries - 1)
                    {
                        battery1 = battery.second;
                        break;
                    }
                }

                // pick the second battery with the higest joltage as long as it is located after the first battery
                // walk through the sorted map elements
                for (auto& battery : batteryBank)
                {
                    if (battery.second.position_ > battery1.position_)
                    {
                        battery2 = battery.second;
                        break;
                    }
                }

                joltage = 10 * battery1.joltage_ + battery2.joltage_;
                result += joltage;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Calculate the sum of all silly numbers
            BigNumber result = 0;


            std::cout << "Total: " << result;
            std::cout << std::endl;
         }

        std::cout << std::endl;
    }
}
