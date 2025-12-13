#include <iostream>
#include <iomanip>
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
#include <bitset>

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef long long BigNumber;
typedef std::vector<std::string> Map;

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

        Point(const Point& point)
            : x_(point.x_)
            , y_(point.y_)
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

        BigNumber x()
        {
            return x_;
        }

        BigNumber y()
        {
            return y_;
        }

        std::string asString()
        {
            return std::to_string(x_) + ":" + std::to_string(y_);
        }

    private:
        BigNumber x_;
        BigNumber y_;
};

typedef Point Direction;

std::vector<Direction> Directions =
{
    Direction(1, 0),
    Direction(1, 1),
    Direction(0, 1),
    Direction(-1, 1),
    Direction(-1, 0),
    Direction(-1, -1),
    Direction(0, -1),
    Direction(1, -1)
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


std::string trimLeft(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the first non-whitespace character
    size_t pos = str.find_first_not_of(pattern);

    // Extract the substring from the first non-whitespace to the end
    return (pos == std::string::npos) ? "" : str.substr(pos);
}


std::string trimRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the last non-whitespace character
    size_t pos = str.find_last_not_of(pattern);

    // Extract the substring from the beginning to the last non-whitespace 
    return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
}


std::string trimLeftRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    return trimLeft(trimRight(str, pattern), pattern);
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
            switch (state)
            {
                case 1:
                {
                    // Read ranges
                    if (!line.empty())
                    {
                        map.emplace_back(line);
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    break;
                }
            }
        }
        myFile.close();
    }
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
            BigNumber result = 0;

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
