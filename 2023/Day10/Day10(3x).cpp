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

typedef std::vector<std::string> Map;

enum Pipe
{
    Vertical = '|',
    Horizontal = '-',
    NorthAndEast = 'L',
    NorthAndWest = 'J',
    SouthAndWest = '7',
    SouthAndEast = 'F',
    Nothing = '.',
    Start = 'S',
    Star = '*'
};

enum Direction
{
    North,
    South,
    West,
    East,
    Any
};


typedef unsigned long long BigNumber;


void stretch3x(Map& mapOriginal, Map& map3x)
{
    for (auto& line : mapOriginal)
    {
        std::string l1, l2, l3;
        for (auto& c : line)
        {
            switch (c)
            {
            case Vertical: // '|'
                l1 += " | ";
                l2 += " | ";
                l3 += " | ";
                break;
            case Horizontal: // '-'
                l1 += "   ";
                l2 += "---";
                l3 += "   ";
                break;
            case NorthAndEast: // 'L'
                l1 += " | ";
                l2 += " L-";
                l3 += "   ";
                break;
            case NorthAndWest: // 'J'
                l1 += " | ";
                l2 += "-J ";
                l3 += "   ";
                break;
            case SouthAndWest: // '7'
                l1 += "   ";
                l2 += "-7 ";
                l3 += " | ";
                break;
            case SouthAndEast: // 'F'
                l1 += "   ";
                l2 += " F-";
                l3 += " | ";
                break;
            case Nothing: // '.'
                l1 += "   ";
                l2 += " . ";
                l3 += "   ";
                break;
            case Start: // 'S'
                l1 += " | ";
                l2 += " S ";
                l3 += " | ";
                break;
            }
        }
        map3x.emplace_back(l1);
        map3x.emplace_back(l2);
        map3x.emplace_back(l3);
    }

}



