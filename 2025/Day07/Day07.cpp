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

typedef std::vector<std::string> Map;

static const char START = 'S';
static const char SPACE = '.';
static const char SPLITTER = '^';
static const char BEAM = '|';
static const char LEFT = '<';
static const char RIGHT = '>';

typedef std::set<std::string> Visited;

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


void readInputFile(std::string fileName, Map& map)
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
                    // Read ranges
                    if (!line.empty())
                    {
                        map.emplace_back(line);
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

    Map map;

    readInputFile(inputFileName, map);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::vector<BigNumber> accumulator;
        for (int y = 0; y < map.size(); y++)
        {
            accumulator.emplace_back(0);
        }

        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

           for (int y = 0; y < map.size(); y++)
           {
                for (int x = 0; x < map[0].size(); x++)
                {
                    switch (map[y][x])
                    {
                        case START:
                        {
                            if (y + 1 < map.size() - 1 && map[y + 1][x] == SPACE)
                            {
                                map[y + 1][x] = BEAM;
                                accumulator[x] += 1;
                            }
                            break;
                        }
                        case SPLITTER:
                        {
                            if (y > 0 && map[y - 1][x] == BEAM)
                            {
                                
                                if ((map[y][x - 1] == SPACE || map[y][x - 1] == BEAM) && (map[y][x + 1] == SPACE || map[y][x + 1] == BEAM))
                                {
                                    map[y][x - 1] = BEAM;
                                    accumulator[x - 1] += accumulator[x];

                                    map[y][x + 1] = BEAM;
                                    accumulator[x + 1] += accumulator[x];

                                    accumulator[x] = 0;

                                    ++result;
                                }
                            }
                            break;
                        }
                        case BEAM:
                        {
                            if (y + 1 < map.size()-1 && map[y + 1][x] == SPACE)
                            {
                                map[y + 1][x] = BEAM;
                            }
                            break;
                        }
                        case SPACE:
                        {
                            if (y > 0 && map[y - 1][x] == BEAM)
                            {
                                map[y][x] = BEAM;
                            }
                            break;
                        }
                        default:
                        {
                            // do nothing
                            break;
                        }
                    }
                }
            }


            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            for (auto& visited : accumulator)
            {
                result += visited;
            }
            

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
