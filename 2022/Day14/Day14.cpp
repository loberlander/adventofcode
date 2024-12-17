#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";

#define CAVE_SIZE 1000
#define ROCK '#'
#define AIR '.'
#define SAND 'o'


static const std::string delimiterElement = " -> ";
static const std::string delimiterComma = ",";

class Point
{
public:
    Point(int x, int y)
        : x_(x)
        , y_(y)
    {

    }

    int x_;
    int y_;
};

typedef std::vector<Point> Rock;
/*
class Rock
{
public:
    Rock(Points points)
        : points_(points)
    {}

    Points points_;
};
*/
typedef std::vector<Rock> Rocks;

typedef std::vector<std::vector<char>> Cave;


Point parsePoint(std::string line)
{
    int x = 0;
    int y = 0;

    size_t tokenPosition = line.find(delimiterComma, 0);
    if (tokenPosition != std::string::npos)
    {
        x = std::stoi(line.substr(0, tokenPosition));
        //x -= 480;
        line.erase(0, tokenPosition + delimiterComma.length());
        y = std::stoi(line);
    }

    return Point(x, y);
}

void readInputFile(std::string fileName, Rocks& rocks)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                switch (state)
                {
                    case 1:
                    {
                        Rock rock;
                        while (!line.empty())
                        {
                            size_t tokenPosition = line.find(delimiterElement, 0);
                            if (tokenPosition != std::string::npos)
                            {
                                rock.emplace_back(parsePoint(line.substr(0, tokenPosition)));
                                line.erase(0, tokenPosition + delimiterElement.length());
                            }
                            else
                            {
                                rock.emplace_back(parsePoint(line.substr(0, tokenPosition)));
                                line.erase(0, line.size());
                            }
                        }
                        rocks.emplace_back(rock);
                        break;
                    }
                }
            }
            else
            {
                state = 1;
            }
        }
        myFile.close();
    }
}

void placeRock(Cave& cave, Rock& rock)
{
    int xPrevious = 0;
    int yPrevious = 0;
    bool firstTime = true;
    for (auto& point : rock)
    {
        if (!firstTime)
        {
            int dX = point.x_ - xPrevious;
            int dY = point.y_ - yPrevious;
            int dir = 1;
            if (dX == 0)
            {
                if (dY < 0) { dir = -1; }

                for (int y = yPrevious; dir == 1 ? y <= point.y_ : point.y_ <= y; y += dir)
                {
                    cave[y][point.x_] = ROCK;
                }
            }
            else
            {
                if (dX < 0) { dir = -1; }

                for (int x = xPrevious; dir == 1 ? x <= point.x_ : point.x_ <= x; x += dir)
                {
                    cave[point.y_][x] = ROCK;
                }
            }
        }
        else
        {
            firstTime = false;
        }

        xPrevious = point.x_;
        yPrevious = point.y_;
    }
    
}

void placeRocks(Cave& cave, Rocks& rocks)
{
    for (auto& rock : rocks)
    {
        placeRock(cave, rock);
    }
}


void initCave(Cave& cave, Rocks& rocks)
{
    std::vector<char> row(CAVE_SIZE, AIR);

    for (int y = 0; y < CAVE_SIZE; y++)
    {
        cave.emplace_back(row);
    }
    placeRocks(cave, rocks);
}

bool addGrain(Cave& cave, Point grain)
{
    int currentX = grain.x_;
    int currentY = grain.y_;
    bool stopped = false;

    if (cave[currentY][currentX] != AIR)
    {
        return false;
    }

    while (!stopped)
    {
        if (currentY >= CAVE_SIZE - 1)
        {
            // done?
            return false;
        }
        if (currentX >= CAVE_SIZE - 1)
        {
            // put breakpoint here, increase cave size if ever triggering
            return false;
        }
        if (currentX <= 0)
        {
            // put breakpoint here, shouldnever get here
            return false;
        }

        char elementS = cave[currentY + 1][currentX];
        char elementSW = cave[currentY + 1][currentX - 1];
        char elementSE = cave[currentY + 1][currentX + 1];

        if (elementS == AIR)
        {
            currentY = currentY + 1;
        }
        else if (elementSW == AIR)
        {
            currentY = currentY + 1;
            currentX = currentX - 1;
        }
        else if (elementSE == AIR)
        {
            currentY = currentY + 1;
            currentX = currentX + 1;
        }
        else
        {
            // cannot move
            cave[currentY][currentX] = SAND;
            return true;
        }
    }

    return false;
}

int placeSand(Cave& cave, Point start)
{
    bool done = false;
    int grainCount = 0;
    while (!done)
    {
        Point grain = start;
        if (addGrain(cave, grain))
        {
            grainCount++;
        }
        else
        {
            done = true;
        }
    }

    return grainCount;
}

int findBottomRock(Cave& cave)
{
    int rockY = 0;

    for (auto it = cave.rbegin(); it <= cave.rend(); it++)
    {
        for (auto& col : *it)
        {
            if (col == ROCK)
            {
                rockY = std::distance(it, cave.rend()) - 1;
                return rockY;
            }
        }
    }

    return -1;
}

void placeFloor(Cave& cave)
{
    int rockY = findBottomRock(cave) + 2;

    for (int x = 0; x < CAVE_SIZE; x++)
    {
        cave[rockY][x] = ROCK;
    }

}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Rocks rocks;
    Cave cave1;
    Cave cave2;

    readInputFile(inputFileName, rocks);

    initCave(cave1, rocks);
    initCave(cave2, rocks);

    if (rocks.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            int grains = placeSand(cave1, Point(500, 0));
            std::cout << "Number of grains of sand until abyss: " << grains;

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;

            placeFloor(cave2);

            int grains = placeSand(cave2, Point(500, 0));
            std::cout << "Number of grains of sand until abyss: " << grains;
            std::cout << std::endl;
        }
    }
}
