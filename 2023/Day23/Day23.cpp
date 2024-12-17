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

#include "helper.h"

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";

static const char Path = '.';
static const char Slopes[] = { '^', '>', 'v', '<' };
static const char Forest = '#';


typedef long long BigNumber;

typedef std::vector<std::string> Map;
typedef std::set<std::string> Visited; // x:y

class Point
{
public:
    Point()
        : x_(0)
        , y_(0)
        , extra_(0)
    {}

    Point(int x, int y, int extra = 0)
        : x_(x)
        , y_(y)
        , extra_(extra)
    {}

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
        , extra_(position.extra_)
    {}

    Point(const Point& position, int extra)
        : x_(position.x_)
        , y_(position.y_)
        , extra_(extra)
    {}

    bool inRange(const int minX, const int maxX, const int minY, const int maxY) const
    {
        return (minX <= x_) && (x_ <= maxX) && (minY <= y_) && (y_ <= maxY);
    }

    bool operator==(const Point& other) const
    {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    bool operator!=(const Point& other) const
    {
        return !operator==(other);
    }

    Point& operator+=(const Point& other)
    {
        x_ += other.x_;
        y_ += other.y_;
        extra_ += other.extra_;
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        extra_ -= other.extra_;
        return *this;
    }

    Point operator+(const Point& other) const
    {
        return Point(x_ + other.x_, y_ + other.y_, extra_ + other.extra_);
    }

    Point operator-(const Point& other) const
    {
        return Point(x_ - other.x_, y_ - other.y_, extra_ - other.extra_);
    }

    std::string asString() const
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    std::string asStringWithExtra()
    {
        return std::to_string(extra_) + ":" + std::to_string(x_) + ":" + std::to_string(y_);
    }

    bool operator<(const Point& other) const
    {
        return asString() < other.asString();
    }

    int x_;
    int y_;
    int extra_;
};

// Directions
typedef Point Dir;
typedef std::vector<Dir> Dirs;

typedef std::map<std::string, Point> VisitedPoints;
typedef std::list<Point> Points;
typedef std::map<Point, Points> Graph;
typedef std::set<Point> PointsSet;

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


Points findMaxPathLength(Map& map, std::set<std::string>& visited, Dir dir, Point start, Point end, bool part2)
{
    Points result;
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();

    if (!start.inRange(0, (int) maxX, 0, (int) maxY))
    {
        // Out of bounds
        return result;
    }

    if (map[start.y_][start.x_] == Forest)
    {
        return result;
    }

    Dirs dirs = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
    Point previous = start - dir;

    if (part2)
    {
        if (map[start.y_][start.x_] == '#')
        {
            return result;
        }
    }
    else
    {
        switch (map[start.y_][start.x_])
        {
            case '#':
            {
                return result;
            }
            case '.':
            {
                break;
            }
            case '>':
            {
                // Can only pass if coming from the left
                if (dir != dirs[2])
                {
                    return result;
                }
                break;
            }
            case '<':
            {
                if (dir != dirs[3])
                {
                    return result;
                }
                // Can only pass if coming from the right
                break;
            }
            case 'v':
            {
                // Can only pass if coming from above
                if (dir != dirs[0])
                {
                    return result;
                }
                break;
            }
            case '^':
            {
                // Can only pass if coming from below
                if (dir != dirs[1])
                {
                    return result;
                }
                break;
            }
        }
    }

    if (visited.find(start.asString()) != visited.end())
    {
        // Already visited before
        // We are not looping
        return result;
    }

    if (start == end)
    {
        // Reached the end, hooray!
        return { end };
    }

    visited.emplace(start.asString());

    size_t maxLength = 0;
    for (auto& dir : dirs)
    {
        Point newPoint(start + dir);
        Points pointsTmp = findMaxPathLength(map, visited, dir, newPoint, end, part2);
        if (pointsTmp.size() > maxLength)
        {
            maxLength = pointsTmp.size();
            result = pointsTmp;
        }
    }

    auto it = visited.find(start.asString());
    if (it != visited.end())
    {
        visited.erase(it);
    }

    // Add current point
    result.emplace_front(start);

    return result;
}


PointsSet findPoints(Map& map, Point start, Point end)
{
    PointsSet result;
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();

    result.emplace(start);
    result.emplace(end);

    // Iterate through all rows and columns and identify all points that fork
    for (int y=0; y<=maxY; y++)
    {
        for (int x = 0; x <= maxX; x++)
        {
            if (map[y][x] == '#')
            {
                continue;
            }

            Dirs dirs = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
            int neighbors = 0;
            for (auto& dir : dirs)
            {
                Point testPoint = Point(x + dir.x_, y + dir.y_);
                
                if (testPoint.inRange(0, (int) maxX, 0, (int) maxY) && (map[testPoint.y_][testPoint.x_] != '#'))
                {
                    neighbors++;
                }
            }

            if (neighbors >= 3)
            {
                result.emplace(x, y);
            }
        }
    }


    return result;
}

Graph createGraph(Map& map, PointsSet& points)
{
    // This is basically edge contraction
    // https://en.wikipedia.org/wiki/Edge_contraction
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();

    // Create an "empty" graph
    Graph graph;
    for (auto& point : points)
    {
        graph.emplace(point, Points());
    }

    // For each point, find its neighbors and calculate the distance between them
    for (auto& source : points)
    {
        std::deque<Point> q;
        std::set<Point> seen;

        q.emplace_back(source.x_, source.y_, 0);
        seen.emplace(source);

        while (!q.empty())
        {
            Point destination = q.front();
            q.pop_front();

            // Is destination a point?
            if (destination.extra_ != 0 && points.find(destination) != points.end())
            {
                // Yes, save it!
                graph[source].emplace_back(destination);
                continue;
            }

            Dirs dirs = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
            for (auto& dir : dirs)
            {
                Point testPoint = destination + dir;

                if (testPoint.inRange(0, (int)maxX, 0, (int)maxY)
                    && (map[testPoint.y_][testPoint.x_] != '#')
                    && seen.find(testPoint) == seen.end())
                {
                    testPoint.extra_++;
                    q.emplace_back(testPoint);
                    seen.emplace(testPoint);
                }
            }
        }
    }

    return graph;
}

int findMaxPathLength2(Graph& graph, std::set<Point>& visited, Point start, Point end)
{
    if (start == end)
    {
        return 0;
    }

    int m = 0;

    // Brute force find the longest path by walking all possiblilities

    visited.emplace(start);

    for (auto& next : graph[start])
    {
        if (visited.find(next) == visited.end())
        {
            m = std::max(m, findMaxPathLength2(graph, visited, next, end) + next.extra_);
        }
    }

    auto it = visited.find(start);
    if (it != visited.end())
    {
        visited.erase(it);
    }

    return m;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();

    Point start = Point(1, 0);
    Point end = Point((int)maxX - 1, (int) maxY);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;
            std::set<std::string> visited;

            Points points = findMaxPathLength(map, visited, {0, 0}, start, end, false);
            total = points.size() - 1;

/*
            for (auto& item : points)
            {
                map[item.y_][item.x_] = '*';
            }

            std::ofstream myFile("output.txt");
            if (myFile.is_open())
            {
                myFile << map << std::endl;
                //for (auto& line : map)
                //{
                //    myFile << line << std::endl; // .write(line.c_str(), line.size());
                //}
                myFile.close();
            }
*/
            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            std::cout << "Finding Points" << std::endl;
            PointsSet pointsSet = findPoints(map, start, end);
            std::cout << "Building Graph" << std::endl;
            Graph graph = createGraph(map, pointsSet);
            std::set<Point> visited;

            std::cout << "Finding max length (will take 2-3 minutes)" << std::endl;
            int total = findMaxPathLength2(graph, visited, start, end);

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;
    }
}
