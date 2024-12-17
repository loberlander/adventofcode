#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";


typedef std::vector<std::vector<char>> Map;
typedef std::vector<char> Path;


class Point
{
public:
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
        return (x_ == other.x_) && (y_==other.y_);
    }

    std::string asString()
    {
        return std::to_string(x_) + ":" + std::to_string(y_);
    }

    int x_;
    int y_;
};

class Node
{
public:
    Node(std::string id, int distance, Point point)
        : id_(id)
        , distance_(distance)
        , point_(point)
    {}

    std::string id_;
    int distance_;
    Point point_;
};

typedef std::map<std::string, Node> Visited; 

void readInputFile(std::string fileName, Map& map)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            std::vector<char> charLine;
            if(!line.empty())
            {
                for(auto& ch: line)
                {
                    charLine.emplace_back(ch);
                }
                map.emplace_back(charLine);
            }
        }
        myFile.close();
    }
}


bool isAllowedMove(Map& map, Point currentPoint, Point newPoint)
{
    char currentHeight = map[currentPoint.y_][currentPoint.x_];
    char newHeight = map[newPoint.y_][newPoint.x_];

    // Make sure it is not a point already visited
    if (newHeight >= 'a' && newHeight <= 'z')
    {
        // going same level or down is always okay
        if (newHeight <= currentHeight)
        {
            return true;
        }

        if (newHeight == currentHeight + 1)
        {
            return true;
        }
    }

    return false;
}

bool isAllowedMoveBack(Map& map, Point currentPoint, Point newPoint)
{
    char currentHeight = map[currentPoint.y_][currentPoint.x_];
    char newHeight = map[newPoint.y_][newPoint.x_];

    // Make sure it is not a point already visited
    if (newHeight >= 'a' && newHeight <= 'z')
    {
        // going same level or up is always okay
        if (newHeight >= currentHeight)
        {
            return true;
        }

        if (newHeight + 1 == currentHeight)
        {
            return true;
        }
    }

    return false;
}

int findBestPath(Map& map, Visited& visited, Point currentPoint, Point end)
{
    std::deque<Node> q;

    int distance = 0;
    // Add start position the queue and visited nodes map
    Node startNode(currentPoint.asString(), distance, currentPoint);
    visited.emplace(currentPoint.asString(), startNode);
    q.emplace_back(startNode);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    while(!q.empty())
    {
        Node currentNode = q.front();
        q.pop_front();

        std::vector<Point> moveOptions;

        if (currentNode.point_.y_ + 1 <= maxY)
        {
            // attempt to go South
            moveOptions.emplace_back(Point(currentNode.point_.x_, currentNode.point_.y_ + 1));
        }
        if (currentNode.point_.y_ - 1 >= 0)
        {
            // attempt to go North
            moveOptions.emplace_back(Point(currentNode.point_.x_, currentNode.point_.y_ - 1));
        }
        if (currentNode.point_.x_ + 1 <= maxX)
        {
            // attempt to go East
            moveOptions.emplace_back(Point(currentNode.point_.x_ + 1, currentNode.point_.y_));
        }
        if (currentNode.point_.x_ - 1 >= 0 )
        {
            // attempt to go West
            moveOptions.emplace_back(Point(currentNode.point_.x_ - 1, currentNode.point_.y_));
        }

        for(auto newPoint: moveOptions)
        {
            if (visited.find(newPoint.asString()) != visited.end())
            {
                // Skip locations that have been already visited
                continue;
            }
            if (!isAllowedMove(map, currentNode.point_, newPoint))
            {
                // Is it a point that we can move to?
                continue;
            }
            if (newPoint == end)
            {
                // Check if the end position is found
                return currentNode.distance_ + 1;
            }

            Node newNode(newPoint.asString(), currentNode.distance_ + 1, newPoint);
            visited.emplace(newPoint.asString(), newNode);
            q.emplace_back(newNode);
        }
    }

    return -1;
}

