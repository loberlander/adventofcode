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

    std::string asString() const
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
};

bool operator<(const Point& sk1, const Point& sk2)
{
    return sk1.asString() < sk2.asString();
}

typedef std::set<Point> Points;
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

    const std::string asString()
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

Directions DirectionPossibilities =
{
    Direction(0, -1),
    Direction(1, 0),
    Direction(0, 1),
    Direction(-1, 0)
};

class Region
{
public:
    Region()
        : type_('?')
    { }

    Region(Points points, char type)
        : type_(type)
        , points_(points)
    { }

    size_t area()
    {
        return points_.size();
    }

    int perimeter()
    {
        int per = 0;

        for (auto& point : points_)
        {
            int numNeighbors = 0;
            for (auto& dir : DirectionPossibilities)
            {
                auto cpoint = point;
                Point newPoint(cpoint + dir);
                if (points_.find(newPoint) != points_.end())
                {
                    numNeighbors++;
                }
            }
            per += (4 - numNeighbors);
        }

        return per;
    }

    int sides()
    {
        // The number of sides equals to the number of corners in the shape of the region
        // So let's find all the corners
        int numCorners = 0;

        // Since corners are theoretically 0.5 units away from center we would need a floating point coordinate system
        // Instead of having floating point units, let's multiply by 10 for the corner coordinates!
        std::set<Point> corners;

        for (auto& point : points_)
        {
            for (auto& dir : { Direction{1, 1}, Direction{1, -1}, Direction{-1, -1}, Direction{-1, 1} })
            {
                // Since it is a set, it will automagically eliminate duplicates
                corners.insert(Point(10 * point.x_ + 5 * dir.x_, 10 * point.y_ + 5 * dir.y_));
            }
        }

        // Walk through all corners and see which blocks it is neighboring
        for (auto& corner : corners)
        {
            int numNeighboringCorners = 0;
            std::vector<bool> NeighboringCorners;
            // Walk the 4 neighbors of the corner in the order defined by DirectionPossibilities
            for (auto& dir : { Direction{1, 1}, Direction{1, -1}, Direction{-1, -1}, Direction{-1, 1} })
            {
                Point neighbor((corner.x_ + 5 * dir.x_) / 10, (corner.y_ + 5 * dir.y_) / 10);
                if (points_.find(neighbor) != points_.end())
                {
                    // There is a neighbor here
                    numNeighboringCorners++;
                    NeighboringCorners.push_back(true);
                }
                else
                {
                    // There is NO neighbor here
                    NeighboringCorners.push_back(false);
                }
            }

            switch (numNeighboringCorners)
            {
            case 0:
                // This should never happen
                break;
            case 1:
            case 3:
                numCorners += 1;
                break;
            case 2:
                // This is a cut out corner inside of a shape
                if ((NeighboringCorners[0] && NeighboringCorners[2]) || (NeighboringCorners[1] && NeighboringCorners[3]))
                {
                    numCorners += 2;
                }
                break;
            case 4:
                // nothing to do here, it is not an overall corner
                break;
            }
        }

        return numCorners;
    }

    Points points_;
    char type_;
};


typedef std::vector<Region> Regions;
typedef long long BigNumber;
typedef std::set<std::string> Visited; // x:y
typedef std::map<std::string, BigNumber> VisitedDist; // x:y -> distance


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

Region discover(Map& map, Point point, Visited& visited)
{
    std::deque<Point> q;
    Region region;
    char type = map[point.y_][point.x_];
    region.type_ = type;

    // Place the starting point on the Q
    q.emplace_back(point);

    while (!q.empty())
    {
        auto current = q.front();
        q.pop_front();

        // Has this spot been reached before
        if (visited.find(current.asString()) != visited.end())
        {
            // Yes, ignore any additional routes
            // I.e. count every destination only once
            continue;
        }

        visited.emplace(current.asString());
        region.points_.emplace(current);

        for (auto& dir : DirectionPossibilities)
        {
            Point newPoint(current + dir);
            if (isWithinBounds(map, newPoint))
            {
                if (map[newPoint.y_][newPoint.x_] == type)
                {
                    q.emplace_back(newPoint);
                }
            }
        }
    }

    return region;
}


void findRegions(Map& map, Regions& regions)
{
    Visited visited;

    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            Point point(x, y);
            bool found = visited.find(point.asString()) != visited.end();
            if (!found)
            {
                // New region needs to be discovered
                Region region = discover(map, point, visited);
                regions.push_back(region);
            }
        }
    }
}



int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;
    Regions regions;

    readInputFile(inputFileName, map);

    std::cout << "Finding regions..." << std::endl;
    findRegions(map, regions);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;

            for (auto& region : regions)
            {
                sum += region.area() * region.perimeter();
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            for (auto& region : regions)
            {
                sum += region.area() * region.sides();
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
