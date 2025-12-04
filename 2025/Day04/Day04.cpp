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
#include <ranges> // For std::views::reverse was added in C++20

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";

typedef long long BigNumber;

typedef std::vector<char> Line;
typedef std::vector<Line> Map;

static const char PaperRoll = '@';
static const char Space = '.';

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

typedef Point Direction;

std::vector<Direction> Directions =
{
    Direction(1, 0),
    Direction(1, 1),
    Direction(0, 1),
    Direction(-1, 1),
    Direction(-1, 0),
    Direction(-1, -1),
    Direction(0, -1),
    Direction(1, -1)
};

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
                    if (!line.empty())
                    {
                        Line data(line.begin(), line.end());
                        map.push_back(data);
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


bool isWithinBounds(Map& map, Point p)
{
    // Boundary checks
    if (p.x() < 0 || p.x() >= (BigNumber) map[0].size())
    {
        return false;
    }
    if (p.y() < 0 || p.y() >= (BigNumber) map.size())
    {
        return false;
    }

    return true;
}

BigNumber countWithNeighbors(Map& map, Point p, char neighborDef)
{
    BigNumber neighbors = 0;

    for (auto& direction : Directions)
    {
        Point neighbor = p + direction;

        if (isWithinBounds(map, neighbor) 
            && map[neighbor.y()][neighbor.x()] == neighborDef)
        {
            ++neighbors;
        }
    }

    return neighbors;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;

    readInputFile(inputFileName, map);

    auto sizeY = (BigNumber)map.size();
    auto sizeX = (BigNumber)map[0].size();

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

            for (BigNumber y = 0; y < sizeY; y++)
            {
                for (BigNumber x = 0; x < sizeX; x++)
                {
                    if (map[y][x] == PaperRoll)
                    {
                        if (countWithNeighbors(map, Point(x, y), PaperRoll) < 4)
                        {
                            ++result;
                        }
                    }
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber prevResult = -1;
            BigNumber result = 0;

            // Loop until no mpore changes
            while (prevResult != result)
            {
                prevResult = result;
                for (BigNumber y = 0; y < sizeY; y++)
                {
                    for (BigNumber x = 0; x < sizeX; x++)
                    {
                        if (map[y][x] == PaperRoll)
                        {
                            if (countWithNeighbors(map, Point(x, y), PaperRoll) < 4)
                            {
                                ++result;
                                map[y][x] = Space;
                            }
                        }
                    }
                }
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
         }

        std::cout << std::endl;
    }
}
