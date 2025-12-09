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


typedef std::vector<std::string> List;
typedef std::vector<List> Lists;

typedef std::vector<char> Operations;

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
            line.erase(0, tokenPosition + delimiter.length());
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


std::string trimLeft(const std::string& str)
{
    // Find the first non-whitespace character
    size_t pos = str.find_first_not_of(" \t\n\r\f\v");

    // Extract the substring from the first non-whitespace to the end
    return (pos == std::string::npos) ? "" : str.substr(pos);
}


std::string trimRight(const std::string& str)
{
    // Find the last non-whitespace character
    size_t pos = str.find_last_not_of(" \t\n\r\f\v");

    // Extract the substring from the beginning to the last non-whitespace 
    return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
}


std::string trimLeftRight(const std::string& str)
{
    return trimLeft(trimRight(str));
}


void readInputFile(std::string fileName, Lists& listsP1, Lists& listsP2, Operations& operations)
{
    std::string line;
    std::vector<std::string> lines;
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
                    // Read ranges
                    if (!line.empty())
                    {
                        auto elements = split(line, { " " });
                        if (elements.size() >= 1)
                        {
                            if (std::isdigit(elements[0][0]))
                            {
                                lines.emplace_back(line); // save it for later to transpose the data
                                List list;
                                for (auto& element : elements)
                                {
                                    list.emplace_back(element);
                                }
                                listsP1.emplace_back(list);
                            }
                            else
                            {
                                for (auto& element : elements)
                                {
                                    operations.emplace_back(element[element.size() - 1]);
                                }
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

    // transpose
    std::vector<std::string> linesTranspose;

    for (int x = 0; x < lines[0].size(); x++)
    {
        std::string newLine;
        for (int y = 0; y < lines.size(); y++)
        {
            //newLine.insert(0, 1, lines[y][x]);
            newLine.append(1, lines[y][x]);
        }
        linesTranspose.emplace_back(newLine);
    }

    // Add an extra space to separate the last element
    // Must have an empty element for the regroup logic below to properly finish the last element!
    linesTranspose.emplace_back(" ");

    // regroup
    List listT;
    for (auto element: linesTranspose)
    {
        // eliminate the space separators between values
        if (trimLeftRight(element).empty())
        {
            listsP2.emplace_back(listT);
            listT.clear();
        }
        else
        {
            listT.emplace_back(element);
        }
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Lists listsP1;
    Lists listsP2;
    Operations operations;
    readInputFile(inputFileName, listsP1, listsP2, operations);

    if (listsP1.empty() || listsP2.empty() || operations.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;
            auto numOperations = (BigNumber) operations.size();
            auto numProblems = (BigNumber) listsP1[0].size();

            for (BigNumber i = 0; i < numProblems; i++)
            {
                BigNumber solution = 0;
                switch (operations[i])
                {
                    case '+':
                        solution = 0;
                        break;
                    case '*':
                        solution = 1;
                        break;
                    default:
                        break;
                }

                for (auto& list : listsP1)
                {
                    switch (operations[i])
                    {
                        case '+':
                            solution += std::stoll(list[i]);
                            break;
                        case '*':
                            solution *= std::stoll(list[i]);
                            break;
                        default:
                            break;
                    }
                }

                result += solution;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;
            auto numOperations = (BigNumber) operations.size();
            auto numProblems = (BigNumber) listsP2.size();

            for (BigNumber i = 0; i < numProblems; i++)
            {
                BigNumber solution = 0;
                switch (operations[i])
                {
                    case '+':
                        solution = 0;
                        break;
                    case '*':
                        solution = 1;
                        break;
                    default:
                        break;
                }

                for (auto& element : listsP2[i])
                {
                    switch (operations[i])
                    {
                        case '+':
                            solution += std::stoll(element);
                            break;
                        case '*':
                            solution *= std::stoll(element);
                            break;
                        default:
                            break;
                    }
                }

                result += solution;
            }
            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
