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

class Rotation
{
    public:
        Rotation()
            : amount_(0)
            , direction_(0)
        { }

        Rotation(BigNumber amount, BigNumber direction)
            : amount_(amount)
            , direction_(direction)
        { }

        BigNumber amount()
        {
            return amount_;
        }

        BigNumber direction()
        {
            return direction_;
        }

        BigNumber move()
        {
            return direction_ * amount_;
        }

    private:
        BigNumber amount_;
        BigNumber direction_;
};

typedef std::vector<Rotation> Rotations;


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


void readInputFile(std::string fileName, Rotations& rotations)
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
                        BigNumber direction = line[0] == 'L' ? -1 : 1;
                        line.erase(0, 1);

                        BigNumber value = std::stoll(line);
                        Rotation rotation(value, direction);

                        rotations.emplace_back(rotation);
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

    Rotations rotations;

    readInputFile(inputFileName, rotations);

    if (rotations.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            // Count the number of times zero is pointed to at the end of each rotation
            BigNumber result = 0;
            BigNumber value = 50;

            for (auto& rotation : rotations)
            {
                // modulo division of a negative number will also be negative!
                // make it positive: add 100 and mod 100 again
                value = (((value + rotation.move()) % 100) + 100) % 100;
                if (value == 0)
                {
                    ++result;
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Count the number of times zero is passed over
            BigNumber result = 0;
            BigNumber value = 50;

/*
            // Brute force
            for (auto& rotation : rotations)
            {
                BigNumber amount = rotation.amount();
                for (int i = 0; i < amount; i++)
                {
                    // modulo division of a negative number will also be negative!
                    // make it positive: add 100 and mod 100 again
                    value = (((value + rotation.direction()) % 100) + 100) % 100;
                    if (value == 0)
                    {
                        ++result;
                    }
                }
            }
*/

            // Intelligent
            for (auto& rotation : rotations)
            {
                auto move = rotation.amount();
                auto rotDiv = move / 100;
                auto rotMod = move % 100;
                result += rotDiv;
                if (rotation.direction() > 0)
                {
                    if (value + rotMod >= 100)
                    {
                        ++result;
                    }
                }
                else
                {
                    if (value != 0 && value - rotMod <= 0)
                    {
                        ++result;
                    }
                }

                // modulo division of a negative number will also be negative!
                // make it positive: add 100 and mod 100 again
                value = (((value + rotation.move()) % 100) + 100) % 100;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
         }

        std::cout << std::endl;
    }
}
