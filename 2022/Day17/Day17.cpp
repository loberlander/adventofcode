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

/*
Part two is solved empirically:

The number of new rows added after each rock eventually starts repeating.
In my case after careful observation of the height growth (heightGrowth.txt) the first 160 characters are unique but after that the 
pattern repeats after 1695 characters so recognizing that the repeated pattern can be simply replaced with a constant and recognizing
that 1000000000000 = 589970501 * 1695 + 805, it is enough to look at the height of the first 805 rocks and then every 805 + n * 1695
and interpolate from there to get the final formula for my sequence: 3825 + (589970501 - 1) * 2610 = 1539823008825
Is there a programmatic way of doing this? Certainly, but that would take more effort to actually find the pattern...

160 - 1695

1000000000000 mod 1695 = 805
589970501 * 1695 + 805


#                  height
160             ->  248
160 + 1 * 1695  -> 2858 (+2610)
160 + 2 * 1695  -> 5468 (+2610)
160 + 3 * 1695  -> 8078 (+2610)

#                  height
805             -> 1215
805 + 1 * 1695  -> 3825 (+2580)
805 + 2 * 1695  -> 6435 (+2610)
805 + 3 * 1695  -> 9045 (+2610)
...

3825 + (589970501 - 1) * 2610 = 1539823008825

*/

static const int CAVE_WIDTH = 7;

static const char AIR = '.';
static const char ROCK = '#';

typedef std::vector<char> Row;
typedef std::vector<Row> Cave;

struct Coords
{
    int x;
    int y;
};

typedef std::vector<std::vector<Coords>> RockShapes;

static const std::vector<Coords> RockA = { { 0,0 }, {1,0}, {2,0}, {3,0} };
static const std::vector<Coords> RockB = { { 1,0 }, {0,1}, {1,1}, {2,1}, {1,2} };
static const std::vector<Coords> RockC = { { 0,0 }, {1,0}, {2,0}, {2,1}, {2,2} };
static const std::vector<Coords> RockD = { { 0,0 }, {0,1}, {0,2}, {0,3} };
static const std::vector<Coords> RockE = { { 0,0 }, {1,0}, {0,1}, {1,1} };

static const RockShapes rockShapes = { RockA , RockB, RockC, RockD, RockE };

class Rocks
{
public:
    Rocks()
        : currentPosition_(0)
    {}

    void advanceRock()
    {
        if (rockShapes.empty() || currentPosition_ >= rockShapes.size() - 1)
        {
            currentPosition_ = 0;
        }
        else
        {
            currentPosition_++;
        }
    }

    int currentRock()
    {
        return currentPosition_;
    }

    void placeRock(Cave& cave, int x, int y, char image = ROCK)
    {
        for (auto& rockCoords : rockShapes[currentPosition_])
        {
            while (y + rockCoords.y >= cave.size())
            {
                Row row(CAVE_WIDTH, AIR); // Add an empty row of AIR
                cave.emplace_back(row);
            }
            if (cave[y + rockCoords.y][x + rockCoords.x] == AIR)
            {
                cave[y + rockCoords.y][x + rockCoords.x] = image;
            }
            else
            {
                // This should not happen!
                // Put a breakpoint here
                continue;
            }
        }
    }

    bool isMoveAllowed(Cave &cave, int x, int y)
    {
        for (auto& rockCoords : rockShapes[currentPosition_])
        {
            if (x + rockCoords.x < 0 || x + rockCoords.x > CAVE_WIDTH - 1)
            {
                // cannot go beyond the wall
                return false;
            }

            int depth = cave.size();
            if (y + rockCoords.y >= depth)
            {
                continue;
            }
            if (y + rockCoords.y < 0) // cannot go below the bottom of cave
            {
                return false;
            }
            if (cave[y + rockCoords.y][x + rockCoords.x] != AIR)
            {
                return false;
            }
        }

        return true;
    }

    int currentPosition_;
};

class Jets
{
public:
    Jets()
        : currentPosition_(0)
    {}

    void setJets(std::string& jets)
    {
        jets_ = jets;
    }

    int currentJet()
    {
        return jets_.at(currentPosition_) == '>' ? 1 : -1;
    }

    void advanceJets()
    {
        if (jets_.empty() || currentPosition_ >= jets_.size() - 1)
        {
            currentPosition_ = 0;
        }
        else
        {
            currentPosition_++;
        }
    }

    int currentPosition_;
    std::string jets_;
};


void readInputFile(std::string fileName, Jets& jets)
{
    std::string line;
    std::string lineLeft;
    std::string lineRight;
    std::ifstream myFile(fileName);

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
                        jets.setJets(line);
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

// find the first empty row from the top
int findTop(Cave& cave)
{
    Row emptyRow(CAVE_WIDTH, AIR);
    int depth = cave.size();
    int maxY = 0;

    if (depth == 0)
    {
        return 0;
    }

    int index = depth - 1;

    while (index >= 0)
    {
        if (cave[index] != emptyRow)
        {
            break;
        }

        index--;
    }

    maxY = index + 1;

    return maxY;
}

void displayCave(Cave& cave)
{
    return;

    for (auto it = cave.rbegin(); it != cave.rend(); it++)
    {
        for (auto& ch : *it)
        {
            std::cout << ch;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Rocks rocks;
    Jets jets;

    readInputFile(inputFileName, jets);

    // Initial conditions
    Cave cave;
    Row row(CAVE_WIDTH, AIR); //empty row of 7
    cave.emplace_back(row);
    cave.emplace_back(row);
    cave.emplace_back(row);

    int NUM_MOVES = 2022;

    if (jets.jets_.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            int prevHeight = findTop(cave);

            for (int i = 0; i < NUM_MOVES; i++)
            {
                int x = 2;
                int y = findTop(cave) + 3;

                bool done = false;

                while(!done)
                {
                    {
                        Cave cave2 = cave;
                        rocks.placeRock(cave2, x, y, '@');
                        displayCave(cave2);
                    }

                    int dx = jets.currentJet();
                    jets.advanceJets();
                    if (rocks.isMoveAllowed(cave, x + dx, y))
                    {
                        x = x + dx;
                    }

                    int dy = -1; // gravity
                    if (rocks.isMoveAllowed(cave, x, y + dy))
                    {
                        y = y + dy;
                    }
                    else
                    {
                        {
                            Cave cave2 = cave;
                            rocks.placeRock(cave2, x, y, '@');
                            displayCave(cave2);
                        }

                        rocks.placeRock(cave, x, y);
                        done = true;
                    }
                }
                int height = findTop(cave);

                std::cout << height - prevHeight;
                prevHeight = height;

                {
                    displayCave(cave);
                }
                rocks.advanceRock();
            }

            std::cout << std::endl <<"The top is at " << findTop(cave);

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;

            unsigned long long top = (unsigned long long)3825 + ((unsigned long long)589970501 - (unsigned long long)1) * (unsigned long long)2610;

            std::cout << std::endl << "The top is at " << top;

            std::cout << std::endl;
        }
    }
}
