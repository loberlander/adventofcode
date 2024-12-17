#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <algorithm>


enum Faces
{
    F0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6
};

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";
static const std::string delimiterComma = ",";
static const std::string delimiterColon = ": ";
static const std::string delimiterSpace = " ";

static const char WALL = '#';
static const char OPEN = '.';
static const char SPACE = ' ';

static const char openOrWall[] = {OPEN, WALL};

typedef std::vector<std::string> Map;

struct Dir
{
    int dx;
    int dy;
    int id;
    char turn;
};

static const std::vector<Dir> Turns = { {1, 0, 0, ' '}, {0, 1, 1, ' '}, {-1, 0, 2, ' '}, {0, -1, 3, ' '}};

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
    char d = ' '; // Initial element has no turn

    while (!line.empty())
    {
        // start with finding a direction
        size_t firstNumberPosition = line.find_first_not_of("0123456789");
        if (firstNumberPosition == 0)
        {
            d = line[0];
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

        auto dir = Turns[di];
        dir.turn = d;
        steps.emplace_back(dir, count);
    }
}

// Layout of the faces of the cube
//
//      1 2
//      3
//    4 5
//    6
//
void readInputFile(std::string fileName, Map& map, Steps& steps, int& dim)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);
    int dim = 0;
    bool areDimensionsKnown = false;

    if (myFile.is_open())
    {
        int state = 1;
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
                    case 1:
                    {
                        map.emplace_back(line);
                        break;
                    }
                    case 2:
                    {
                        parseSteps(line, steps);
                        break;
                    }
                }
            }
            else
            {
                state = state + 1;
            }
        }
        myFile.close();
    }
}

bool isPointOnMap(Map& map, int x, int y)
{
    int yMin = 0;
    int yMax = (int)map.size() - 1;

    if (y < yMin || yMax < y)
    {
        return false;
    }

    int xMin = (int)map[y].find_first_not_of(SPACE);
    int xMax = (int)map[y].find_last_of(openOrWall);

    if (x < xMin || xMax < x)
    {
        return false;
    }

    return true;
}

int processSteps2D(Map& map, Steps& steps)
{
    int x = 0;
    int y = 0;
    int dir = 0;

    // Starting position
    x = (int) map[0].find_first_of(OPEN);
    y = 0;

    for (auto& step : steps)
    {
        dir = step.dir_.id;
        for (int i = 0; i < step.count_; i++)
        {
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

    return 1000 * (y + 1) + 4 * (x + 1) + dir;
}

// Layout of the faces of the cube
//
//      1 2
//      3
//    4 5
//    6
//
Faces getFace(int x, int y, int dim)
{
    if (x >= dim && x < 2 * dim && y >=0 && y < dim)
    {
        return F1;
    }
    else if (x >= 2 * dim && x < 3 * dim && y >= 0 && y < dim)
    {
        return F2;
    }
    else if (x >= dim && x < 2 * dim && y >= dim && y < 2 * dim)
    {
        return F3;
    }
    else if (x >= 0 && x < dim && y >= 2 * dim && y < 3 * dim)
    {
        return F4;
    }
    else if (x >= dim && x < 2 * dim && y >= 2 * dim && y < 3 * dim)
    {
        return F5;
    }
    else if (x >= 0 && x < dim && y >= 3 * dim && y < 4 * dim)
    {
        return F6;
    }

    // Should never get here
    return F0;
}

int processSteps3D(Map& map, Steps& steps, int& dim)
{
    int x = 0;
    int y = 0;
    int dir = 0;

    // Starting position
    x = (int)map[0].find_first_of(OPEN);
    y = 0;
    Dir heading = Turns[dir];

    for (auto& step : steps)
    {
        switch (step.dir_.turn)
        {
            default:
            case ' ':
            {
                // keep current direction
                break;
            }
            case 'R':
            {
                dir++;
                break;
            }
            case 'L':
            {
                dir--;
                break;
            }
        }

        dir = dir % Turns.size();

        for (int i = 0; i < step.count_; i++)
        {
            int xNew = x + Turns[dir].dx;
            int yNew = y + Turns[dir].dy;
            int dirNew = dir;

            if (!isPointOnMap(map, xNew, yNew))
            {
                // No longer on map, must be crossing an edge
                // Find new edge
                // Find new x, y
                // Find new direction

                switch (getFace(x, y, dim))
                {
                    case F1:
                    {
                        switch (dir)
                        {
                            case 3:
                            {
                                // going up
                                dirNew = 0; //going right
                                int xTemp = xNew;
                                xNew = -yNew;
                                yNew = xTemp + 2 * dim;
                                break;
                            }
                            case 2:
                            {
                                // going left
                                dirNew = 0; //going right
                                yNew = 3 * dim - yNew;
                                xNew = dim - xNew;
                                break;
                            }
                            default:
                                // should never get here
                            break;
                        }

                        break;
                    }
                    case F2:
                    {
                        break;
                    }
                    case F3:
                    {
                        break;
                    }
                    case F4:
                    {
                        break;
                    }
                    case F5:
                    {
                        break;
                    }
                    case F6:
                    {
                        break;
                    }
                }

            }

            switch (map[yNew][xNew])
            {
                case OPEN:
                {
                    // We can move there
                    x = xNew;
                    y = yNew;
                    dir = dirNew;
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

    return 1000 * (y + 1) + 4 * (x + 1) + dir;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Map map;
    Steps steps;
    int dim;

    readInputFile(inputFileName, map, steps, dim);

    if (map.empty() || steps.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;
            std::cout << "The password is " << processSteps2D(map, steps);
            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;
            std::cout << "The password is " << processSteps3D(map, steps, dim);
            std::cout << std::endl;
        }
    }
}
