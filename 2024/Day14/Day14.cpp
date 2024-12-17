#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";
static const int X_SIZE = 101;
static const int Y_SIZE = 103;

//static const char inputFileName[] = "input_test.txt";
//static const int X_SIZE = 11;
//static const int Y_SIZE = 7;

typedef long long BigNumber;

class Point
{
public:
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
typedef std::vector<Direction> Directions;

class Robot
{
public:
    Robot(Point pos, Direction dir)
        : pos_(pos)
        , dir_(dir)
    { }

    Point pos_;
    Direction dir_;
};


typedef std::vector<Robot> Robots;


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


void readInputFile(std::string fileName, Robots& robots)
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
                    // Read robots
                    if (!line.empty())
                    {
                        std::vector<std::string> RobotAString = split(line, {"p=", ",", " v=", ","});
                        robots.push_back({
                            {stoll(RobotAString[0]), stoll(RobotAString[1])},
                            {stoll(RobotAString[2]), stoll(RobotAString[3])}
                        });
                    }
                    else
                    {
                        // state++;
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}

int iterate(Robots& robots, BigNumber count)
{
    int q1 = 0;
    int q2 = 0;
    int q3 = 0;
    int q4 = 0;

    for (auto& robot : robots)
    {
        robot.pos_.x_ += ((robot.dir_.x_) * count);
        robot.pos_.x_ = (robot.pos_.x_ + X_SIZE * count) % X_SIZE;

        robot.pos_.y_ += ((robot.dir_.y_) * count);
        robot.pos_.y_ = (robot.pos_.y_ + Y_SIZE * count) % Y_SIZE;

        if (robot.pos_.x_ < X_SIZE / 2)
        {
            if (robot.pos_.y_ < Y_SIZE / 2)
            {
                q1++;
            }
            else if (robot.pos_.y_ > Y_SIZE / 2)
            {
                q2++;
            }
        }
        else if (robot.pos_.x_ > X_SIZE / 2)
        {
            if (robot.pos_.y_ < Y_SIZE / 2)
            {
                q3++;
            }
            else if (robot.pos_.y_ > Y_SIZE / 2)
            {
                q4++;
            }
        }
    }

    return (q1 * q2 * q3 * q4);
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Robots robots;

    readInputFile(inputFileName, robots);

    if (robots.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;

            auto robotsTest = robots;
            sum = iterate(robotsTest, 100);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber minSum = LLONG_MAX;
            BigNumber itCountAtMin = 0;

            // Assumption: image will be in a single quadrant with minimal number of robots in the other quadrants
            // therefore, the solution is when the quadrant (q1 * q2 * q3 * q4) score is the lowest 

            BigNumber MaxIt = X_SIZE * Y_SIZE;

            for(BigNumber i = 1; i < MaxIt; i++)
            {
                auto robotsTest = robots;
                BigNumber sumTest = iterate(robotsTest, i);
                if (sumTest != 0 && sumTest < minSum)
                {
                    minSum = sumTest;
                    itCountAtMin = i;
                    // 6393 too low
                }
            }

            std::cout << "Total: " << itCountAtMin;
        }

        std::cout << std::endl;
    }
}
