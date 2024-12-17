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

static const char magic1[] = "XMAS";

typedef std::vector<char> Line;
typedef std::vector<Line> Map;


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
            switch (state)
            {
                case 1:
                {
                    if (!line.empty())
                    {
                        Line dataLine;
                        for (auto& c : line)
                        {
                            dataLine.push_back(c);
                        }
                        map.push_back(dataLine);
                        break;
                    }
                    else
                    {
                        // no more characters to process, we are done
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}


bool isMagic1(Map& map, int pos, int x, int y, int xDir, int yDir)
{
    // Boundary checks
    if (x < 0 || x >= map[0].size())
    {
        return false;
    }
    if (y < 0 || y >= map.size())
    {
        return false;
    }
    // Is the requested position a match?
    if (map[y][x] == magic1[pos])
    {
        pos++;
        if (pos == sizeof(magic1) - 1)
        {
            // Found the magic word!
            return true;
        }
        x += xDir;
        y += yDir;

        return isMagic1(map, pos, x, y, xDir, yDir);
    }

    return false;
}


bool isMagic2(Map& map, int x, int y)
{
    // Boundary checks
    if (x < 1 || x >= map[0].size() - 1)
    {
        return false;
    }
    if (y < 1 || y >= map.size() - 1)
    {
        return false;
    }

    if (map[y][x] != 'A')
    {
        return false;
    }

    char a1 = map[y - 1][x - 1];
    char a2 = map[y + 1][x + 1];
    char b1 = map[y - 1][x + 1];
    char b2 = map[y + 1][x - 1];
    if ((a1 == 'M' && a2 == 'S') || (a1 == 'S' && a2 == 'M'))
    {
        if ((b1 == 'M' && b2 == 'S') || (b1 == 'S' && b2 == 'M'))
        {
            return true;
        }
    }

    return false;
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
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            int sum = 0;

            for (int y = 0; y < map.size(); y++)
            {
                for (int x = 0; x < map[0].size(); x++)
                {
                    sum = isMagic1(map, 0, x, y, 1, 0) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, 1, 1) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, 0, 1) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, -1, 1) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, -1, 0) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, -1, -1) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, 0, -1) ? sum + 1 : sum;
                    sum = isMagic1(map, 0, x, y, 1, -1) ? sum + 1 : sum;
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            int sum = 0;

            for (int y = 0; y < map.size(); y++)
            {
                for (int x = 0; x < map[0].size(); x++)
                {
                    sum = isMagic2(map, x, y) ? sum + 1 : sum;
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}