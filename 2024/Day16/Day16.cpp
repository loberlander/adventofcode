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
    Node(BigNumber score, Point point, char direction, std::string path)
        : score_(score)
        , point_(point)
        , direction_(direction)
        , path_(path)
    {
    }

    std::string asString()
    {
        return std::to_string(point_.x_) + ":" + std::to_string(point_.y_);
    }

    void nextDirection(bool isClockwise)
    {
        static const std::vector<char> DirectionOrder = { '>', 'v', '<', '^' };

        for (int i = 0; i < DirectionOrder.size(); i++)
        {
            if (DirectionOrder[i] == direction_)
            {
                direction_ = DirectionOrder[((isClockwise ? 1 : 3) +  i) % DirectionOrder.size()];
                return;
            }
        }
    }

    BigNumber score_;
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
        if (lhs.score_ > rhs.score_)
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


BigNumber findBestPath1(Map& map, Visited& visited, Point start, Point end)
{
    BigNumber score = 0;

    std::priority_queue<Node, std::deque<Node>, NodeComparator> q;

    // Add start position the queue and visited nodes map
    Node startNode(score, start, '>', "S");

    q.emplace(startNode);
    visited.emplace(start.asString() + startNode.direction_, startNode);

    while (!q.empty())
    {
        Node currentNode = q.top();
        q.pop();

        // Add the best score here once we know it is in fact the best score,
        // i.e. taking it out of the priority queue
        visited.emplace(currentNode.point_.asString() + currentNode.direction_, currentNode);

        if (currentNode.point_.x_ == end.x_ && currentNode.point_.y_ == end.y_)
        {
            return currentNode.score_;
        }

        // Available moves:
        // - Continue in the same direction
        // - Turn 90% clockwise
        // - Turn 90% counterclockwise

        {
            // Continue in the same direction if possible
            Node newNode(currentNode.score_ + 1, currentNode.point_ + Directions[currentNode.direction_], currentNode.direction_, currentNode.path_ + currentNode.direction_);

            // Have we been here before? 
            auto visitedIt = visited.find(newNode.point_.asString() + newNode.direction_);
            if (visitedIt == visited.end())
            {
                // no
                if (map[newNode.point_.y_][newNode.point_.x_] != '#')
                {
                    //visited.emplace(newNode.point_.asString() + newNode.direction_, newNode);
                    q.emplace(newNode);
                }
            }
        }

        {
            // Turn 90 degrees clockwise
            Node newNode(currentNode.score_ + 1000, currentNode.point_, currentNode.direction_, currentNode.path_);
            newNode.nextDirection(true);
            auto visitedIt = visited.find(newNode.point_.asString() + newNode.direction_);
            if (visitedIt == visited.end())
            {
                //visited.emplace(newNode.point_.asString() + newNode.direction_, newNode);
                q.emplace(newNode);
            }
        }

        {
            // Turn 90 degrees counterclockwise
            Node newNode(currentNode.score_ + 1000, currentNode.point_, currentNode.direction_, currentNode.path_);
            newNode.nextDirection(false);
            auto visitedIt = visited.find(newNode.point_.asString() + newNode.direction_);
            if (visitedIt == visited.end())
            {
                //visited.emplace(newNode.point_.asString() + newNode.direction_, newNode);
                q.emplace(newNode);
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


BigNumber findBestPath2(Map& map, Visited& visited, Point start, Point end)
{
    BigNumber bestScore = LLONG_MAX;
    Seen seen;

    std::priority_queue<Node, std::deque<Node>, NodeComparator> q;

    // Add start position the queue and visited nodes map
    Node startNode(0, start, '>', "S");

    q.emplace(startNode);
    visited.emplace(start.asString() + startNode.direction_, startNode);

    while (!q.empty())
    {
        Node currentNode = q.top();
        q.pop();

        // Add the best score here once we know it is in fact the best score,
        // i.e. taking it out of the priority queue
        visited.emplace(currentNode.point_.asString() + currentNode.direction_, currentNode);

        if (currentNode.point_.x_ == end.x_ && currentNode.point_.y_ == end.y_)
        {
            if (currentNode.score_ <= bestScore)
            {
                updateSeen(seen, start, end, currentNode);
                bestScore = currentNode.score_;
            }
            else
            {
                return seen.size();
            }
        }

        // Available moves:
        // - Continue in the same direction
        // - Turn 90% clockwise
        // - Turn 90% counterclockwise

        {
            // Continue in the same direction if possible
            Node newNode(currentNode.score_ + 1, currentNode.point_ + Directions[currentNode.direction_], currentNode.direction_, currentNode.path_ + currentNode.direction_);

            // Have we been here before? 
            auto visitedIt = visited.find(newNode.point_.asString() + newNode.direction_);
            if (visitedIt == visited.end())
            {
                // no
                if (map[newNode.point_.y_][newNode.point_.x_] != '#')
                {
                    //visited.emplace(newNode.point_.asString() + newNode.direction_, newNode);
                    q.emplace(newNode);
                }
            }
        }

        {
            // Turn 90 degrees clockwise
            Node newNode(currentNode.score_ + 1000, currentNode.point_, currentNode.direction_, currentNode.path_);
            newNode.nextDirection(true);
            auto visitedIt = visited.find(newNode.point_.asString() + newNode.direction_);
            if (visitedIt == visited.end())
            {
                //visited.emplace(newNode.point_.asString() + newNode.direction_, newNode);
                q.emplace(newNode);
            }
        }

        {
            // Turn 90 degrees counterclockwise
            Node newNode(currentNode.score_ + 1000, currentNode.point_, currentNode.direction_, currentNode.path_);
            newNode.nextDirection(false);
            auto visitedIt = visited.find(newNode.point_.asString() + newNode.direction_);
            if (visitedIt == visited.end())
            {
                //visited.emplace(newNode.point_.asString() + newNode.direction_, newNode);
                q.emplace(newNode);
            }
        }
    }

    // should never get here
    return bestScore;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;

    readInputFile(inputFileName, map);

    Point start = findPoint(map, 'S');
    Point end = findPoint(map, 'E');
    Direction dir = Directions['>'];

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

            sum = findBestPath1(map, visited, start, end);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            Visited visited; // to keep track of the nodes already visited
            sum = findBestPath2(map, visited, start, end);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
