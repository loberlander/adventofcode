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


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    Point start = findStart(map);
    size_t maxX = map[0].length() - 1;
    size_t maxY = map.size() - 1;


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

            class PointPlus
            {
            public:
                PointPlus()
                {}

                PointPlus(Point point, int steps)
                    : point_(point)
                    , steps_(steps)
                {}

                Point point_;
                int steps_;
            };

            Visited visited;
            Visited visitedMaxStep;
            std::deque<PointPlus> q;
            q.emplace_back( PointPlus(start, 0) );
            visited.emplace(start.asString()); // Add start to visited
            static const int MaxSteps = 64;

            while (!q.empty())
            {
                PointPlus current = q.front();
                q.pop_front();

                std::vector<Point> moveOptions;

                if (current.steps_ == MaxSteps)
                {
                    visitedMaxStep.emplace(current.point_.asString());
                    continue;
                }

                if (current.point_.y_ + 1 <= maxY)
                {
                    // attempt to go South
                    moveOptions.emplace_back(Point(current.point_.x_, current.point_.y_ + 1));
                }
                if (current.point_.y_ - 1 >= 0)
                {
                    // attempt to go North
                    moveOptions.emplace_back(Point(current.point_.x_, current.point_.y_ - 1));
                }
                if (current.point_.x_ + 1 <= maxX)
                {
                    // attempt to go East
                    moveOptions.emplace_back(Point(current.point_.x_ + 1, current.point_.y_));
                }
                if (current.point_.x_ - 1 >= 0)
                {
                    // attempt to go West
                    moveOptions.emplace_back(Point(current.point_.x_ - 1, current.point_.y_));
                }

                for (auto newPoint : moveOptions)
                {
                    //if ((map[newPoint.y_][newPoint.x_] == Garden) &&
                    //    (visited.find(newPoint.asString()) == visited.end()))
                    if (map[newPoint.y_][newPoint.x_] == Garden || map[newPoint.y_][newPoint.x_] == Start)
                    {
                        q.emplace_back(PointPlus(newPoint, current.steps_ + 1));
                        visited.emplace(newPoint.asString()); // Add start to visited
                    }
                }
            }


            std::cout << "The total is: " << visitedMaxStep.size();
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;

            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;

    }
}
