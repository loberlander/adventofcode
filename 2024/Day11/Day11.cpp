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

static const std::string delimiterNumber = " ";

typedef std::vector<std::string> Map;

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

typedef std::vector<Point> Points;
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

typedef long long BigNumber;
typedef std::set<std::string> Visited; // x:y
typedef std::map<std::string, BigNumber> VisitedDist; // x:y -> distance


Directions DirectionPossibilities=
{
    Direction(0, -1),
    Direction(1, 0),
    Direction(0, 1),
    Direction(-1, 0)
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
                        map = split(line, delimiterNumber);
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


void findStarts(Map& map, Points& p)
{
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] == '0')
            {
                p.emplace_back(Point(x, y));
            }
        }
    }
}

std::string removeLeadingZeros(std::string a)
{
    while (a.size() > 1 && a[0] == '0')
    {
        a.erase(0, 1);
    }

    return a;
}

void iterate(Map& map)
{
    for (auto it = map.begin(); it != map.end(); it++)
    {
        if (*it == "0")
        {
            *it = "1";
        }
        else if (it->size() % 2 == 0)
        {
            size_t len = it->size() / 2;
            std::string left = removeLeadingZeros(it->substr(0, len));
            std::string right = removeLeadingZeros(it->substr(len, len));
            it = map.insert(it, left);
            it++;
            *it = right;
        }
        else
        {
            *it = std::to_string(stoll(*it) * 2024);
        }
    }
}

BigNumber count(std::string stoneId, int numSteps, VisitedDist& visited)
{
    BigNumber result;

    std::string key = stoneId + ":" + std::to_string(numSteps);
    auto seen = visited.find(key);

    if (seen != visited.end())
    {
        return(seen->second);
    }

    if (numSteps == 0)
    {
        result = 1;
    }
    else if (stoneId == "0")
    {
        // Change stone ID and count
        result = count("1", numSteps - 1, visited);
    }
    else
    {
        size_t len = stoneId.size();
        if (len % 2 == 0)
        {
            // Split
            len = len / 2;
            std::string left = removeLeadingZeros(stoneId.substr(0, len));
            std::string right = removeLeadingZeros(stoneId.substr(len, len));
            result = count(left, numSteps - 1, visited) + count(right, numSteps - 1, visited);
        }
        else
        {
            result = count(std::to_string(stoll(stoneId) * 2024), numSteps - 1, visited);
        }
    }

    visited.emplace(key, result);

    return result;
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
        /*
        {
            // Unoptimized solution
            int sum = 0;
            Map map1(map);

            for (int i = 0; i < 25; i++)
            {
                iterate(map1);
            }

            std::cout << "Total: " << map1.size();
        }
*/

        {
            BigNumber sum = 0;
            VisitedDist visited;

            for (auto& stone : map)
            {
                sum += count(stone, 25, visited);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;
            VisitedDist visited;

            for (auto& stone : map)
            {
                sum += count(stone, 75, visited);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
