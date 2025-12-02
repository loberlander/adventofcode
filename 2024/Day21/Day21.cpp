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

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "input_test.txt";
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

    Point operator-(const Point& other)
    {
        return Point(x_ - other.x_, y_ - other.y_);
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
    Node(Point point, BigNumber score, std::string path)
        : score_(score)
        , point_(point)
        , path_(path)
    {
    }

    std::string asString()
    {
        return std::to_string(point_.x_) + ":" + std::to_string(point_.y_);
    }

    BigNumber score_;
    Point point_;
    std::string path_;
};

/*
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
*/

typedef std::map<std::string, Node> Visited;
typedef std::set<std::string> Seen;



enum KeypadType
{
    Numeric = 0x00,
    Directional = 0x01
};

static Map NumericKeypad =
{
    {"789"},
    {"456"},
    {"123"},
    {" 0A"}
};


static Map DirectionalKeypad =
{
    {" ^A"},
    {"<v>"}
};

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

    return { -1, -1 };
}


bool isWithinBounds(Map& map, Point p)
{
    if (p.x_ < 0 || p.y_ < 0 || p.x_ >= (BigNumber)map[0].size() || p.y_ >= (BigNumber)map.size())
    {
        return false;
    }
    return true;
}


class Keypad
{
public:
    Keypad(std::vector<std::string> input, KeypadType keypadType)
        : input_(input)
        , keypadType_(keypadType)
    {
        switch (keypadType_)
        {
            case Numeric:
            {
                keypad_ = NumericKeypad;
                break;
            }
            case Directional:
            {
                keypad_ = DirectionalKeypad;
                break;
            }
        }
    }

    std::vector<std::string> output()
    {
        if (output_.empty())
        {
            for (auto& line : input_)
            {
                // Always start from 'A'
                Point currentPoint = findPoint(keypad_, 'A');

                for (auto& target : line)
                {
                    std::vector<std::string> output1;

                    Point targetPoint = findPoint(keypad_, target);

                    if (currentPoint == targetPoint)
                    {
                        // Already on the target, activte!
                        output1.emplace_back("A");
                    }
                    else
                    {
                        std::deque<Node> q;
                        // Place the starting point on the Q
                        q.emplace_back(Node(currentPoint, 0, ""));

                        BigNumber minScore = LLONG_MAX;

                        while (!q.empty())
                        {
                            Node currentNode = q.front();
                            q.pop_front();
                            if (keypad_[currentNode.point_.y_][currentNode.point_.x_] == target)
                            {
                                if (currentNode.score_ < minScore)
                                {
                                    minScore = currentNode.score_;
                                }

                                if (currentNode.score_ == minScore)
                                {
                                    output1.emplace_back(currentNode.path_);
                                }
                                continue;
                            }
                            if (keypad_[currentNode.point_.y_][currentNode.point_.x_] == ' ')
                            {
                                // Ignore the empty spot(s)
                                continue;
                            }
                            if (currentNode.score_ > minScore)
                            {
                                // Do not care about paths that are longer than the minimum
                                continue;
                            }

                            for (auto& dir : Directions)
                            {
                                Point newPoint = currentNode.point_ + dir.second;
                                if (isWithinBounds(keypad_, newPoint))
                                {
                                    Node newNode(newPoint, currentNode.score_ + 1, currentNode.path_ + dir.first);
                                    q.emplace_back(newNode);
                                }
                            }
                        }
                    }
                    // continue from here...
                    currentPoint = targetPoint;

                    if (!output_.empty())
                    {
                        size_t minSize = LLONG_MAX;

                        std::vector<std::string> newOutput;
                        for (auto& left : output_)
                        {
                            for (auto& right : output1)
                            {
                                std::string newLine = left + right + "A";
                                newOutput.emplace_back(newLine);
                                if (newLine.size() < minSize)
                                {
                                    minSize = newLine.size();
                                }
                            }
                        }
                        for (auto it = newOutput.begin(); it != newOutput.end(); it++)
                        {
                            if (it->size() > minSize)
                            {
                                it = newOutput.erase(it);
                            }
                        }

                        output_ = newOutput;
                    }
                    else
                    {
                        for (auto& right : output1)
                        {
                            output_.emplace_back(right + "A");
                        }
                    }
                }
            }
        }
        return output_;
    }





    std::vector<std::string> input_;
    std::vector<std::string> output_;
    KeypadType keypadType_;
    Map keypad_;
};





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


int numVal(std::string line)
{
    while (line[0] == '0')
    {
        line.erase(0, 1);
    }

    while (line[line.size() - 1] == 'A')
    {
        line.erase(line.size() - 1, 1);
    }

    return stoi(line);
}




int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;

    readInputFile(inputFileName, map);

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
            for (auto& line : map)
            {
                Keypad keypadD({ line }, KeypadType::Numeric);
                Keypad keypad1(keypadD.output(), KeypadType::Directional);
                Keypad keypad2(keypad1.output(), KeypadType::Directional);
//                Keypad keypad3(keypad2.output(), KeypadType::Directional);

                auto result = keypad2.output();

                for (auto& line : result)
                {
                    std::cout << line;
                    std::cout << std::endl;
                }

                std::cout << std::endl;

                sum += result.size() * numVal(line);


                // too high: 221520
                // too high: 220984
                std::cout << std::endl;
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
