#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

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
        return (x_ == other.x_) && (y_ == other.y_);
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
    Node(Point point, Point direction)
        : point_(point)
        , direction_(direction)
    {}

    Node()
        : point_(0, 0)
        , direction_(0, 0)
    {}

    std::string asString()
    {
        return point_.asString() + ":" + direction_.asString();
    }

    std::string asStringCount()
    {
        return point_.asString();
    }

    Point point_;
    Point direction_;
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


bool withinBounds(Map& map, Node& node)
{
    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;
    if (node.point_.x_ < 0 || node.point_.x_ > maxX)
    {
        return false;
    }
    if (node.point_.y_ < 0 || node.point_.y_ > maxY)
    {
        return false;
    }
    return true;
}


void trace(Map& map, Visited& visitedUnique, Visited& visitedCount, Node start)
{
    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    bool done = false;

    while (!done)
    {
        Node newNode1 = start;
        Node newNode2 = start; // Used only when splitting
        char element = map[start.point_.y_][start.point_.x_];

        switch (element)
        {
            case '.':
            {
                // Keep moving in the same direction
                newNode1.point_.x_ += newNode1.direction_.x_;
                newNode1.point_.y_ += newNode1.direction_.y_;
                break;
            }
            case '\\':
            {
                // Change direction
                if (start.direction_ == Point(1, 0))
                {
                    newNode1.direction_ = Point(0, 1);
                }
                else if (start.direction_ == Point(0, -1))
                {
                    newNode1.direction_ = Point(-1, 0);
                }
                else if (start.direction_ == Point(0, 1))
                {
                    newNode1.direction_ = Point(1, 0);
                }
                else if (start.direction_ == Point(-1, 0))
                {
                    newNode1.direction_ = Point(0, -1);
                }
                newNode1.point_.x_ += newNode1.direction_.x_;
                newNode1.point_.y_ += newNode1.direction_.y_;
                break;
            }
            case '/':
            {
                // Change direction
                if (start.direction_ == Point(1, 0))
                {
                    newNode1.direction_ = Point(0, -1);
                }
                else if (start.direction_ == Point(0, -1))
                {
                    newNode1.direction_ = Point(1, 0);
                }
                else if (start.direction_ == Point(0, 1))
                {
                    newNode1.direction_ = Point(-1, 0);
                }
                else if (start.direction_ == Point(-1, 0))
                {
                    newNode1.direction_ = Point(0, 1);
                }
                newNode1.point_.x_ += newNode1.direction_.x_;
                newNode1.point_.y_ += newNode1.direction_.y_;
                break;
            }
            case '|':
            {
                if (start.direction_ == Point(0, 1) || start.direction_ == Point(0, -1))
                {
                    // Keep moving in the same direction
                    newNode1.point_.x_ += newNode1.direction_.x_;
                    newNode1.point_.y_ += newNode1.direction_.y_;
                }
                else
                {
                    newNode1.direction_ = Point(0, 1);
                    newNode1.point_.x_ += newNode1.direction_.x_;
                    newNode1.point_.y_ += newNode1.direction_.y_;

                    newNode2.direction_ = Point(0, -1);
                    newNode2.point_.x_ += newNode2.direction_.x_;
                    newNode2.point_.y_ += newNode2.direction_.y_;
                    if (withinBounds(map, newNode2))
                    {
                        visitedCount.emplace(newNode2.asStringCount(), newNode2);
                        // Has this node been already visited?
                        auto visitedIt = visitedUnique.find(newNode2.asString());
                        if (visitedIt != visitedUnique.end())
                        {
                            // Yes
                            return;
                        }

                        visitedUnique.emplace(newNode2.asString(), newNode2);

                        // Follow the other trace (recursively)
                        trace(map, visitedUnique, visitedCount, newNode2);
                    }
                    
                    if (!withinBounds(map, newNode1))
                    {
                        // Done tracing this ray
                        return;
                    }
                    else
                    {
                        // Keep going
                        visitedCount.emplace(newNode1.asStringCount(), newNode1);
                        start = newNode1;
                        visitedUnique.emplace(start.asString(), start);
                        continue;
                    }
                }
                break;
            }
            case '-':
            {
                if (start.direction_ == Point(1, 0) || start.direction_ == Point(-1, 0))
                {
                    // Keep moving in the same direction
                    newNode1.point_.x_ += newNode1.direction_.x_;
                    newNode1.point_.y_ += newNode1.direction_.y_;
                }
                else
                {
                    newNode1.direction_ = Point(1, 0);
                    newNode1.point_.x_ += newNode1.direction_.x_;
                    newNode1.point_.y_ += newNode1.direction_.y_;

                    newNode2.direction_ = Point(-1, 0);
                    newNode2.point_.x_ += newNode2.direction_.x_;
                    newNode2.point_.y_ += newNode2.direction_.y_;
                    if (withinBounds(map, newNode2))
                    {
                        visitedCount.emplace(newNode2.asStringCount(), newNode2);
                        // Has this node been already visited?
                        auto visitedIt = visitedUnique.find(newNode2.asString());
                        if (visitedIt != visitedUnique.end())
                        {
                            // Yes
                            return;
                        }

                        visitedUnique.emplace(newNode2.asString(), newNode2);

                        // Follow the other trace (recursively)
                        trace(map, visitedUnique, visitedCount, newNode2);
                    }

                    if (!withinBounds(map, newNode1))
                    {
                        // Done tracing this ray
                        return;
                    }
                    else
                    {
                        visitedCount.emplace(newNode1.asStringCount(), newNode1);
                        // Keep going
                        start = newNode1;
                        visitedUnique.emplace(start.asString(), start);
                        continue;
                    }
                }
                break;
            }
        }

        if (!withinBounds(map, newNode1))
        {
            // Done tracing this ray
            return;
        }
        else
        {
            visitedCount.emplace(newNode1.asStringCount(), newNode1);
            // Has this node been already visited?
            auto visitedIt = visitedUnique.find(newNode1.asString());
            if (visitedIt != visitedUnique.end())
            {
                // Yes
                return;
            }

            // Keep going
            start = newNode1;
            visitedUnique.emplace(start.asString(), start);
        }
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;

    readInputFile(inputFileName, map);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    if (map.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            Node start(Point(0, 0), Point(1, 0));
            Visited visitedCount; // to keep track of the nodes already visited
            Visited visitedUnique; // to keep track of the nodes already visited
            visitedCount.emplace(start.asStringCount(), start);
            visitedUnique.emplace(start.asString(), start);
            trace(map, visitedUnique, visitedCount, start);

            size_t total = visitedCount.size();
            std::cout << "Total visited: " << total;
            std::cout << std::endl;
            size_t total2 = visitedUnique.size();
            std::cout << "Total unique visited: " << total2;
        }
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            size_t total = 0;
            for (int y = 0; y <= maxY; y++)
            {
                Node start(Point(0, y), Point(1, 0));
                Visited visitedCount; // to keep track of the nodes already visited
                Visited visitedUnique; // to keep track of the nodes already visited
                visitedCount.emplace(start.asStringCount(), start);
                visitedUnique.emplace(start.asString(), start);
                trace(map, visitedUnique, visitedCount, start);

                size_t runCount = visitedCount.size();
                if (runCount > total)
                {
                    total = runCount;
                }
            }

            for (int y = 0; y <= maxY; y++)
            {
                Node start(Point(maxX, y), Point(-1, 0));
                Visited visitedCount; // to keep track of the nodes already visited
                Visited visitedUnique; // to keep track of the nodes already visited
                visitedCount.emplace(start.asStringCount(), start);
                visitedUnique.emplace(start.asString(), start);
                trace(map, visitedUnique, visitedCount, start);

                size_t runCount = visitedCount.size();
                if (runCount > total)
                {
                    total = runCount;
                }
            }

            for (int x = 0; x <= maxX; x++)
            {
                Node start(Point(x, 0), Point(0, 1));
                Visited visitedCount; // to keep track of the nodes already visited
                Visited visitedUnique; // to keep track of the nodes already visited
                visitedCount.emplace(start.asStringCount(), start);
                visitedUnique.emplace(start.asString(), start);
                trace(map, visitedUnique, visitedCount, start);

                size_t runCount = visitedCount.size();
                if (runCount > total)
                {
                    total = runCount;
                }
            }

            for (int x = 0; x <= maxX; x++)
            {
                Node start(Point(x, maxY), Point(0, -1));
                Visited visitedCount; // to keep track of the nodes already visited
                Visited visitedUnique; // to keep track of the nodes already visited
                visitedCount.emplace(start.asStringCount(), start);
                visitedUnique.emplace(start.asString(), start);
                trace(map, visitedUnique, visitedCount, start);

                size_t runCount = visitedCount.size();
                if (runCount > total)
                {
                    total = runCount;
                }
            }

            std::cout << "Total(max) visited: " << total;
            std::cout << std::endl;

            std::cout << std::endl;
        }
    }
}
