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
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";

typedef std::vector<std::string> Map;

enum Nodes
{
    Galaxy = '#',
    Space = '.'
};


typedef long long BigNumber;

class Galaxy
{
public:
    Galaxy()
        : x_(0)
        , y_(0)
    {

    }
    Galaxy(size_t x, size_t y)
        : x_(x)
        , y_(y)
    {

    }
    size_t x_;
    size_t y_;
};

typedef std::vector<class Galaxy> Galaxies;

typedef std::set<int> ExpansionsX;
typedef std::set<int> ExpansionsY;

static const long long expansionFactor1 = 2;
static const long long expansionFactor2 = 1000000;

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

bool isGalaxyPresent(Map& map, size_t x)
{
    size_t yMax = map.size();
    for (size_t y = 0; y < yMax; y++)
    {
        if (map[y][x] == Galaxy)
        {
            return true;
        }
    }

    return false;
}

void expand(Map& map, ExpansionsX& expansionsX, ExpansionsY& expansionsY)
{
    size_t xMax = map[0].size();
    size_t yMax = map.size();

    for (size_t y = 0; y < yMax; y++)
    {
        if (map[y].find(Galaxy) == std::string::npos)
        {
            expansionsY.emplace(y);
        }
    }

    for (size_t x = 0; x < xMax; x++)
    {
        if (!isGalaxyPresent(map, x))
        {
            expansionsX.emplace(x);
        }
    }

/*
    Map output1;
    Map output2;

    //Expand rows
    for (auto& row : map)
    {
        output1.emplace_back(row);
        output2.emplace_back();
        if (row.find(Galaxy) == std::string::npos)
        {
            output1.emplace_back(row);
            output2.emplace_back();
        }
    }

    //Expand columns
    size_t xMax = output1[0].size();
    size_t yMax = output1.size();
    for (size_t x = 0; x < xMax; x++)
    {
        bool addColumn = !isGalaxyPresent(output1, x);
        for (size_t y = 0; y < yMax; y++)
        {
            output2[y] += output1[y][x];
            if (addColumn)
            {
                output2[y] += Space;
            }
        }
    }

    return output2;
*/
}

Galaxies findGalaxies(Map map)
{
    Galaxies galaxies;
    size_t xMax = map[0].size();
    size_t yMax = map.size();

    for (size_t y = 0; y < yMax; y++)
    {
        for (size_t x = 0; x < xMax; x++)
        {
            if (map[y][x] == Galaxy)
            {
                galaxies.emplace_back(x, y);
            }
        }
    }
    return galaxies;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;
    ExpansionsX expansionsX;
    ExpansionsY expansionsY;

    readInputFile(inputFileName, map);

    expand(map, expansionsX, expansionsY);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            Galaxies galaxies = findGalaxies(map);
            BigNumber total = 0;
            auto expansionFactor = expansionFactor2;

            size_t numGalaxies = galaxies.size();

            for (size_t n1 = 0; n1 < numGalaxies; n1++)
            {
                for (size_t n2 = n1 + 1; n2 < numGalaxies; n2++)
                {
                    int distance = std::abs((int) galaxies[n1].x_ - (int)galaxies[n2].x_) + std::abs((int)galaxies[n1].y_ - (int)galaxies[n2].y_);

                    // Check for expansions
                    auto xStart = std::min(galaxies[n1].x_, galaxies[n2].x_);
                    auto xEnd = std::max(galaxies[n1].x_, galaxies[n2].x_);
                    for (int x = xStart; x < xEnd; x++)
                    {
                        if (expansionsX.find(x) != expansionsX.end())
                        {
                            distance--;
                            distance += expansionFactor;
                        }
                    }

                    auto yStart = std::min(galaxies[n1].y_, galaxies[n2].y_);
                    auto yEnd = std::max(galaxies[n1].y_, galaxies[n2].y_);
                    for (int y = yStart; y < yEnd; y++)
                    {
                        if (expansionsY.find(y) != expansionsY.end())
                        {
                            distance--;
                            distance += expansionFactor;
                        }
                    }

                    total += distance;
                }
            }


/*
            std::ofstream myFile("output2.txt");
            if (myFile.is_open())
            {
                for (auto& line : map)
                {
                    myFile << line << std::endl; // .write(line.c_str(), line.size());
                }
                myFile.close();
            }
*/
            std::cout << "The total is: " << total;
            std::cout << std::endl;
        }

        std::cout << std::endl;
        {
            BigNumber total = 0;



        }

        std::cout << std::endl;

    }
}
