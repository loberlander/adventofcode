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

typedef std::set<std::string> Visited; // x:y
typedef std::map<std::string, int> VisitedDist; // x:y -> distance
typedef long long BigNumber;

Directions DirectionPossibilities=
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


int reachable(Map& map, Point& point, bool useDistinct)
{
    int score = 0;
    
    std::deque<Node> q;
    Visited visited;

    // Place the starting point on the Q
    q.emplace_back(Node(point, Direction(0, 0)));

    while (!q.empty())
    {
        auto current = q.front();
        q.pop_front();

        int currentHeight = stoi(std::string(1, map[current.point_.y_][current.point_.x_]));

        // Reached the top
        if (currentHeight == 9)
        {
            if (!useDistinct)
            {
                // Has this spot been reached before
                if (visited.find(current.point_.asString()) != visited.end())
                {
                    // Yes, ignore any additional routes
                    // I.e. count every destination only once
                    continue;
                }

                // This point has been visted...
                visited.emplace(current.point_.asString());
            }
            score++;
            continue;
        }

        for (auto& dir : DirectionPossibilities)
        {
            Point newPoint(current.point_ + dir);
            if (isWithinBounds(map, newPoint))
            {
                if (currentHeight + 1 == stoi(std::string(1, map[newPoint.y_][newPoint.x_])))
                {
                    q.emplace_back(Node(newPoint, Direction(0, 0)));
                }
            }
        }
    }

    return score;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    Points startPoints;

    readInputFile(inputFileName, map);

    findStarts(map, startPoints);

    if (map.empty() || startPoints.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            int sum = 0;

            for (auto& startPoint : startPoints)
            {
                sum += reachable(map, startPoint, false);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            int sum = 0;

            for (auto& startPoint : startPoints)
            {
                sum += reachable(map, startPoint, true);
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
