#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <algorithm>


//static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";
static const char inputFileName[] = "inputTest3.txt";
static const std::string delimiterComma = ",";
static const std::string delimiterColon = ": ";
static const std::string delimiterSpace = " ";

static const char WALL = '#';
static const char OPEN = '.';
static const char SPACE = ' ';

static const char openOrWall[] = {OPEN, WALL};
static const float spacing = 2.0;
static const float offset = spacing / 2;

class Point
{
public:
    Point(float xx, float yy, float zz)
        : x(xx)
        , y(yy)
        , z(zz)
    {}
    float x;
    float y;
    float z;
};


bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator < (const Point& lhs, const Point& rhs)
{
    std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y) + ":" + std::to_string(lhs.z));
    std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y) + ":" + std::to_string(rhs.z));

    return lhsString < rhsString;
}


struct cmpPoints
{
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y) + ":" + std::to_string(lhs.z));
        std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y) + ":" + std::to_string(rhs.z));

        return lhsString < rhsString;
    }
};


typedef std::map<Point, char, cmpPoints> Map;


struct Dir
{
    int dx;
    int dy;
    int id;
};

static const std::vector<Dir> Turns = { {1, 0, 0}, { 0, 1, 1}, { -1, 0, 2}, { 0, -1, 3} };

class Step
{
public:
    Step(Dir dir, int count)
        : dir_(dir)
        , count_(count)
    {}

    Dir dir_;
    int count_;
};

typedef std::vector<Step> Steps;

void parseSteps(std::string& line, Steps& steps)
{
    int di = 0; // direction index in Turns, initial element

    while (!line.empty())
    {
        // start with finding a direction
        size_t firstNumberPosition = line.find_first_not_of("0123456789");
        if (firstNumberPosition == 0)
        {
            char d = line[0];
            line.erase(0, 1);

            if (d == 'R')
            {
                di++;
            }
            else
            {
                di--;
            }
            di = di % Turns.size();
        }

        int count = 0;
        size_t firstNotNumberPosition = line.find_first_not_of("0123456789");
        count = std::stoi(line.substr(0, firstNotNumberPosition));
        line.erase(0, firstNotNumberPosition);

        steps.emplace_back(Turns[di], count);
    }
}

// Layout of the faces of the cube
//
//      1 2
//      3
//    4 5
//    6
//
void readInputFile(std::string fileName, Map& map, Steps& steps)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);
    bool areDimensionsKnown;
    int dim;
    int lineNumber;
    float x, y, z;

    areDimensionsKnown = false;
    dim = 0;
    lineNumber = 0;


    if (myFile.is_open())
    {
        int state = 0;
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                if (!areDimensionsKnown)
                {
                    dim = 0;
                    for (int i = 0; i < line.size(); i++)
                    {
                        if (line[i] != SPACE)
                        {
                            dim++;
                        }
                    }
                    dim = dim / 2;
                    areDimensionsKnown = true;
                }

                switch (state)
                {
                    case 0:
                    {
                        // Face #1
                        z = 0;
                        y = offset + (lineNumber % dim) * spacing;
                        for (int i = 0; i < dim; i++)
                        {
                            x = offset + i * spacing;
                            if (line[dim + i] == WALL)
                            {
                                map.emplace(Point(x, y, z), WALL);
                            }
                        }

                        // Face #2
                        x = dim * spacing;
                        y = offset + (lineNumber % dim) * spacing;
                        for (int i = 0; i < dim; i++)
                        {
                            z = offset + i * spacing;
                            if (line[dim + dim + i] == WALL)
                            {
                                map.emplace(Point(x, y, z), WALL);
                            }
                        }

                        break;
                    }
                    case 1:
                    {
                        // Face #3
                        y = dim * spacing;
                        z = offset + (lineNumber % dim) * spacing;
                        for (int i = 0; i < dim; i++)
                        {
                            x = offset + i * spacing;
                            if (line[dim + i] == WALL)
                            {
                                map.emplace(Point(x, y, z), WALL);
                            }
                        }

                        break;
                    }
                    case 2:
                    {
                        // Face #4
                        x = 0;
                        y = offset + spacing * (dim - 1 - lineNumber % dim);
                        for (int i = 0; i < dim; i++)
                        {
                            z = offset + i * spacing;
                            if (line[i] == WALL)
                            {
                                map.emplace(Point(x, y, z), WALL);
                            }
                        }

                        // Face #5
                        z = dim * spacing;;
                        y = offset + spacing * (dim - 1 - lineNumber % dim);
                        for (int i = 0; i < dim; i++)
                        {
                            x = offset + i * spacing;
                            if (line[dim + i] == WALL)
                            {
                                map.emplace(Point(x, y, z), WALL);
                            }
                        }

                        break;
                    }
                    case 3:
                    {
                        // Face 6
                        y = 0;
                        x = offset + (lineNumber % dim) * spacing;
                        for (int i = 0; i < dim; i++)
                        {
                            z = offset + i * spacing;
                            if (line[i] == WALL)
                            {
                                map.emplace(Point(x, y, z), WALL);
                            }
                        }
                        break;
                    }
                    case 4:
                    {
                        parseSteps(line, steps);
                        break;
                    }
                }

                lineNumber++;
                state = lineNumber / dim;
            }
            else
            {
                state = 4;
            }
        }
        myFile.close();
    }
}

