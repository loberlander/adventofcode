#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "inputTest.txt";
//static const char inputFileName[] = "inputTest2.txt";

static const std::string delimiterElf = "#";

struct Point
{
    int x;
    int y;
};

struct Elf
{
    Point position;
    Point possiblePosition;
    int id;
};

struct BoundingBox
{
    Point min;
    Point max;
};

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator < (const Point& lhs, const Point& rhs)
{
    std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y));
    std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y));

    return lhsString < rhsString;
}

struct cmpPoints
{
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        std::string lhsString(std::to_string(lhs.x) + ":" + std::to_string(lhs.y));
        std::string rhsString(std::to_string(rhs.x) + ":" + std::to_string(rhs.y));

        return lhsString < rhsString;
    }
};


struct Movement
{
    std::vector<Point> check;
    Point move;
};

typedef std::vector<Movement> Movements;

// Encode the locations to check and the move to make
static const Movements movements =
{
    { {{ -1, -1 }, { 0, -1 }, { 1, -1 }}, { 0, -1 } },
    { {{ -1,  1 }, { 0,  1 }, { 1,  1 }}, { 0,  1 } },
    { {{ -1, -1 }, { -1, 0 }, { -1, 1 }}, { -1, 0 } },
    { {{  1, -1 }, {  1, 0 }, { 1,  1 }}, { 1 , 0 } },
};

static const int NUM_MOVES = 10;


typedef std::map<Point, Elf, cmpPoints> Elves; // Store their x, y coordinates and their ID (sequential number)

typedef std::multimap<Point, Elf, cmpPoints> ElvesSteps;


void readInputFile(std::string fileName, Elves& elves)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

    int y = 0;
    int elfId = 0;

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
                        int x = 0;

                        while (!line.empty())
                        {
                            size_t tokenPositionElf = line.find(delimiterElf, 0);
                            if (tokenPositionElf != std::string::npos)
                            {
                                x += (int) tokenPositionElf;
                                line.erase(0, tokenPositionElf + delimiterElf.length());

                                Point point;
                                point.x = x;
                                point.y = y;
                                Elf elf;
                                elf.position = point;
                                elf.possiblePosition = point;
                                elf.id = elfId;
                                elves.emplace(point, elf);

                                x += (int) delimiterElf.length();
                                elfId++;
                            }
                            else
                            {
                                line.erase();
                            }
                        }
                        y++;
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


int emptyTiles(Elves& elves)
{
    int numElves = (int) elves.size();
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (auto& elf : elves)
    {
        if (elf.second.position.x > maxX) { maxX = elf.second.position.x; }
        if (elf.second.position.x < minX) { minX = elf.second.position.x; }
        if (elf.second.position.y > maxY) { maxY = elf.second.position.y; }
        if (elf.second.position.y < minY) { minY = elf.second.position.y; }
    }

    return (maxX - minX + 1) * (maxY - minY + 1) - numElves;
}

bool isMovable(Elf& elf, Elves& elves, Movement& movement)
{
    if (elves.find({ elf.position.x + 0, elf.position.y - 1 }) == elves.end() &&
        elves.find({ elf.position.x + 1, elf.position.y - 1 }) == elves.end() &&
        elves.find({ elf.position.x + 1, elf.position.y + 0 }) == elves.end() &&
        elves.find({ elf.position.x + 1, elf.position.y + 1 }) == elves.end() &&
        elves.find({ elf.position.x + 0, elf.position.y + 1 }) == elves.end() &&
        elves.find({ elf.position.x - 1, elf.position.y + 1 }) == elves.end() &&
        elves.find({ elf.position.x - 1, elf.position.y + 0 }) == elves.end() &&
        elves.find({ elf.position.x - 1, elf.position.y - 1 }) == elves.end())
    {
        return false;
    }

    if (elves.find({ elf.position.x + movement.check[0].x, elf.position.y + movement.check[0].y }) != elves.end())
    {
        return false;
    }

    if (elves.find({ elf.position.x + movement.check[1].x, elf.position.y + movement.check[1].y }) != elves.end())
    {
        return false;
    }

    if (elves.find({ elf.position.x + movement.check[2].x, elf.position.y + movement.check[2].y }) != elves.end())
    {
        return false;
    }

    return true;
}