int findBestPathTillA(Map& map, Visited& visited, Point currentPoint)
{
    std::deque<Node> q;

    int distance = 0;
    // Add start position the queue and visited nodes map
    Node startNode(currentPoint.asString(), distance, currentPoint);
    visited.emplace(currentPoint.asString(), startNode);
    q.emplace_back(startNode);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    while(!q.empty())
    {
        Node currentNode = q.front();
        q.pop_front();

        std::vector<Point> moveOptions;

        if (currentNode.point_.y_ + 1 <= maxY)
        {
            // attempt to go South
            moveOptions.emplace_back(Point(currentNode.point_.x_, currentNode.point_.y_ + 1));
        }
        if (currentNode.point_.y_ - 1 >= 0)
        {
            // attempt to go North
            moveOptions.emplace_back(Point(currentNode.point_.x_, currentNode.point_.y_ - 1));
        }
        if (currentNode.point_.x_ + 1 <= maxX)
        {
            // attempt to go East
            moveOptions.emplace_back(Point(currentNode.point_.x_ + 1, currentNode.point_.y_));
        }
        if (currentNode.point_.x_ - 1 >= 0 )
        {
            // attempt to go West
            moveOptions.emplace_back(Point(currentNode.point_.x_ - 1, currentNode.point_.y_));
        }

        for(auto newPoint: moveOptions)
        {
            if (visited.find(newPoint.asString()) != visited.end())
            {
                // Skip locations that have been already visited
                continue;
            }
            if (!isAllowedMoveBack(map, currentNode.point_, newPoint))
            {
                // Is it a point that we can move to?
                continue;
            }
            if (map[newPoint.y_][newPoint.x_] == 'a')
            {
                // Check if the end position is found
                return currentNode.distance_ + 1;
            }

            Node newNode(newPoint.asString(), currentNode.distance_ + 1, newPoint);
            visited.emplace(newPoint.asString(), newNode);
            q.emplace_back(newNode);
        }
    }

    return -1;
}

Point findEnd(Map& map)
{
    Point target(0, 0);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    for(int y = 0; y <= maxY; y++)
    {
        for(int x = 0; x <= maxX; x++)
        {
            if (map[y][x] == 'E')
            {
                target.x_= x;
                target.y_= y;
                map[y][x] = 'z';
                return target;
            }
        }
    }
    return target;
}

Point findStart(Map& map)
{
    Point target(0, 0);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    for(int y = 0; y <= maxY; y++)
    {
        for(int x = 0; x <= maxX; x++)
        {
            if (map[y][x] == 'S')
            {
                target.x_= x;
                target.y_= y;
                map[y][x] = 'a';
                return target;
            }
        }
    }
    return target;
}

std::vector<Point> findLevelAs(Map& map)
{
    std::vector<Point> points;

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    for(int y = 0; y <= maxY; y++)
    {
        for(int x = 0; x <= maxX; x++)
        {
            if (map[y][x] == 'a')
            {
                points.emplace_back(Point(x, y));
            }
        }
    }
    return points;
}

// Solution us based on
// https://en.wikipedia.org/wiki/Breadth-first_search
int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    Point start(findStart(map)); // also replaces 'S' with 'a'
    Point end(findEnd(map));  // also replaces 'E' with 'z'

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            // Going from S to E
            Visited visited; // to keep track of the nodes already visited
            int result = findBestPath(map, visited, start, end);
            std::cout << "Best Path length: " << result;
            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {
            {
                // Going from E to first 'a'
                // Search backwards until the first 'a' is found
                Visited visited; // to keep track of the nodes already visited
                int result = findBestPathTillA(map, visited, end);
                std::cout << "Best Path length: " << result;
                std::cout << std::endl;
            }
            {
                /*
                // Going from all 'a' to E and finding the minimum
                // Brute force search of all 'a' starting points, kind of slow
                std::vector<Point> startPoints = findLevelAs(map);
                std::vector<int> steps;

                for(auto& point: startPoints)
                {
                    Visited visited; // to keep track of the nodes already visited
                    int result = findBestPath(map, visited, point, end);
                    std::cout << "Best Path from (" << point.x_ << ", " << point.y_ << "): " << result << std::endl;
                    if (result > 0)
                    {
                        steps.emplace_back(result);
                    }
                }

                std::sort(steps.begin(), steps.end());
                std::cout << "Best Path length: " << steps[0];

                std::cout << std::endl;
                */
            }
        }
    }
}
