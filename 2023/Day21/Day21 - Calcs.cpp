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

static const char Start = 'S';
static const char Garden = '.';
static const char Rock = '#';


typedef long long BigNumber;

typedef std::vector<std::string> Map;
typedef std::set<std::string> Visited; // x:y

class Point
{
public:
    Point()
        : x_(0)
        , y_(0)
    {}

    Point(int x, int y)
        : x_(x)
        , y_(y)
    {}

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
    {}

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
};

typedef std::map<std::string, Point> VisitedPoints;

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

BigNumber lcm(std::vector<BigNumber> values)
{
    // Requires C++17 that provides std::gcd()
    // calculate the LCM of values in a vector
    // lcm(a, b) = (a*b)/gcd(a,b)
    BigNumber lcmTmp = values.back();
    values.pop_back();

    for (auto value : values)
    {
        lcmTmp = lcmTmp * value / std::gcd(lcmTmp, value);
    }

    return lcmTmp;
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
                    // Read Workflows
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

            }
        }
        myFile.close();
    }
}

Point findStart(Map& map)
{
    Point start;

    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    for (int y = 0; y <= maxY; y++)
    {
        for (int x = 0; x <= maxX; x++)
        {
            if (map[y][x] == Start)
            {
                return Point(x, y);
            }
        }
    }

    return start;
}

BigNumber calc(Map& map, Point start, BigNumber steps)
{
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();

    VisitedPoints visitedPoints;
    visitedPoints.emplace(start.asString(), start);

    static const int MaxSteps = 64;

    for (int i = 0; i < MaxSteps; i++)
    {
        VisitedPoints visitedPointsNew;
        for (auto& point : visitedPoints)
        {
            std::vector<Point> moveOptions;

            if (point.second.y_ + 1 <= maxY)
            {
                // attempt to go South
                moveOptions.emplace_back(Point(point.second.x_, point.second.y_ + 1));
            }
            if (point.second.y_ - 1 >= 0)
            {
                // attempt to go North
                moveOptions.emplace_back(Point(point.second.x_, point.second.y_ - 1));
            }
            if (point.second.x_ + 1 <= maxX)
            {
                // attempt to go East
                moveOptions.emplace_back(Point(point.second.x_ + 1, point.second.y_));
            }
            if (point.second.x_ - 1 >= 0)
            {
                // attempt to go West
                moveOptions.emplace_back(Point(point.second.x_ - 1, point.second.y_));
            }
            for (auto newPoint : moveOptions)
            {
                if (map[newPoint.y_][newPoint.x_] == Garden || map[newPoint.y_][newPoint.x_] == Start)
                {
                    visitedPointsNew.emplace(newPoint.asString(), newPoint);
                }
            }
        }
        visitedPoints = visitedPointsNew;
    }

    return visitedPoints.size();
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    Point start = findStart(map);
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();


    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;
        {
            std::cout << "The total is: " << calc(map, start, 64);
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            VisitedPoints visitedPoints;
            visitedPoints.emplace(start.asString(), start);

            static const int MaxSteps = 100;

            for (int i = 0; i < MaxSteps; i++)
            {
                VisitedPoints visitedPointsNew;
                for (auto& point : visitedPoints)
                {
                    std::vector<Point> moveOptions;

                    // attempt to go South
                    moveOptions.emplace_back(Point(point.second.x_, point.second.y_ + 1));
                    // attempt to go North
                    moveOptions.emplace_back(Point(point.second.x_, point.second.y_ - 1));
                    // attempt to go East
                    moveOptions.emplace_back(Point(point.second.x_ + 1, point.second.y_));
                    // attempt to go West
                    moveOptions.emplace_back(Point(point.second.x_ - 1, point.second.y_));

                    for (auto newPoint : moveOptions)
                    {
                        // https://en.wikipedia.org/wiki/Modulo
                        //int x = (newPoint.x_ + sizeX) % sizeX;
                        //int x = ((newPoint.x_ % sizeX) + sizeX) % sizeX;
                        int x = std::fmod(std::fmod(newPoint.x_, sizeX) + sizeX, sizeX);
/*
                        if (x < 0)
                        {
                            x = (-x) % sizeX;
                        }
                        else
                        {
                            x = x % sizeX;
                        }
*/
                        //int y = (newPoint.y_ + sizeY) % sizeY;
                        //int y = ((newPoint.y_ % sizeY) + sizeY) % sizeY;
                        int y = std::fmod(std::fmod(newPoint.y_, sizeY) + sizeY, sizeY);

/*
                        if (y < 0)
                        {
                            y = (-y) % sizeY;
                        }
                        else
                        {
                            y = y % sizeY;
                        }
*/
                        char c = map[y][x];
                        if (c == Garden || c == Start)
                        {
                            visitedPointsNew.emplace(newPoint.asString(), newPoint);
                        }
                    }
                }
                visitedPoints = visitedPointsNew;
            }
            std::cout << "The total is: " << visitedPoints.size();
        }

        std::cout << std::endl;

    }
}