// return point as move direction
Point findMove(Elf& elf, Elves& elves, int movementState)
{
    Point move;

    move.x = 0;
    move.y = 0;

    for (int i = 0; i < movements.size(); i++)
    {
        auto movement = movements[movementState];
        if (isMovable(elf, elves, movement))
        {
            move = movement.move;
            return move;
        }

        // Rotate to the next movement order
        movementState = (movementState + 1) % movements.size();
    }

    return move;
}

void print(Elves& elves)
{
    size_t numRows = 20;
    size_t numCols = 20;

    int offsetX = numCols / 2;
    int offsetY = numRows / 2;

    std::string line;
    std::vector<std::string> map;

    for (int x = 0; x < numCols; x++)
    {
        line += '.';
    }

    // fill map with spaces
    for (int y = 0; y < numRows; y++)
    {
        map.emplace_back(line);
    }

    for (auto& elf : elves)
    {
        map[offsetY + elf.second.position.y][offsetX + elf.second.position.x] = '#';
    }

    for (int y = 0; y < numRows; y++)
    {
        std::cout << map[y] << std::endl;
    }

    std::cout << std::endl;
}

void moveElves(Elves& elves, const Movements& movements, int roundCount)
{
    int movementStartState = 0;
    ElvesSteps elvesSteps;
    
    //print(elves);

    for (int i = 0; i < roundCount; i++)
    {
        int numMoves = 0;
        std::cout << "Round #" << i+1 << std::endl;
        Elves newElves;

        // Step 1: find possible moves and place possible locations of elves in a multimap
        elvesSteps.clear();
        for (auto& elf : elves)
        {
            Point move = findMove(elf.second, elves, movementStartState);

            Point possiblePoint;
            Elf newElf = elf.second;
            possiblePoint.x = newElf.position.x + move.x;
            possiblePoint.y = newElf.position.y + move.y;
            newElf.possiblePosition = possiblePoint; // Save it so that we can back track

            elvesSteps.emplace(possiblePoint, newElf);
        }

        // Step 2: remove any for collisions or rather create a new one without any duplicates
        newElves.clear();
        for (auto& elf : elvesSteps)
        {
            Elf newElf = elf.second;

            // Let's see if there are multiple elves for this one...
            auto range = elvesSteps.equal_range(elf.first);

            if (std::distance(range.first, range.second) == 1)
            {
                // There is exacly one, so use the new position
                if (newElf.position.x != newElf.possiblePosition.x || newElf.position.y != newElf.possiblePosition.y)
                {
                    numMoves++;
                }


                newElf.position = newElf.possiblePosition; // Promote possible position
                newElves.emplace(newElf.position, newElf);
            }
            else
            {
                // There is more than one, cannot move so use the old position
                newElves.emplace(newElf.position, newElf);
            }
        }

        // Swap the two maps
        elves.swap(newElves);

        std::cout << "Number of elves that moved in this round: " << numMoves << std::endl;

        if (numMoves == 0)
        {
            exit(0);
        }

        //print(elves);

        // Rotate to the next movement order
        movementStartState = (movementStartState + 1) % movements.size();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Elves elves1;
    Elves elves2;

    readInputFile(inputFileName, elves1);
    readInputFile(inputFileName, elves2);


    if (elves1.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            moveElves(elves1, movements, NUM_MOVES);

            std::cout << "Empty tiles: " << emptyTiles(elves1);
            std::cout << std::endl;
            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;
            moveElves(elves2, movements, 5000);
            std::cout << std::endl;
        }
    }
}
