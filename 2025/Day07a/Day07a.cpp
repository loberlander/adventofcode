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

class Point
{
    public:
        Point()
            : x_(0)
            , y_(0)
        {
        }

        Point(BigNumber x, BigNumber y)
            : x_(x)
            , y_(y)
        {
        }

        Point(const Point& point)
            : x_(point.x_)
            , y_(point.y_)
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

        BigNumber x()
        {
            return x_;
        }

        BigNumber y()
        {
            return y_;
        }

        std::string asString()
        {
            return std::to_string(x_) + ":" + std::to_string(y_);
        }

    private:
        BigNumber x_;
        BigNumber y_;
};

typedef std::set<std::string> Visited;
typedef std::map<std::string, BigNumber> Cache;

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

Point findStart(Map& map)
{
    for (auto y = 0; y < map.size(); y++)
    {
        for (auto x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] == START)
            {
                return Point(x, y);
            }
        }
    }
    return Point(-1, -1);
}


void addBeam(std::deque<Point>& q, Point point, Visited& visited)
{
    // Has this spot been reached before
    if (visited.find(point.asString()) != visited.end())
    {
        // Each beam counts only once
        // Eliminate duplicates
        return;
    }
    visited.emplace(point.asString());
    q.emplace_back(point);
}

int countSplits(Map& map, Point pointStart)
{
    int score = 0;

    std::deque<Point> q;
    Visited visited;

    // Place the starting point on the Q
    q.emplace_back(pointStart);

    while (!q.empty())
    {
        auto currentPoint = q.front();
        q.pop_front();

        auto current = map[currentPoint.y()][currentPoint.x()];

        if (current == START || current == SPACE)
        {
            if (currentPoint.y() == map.size() - 1)
            {
                // reached the bottom
                continue;
            }
            addBeam(q, Point(currentPoint.x(), currentPoint.y() + 1), visited); // move down
        }
        else if (current == SPLITTER)
        {
            score++;
            addBeam(q, Point(currentPoint.x() + 1, currentPoint.y()), visited); // move to the right
            addBeam(q, Point(currentPoint.x() - 1, currentPoint.y()), visited); // move to the left
        }
    }

    return score;
}


BigNumber countPaths(Map& map, Point point, Cache& cache)
{
    // There are many paths that are taken over and over again, so cache the results
    // and use them if we have already seen this point
    auto it = cache.find(point.asString());
    if (it != cache.end())
    {
        return it->second;
    }

    if (point.y() == (BigNumber) map.size() - 1)
    {
        // reached the bottom
        BigNumber count = 1;
        cache.emplace(point.asString(), count);
        return count;
    }

    auto current = map[point.y()][point.x()];

    if (current == START || current == SPACE)
    {
        BigNumber count = countPaths(map, Point(point.x(), point.y() + 1), cache);
        cache.emplace(point.asString(), count);
        return count;
    }
    else if (current == SPLITTER)
    {
        BigNumber count = countPaths(map, Point(point.x() + 1, point.y()), cache) + countPaths(map, Point(point.x() - 1, point.y()), cache);
        cache.emplace(point.asString(), count);
        return count;
    }

    // should never get here
    return 0;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;

    readInputFile(inputFileName, map);
    Point pointStart = findStart(map);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

            result = countSplits(map, pointStart);

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            Cache cache;
            result = countPaths(map, pointStart, cache);

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