bool isPointWall(Map& map, Point& point)
{
    if (map.find(point) != map.end())
    {
        return true;
    }

    return false;
}


int processSteps3D(Map& map, Steps& steps)
{

    int x = 0;
    int y = 0;
    int dir = 0;
/*
    // Starting position
    x = (int)map[0].find_first_of(OPEN);
    y = 0;

    for (auto& step : steps)
    {
        for (int i = 0; i < step.count_; i++)
        {
            dir = step.dir_.id;
            int xNew = x + step.dir_.dx;
            int yNew = y + step.dir_.dy;

            switch (dir)
            {
            case 1:
            {
                // Going down
                if (!isPointOnMap(map, xNew, yNew))
                {
                    for (int ySearch = 0; ySearch < (int)map.size() - 1; ySearch++)
                    {
                        if (isPointOnMap(map, xNew, ySearch))
                        {
                            yNew = ySearch;
                            break;
                        }
                    }
                }
                break;
            }
            case 3:
            {
                // Going up
                if (!isPointOnMap(map, xNew, yNew))
                {
                    for (int ySearch = (int)map.size() - 1; ySearch > 0; ySearch--)
                    {
                        if (isPointOnMap(map, xNew, ySearch))
                        {
                            yNew = ySearch;
                            break;
                        }
                    }
                }
                break;
            }
            case 0:
            {
                // going right
                if (!isPointOnMap(map, xNew, yNew))
                {
                    xNew = (int)map[yNew].find_first_not_of(SPACE);
                }
                break;
            }
            case 2:
            {
                // going left
                if (!isPointOnMap(map, xNew, yNew))
                {
                    //xNew = (int)map[yNew].find_last_not_of(SPACE);
                    xNew = (int)map[yNew].find_last_of(openOrWall);
                }
                break;
            }
            default:
            {
                // should never get here
                xNew = xNew;
                yNew = yNew;
                break;
            }
            }

            switch (map[yNew][xNew])
            {
            case OPEN:
            {
                // We can move there
                x = xNew;
                y = yNew;
                break;
            }
            case WALL:
            {
                // do nothing
                x = x;
                y = y;
                break;
            }
            case SPACE:
            {
                // should never happen
                x = x;
                y = y;
                break;
            }
            default:
            {
                break;
            }
            }
        }

    }
    */
    return 1000 * (y + 1) + 4 * (x + 1) + dir;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;
    Steps steps;

    readInputFile(inputFileName, map, steps);

    if (map.empty() || steps.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;
            std::cout << "The password is " << processSteps3D(map, steps);
            std::cout << std::endl;
        }
    }
}
