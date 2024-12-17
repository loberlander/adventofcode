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


typedef std::vector<std::string> Map;

enum Nodes
{
    Junk = '#',
    Space = '.'
};

class Point
{
public:
    Point(int x, int y)
        : x_(x)
        , y_(y)
    {
    }

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
    {
    }

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    Point operator+(const Point& other)
    {
        return Point(x_ + other.x_, y_ + other.y_);
    }

    Point& operator+=(const Point& other)
    {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
};

typedef Point Direction;
typedef std::vector<Direction> Directions;

class Node
{
public:
    Node(Point point, Direction direction)
        : point_(point)
        , direction_(direction)
    {
    }

    Node()
        : point_(0, 0)
        , direction_(0, 0)
    {
    }

    std::string asString()
    {
        return point_.asString() + ":" + direction_.asString();
    }

    std::string asStringCount()
    {
        return point_.asString();
    }

    Point point_;
    Direction direction_;
};

typedef std::set<std::string> Visited;

typedef long long BigNumber;

Directions DirectionOrders=
{
    Point(0, -1),
    Point(1, 0),
    Point(0, 1),
    Point(-1, 0)
};

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
                    // Read Map
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


bool isWithinBounds(Map& map, Point p)
{
    if (p.x_ < 0 || p.y_ < 0 || p.x_ >= map[0].size() || p.y_ >= map.size())
    {
        return false;
    }
    return true;
}


bool isJunkPresent(Map& map, Point p)
{
    if (!isWithinBounds(map, p))
    {
        // Assume no junk outside of the boundaries
        return false;
    }

    if (map[p.y_][p.x_] == Junk)
    {
        return true;
    }

    return false;
}


void findStart(Map& map, Point& p, Direction& d)
{
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] == '^')
            {
                p.x_ = x;
                p.y_ = y;

                d.x_ = 0;
                d.y_ = -1;
                return;
            }
        }
    }
}


Direction selectNextDirection(Direction currentDirection)
{
    int orderNumber = -1;

    for (int i = 0; i < DirectionOrders.size(); i++)
    {
        if (DirectionOrders[i] == currentDirection)
        {
            orderNumber = i;
            break;
        }
    }

    return DirectionOrders[(orderNumber + 1) % DirectionOrders.size()];
}


size_t findSteps(Map& map, Point start, Direction dir)
{
    bool done = false;
    bool isLoop = false;
    Point currentPoint = start;
    Direction currentDir = dir;

    Visited visitedNodes;
    Visited visitedPoints;
    visitedNodes.insert(Node(currentPoint, currentDir).asString());
    visitedPoints.insert(currentPoint.asString());

    while (!done)
    {
        if (isJunkPresent(map, currentPoint + currentDir))
        {
            currentDir = selectNextDirection(currentDir);
        }
        else
        {
            currentPoint += currentDir;
            if (isWithinBounds(map, currentPoint))
            {
                auto searchResult = visitedNodes.find(Node(currentPoint, currentDir).asString());
                if (searchResult != visitedNodes.end())
                {
                    // Got into a loop
                    done = true;
                    isLoop = true;
                }

                visitedNodes.insert(Node(currentPoint, currentDir).asString());
                visitedPoints.insert(currentPoint.asString());
            }
            else
            {
                done = true;
            }
        }
    }

    size_t numPoints = visitedPoints.size();
    size_t numNodes = visitedNodes.size();

    return isLoop ? 0 : numPoints;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    Point start(0, 0);
    Point dir(0, 0);
    Visited visited;

    readInputFile(inputFileName, map);

    findStart(map, start, dir);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            std::cout << "Total: " << findSteps(map, start, dir);
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Brute force, try every combination
            // Use optimized build to speed things up
            int sum = 0;

            for (int y = 0; y < map.size(); y++)
            {
                for (int x = 0; x < map[0].size(); x++)
                {
                    auto mapTry = map;
                    if (mapTry[y][x] == Space)
                    {
                        mapTry[y][x] = Junk;

                        if (findSteps(mapTry, start, dir) == 0)
                        {
                            sum += 1;
                        }
                    }
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
