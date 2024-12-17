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
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";

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
    {'<', Direction(-1, 0)},
    {'>', Direction(1, 0)},
    {'v', Direction(0, 1)},
    {'^', Direction(0, -1)}
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


void readInputFile(std::string fileName, Map& map, Moves& moves)
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
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    if (!line.empty())
                    {
                        moves += line;
                    }
                    else
                    {

                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}


Point findStart(Map& map)
{
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] == '@')
            {
                return Point(x, y);
            }
        }
    }

    return {-1, -1};
}


bool canPushBox(Map& map, Point point, Direction dir)
{
    Point next = point + dir;
    if (map[next.y_][next.x_] == '.')
    {
        return true;
    }
    else if (map[next.y_][next.x_] == '#')
    {
        return false;
    }
    else if (map[next.y_][next.x_] == 'O')
    {
        return canPushBox(map, next, dir);
    }
    else if (map[next.y_][next.x_] == '[' || map[next.y_][next.x_] == ']')
    {
        if (dir == Direction{ 1, 0 } || dir == Direction{ -1, 0 })
        {
            return canPushBox(map, next, dir);
        }
        else if (dir == Direction{ 0, 1 } || dir == Direction{ 0, -1 })
        {
            Point nextOther = next;
            if (map[next.y_][next.x_] == '[')
            {
                nextOther.x_++;
            }
            else
            {
                nextOther.x_--;
            }

            return canPushBox(map, next, dir) && canPushBox(map, nextOther, dir);
        }
    }

    // Should never get here
    return false;
}


void pushBox(Map& map, Point point, Direction dir)
{
    Point next = point + dir;
    if (map[next.y_][next.x_] == '.')
    {
        map[next.y_][next.x_] = map[point.y_][point.x_];
        map[point.y_][point.x_] = '.';
        return;
    }
    else if (map[next.y_][next.x_] == '#')
    {
        // should never get here
        return;
    }
    else if (map[next.y_][next.x_] == 'O')
    {
        pushBox(map, next, dir);
        map[next.y_][next.x_] = map[point.y_][point.x_];
        map[point.y_][point.x_] = '.';
        return;
    }
    else if (map[next.y_][next.x_] == '[' || map[next.y_][next.x_] == ']')
    {
        if (dir == Direction{ 1, 0 } || dir == Direction{ -1, 0 })
        {
            pushBox(map, next, dir);
            map[next.y_][next.x_] = map[point.y_][point.x_];
            map[point.y_][point.x_] = '.';
            return;
        }
        else if (dir == Direction{ 0, 1 } || dir == Direction{ 0, -1 })
        {
            Point nextOther = next;
            if (map[next.y_][next.x_] == '[')
            {
                nextOther.x_++;
            }
            else
            {
                nextOther.x_--;
            }

            pushBox(map, next, dir);
            pushBox(map, nextOther, dir);
            map[next.y_][next.x_] = map[point.y_][point.x_];
            map[point.y_][point.x_] = '.';
            return;
        }
    }

    // Should never get here
    return;
}


void iterate(Map& map, Moves moves, Point current)
{
    for (auto& move : moves)
    {
        Direction dir = Directions[move];
        Point next = current + dir;

        if (map[next.y_][next.x_] == '.')
        {
            // Empty space, let's move there
            map[current.y_][current.x_] = '.';
            map[next.y_][next.x_] = '@';
            current = next;
        }
        else if (map[next.y_][next.x_] == '#')
        {
            // wall, skip
            continue;
        }
        else if (map[next.y_][next.x_] == 'O')
        {
            // Box is present
            if (canPushBox(map, next, dir))
            {
                pushBox(map, next, dir);
                map[current.y_][current.x_] = '.';
                map[next.y_][next.x_] = '@';
                current = next;
            }
            else
            {
                // Non-pushable
                continue;
            }
        }
        else if (map[next.y_][next.x_] == '[' || map[next.y_][next.x_] == ']')
        {
            // Box is present
            if (dir == Direction{ 1, 0 } || dir == Direction{ -1, 0 })
            {
                if (canPushBox(map, next, dir))
                {
                    pushBox(map, next, dir);
                    map[current.y_][current.x_] = '.';
                    map[next.y_][next.x_] = '@';
                    current = next;
                }
            }
            else if (dir == Direction{ 0, 1 } || dir == Direction{ 0, -1 })
            {
                Point nextOther = next;
                if (map[next.y_][next.x_] == '[')
                {
                    nextOther.x_++;
                }
                else
                {
                    nextOther.x_--;
                }
                if (canPushBox(map, next, dir) && canPushBox(map, nextOther, dir))
                {
                    pushBox(map, next, dir);
                    pushBox(map, nextOther, dir);
                    map[current.y_][current.x_] = '.';
                    map[next.y_][next.x_] = '@';
                    current = next;
                }
            }
            else
            {
                // Non-pushable
                continue;
            }
        }
    }
}


BigNumber score(Map& map)
{
    BigNumber sum = 0;

    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            if (map[y][x] == 'O' || map[y][x] == '[')
            {
                sum += (100 * y + x);
            }
        }
    }


    return sum;
}

Map remap(Map& map)
{
    Map newMap;

    for (int y = 0; y < map.size(); y++)
    {
        std::string line;
        for (int x = 0; x < map[0].size(); x++)
        {
            switch (map[y][x])
            {
            case '#':
                line += "##";
                break;
            case 'O':
                line += "[]";
                break;
            case '.':
                line += "..";
                break;
            case '@':
                line += "@.";
                break;
            default:
                break;
            }
        }
        newMap.emplace_back(line);
    }

    return newMap;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;
    Moves moves;

    readInputFile(inputFileName, map, moves);

    Point start1 = findStart(map);

    Map newMap = remap(map);
    Point start2 = findStart(newMap);

    if (map.empty() || moves.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;

            iterate(map, moves, start1);
            sum = score(map);

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            iterate(newMap, moves, start2);
            sum = score(newMap);


            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;
    }
}
