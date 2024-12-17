#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <map>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

typedef std::vector<std::string> Data;


typedef std::multimap<std::string, int> Gears;// gear ID: to_string(x) + ":" + to_string(y), partnumber

bool isSymbol(char c)
{
    if (std::isdigit(c))
    {
        return false;
    }

    if (c == '.')
    {
        return false;
    }

    return true;
}

bool isGear(char c)
{
    if (c == '*')
    {
        return true;
    }

    return false;
}

bool isNextToSymbol(Data& data, int x, int y)
{
    size_t maxX = data[0].length() - 1;
    size_t maxY = data.size() - 1;

    // (x+1, y)
    if (x+1 <= maxX && isSymbol(data[y][x + 1]))
    {
        return true;
    }

    // (x-1, y)
    if (x - 1 >= 0 && isSymbol(data[y][x - 1]))
    {
        return true;
    }

    // (x, y+1)
    if (y + 1 <= maxY && isSymbol(data[y + 1][x]))
    {
        return true;
    }

    // (x, y-1)
    if (y - 1 >= 0 && isSymbol(data[y - 1][x]))
    {
        return true;
    }

    // (x+1, y+1)
    if (x + 1 <= maxX && y + 1 <= maxY && isSymbol(data[y + 1][x + 1]))
    {
        return true;
    }

    // (x+1, y-1)
    if (x + 1 <= maxX && y - 1 >= 0 && isSymbol(data[y - 1][x + 1]))
    {
        return true;
    }

    // (x-1, y-1)
    if (x - 1 >= 0 && y - 1 >= 0 && isSymbol(data[y - 1][x - 1]))
    {
        return true;
    }

    // (x-1, y+1)
    if (x - 1 >= 0 && y + 1 <= maxY && isSymbol(data[y + 1][x - 1]))
    {
        return true;
    }

    return false;
}

bool isNextToGear(Data& data, int x, int y, int& gearX, int& gearY)
{
    size_t maxX = data[0].length() - 1;
    size_t maxY = data.size() - 1;

    // (x+1, y)
    if (x + 1 <= maxX && isGear(data[y][x + 1]))
    {
        gearX = x + 1;
        gearY = y;
        return true;
    }

    // (x-1, y)
    if (x - 1 >= 0 && isGear(data[y][x - 1]))
    {
        gearX = x - 1;
        gearY = y;
        return true;
    }

    // (x, y+1)
    if (y + 1 <= maxY && isGear(data[y + 1][x]))
    {
        gearX = x;
        gearY = y + 1;
        return true;
    }

    // (x, y-1)
    if (y - 1 >= 0 && isGear(data[y - 1][x]))
    {
        gearX = x;
        gearY = y - 1;
        return true;
    }

    // (x+1, y+1)
    if (x + 1 <= maxX && y + 1 <= maxY && isGear(data[y + 1][x + 1]))
    {
        gearX = x + 1;
        gearY = y + 1;
        return true;
    }

    // (x+1, y-1)
    if (x + 1 <= maxX && y - 1 >= 0 && isGear(data[y - 1][x + 1]))
    {
        gearX = x + 1;
        gearY = y - 1;
        return true;
    }

    // (x-1, y-1)
    if (x - 1 >= 0 && y - 1 >= 0 && isGear(data[y - 1][x - 1]))
    {
        gearX = x - 1;
        gearY = y - 1;
        return true;
    }

    // (x-1, y+1)
    if (x - 1 >= 0 && y + 1 <= maxY && isGear(data[y + 1][x - 1]))
    {
        gearX = x - 1;
        gearY = y + 1;
        return true;
    }

    return false;
}

void readInputFile(std::string fileName, Data& data)
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

bool isLastDigit(Data& data, int x, int y)
{
    size_t maxX = data[0].length() - 1;

    if (x == maxX)
    {
        return true;
    }
    else if (!std::isdigit(data[y][x + 1]))
    {
        return true;
    }

    return false;
}

void accumulate(long long int& sum, int value)
{
    sum += value;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Data data;
    Gears gears;
    readInputFile(inputFileName, data);


    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            long long int sum = 0;

            size_t maxX = data[0].length() - 1;
            size_t maxY = data.size() - 1;

            for (int y = 0; y <= maxY; y++)
            {
                int partNumber = 0;
                int gearX = 0;
                int gearY = 0;
                bool isPartNumber = false;
                bool isConnectedToGear = false;

                for (int x = 0; x <= maxX; x++)
                {
                    if (std::isdigit(data[y][x]))
                    {
                        int digit = data[y][x] - '0';
                        // first digit
                        if (partNumber == 0)
                        {
                            partNumber = digit;
                        }
                        else
                        {
                            partNumber = partNumber * 10 + digit;
                        }

                        isPartNumber |= isNextToSymbol(data, x, y);
                        isConnectedToGear |= isNextToGear(data, x, y, gearX, gearY);
                        if (isLastDigit(data, x, y))
                        {
                            if (isPartNumber)
                            {
                                accumulate(sum, partNumber);
                            }

                            if (isConnectedToGear)
                            {
                                gears.emplace(std::to_string(gearX) + ":" + std::to_string(gearY), partNumber);
                            }

                            partNumber = 0;
                            isPartNumber = false;

                            isConnectedToGear = false;
                            gearX = 0;
                            gearY = 0;
                        }
                    }
                }
            }

            std::cout << "Total sum of part numbers: " << sum;
            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {
            long long int sum = 0;
            std::string key1;
            std::string key2;
            int part1;
            int part2;

            for (auto& gear : gears)
            {
                if (key1.empty())
                {
                    key1 = gear.first;
                    part1 = gear.second;
                }
                else
                {
                    if (key2.empty())
                    {
                        key2 = gear.first;
                        part2 = gear.second;

                        if (key1 == key2)
                        {
                            accumulate(sum, part1 * part2);
                            key1.clear();
                            part1 = 0;
                            key2.clear();
                            part2 = 0;
                        }
                        else
                        {
                            key1 = key2;
                            part1 = part2;
                            key2.clear();
                            part2 = 0;
                        }
                    }
                }
            }

            std::cout << "Total sum of gear ratios: " << sum;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
