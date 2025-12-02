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


//typedef std::map<std::string, Node> Visited;
typedef std::map<std::string, BigNumber> Visited;
typedef std::set<std::string> Seen;

typedef std::set<std::string> Obstacles;

typedef std::string Towel;
typedef std::set<Towel> Towels;

typedef std::string Pattern;
typedef std::vector<Pattern> Patterns;


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


void readInputFile(std::string fileName, Towels& towels, Patterns& patterns)
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
                    // Read towels
                    if (!line.empty())
                    {
                        auto towelString = split(line, { ", " });

                        for (auto& towel : towelString)
                        {
                            towels.insert(towel);
                        }
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    // Read patterns
                    if (!line.empty())
                    {
                        patterns.emplace_back(line);
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}


BigNumber designPossibilities(Towels& towels, Pattern pattern, Visited& visited)
{
    if (visited.find(pattern) != visited.end())
    {
        return visited[pattern];
    }

    BigNumber possibilities = 0;
    auto patternLength = pattern.size();

    if (patternLength == 0)
    {
        possibilities = 1;
    }
    else
    {
        for (auto pos = 1; pos < patternLength + 1; pos++)
        {
            auto patternLeft = pattern.substr(0, pos);
            auto patternRight = pattern.substr(pos);

            if (towels.find(patternLeft) != towels.end())
            {
                BigNumber designPossibilitiesRight = designPossibilities(towels, patternRight, visited);
                possibilities += designPossibilitiesRight;
            }
        }

    }

    visited.emplace(pattern, possibilities);
    return possibilities;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Towels towels;
    Patterns patterns;

    readInputFile(inputFileName, towels, patterns);

    if (towels.empty() || patterns.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;

            Visited visited;

            for (auto pattern : patterns)
            {
                if (designPossibilities(towels, pattern, visited) > 0)
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

            Visited visited;

            for (auto pattern : patterns)
            {
                sum += designPossibilities(towels, pattern, visited);
            }
            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
