#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const int magicNumber = 2020;
static const char inputFileName[] = "input.txt";

std::vector<int> readInputFile(std::string fileName)
{
    std::vector<int> data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            data.push_back(std::stoi(line));
        }
        myFile.close();
    }

    return data;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<int> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        for (auto x : data)
        {
            for (auto y : data)
            {
                if (x + y == magicNumber)
                {
                    std::cout << x << " " << y << " : " << x * y << std::endl;
                }
            }
        }

        std::cout << "Part 2:" << std::endl;
        for (auto x : data)
        {
            for (auto y : data)
            {
                for (auto z : data)
                {
                    if (x + y + z == magicNumber)
                    {
                        std::cout << x << " " << y << " " << z << " : " << x * y * z << std::endl;
                    }
                }
            }
        }
    }
}
