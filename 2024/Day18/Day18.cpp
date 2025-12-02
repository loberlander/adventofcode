#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <cmath>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";
//static const char inputFileName[] = "input_test6.txt";
//static const char inputFileName[] = "input_test7.txt";

//static const int GRID_MAX = 6;
//static const int NUM_OBJECTS1 = 12;

static const int  GRID_MAX = 70;
static const int  NUM_OBJECTS1 = 1024;

typedef long long BigNumber;


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

    BigNumber x_;
    BigNumber y_;
};

typedef std::vector<Point> Points;
typedef Point Direction;
//typedef std::vector<Direction> Directions;

std::map<char, Direction> Directions =
{
    {'>', Direction(1, 0)},
    {'v', Direction(0, 1)},
    {'<', Direction(-1, 0)},
    {'^', Direction(0, -1)}
};


class Node
{
public:
    Node(Point point, Points path)
        : point_(point)
        , path_(path)
    {
    }

    std::string asString()
    {
        return std::to_string(point_.x_) + ":" + std::to_string(point_.y_);
    }

    Point point_;
    Points path_;
};

/*
struct NodeComparator
{
    //bool operator()(const Hand& lhs, const Hand& rhs) const;
    bool operator()(const Node& lhs, const Node& rhs) const
    {
        // lhs is less return true
        // otherwise (rhs is less) return false
        if (lhs.score_ > rhs.score_)
        {
            return true;
        }

        return false;
    }
};
*/


typedef std::map<std::string, Node> Visited;
typedef std::set<std::string> Seen;

typedef std::set<std::string> Obstacles;


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


void readInputFile(std::string fileName, Points& points)
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
                    // Read map
                    if (!line.empty())
                    {
                        auto poinString = split(line, { "," });

                        if (poinString.size() == 2)
                        {
                            points.emplace_back(stoi(poinString[0]), stoi(poinString[1]));
                        }
                    }
                    else
                    {
                        //state++;
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}


int findBestPath(Obstacles& obstacles, Point& start, Point& end)
{
    std::deque<Node> q;
    Seen visited;

    // Place the starting point on the Q
    Points path;
    path.emplace_back(start);
    q.emplace_back(Node(start, path));

    while (!q.empty())
    {
        auto current = q.front();
        q.pop_front();

        if (current.point_ == end)
        {
            return (int) current.path_.size() - 1;
        }
        
        for (auto& dir : Directions)
        {
            Point newPoint(current.point_ + dir.second);

            if (newPoint.x_ < 0 || newPoint.x_ > GRID_MAX || newPoint.y_ < 0 || newPoint.y_ > GRID_MAX)
            {
                // out of bounds
                continue;
            }

            if (obstacles.find(newPoint.asString()) != obstacles.end())
            {
                // There is an obstacle there, cannot go there
                continue;
            }

            // Has this spot been reached before
            if (visited.find(newPoint.asString()) != visited.end())
            {
                // ignore
                continue;
            }

            Points path = current.path_;
            path.emplace_back(newPoint);
            q.emplace_back(Node(newPoint, path));
            visited.emplace(newPoint.asString());
        }
    }

    return 0;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    std::string program;
    Points points;

    readInputFile(inputFileName, points);

    if (points.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;
            Obstacles obstacles;
            Point start(0, 0);
            Point end(GRID_MAX, GRID_MAX);

            for (int i = 0; i < NUM_OBJECTS1; i++)
            {
                obstacles.emplace(points[i].asString());
            }

            sum = findBestPath(obstacles, start, end);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;
            Obstacles obstacles;
            Point start(0, 0);
            Point end(GRID_MAX, GRID_MAX);

            for (int i = 0; i < points.size(); i++)
            {
                obstacles.emplace(points[i].asString());

                if (i >= NUM_OBJECTS1)
                {
                    if (findBestPath(obstacles, start, end) == 0)
                    {
                        std::cout << points[i].x_ << "," << points[i].y_;
                        std::cout << std::endl;
                        sum = i;
                        break;
                    }
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
