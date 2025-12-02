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
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef std::vector<std::string> Map;
typedef std::string Moves;

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
    Node(BigNumber distFromStart, Point point, char direction, std::string path)
        : distFromStart_(distFromStart)
        , point_(point)
        , direction_(direction)
        , path_(path)
    {
    }

    std::string asString()
    {
        return std::to_string(point_.x_) + ":" + std::to_string(point_.y_);
    }

    BigNumber distFromStart_;
    Point point_;
    char direction_;
    std::string path_;
};


struct NodeComparator
{
    //bool operator()(const Hand& lhs, const Hand& rhs) const;
    bool operator()(const Node& lhs, const Node& rhs) const
    {
        // lhs is less return true
        // otherwise (rhs is less) return false
        if (lhs.distFromStart_ > rhs.distFromStart_)
        {
            return true;
        }

        return false;
    }
};


typedef std::map<std::string, Node> Visited;
typedef std::set<std::string> Seen;


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
            switch(state)
            {
                case 1:
                {
                    // Read map
                    if (!line.empty())
                    {
                        map.emplace_back(line);
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


Point findPoint(Map& map, char type)
{
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] == type)
            {
                return Point(x, y);
            }
        }
    }

    return {-1, -1};
}


bool isWithinBounds(Map& map, Point p)
{
    if (p.x_ < 0 || p.y_ < 0 || p.x_ >= (BigNumber) map[0].size() || p.y_ >= (BigNumber) map.size())
    {
        return false;
    }
    return true;
}


// Possible approach to solve
// - Build a database for all points (x, y) on the track to find out their on track distance to the end point
// - Walk the established path and check the closest track points to the end point that are reachable (straight line x and y with distance = x + y diff)
//   within the max distance constraint
BigNumber findScore(Map& map, Visited& visited, Point start, Point end)
{
    BigNumber score = 0;

    std::deque<Node> q;

    // Add start position the queue and visited nodes map
    Node startNode(score, start, 'S', "S");

    q.emplace_back(startNode);

    while (!q.empty())
    {
        Node currentNode = q.front();
        q.pop_front();

        visited.emplace(currentNode.point_.asString(), currentNode);

        if (currentNode.point_.x_ == end.x_ && currentNode.point_.y_ == end.y_)
        {
            return currentNode.distFromStart_;
        }

        for (auto& dir : Directions)
        {
            Node newNode(currentNode.distFromStart_ + 1, currentNode.point_ + dir.second, dir.first, currentNode.path_ + dir.first);

            // Have we been here before? 
            auto visitedIt = visited.find(newNode.point_.asString());
            if (visitedIt == visited.end())
            {
                // no
                if (map[newNode.point_.y_][newNode.point_.x_] != '#')
                {
                    q.emplace_back(newNode);
                }
            }
        }
    }

    // should never get here
    return score;
}


BigNumber findScoreBruteForce(Map& map, Visited& visited, Point start, Point end)
{
    BigNumber score = 0;

    std::deque<Node> q;

    // Add start position the queue and visited nodes map
    Node startNode(score, start, 'S', "S");

    q.emplace_back(startNode);

    while (!q.empty())
    {
        Node currentNode = q.front();
        q.pop_front();

        visited.emplace(currentNode.point_.asString(), currentNode);

        if (currentNode.point_.x_ == end.x_ && currentNode.point_.y_ == end.y_)
        {
            return currentNode.distFromStart_;
        }

        for (auto& dir : Directions)
        {
            Node newNode(currentNode.distFromStart_ + 1, currentNode.point_ + dir.second, dir.first, currentNode.path_ + dir.first);

            // Have we been here before? 
            auto visitedIt = visited.find(newNode.point_.asString());
            if (visitedIt == visited.end())
            {
                // no
                if (map[newNode.point_.y_][newNode.point_.x_] != '#')
                {
                    q.emplace_back(newNode);
                }
            }
        }
    }

    // should never get here
    return score;
}


void updateSeen(Seen& seen, Point start, Point end, Node node)
{
    Point point(start);
    seen.insert(point.asString());

    for (auto& dir : node.path_)
    {
        point = point + Directions[dir];
        seen.insert(point.asString());
    }

    if (point == end)
    {
        return;
    }
    else
    {
        // should never get here
    }
    return;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;

    readInputFile(inputFileName, map);

    Point start = findPoint(map, 'S');
    Point end = findPoint(map, 'E');

    BigNumber MIN_Y = 1;
    BigNumber MIN_X = 1;
    BigNumber MAX_Y = map.size() - 1 - 1;
    BigNumber MAX_X = map[0].size() - 1 - 1;

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
            Visited visited; // to keep track of the nodes already visited
            Visited cheats;
            
            BigNumber trackDistance = findScore(map, visited, start, end);

            BigNumber cheatLength = 2;
            for (auto& node : visited)
            {
                for (BigNumber y = -cheatLength; y <= cheatLength; y++)
                {
                    for (BigNumber x = -cheatLength; x <= cheatLength; x++)
                    {
                        BigNumber currentCheatLength = std::abs(x) + std::abs(y);
                        if (currentCheatLength <= cheatLength)
                        {
                            Point target(node.second.point_ + Point(x, y));
                            if (isWithinBounds(map, target) && map[target.y_][target.x_] != '#')
                            {
                                auto visitedIt = visited.find(target.asString());
                                if (visitedIt != visited.end())
                                {
                                    if (visitedIt->second.distFromStart_ - node.second.distFromStart_ - currentCheatLength >= 100)
                                    {
                                        sum++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;
            Visited visited; // to keep track of the nodes already visited
            Visited cheats;

            BigNumber trackDistance = findScore(map, visited, start, end);

            BigNumber cheatLength = 20;
            for (auto& node : visited)
            {
                for (BigNumber y = -cheatLength; y <= cheatLength; y++)
                {
                    for (BigNumber x = -cheatLength; x <= cheatLength; x++)
                    {
                        BigNumber currentCheatLength = std::abs(x) + std::abs(y);
                        if (currentCheatLength <= cheatLength)
                        {
                            Point target(node.second.point_ + Point(x, y));
                            if (isWithinBounds(map, target) && map[target.y_][target.x_] != '#')
                            {
                                auto visitedIt = visited.find(target.asString());
                                if (visitedIt != visited.end())
                                {
                                    if (visitedIt->second.distFromStart_ - node.second.distFromStart_ - currentCheatLength >= 100)
                                    {
                                        sum++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
