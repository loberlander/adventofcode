#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";


typedef std::vector<int> Map;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::string delimiter)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());
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


void readInputFile(std::string fileName, Map& map1, Map& map2)
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
                    // Read Instructions
                    if (!line.empty())
                    {
                        auto lineSplit = split(line, "   ");

                        map1.emplace_back(std::stoi(lineSplit[0]));
                        map2.emplace_back(std::stoi(lineSplit[1]));
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


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map1;
    Map map2;

    readInputFile(inputFileName, map1, map2);

    if (map1.empty() || map2.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            Map mapA = map1;
            Map mapB = map2;

            std::sort(mapA.begin(), mapA.end());
            std::sort(mapB.begin(), mapB.end());

            int sum = 0;

            for (int i = 0; i < mapA.size(); i++)
            {
                sum += std::abs(mapA[i] - mapB[i]);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            Map mapA = map1;
            Map mapB = map2;

            std::sort(mapA.begin(), mapA.end());
            std::sort(mapB.begin(), mapB.end());

            int sum = 0;

            for (int i = 0; i < mapA.size(); i++)
            {
                int value = mapA[i];
                int sumB = 0;

                for (int j = 0; j < mapB.size(); j++)
                {
                    if (value == mapB[j])
                    {
                        sumB++;
                    }
                }

                sum += value * sumB;
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}