#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";


typedef std::vector<std::vector<char>> Map;
typedef std::vector<char> Path;


class Point
{
public:
    Point(int x, int y)
        : x_(x)
        , y_(y)
    {}

    Point(const Point& position)
        : x_(position.x_)
        , y_(position.y_)
    {}

    int x_;
    int y_;
};

void readInputFile(std::string fileName, Map& map)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            std::vector<char> charLine;
            if(!line.empty())
            {
                for(auto& ch: line)
                {
                    charLine.emplace_back(ch);
                }
                map.emplace_back(charLine);
            }
        }
        myFile.close();
    }
}


bool isAllowedMove(Map& map, Map& path, Point currentPoint, Point newPoint)
{
    char currentHeight = map[currentPoint.y_][currentPoint.x_];
    char newHeight = map[newPoint.y_][newPoint.x_];
    char pathHeight = path[newPoint.y_][newPoint.x_];

    if (newHeight == 'E')
    {
        newHeight = 'z';
        pathHeight = 'z';
    }

    if (currentHeight == 'S')
    {
        currentHeight = 'a';
    }

    // Make sure it is not a point already visited
    if (pathHeight >= 'a' && pathHeight <= 'z')
    {
        if (newHeight >= 'a' && newHeight <= 'z')
        {
            // going same level or down is always okay
            if (newHeight <= currentHeight)
            {
                return true;
            }

            if (newHeight == currentHeight + 1)
            {
                return true;
            }
        }
    }

    return false;
}

std::vector<int> findPaths(Map& map, Map path, Point currentPoint)
{
    std::vector<int> result;

    // check for the end goal of finding 'E', are we done?
    if (path[currentPoint.y_][currentPoint.x_] == 'E')
    {
        result.emplace_back(0);
        return result;
    }

    // mark current position used up
    path[currentPoint.y_][currentPoint.x_] = 'X';

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    if (currentPoint.x_ + 1 <= maxX)
    {
        // attempt to go East
        Point newPoint(currentPoint.x_ + 1, currentPoint.y_);

        if (isAllowedMove(map, path, currentPoint, newPoint))
        {
            std::vector<int> subResults = findPaths(map, path, newPoint);
            size_t numResults = subResults.size();
            if (numResults > 0)
            {
                for(int i = 0; i < numResults; i++)
                {
                    subResults[i] = subResults[i] + 1;
                }
                result.insert( result.end(), subResults.begin(), subResults.end() );
            }
        }
    }

    if (currentPoint.y_ + 1 <= maxY)
    {
        // attempt to go South
        Point newPoint(currentPoint.x_, currentPoint.y_ + 1);

        if (isAllowedMove(map, path, currentPoint, newPoint))
        {
            std::vector<int> subResults = findPaths(map, path, newPoint);
            size_t numResults = subResults.size();
            if (numResults > 0)
            {
                for(int i = 0; i < numResults; i++)
                {
                    subResults[i] = subResults[i] + 1;
                }
                result.insert( result.end(), subResults.begin(), subResults.end() );
            }
        }
    }

    if (currentPoint.x_ - 1 >= 0 )
    {
        // attempt to go West
        Point newPoint(currentPoint.x_ - 1, currentPoint.y_);

        if (isAllowedMove(map, path, currentPoint, newPoint))
        {
            std::vector<int> subResults = findPaths(map, path, newPoint);
            size_t numResults = subResults.size();
            if (numResults > 0)
            {
                for(int i = 0; i < numResults; i++)
                {
                    subResults[i] = subResults[i] + 1;
                }
                result.insert( result.end(), subResults.begin(), subResults.end() );
            }
        }
    }

    if (currentPoint.y_ - 1 >= 0)
    {
        // attempt to go North
        Point newPoint(currentPoint.x_, currentPoint.y_ - 1);

        if (isAllowedMove(map, path, currentPoint, newPoint))
        {
            std::vector<int> subResults = findPaths(map, path, newPoint);
            size_t numResults = subResults.size();
            if (numResults > 0)
            {
                for(int i = 0; i < numResults; i++)
                {
                    subResults[i] = subResults[i] + 1;
                }
                result.insert( result.end(), subResults.begin(), subResults.end() );
            }
        }
    }

    return result;
}

int findBestPath(Map& map, Point position)
{
    std::vector<int> paths = findPaths(map, map, position);

    // Eliminate paths that are incomplete (aborted with zero length)
    for(auto it = paths.begin(); it != paths.end(); it++)
    {
        if (*it == 0)
        {
            it = paths.erase(it);
        }
        else
        {
            it++;
        }
    }

    if (paths.empty())
    {
        return 0;
    }

    std::sort(paths.begin(), paths.end());

    return paths[0];
}

Point findEnd(Map map)
{
    Point target(0, 0);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if (map[y][x] == 'E')
            {
                target.x_= x;
                target.y_= y;
                return target;
            }
        }
    }
    return target;
}

Point findStart(Map map)
{
    Point target(0, 0);

    size_t maxY = map.size() - 1;
    size_t maxX = map[0].size() - 1;

    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if (map[y][x] == 'S')
            {
                target.x_= x;
                target.y_= y;
                return target;
            }
        }
    }
    return target;
}


// https://en.wikipedia.org/wiki/Breadth-first_search
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
            Point start(findStart(map));
            Point end(findEnd(map));
            int result = findBestPath(map, start);
            std::cout << "Best Path length: " << result;
            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {

            std::cout << std::endl;
        }
    }
}
