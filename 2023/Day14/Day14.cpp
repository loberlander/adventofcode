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
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";

typedef std::vector<std::string> Map;

struct MapWithIndex
{
    Map map;
    int Index;
};


typedef std::map<std::string, MapWithIndex> Cache;

typedef long long BigNumber;

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


void readInputFile(std::string fileName, Map& map)
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
                        map.emplace_back(line);
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

Map transpose(Map& map)
{
    Map transposedMap;

    size_t numRows = map.size();
    size_t numcolumns = map[0].size();

    for (size_t column = 0; column < numcolumns; column++)
    {
        std::string line;
        for (size_t row = 0; row < numRows; row++)
        {
            line += map[row][column];
        }
        transposedMap.push_back(line);
    }

    return transposedMap;
}

Map rotate90(Map& map)
{
    Map rotatedMap;

    size_t numRows = map.size();
    size_t numColumns = map[0].size();

    for (size_t column = 0; column < numColumns; column++)
    {
        std::string line;
        for (size_t row = numRows; row > 0; row--)
        {
            line += map[row-1][column];
        }
        rotatedMap.push_back(line);
    }

    return rotatedMap;
}

void moveRocks(std::string& column)
{
    size_t len = column.size();
    bool done = false;
    size_t startFrom = 0;

    while (!done)
    {
        auto space = column.find('.', startFrom);

        if (space != std::string::npos)
        {
            size_t firstSquareRock = column.find('#', space);
            if (firstSquareRock == std::string::npos)
            {
                firstSquareRock = len - 1;
            }

            size_t firstRoundRock = column.find('O', space);
            if (firstRoundRock == std::string::npos)
            {
                done = true;
            }
            else
            {
                if (firstRoundRock <= firstSquareRock)
                {
                    // movable
                    column[space] = 'O';
                    column[firstRoundRock] = '.';
                }
                else
                {
                    startFrom = firstSquareRock;
                }
            }
        }
        else
        {
            done = true;
        }
    }
}


size_t score(Map& map)
{
    size_t total = 0;
    size_t numRows = map.size();
    size_t numColumns = map[0].size();
    for (size_t column = 0; column < numColumns; column++)
    {
        std::string line;
        for (size_t row = 0; row < numRows; row++)
        {
            if (map[row][column] == 'O')
            {
                total += numRows - row;
            }
        }
    }
    return total;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;
    Cache cache;

    readInputFile(inputFileName, map);

    Map transposedMap = transpose(map);

    if (transposedMap.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;

            for (auto& column : transposedMap)
            {
                moveRocks(column);
                size_t len = column.size();
                for (int i = 0; i < len; i++)
                {
                    if (column[i] == 'O')
                    {
                        total += len - i;
                    }
                }
            }

            Map normalisedMap = transpose(transposedMap);

            std::cout << "Total load: " << score(normalisedMap) << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;

            Map rotateMap = map;

            // Find the periodicity after a some cycles
            // In our case: 42, multiply it by a large number, let's say 1000
            // then add the modulo difided offset to align at the end cycle

            int firstIndex = 0;

            //for (BigNumber cycles = 0; cycles < 1000000000 % 7; cycles++)
            //for (int cycles = 1; cycles < 4200 + 1000000000 % 42; cycles++)
            for (int cycles = 1; cycles < 10000; cycles++)
            //for (BigNumber cycles = 0; cycles < 10; cycles++)
            {
                Map startMap = rotateMap;
                std::string key;
                for (auto& row : rotateMap)
                {
                    key += row;
                }

                auto cacheHitIt = cache.find(key);
                if (cacheHitIt != cache.end())
                {
                    rotateMap = cacheHitIt->second.map;

                    if (firstIndex == 0)
                    {
                        firstIndex = cacheHitIt->second.Index;

                        // Found what we are looking for, done!
                        break;
                    }
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        transposedMap = transpose(rotateMap);
                        for (auto& column : transposedMap)
                        {
                            moveRocks(column);
                        }
                        rotateMap = transpose(transposedMap);
                        rotateMap = rotate90(rotateMap);
                    }

                    MapWithIndex mapWithIndex;
                    mapWithIndex.map = rotateMap;
                    mapWithIndex.Index = cycles;
                    cache.emplace(key, mapWithIndex);
                }

                //Let's score the result
                std::cout << "Total after "<< cycles << " cycles: " << score(rotateMap) << std::endl;
            }

            std::cout << std::endl << "Cache size: " << cache.size() << std::endl;
            std::cout << "Map's repeat start at index: " << firstIndex << std::endl;
            size_t period = cache.size() + 1 - firstIndex;
            std::cout << "Map's period: " << period << std::endl;

            int mapWithIndexNeeded = firstIndex + (1000000000 - firstIndex) % period;

            std::cout << "Map with index needed: " << mapWithIndexNeeded << std::endl;

            // Let's find the map associated with the index needed
            Map finalMap;
            for (auto& item : cache)
            {
                if (item.second.Index == mapWithIndexNeeded)
                {
                    finalMap = item.second.map;
                }
            }

            //Let's score the result
            std::cout << "Total load: " << score(finalMap) << std::endl;
        }

        std::cout << std::endl;

    }
}
