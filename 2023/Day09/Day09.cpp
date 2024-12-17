#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test2.txt";

static const std::string delimiterNodeId = " = (";
static const std::string delimiterSides = ", ";
static const std::string delimiterEnd = ")";

typedef unsigned long long BigNumber;

typedef std::vector<std::vector<int>> Data;


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


void readInputFile(std::string fileName, Data& data)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read Instructions
                    if (!line.empty())
                    {
                        std::vector<int> series;

                        std::vector<std::string> parts = split(line, " ");
                        for (auto& element : parts)
                        {
                            series.push_back(stoi(element));
                        }
                        data.push_back(series);
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

int solve1(std::vector<int> data)
{
    bool done = false;
    int result = 0;
    std::vector<std::vector<int>> matrix;


    matrix.emplace_back(data);

    while (!done)
    {
        std::vector<int> line;
        size_t yCount = matrix.size();
        size_t xCount = matrix[yCount - 1].size();
        for (int i = 0; i < xCount - 1; i++)
        {
            int difference = matrix[yCount - 1][i + 1] - matrix[yCount - 1][i];
            line.emplace_back(difference);
        }
        matrix.emplace_back(line);

        done = true;
        for (auto& element : matrix[yCount])
        {
            if (element != 0)
            {
                done = false;
                break;
            }
        }
    }

    size_t yCount = matrix.size();
    for (size_t y = yCount - 1 ; y > 0; y--)
    {
        size_t xCount = matrix[y - 1].size();
        result += matrix[y - 1 ][xCount - 1];
    }

    return result;
}

int solve2(std::vector<int> data)
{
    bool done = false;
    int result = 0;
    std::vector<std::vector<int>> matrix;


    matrix.emplace_back(data);

    while (!done)
    {
        std::vector<int> line;
        size_t yCount = matrix.size();
        size_t xCount = matrix[yCount - 1].size();
        for (int i = 0; i < xCount - 1; i++)
        {
            int difference = matrix[yCount - 1][i + 1] - matrix[yCount - 1][i];
            line.emplace_back(difference);
        }
        matrix.emplace_back(line);

        done = true;
        for (auto& element : matrix[yCount])
        {
            if (element != 0)
            {
                done = false;
                break;
            }
        }
    }

    size_t yCount = matrix.size();
    for (size_t y = yCount - 1; y > 0; y--)
    {
        size_t xCount = matrix[y - 1].size();
        result = matrix[y - 1][0] - result;
    }

    return result;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Data data;

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
            BigNumber total = 0;

            for (auto& line : data)
            {
                total += solve1(line);
            }

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
        {
            BigNumber total = 0;

            for (auto& line : data)
            {
                total += solve2(line);
            }

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;

    }
}
