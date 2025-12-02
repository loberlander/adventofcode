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

typedef std::map<std::string, BigNumber> ValueMap;

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


void readInputFile(std::string fileName, BigNumbers& bigNumbers)
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
                        bigNumbers.emplace_back(stoll(line));
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


std::vector<int> iterate(BigNumber& secret, BigNumber count, ValueMap& valueMap)
{
    std::vector<int> result;
    std::set<std::string> seen;

    result.emplace_back(secret % 10);

    for (BigNumber i = 1; i < count + 1; i++)
    {
        secret ^= (secret << 6);
        secret %= 16777216;

        secret ^= secret >> 5;
        secret %= 16777216;

        secret ^= (secret << 11);
        secret %= 16777216;

        result.emplace_back(secret % 10);

        if (i > 3)
        {
            int value = result[i];
            std::string key
                = std::to_string(result[i - 3] - result[i - 4]) + ","
                + std::to_string(result[i - 2] - result[i - 3]) + ","
                + std::to_string(result[i - 1] - result[i - 2]) + ","
                + std::to_string(result[i - 0] - result[i - 1]);

            if (seen.find(key) == seen.end())
            {
                // first occurance in this set
                seen.emplace(key);

                if (valueMap.count(key) == 0)
                {
                    valueMap.emplace(key, value);
                }
                else
                {
                    valueMap[key] += value;
                }
            }
        }
    }
    return result;
}



int main()
{
    std::cout << "Parsing input file..." << std::endl;

    BigNumbers bigNumbers;
    ValueMap valueMap;

    readInputFile(inputFileName, bigNumbers);

    if (bigNumbers.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        { 
            BigNumber sum = 0;
            int i = 0;
            for (auto bigNumber : bigNumbers)
            {
                BigNumber secret = bigNumber;

                iterate(secret, 2000, valueMap);
                i++;

                sum += secret;
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            // After all the calculation in Part 1, all that is left is to find the maximum value

            for (auto& element : valueMap)
            {
                if (sum < valueMap[element.first])
                {
                    sum = valueMap[element.first];
                }
            }

            std::cout << "Maximum: " << sum;
        }

        std::cout << std::endl;
    }
}
