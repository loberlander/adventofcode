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
typedef std::vector<BigNumber> BigNumbers;


typedef std::set<std::string> Connections;

typedef std::map<std::string, Connections> ConnectionsMap;


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


void readInputFile(std::string fileName, ConnectionsMap& connectionsMap)
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
                        auto conns = split(line, { "-" });

                        auto connIt0 = connectionsMap.find(conns[0]);
                        auto connIt1 = connectionsMap.find(conns[1]);
                        if (connIt0 != connectionsMap.end())
                        {
                            connIt0->second.emplace(conns[1]);
                        }
                        else
                        {
                            std::set<std::string> initialSet{ conns[1] };
                            connectionsMap.emplace(conns[0], initialSet);
                        }
                        if (connIt1 != connectionsMap.end())
                        {
                            connIt1->second.emplace(conns[0]);
                        }
                        else
                        {
                            std::set<std::string> initialSet{ conns[0] };
                            connectionsMap.emplace(conns[1], initialSet);
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


void findNeighbors(std::string node, std::set<std::string> neighbors, ConnectionsMap& connectionsMap, Seen& seen)
{
    // Avoid double counting...
    std::string key;
    for (auto& neighbor : neighbors)
    {
        if (!key.empty())
        {
            key += ",";
        }
        key += neighbor;
    }
    if (seen.find(key) != seen.end())
    {
        return;
    }

    seen.emplace(key);

    // For all connections of the node of interest...
    for (auto& connection : connectionsMap[node])
    {
        // If this connection is not part of the known neighbors of the node and is a subset...
        if (neighbors.find(connection) == neighbors.end() && 
            std::includes(connectionsMap[connection].begin(), connectionsMap[connection].end(), neighbors.begin(), neighbors.end()))
        {
            auto newNeighbors = neighbors;
            newNeighbors.emplace(connection);

            findNeighbors(connection, newNeighbors, connectionsMap, seen);
        }
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    ConnectionsMap connectionsMap;

    readInputFile(inputFileName, connectionsMap);

    if (connectionsMap.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        { 
            BigNumber sum = 0;

            // 3-way interconnection
            // Find all the connections that have two neighbors that are also interconnected
            // I.e. a given point's connection is also connected to the original point
            // C1------C2
            //   \    /
            //    \  /
            //     C3
            // Collect the reults in a seen set, so that each set is accounted for only once
            Seen seen;
            for (auto& c1 : connectionsMap)
            {
                for (auto& c2 : c1.second)
                {
                    for (auto& c3 : connectionsMap[c2])
                    {
                        if (c1.second.find(c3) != c1.second.end() && c2 != c1.first)
                        {
                            // Sort the names of the computers to identify the same set regardless of the order of the comupet names
                            std::vector<std::string> conns{c1.first, c2, c3};
                            std::sort(conns.begin(), conns.end());

                            seen.emplace(conns[0] + "," + conns[1] + "," + conns[2]);
                        }
                    }
                }
            }

            for (auto& conn : seen)
            {
                auto parts = split(conn, {","});
                if (parts[0][0] == 't' || parts[1][0] == 't' || parts[2][0] == 't')
                {
                    sum++;
                }

            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            // n-way interconnection (4-way)
            // Find all the connections that have n neighbors that are also interconnected
            // I.e. a given point's is also connected to the ALL other points in the set
            // C1---C2
            // | \ / |
            // |  \  |
            // | / \ |
            // C3---C4
            // Collect the reults in a seen set, so that each set is accounted for only once and then choose the biggest one
            //
            Seen seen;
            for (auto& c : connectionsMap)
            {
                findNeighbors(c.first, {c.first}, connectionsMap, seen);
            }

            std::string maxSeen;
            for (auto& conn : seen)
            {
                if (conn.length() > maxSeen.length())
                {
                    maxSeen = conn;
                }
            }

            std::cout << "Max is at: " << maxSeen;
        }

        std::cout << std::endl;
    }
}
