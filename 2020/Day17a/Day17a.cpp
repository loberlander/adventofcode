#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";
static const char ACTIVE = '#';


struct Playground
{
    Playground() :
        minX(0),
        maxX(0),
        minY(0),
        maxY(0),
        minZ(0),
        maxZ(0)
    {}

    std::map<std::string, struct State> states;

    int minX;
    int maxX;
    int minY;
    int maxY;
    int minZ;
    int maxZ;
};

struct State
{
    State(int x, int y, int z, bool isActive) :
        m_x(x),
        m_y(y),
        m_z(z),
        m_isActive(isActive)
    {}

    int m_x;
    int m_y;
    int m_z;
    bool m_isActive;
};


// Format of input:  "..##.##."

std::string key(int x, int y, int z)
{
    return std::to_string(x) + ":" + std::to_string(y) + ":" + std::to_string(z);
}

void addActive(struct Playground& playground, int x, int y, int z)
{
    playground.states.emplace(key(x, y, z), State(x, y, z, true));

    // Maintain the range we have active elements in
    if (x < playground.minX) playground.minX = x;
    if (x > playground.maxX) playground.maxX = x;
    if (y < playground.minY) playground.minY = y;
    if (y > playground.maxY) playground.maxY = y;
    if (z < playground.minZ) playground.minZ = z;
    if (z > playground.maxZ) playground.maxZ = z;
}

void parseLine(std::string line, struct Playground& playground,  int& x, int& y, int& z)
{
    for (auto state : line)
    {
        if (state == ACTIVE)
        {
            addActive(playground, x, y, z);
        }
        ++x;
    }
}

struct Playground readInputFile(std::string fileName)
{
    struct Playground data;
    std::string line;
    std::ifstream myFile(fileName);
    int x = 0;
    int y = 0;
    int z = 0;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            parseLine(line, data, x, y, z);
            x = 0;
            ++y;
        }
        myFile.close();
    }

    return data;
}

int countActiveNeighbors(struct Playground& playground, struct State state)
{
    int activeNeighbors = 0;
    int x, y, z;

    for (int xDiff = -1; xDiff <= 1; xDiff++)
    {
        for (int yDiff = -1; yDiff <= 1; yDiff++)
        {
            for (int zDiff = -1; zDiff <= 1; zDiff++)
            {
                x = state.m_x + xDiff;
                y = state.m_y + yDiff;
                z = state.m_z + zDiff;
                if ((xDiff == 0) && (yDiff == 0) && (zDiff == 0))
                {
                    // Skip ourself...
                }
                else
                {
                    auto it = playground.states.find(key(x, y, z));
                    if (it != playground.states.end())
                    {
                        if (it->second.m_isActive)
                        {
                            ++activeNeighbors;
                        }
                    }
                }
            }
        }
    }

    return activeNeighbors;
}

size_t countTotalActive(struct Playground& playground)
{
    size_t total = playground.states.size();

    return total;
}

struct Playground doCycle(struct Playground& dataIn)
{
    struct Playground dataOut;

    for (int x = dataIn.minX-1; x <= dataIn.maxX+1; x++)
    {
        for (int y = dataIn.minY-1; y <= dataIn.maxY+1; y++)
        {
            for (int z = dataIn.minZ-1; z <= dataIn.maxZ+1; z++)
            {
                bool currentState = false; // Assume inactive

                auto it = dataIn.states.find(key(x, y, z));
                if (it != dataIn.states.end())
                {
                    currentState = true;
                }
                else
                {
                    // Not found, therefore it is currently inactive
                    bool currentState = false;
                }

                int activeNeighbors = countActiveNeighbors(dataIn, State(x, y, z, currentState));

                // Rule #1: Active remains active when 2 or 3 are active
                if (currentState)
                {
                    if ((activeNeighbors == 2) || (activeNeighbors == 3))
                    {
                        addActive(dataOut, x, y, z);
                    }
                }
                else
                {
                    // Rule #2: Inactive becomes active when exactly 3 are active
                    if (activeNeighbors == 3)
                    {
                        addActive(dataOut, x, y, z);
                    }
                }
            }
        }
    }

    return dataOut;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    struct Playground data;
    data = readInputFile(inputFileName);

    if (data.states.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        struct Playground dataIn = data;
        struct Playground dataOut;
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        for (int i = 0; i < 6; i++)
        {
            dataOut = doCycle(dataIn);
            dataIn = dataOut;
        }

        std::cout << "Total active: " << countTotalActive(dataOut) << std::endl;

        std::cout << "Part 2:" << std::endl;

        std::cout << std::endl;
    }
}