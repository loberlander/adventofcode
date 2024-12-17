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

typedef std::map<std::string, int> VisitedDist; // x:y -> distance

typedef std::map<int, int> StepMap; // key: number of steps, value: number of reachable spots


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

    Point& operator+=(const Point& other)
    {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    Point operator+(const Point& other) const
    {
        return Point(x_ + other.x_, y_ + other.y_);
    }

    Point operator-(const Point& other) const
    {
        return Point(x_ - other.x_, y_ - other.y_);
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    std::string asStringWithExtra()
    {
        return std::to_string(extra_) + ":" + std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
    int extra_;
};

// Directions
typedef Point Dir;
typedef std::vector<Dir> Dirs;

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

int count(VisitedDist& visitedDist, int minDist, int maxDist, bool even = true)
{
    int count = 0;

    for (auto& visited : visitedDist)
    {
        if (even)
        {
            if (minDist <= visited.second && visited.second <= maxDist && visited.second % 2 == 0)
            {
                count++;
            }
        }
        else
        {
            if (minDist <= visited.second && visited.second <= maxDist && visited.second % 2 == 1)
            {
                count++;
            }
        }
    }

    return count;
}

void calcDist(Map& map, VisitedDist& visited, Point start)
{
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;

    size_t sizeX = map[0].length();
    size_t sizeY = map.size();

    std::deque<Point> q;
    q.emplace_back(Point(start, 0));

    // Calculate how many steps it takes to get to each available spot for the first time
    while (!q.empty())
    {
        Point current = q.front();
        q.pop_front();

        // Has this spot been visited before with lower number of steps?
        if (visited.find(current.asString()) != visited.end())
        {
            continue;
        }

        // Save the spot in the visited map
        visited.emplace(current.asString(), current.extra_);

        // Evaluate all 4 possible next spots
        int distance = current.extra_ + 1;
        for (auto& dir : Dirs{ {0, 1}, {0, -1}, {1,0}, {-1,0} })
        {
            Point newPoint(current + dir, distance);
            
            if ((visited.find(newPoint.asString()) == visited.end()) && // This is not absolutely needed, but if we aready know, why bother adding it to the q
                (newPoint.inRange(0, maxX, 0, maxY) &&
                (map[newPoint.y_][newPoint.x_] != Rock)))
            {
                // Have not seen it yet...
                q.emplace_back(newPoint);
            }
        }
    }
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

    StepMap stepMap;
    VisitedDist visitedDist;

    // Assumption:
    // - Once a spot has been visited, it can be re-visited be stepping away and then back
    //   - Once a spot is reached, each spot is either visited on every even or every odd steps

    std::cout << "Calculating ALL distances..." << std::endl;
    calcDist(map, visitedDist, start);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;
        {
            std::cout << "The total is: " << count(visitedDist, 0, 64, true);
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            // Follow illustration at
            // https://github.com/villuna/aoc23/wiki/A-Geometric-solution-to-advent-of-code-2023,-day-21
            BigNumber total = 0;
            BigNumber numSteps = 26501365;

            // Assumptions:
            //   - map is a square (same x and y dimensions
            //   - there is a straight path from Start to the edge in each direction
            //   - corners (not full square blocks) are generated in fours forming 
            //     - there is no need to count individual corners, but instead a set of 4 (i.e. only all 4 corners) 

            if (sizeX != sizeY)
            {
                exit(-1);
            }
            size_t sizeMap = sizeX;

            // Let n be the size of the side length of the square formed by the even numbered blocks
            BigNumber n = (numSteps - sizeMap / 2) / sizeMap;

            // Therefore, the number of blocks of each kind:
            BigNumber numBlocksEven = n * n;
            BigNumber numBlocksOdd = (n + 1) * (n + 1);
            BigNumber numBlocksCornersEven = n;
            BigNumber numBlocksCornersOdd = n + 1;

            BigNumber numStepsFullEven = count(visitedDist, 0, 200, true);
            BigNumber numStepsFullOdd = count(visitedDist, 0, 200, false);
            BigNumber numStepsCornersEven = count(visitedDist, 66 , 200, true);
            BigNumber numStepsCornersOdd = count(visitedDist, 66, 200, false);

            total =
                numBlocksEven * numStepsFullEven
                + numBlocksOdd * numStepsFullOdd
                + numBlocksCornersEven * numStepsCornersEven
                - numBlocksCornersOdd * numStepsCornersOdd;

            std::cout << "The total is: " << total;

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
