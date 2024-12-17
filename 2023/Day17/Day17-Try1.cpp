#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "input_test.txt";


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


int findBestPath(Map& map, Visited& visited, Point currentPoint, Point end)
{
    std::deque<Node> q;

    int heatloss = 0;
    // Add start position the queue and visited nodes map
    Node startNode(currentPoint.asString(), heatloss, currentPoint, 0, Point(0, 0), "S");
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
/*
            if (newPoint == end)
            {
                // Check if the end position is found
                return currentNode.heatloss_ + map[newPoint.y_][newPoint.x_];
            }
*/
            // Calculate possible new heatloss value
            int newHeatloss = currentNode.heatloss_ + (map[newPoint.y_][newPoint.x_] - '0');

            // Has there been another path leading here already?
            auto visitedIt = visited.find(newPoint.asString());
            if (visitedIt != visited.end())
            {
                if (newHeatloss >= visitedIt->second.heatloss_)
                {
                    // new path is not better
                    continue;
                }
                else
                {
                    visited.erase(visitedIt);
                }
            }

            Node newNode(newPoint.asString(), newHeatloss, newPoint, newPathLength, newDirection, currentNode.path_ + newPath);
            visited.emplace(newPoint.asString(), newNode);
            q.emplace_back(newNode);

        }
    }


    auto visitedIt = visited.find(end.asString());
    if (visitedIt != visited.end())
    {
        std::cout << visitedIt->second.path_ << std::endl; 
        return visitedIt->second.heatloss_;
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
            int result = findBestPath(map, visited, start, end);
            std::cout << "Best Path heatloss: " << result;
            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {
            std::cout << std::endl;
        }
    }
}