// Split a line by delimiter
std::vector<std::string> split(std::string line, std::string delimiter)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());
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
                    // Read Instructions
                    if (!line.empty())
                    {
                        map.emplace_back(line);
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

bool isPossibleEast(Map& map, size_t x, size_t y)
{
    Pipe paths[] = { Horizontal, NorthAndWest, SouthAndWest };

    Pipe pipe = (Pipe) map[y][x];

    if (pipe == paths[0] || pipe == paths[1] || pipe == paths[2])
    {
        return true;
    }
    return false;
}

bool isPossibleWest(Map& map, size_t x, size_t y)
{
    Pipe paths[] = { Horizontal, NorthAndEast, SouthAndEast };

    Pipe pipe = (Pipe)map[y][x];

    if (pipe == paths[0] || pipe == paths[1] || pipe == paths[2])
    {
        return true;
    }
    return false;
}

bool isPossibleNorth(Map& map, size_t x, size_t y)
{
    Pipe paths[] = { Vertical, SouthAndEast, SouthAndWest };

    Pipe pipe = (Pipe)map[y][x];

    if (pipe == paths[0] || pipe == paths[1] || pipe == paths[2])
    {
        return true;
    }
    return false;
}

bool isPossibleSouth(Map& map, size_t x, size_t y)
{
    Pipe paths[] = { Vertical, NorthAndEast, NorthAndWest };

    Pipe pipe = (Pipe)map[y][x];

    if (pipe == paths[0] || pipe == paths[1] || pipe == paths[2])
    {
        return true;
    }
    return false;
}

Direction findPossibleDir(Map& map, size_t x, size_t y, Direction directionFrom)
{
    size_t maxX = map[0].size() - 1;
    size_t maxY = map.size() - 1;

    Pipe pipe = (Pipe)map[y][x];

    switch (pipe)
    {
        case Vertical:
            if (directionFrom == North) return South;
                else return North;
            break;
        case Horizontal:
            if (directionFrom == East) return West;
                else return East;
            break;
        case NorthAndEast:
            if (directionFrom == North) return East;
                else return North;
            break;
        case NorthAndWest:
            if (directionFrom == North) return West;
                else return North;
            break;
        case SouthAndWest:
            if (directionFrom == South) return West;
                else return South;
            break;
        case SouthAndEast:
            if (directionFrom == South) return East;
                else return South;
            break;
        case Start:
            if (y > 0 && isPossibleNorth(map, x, y - 1) && directionFrom != North)
            {
                return North;
            }
            if (y < maxY && isPossibleSouth(map, x, y + 1) && directionFrom != South)
            {
                return South;
            }
            if (x < maxX && isPossibleEast(map, x + 1, y) && directionFrom != East)
            {
                return East;
            }
            if (x > 0 && isPossibleWest(map, x - 1, y) && directionFrom != West)
            {
                return West;
            }
            break;
    }

    return Any;
}

Direction inverseDirection(Direction direction)
{
    switch (direction)
    {
        case North: return South;
        case South: return North;
        case East: return West;
        case West: return East;
        default:
        case Any: return Any;
    }
}

bool findStart(Map& map, size_t& x, size_t& y, Direction& direction1, Direction& direction2)
{
    size_t count = 0;
    for (auto& line : map)
    {
        size_t pos = line.find(Start);

        if (pos != std::string::npos)
        {
            x = pos;
            y = count;
            break;
        }
        count++;
    }

    direction1 = findPossibleDir(map, x, y, Any);
    direction2 = findPossibleDir(map, x, y, direction1);
    
    return true;
}

void move(Map& map, size_t& x, size_t& y, Direction dir)
{
    size_t maxX = map[0].size() - 1;
    size_t maxY = map.size() - 1;

    switch (dir)
    {
        case North:
            if (y > 0)
            {
                y--;
            }
            break;
        case South:
            if (y < maxY)
            {
                y++;
            }
            break;
        case East:
            if (x < maxX)
            {
                x++;
            }
            break;
        case West:
            if (x > 0)
            {
                x--;
            }
            break;
    }
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Map map;
    Map map3x;
    size_t xStart, yStart;
    bool isFound = false;

    readInputFile(inputFileName, map);

    stretch3x(map, map3x);

    Direction startDirection1, startDirection2;
    isFound = findStart(map3x, xStart, yStart, startDirection1, startDirection2);

    if (map3x.empty() || !isFound)
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            
            bool done = false;

            size_t maxX = map3x[0].size() - 1;
            size_t maxY = map3x.size() - 1;
            size_t x1 = xStart;
            size_t y1 = yStart;
            size_t x2 = xStart;
            size_t y2 = yStart;
            Direction dir1 = startDirection1;
            Direction dir2 = startDirection2;
            std::vector<Direction> path1;
            std::vector<Direction> path2;

            BigNumber count = 0;

            while (!done)
            {
                count++;
                path1.emplace_back(dir1);
                path2.emplace_back(dir2);
                move(map3x, x1, y1, dir1);
                move(map3x, x2, y2, dir2);

                if (x1 == x2 && y1 == y2)
                {
                    done = true;
                }
                else
                {
                    Direction oldDir1 = dir1;
                    dir1 = findPossibleDir(map3x, x1, y1, inverseDirection(oldDir1));

                    Direction oldDir2 = dir2;
                    dir2 = findPossibleDir(map3x, x2, y2, inverseDirection(oldDir2));
                }
                map3x[y1][x1] = Star;
                map3x[y2][x2] = Star;
            }

            for (auto& line : map3x)
            {
                
            }



            std::ofstream myFile("output2.txt");
            if (myFile.is_open())
            {
                for (auto& line : map3x)
                {
                    myFile << line << std::endl; // .write(line.c_str(), line.size());
                }
                myFile.close();
            }

            std::cout << "The total is: " << count;
        }

        std::cout << std::endl;
        {
            BigNumber total = 0;


            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;

    }
}
