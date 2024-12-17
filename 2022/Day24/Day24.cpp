#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";

//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiterWall = "#";
static const std::string delimiterSpace = ".";
static const std::string delimiterBlizzards = "<>^v";

class Point
{
public:
    Point(int xx, int yy)
        : x(xx)
        , y(yy)
    {}

    const std::string asString() const
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    int x;
    int y;
};

class Blizzard
{
public:
    Blizzard(char dir, int xx, int yy)
        : direction(dir)
        , position(xx, yy)
    {}

    const std::string asString() const
    {
        std::string result;
        result += direction;
        result += ":" + std::to_string(position.x) + ":" + std::to_string(position.y);
        return result;
    }

    char direction;
    Point position;

};


bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


bool operator==(const Blizzard& lhs, const Blizzard& rhs)
{
    return lhs.position == rhs.position && lhs.direction == rhs.direction;
}

/*
bool operator < (const Point& lhs, const Point& rhs)
{
    std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y));
    std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y));;

    return lhsString < rhsString;
}
*/


bool operator < (const Blizzard& lhs, const Blizzard& rhs)
{
    //std::string lhsString(lhs.direction + ":" + std::to_string(lhs.position.x) + ":" + std::to_string(lhs.position.y));
    //std::string rhsString(rhs.direction + ":" + std::to_string(rhs.position.x) + ":" + std::to_string(rhs.position.y));;

    return lhs.asString() < rhs.asString();
}

/*
struct cmpBlizzards
{
    bool operator()(const Blizzard& lhs, const Blizzard& rhs) const
    {
        std::string lhsString(lhs.direction + ":" + std::to_string(lhs.position.x) + ":" + std::to_string(lhs.position.y));
        std::string rhsString(rhs.direction + ":" + std::to_string(rhs.position.x) + ":" + std::to_string(rhs.position.y));;

        return lhsString < rhsString;
    }
};
*/

//typedef std::set<Blizzard, cmpBlizzards> Blizzards;
typedef std::set<Blizzard> Blizzards;


void readInputFile(std::string fileName, Blizzards& blizzards, int& maxX, int& maxY, int& minX, int& minY)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

    int x = 0;
    int y = 0;
    int yOffset = 0;
    char direction;

    maxX = std::numeric_limits<int>::min();
    maxY = std::numeric_limits<int>::min();
    minX = std::numeric_limits<int>::max();
    minY = std::numeric_limits<int>::max();

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                switch (state)
                {
                    case 1:
                    {
                        if (line.find_first_of(delimiterBlizzards) == std::string::npos)
                        {
                            // skip the first and last line
                            line.clear();
                            break;
                        }

                        int xOffset = 0;
                        for (int lx = 0; lx < line.size(); lx++)
                        {
                            if (line[lx] == delimiterWall[0])
                            {
                                // Skip the wall
                                xOffset++;
                                continue;
                            }
                            x = lx - xOffset;
                            if (line[lx] == delimiterBlizzards[0] ||
                                line[lx] == delimiterBlizzards[1] || 
                                line[lx] == delimiterBlizzards[2] || 
                                line[lx] == delimiterBlizzards[3] )
                            {
                                // Found a blizzard
                                direction = line[lx];

                                blizzards.emplace(direction, x, y);
                            }
                            // keep track of the min/max coordinates (size) of the field of interest
                            if (x > maxX) maxX = x;
                            if (x < minX) minX = x;
                        }
                        if (y > maxY) maxY = y;
                        if (y < minY) minY = y;

                        line.clear(); // done with this line
                        y++; // advance to the next line
                        break;
                    }
                }
            }
            else
            {
                state = 1;
            }
        }
        myFile.close();
    }
}

std::vector<Point> moves = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {0, 0} };
std::vector<Blizzard> blizzardDirections = { {'<', -1, 0}, {'>', 1, 0}, {'^', 0, -1}, {'v', 0, 1} };

class State
{
public:
    State(int tt, Point pos, int stg)
        : time(tt)
        , position(pos)
        , stage(stg)
    {}
    int time;
    Point position;
    int stage;
};

bool isPointInTargets(Point& point, std::vector<Point>& targets)
{
    for (auto& target : targets)
    {
        if (target == point)
        {
            return true;
        }
    }
    return false;
}

