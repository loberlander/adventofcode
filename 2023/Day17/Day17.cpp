#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <queue>
#include <algorithm>

// Based on the priority queue version of https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm 

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";


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
    Node(std::string id, int heatloss, Point point, int pathLength, Point lastDirection, std::string path)
        : id_(id)
        , heatloss_(heatloss)
        , point_(point)
        , pathLength_(pathLength)
        , lastDirection_(lastDirection)
        , path_(path)
    {}

    std::string asString()
    {
        return std::to_string(point_.x_) + ":" + std::to_string(point_.y_);
    }

    std::string id_;
    int heatloss_;
    Point point_;
    int pathLength_;
    Point lastDirection_;
    std::string path_;
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


struct NodeComparator
{
    //bool operator()(const Hand& lhs, const Hand& rhs) const;
    bool operator()(const Node& lhs, const Node& rhs) const
    {
        // lhs is less return true
        // otherwise (rhs is less) return false
        if (lhs.heatloss_ > rhs.heatloss_)
        {
            return true;
        }

        return false;
    }
};

int findBestPathPart1(Map& map, Visited& visited, Point currentPoint, Point end)
{
    std::priority_queue<Node, std::deque<Node>, NodeComparator> q;

    int heatloss = 0;
    // Add start position the queue and visited nodes map
    Node startNode(currentPoint.asString(), heatloss, currentPoint, 0, Point(0, 0), "S");
    visited.emplace(currentPoint.asString(), startNode);
    q.emplace(startNode);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    while(!q.empty())
    {
        Node currentNode = q.top();
        q.pop();

        if (currentNode.point_.x_ == end.x_ && currentNode.point_.y_ == end.y_)
        {
            return currentNode.heatloss_;
        }

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
            Point newDirection(newPoint.x_ - currentNode.point_.x_, newPoint.y_ - currentNode.point_.y_);
            std::string newPath;
            if (newDirection == Point(0, 1))
            {
                newPath = "v";
            }
            else if (newDirection == Point(1, 0))
            {
                newPath = ">";
            }
            else if (newDirection == Point(0, -1))
            {
                newPath = "^";
            }
            else if (newDirection == Point(-1, 0))
            {
                newPath = "<";
            }
            else
            {
                newPath = "?";
            }

            int newPathLength = 0;
            if (newDirection == currentNode.lastDirection_)
            {
                newPathLength = currentNode.pathLength_ + 1;
                if (newPathLength == 4)
                {
                    // It is not an allowed point that we can move to!
                    continue;
                }
            }
            else if (Point(currentNode.lastDirection_.x_ * (-1), currentNode.lastDirection_.y_ * (-1)) == newDirection)
            {
                //cannot turn back
                continue;
            }
            else
            {
                newPathLength = 1;
            }

            // Calculate possible new heatloss value
            int newHeatloss = currentNode.heatloss_ + (map[newPoint.y_][newPoint.x_] - '0');


            // Have we been here before? 
            auto visitedIt = visited.find(newPoint.asString() + newDirection.asString() + std::to_string(newPathLength));
            if (visitedIt != visited.end())
            {
                continue;
            }

            Node newNode(newPoint.asString(), newHeatloss, newPoint, newPathLength, newDirection, currentNode.path_ + newPath);
            visited.emplace(newPoint.asString() + newDirection.asString() + std::to_string(newPathLength), newNode);
            q.emplace(newNode);
        }
    }

    return -1;
}

int findBestPathPart2(Map& map, Visited& visited, Point currentPoint, Point end)
{
    std::priority_queue<Node, std::deque<Node>, NodeComparator> q;

    int heatloss = 0;
    // Add start position the queue and visited nodes map
    Node startNode(currentPoint.asString(), heatloss, currentPoint, 0, Point(0, 0), "S");
    visited.emplace(currentPoint.asString(), startNode);
    q.emplace(startNode);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    while (!q.empty())
    {
        Node currentNode = q.top();
        q.pop();

        if (currentNode.point_.x_ == end.x_ && currentNode.point_.y_ == end.y_ && currentNode.pathLength_ >= 4)
        {
            return currentNode.heatloss_;
        }

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
        if (currentNode.point_.x_ - 1 >= 0)
        {
            // attempt to go West
            moveOptions.emplace_back(Point(currentNode.point_.x_ - 1, currentNode.point_.y_));
        }

        for (auto newPoint : moveOptions)
        {
            Point newDirection(newPoint.x_ - currentNode.point_.x_, newPoint.y_ - currentNode.point_.y_);
            std::string newPath;
            if (newDirection == Point(0, 1))
            {
                newPath = "v";
            }
            else if (newDirection == Point(1, 0))
            {
                newPath = ">";
            }
            else if (newDirection == Point(0, -1))
            {
                newPath = "^";
            }
            else if (newDirection == Point(-1, 0))
            {
                newPath = "<";
            }
            else
            {
                newPath = "?";
            }

            int newPathLength = 0;
            if (newDirection == currentNode.lastDirection_)
            {
                newPathLength = currentNode.pathLength_ + 1;
                if (newPathLength == 11)
                {
                    // It is not an allowed point that we can move to!
                    continue;
                }
            }
            else if (Point(currentNode.lastDirection_.x_ * (-1), currentNode.lastDirection_.y_ * (-1)) == newDirection)
            {
                //cannot turn back
                continue;
            }
            else if (!(currentNode.point_ == Point(0, 0)) && !(newDirection == currentNode.lastDirection_) && currentNode.pathLength_ < 4)
            {
                // Must continue on the same path for at least this long
                continue;
            }
            else
            {
                newPathLength = 1;
            }

            // Calculate possible new heatloss value
            int newHeatloss = currentNode.heatloss_ + (map[newPoint.y_][newPoint.x_] - '0');


            // Have we been here before? 
            auto visitedIt = visited.find(newPoint.asString() + newDirection.asString() + std::to_string(newPathLength));
            if (visitedIt != visited.end())
            {
                continue;
            }

            Node newNode(newPoint.asString(), newHeatloss, newPoint, newPathLength, newDirection, currentNode.path_ + newPath);
            visited.emplace(newPoint.asString() + newDirection.asString() + std::to_string(newPathLength), newNode);
            q.emplace(newNode);
        }
    }

    return -1;
}


// Solution us based on
// https://en.wikipedia.org/wiki/Breadth-first_search
int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    Point start(0, 0);
    Point end((int)maxX, (int)maxY);

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            // Going from top left corner to botton right
            Visited visited; // to keep track of the nodes already visited
            int result = findBestPathPart1(map, visited, start, end);
            std::cout << "Best Path heatloss: " << result;
            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {
            // Going from top left corner to botton right
            Visited visited; // to keep track of the nodes already visited
            int result = findBestPathPart2(map, visited, start, end);
            std::cout << "Best Path heatloss: " << result;
            std::cout << std::endl;
        }
    }
}