int findShortestPath(Blizzards& blizzards, int& maxX, int& maxY, int& minX, int& minY, Point& start, std::vector<Point>& targets)
{
    int sizeX = maxX - minX + 1;
    int sizeY = maxY - minY + 1;
    int lcm = std::lcm(sizeX, sizeY);

    std::set<std::string> seen;
    std::deque<State> q; // To contain Breadth-first search algorithm
    q.emplace_back(State(0, start, 0)); // initial starting state: time zero, start point

    while (!q.empty())
    {
        auto currentState = q.front();
        q.pop_front();

        //currentState.time++; // point to the next time

        // attempt each possible move
        for (auto& move : moves)
        {
            // point to the next time
            State newState(currentState.time + 1, Point(currentState.position.x + move.x, currentState.position.y + move.y), currentState.stage);

            // Reached a target?
            if (newState.position == targets[newState.stage])
            {
                // Reached the final target?
                if (newState.stage == targets.size() - 1)
                {
                    return newState.time;
                }
                newState.stage++;
            }

            // No point in going beyond the wall (except for the start and end (i.e. targets), are we at the wall (edge)?
            if ((newState.position.x < 0 || newState.position.x > maxX || newState.position.y < 0 || newState.position.y > maxY)
                && !(newState.position == start) && !isPointInTargets(newState.position, targets))
            {
                continue;
            }

            // Check for collisions with any blizzards, where blizzards exist
            bool inABlizzard = false;
            if (!isPointInTargets(newState.position, targets))
            {
                for (auto& b : blizzardDirections)
                {
                    // C++ modulo division of a negative number will result in a negative number!
                    // I.e. not in the range of 0..(divisor-1) but -(divisor-1)..0
                    // So either we need to check the result and if it is negative add the modulo OR
                    // always add the modulo and then modulo divide again...
                    Blizzard testBlizzard(b.direction,
                        (sizeX + (newState.position.x - b.position.x * newState.time) % sizeX) % sizeX,
                        (sizeY + (newState.position.y - b.position.y * newState.time) % sizeY) % sizeY);
                    if (blizzards.find(testBlizzard) != blizzards.end())
                    {
                        // Cannot go there, there is a blizzard!
                        inABlizzard = true;
                        break;
                    }
                }
            }
            if (!inABlizzard)
            {
                // Optimization: do not evaluate the same state again
                std::string key;
                key = std::to_string(newState.stage) + ":" + std::to_string(newState.position.x) + ":" + std::to_string(newState.position.y) + ":" + std::to_string(newState.time % lcm);

                if (seen.find(key) != seen.end())
                {
                    continue;
                }
                //std::cout << "Move @" << newState.time << ": " << currentState.position.asString() << "->" << newState.position.asString() << std::endl;

                seen.emplace(key);
                q.emplace_back(newState);
            }
        }
    }

    return 0;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    // Keep track of the field boundaries
    int maxX = 0;
    int maxY = 0;
    int minX = 0;
    int minY = 0;

    Blizzards blizzards;

    readInputFile(inputFileName, blizzards, maxX, maxY, minX, minY);


    if (blizzards.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            // The main idea is that instead of moving ALL the blizzards around for each turn, move the current location (state)
            // backwards and see if a blizzard might be present at that location and at that time. So we will be looking for only those 
            // blizzards that have an x or y coordinate that matches the current state

            // Optimization: the field position will repeat every so often, worst case at the maxX * maxY step, best case for every
            // least common multiple step. So at those states there is no need to further grow as those states should be covered already!

            Point start(0, -1);
            Point end(maxX, maxY + 1);
            std::vector<Point> targets;
            targets.emplace_back(end);
            
            int result = findShortestPath(blizzards, maxX, maxY, minX, minY, start, targets);

            std::cout << "Shortest time: " << result << std::endl;

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;

            Point start(0, -1);
            Point end(maxX, maxY + 1);
            std::vector<Point> targets;
            targets.emplace_back(end);
            targets.emplace_back(start);
            targets.emplace_back(end);

            int result = findShortestPath(blizzards, maxX, maxY, minX, minY, start, targets);

            std::cout << "Shortest time: " << result << std::endl;

            std::cout << std::endl;
        }
    }
}
