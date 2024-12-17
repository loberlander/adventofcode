#include <iostream>
#include <fstream>

#include <string>
#include <vector>

static const char inputFileName[] = "input.txt";
static const std::string delimiter1 = "-";
static const std::string delimiter2 = " ";
static const std::string delimiter3 = ": ";

enum Actions
{
    North = 'N',
    South = 'S',
    East = 'E',
    West = 'W',
    Left = 'L',
    Right = 'R',
    Forward = 'F'
};

// Format of input:  "14-16 z: zzzzzzzzzrzzblzw"
struct Instruction
{
    Actions action;
    int value;
};

struct Point
{
    int east;
    int north;
};

struct Location
{
    Actions heading;
    Point ship;
    Point waypoint;
};

struct Instruction parseLine(std::string line)
{
    Instruction instruction;

    instruction.action = (Actions) line[0];
    line.erase(0, 1);
    instruction.value = std::stoi(line.substr(0, line.size()));

    return instruction;
}

std::vector<Instruction> readInputFile(std::string fileName)
{
    std::vector<Instruction> data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            data.push_back(parseLine(line));
        }
        myFile.close();
    }

    return data;
}

void navigate(Instruction instruction, Location& location)
{
    int inverseNorth = 1;
    int inverseEast = 1;
    int inverseRight = 1;
    int degreeRight = 0;
    static const std::vector<Actions> rotArray = { North, East, South, West };

    switch (instruction.action)
    {
    case South:
        inverseNorth = -1;
    case North:
        location.ship.north += inverseNorth * instruction.value;
        break;

    case West:
        inverseEast = -1;
    case East:
        location.ship.east += inverseEast * instruction.value;
        break;

    case Left:
        inverseRight = -1;
    case Right:
        degreeRight = (inverseRight * instruction.value) % 360;
        if (degreeRight < 0)
        {
            degreeRight += 360;
        }

        for (int i = 0; i < rotArray.size(); i++)
        {
            if (location.heading == rotArray[i])
            {
                location.heading = rotArray[(i + degreeRight / 90) % rotArray.size()];
                break;
            }
        }
        break;

    case Forward:
        switch (location.heading)
        {
        case South:
            inverseNorth = -1;
        case North:
            location.ship.north += inverseNorth * instruction.value;
            break;

        case West:
            inverseEast = -1;
        case East:
            location.ship.east += inverseEast * instruction.value;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void navigateWaypoint(Instruction instruction, Location& location)
{
    int inverseNorth = 1;
    int inverseEast = 1;
    int inverseRight = 1;
    int degreeRight = 0;
    static const std::vector<Actions> rotArray = { North, East, South, West };

    switch (instruction.action)
    {
    case South:
        inverseNorth = -1;
    case North:
        location.waypoint.north += inverseNorth * instruction.value;
        break;

    case West:
        inverseEast = -1;
    case East:
        location.waypoint.east += inverseEast * instruction.value;
        break;

    case Left:
        inverseRight = -1;
    case Right:
        degreeRight = (inverseRight * instruction.value) % 360;
        if (degreeRight < 0)
        {
            degreeRight += 360;
        }

        for (int i = 0; i < degreeRight / 90; i++)
        {
            Point waypoint = location.waypoint;
            location.waypoint.east = waypoint.north;
            location.waypoint.north = (-1) * waypoint.east;
        }
        break;

    case Forward:
        location.ship.east += instruction.value * location.waypoint.east;
        location.ship.north += instruction.value * location.waypoint.north;
        break;

    default:
        break;
    }
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::vector<Instruction> data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;
        Location location1 = { East, {0, 0}, {0, 0} };
        for(auto instruction: data)
        {
            navigate(instruction, location1);
        }

        std::cout << "Ship's Manhattan distance is " << std::to_string((abs(location1.ship.east) + abs(location1.ship.north))) << std::endl;

        std::cout << "Part 2:" << std::endl;
        Location location2 = { East, {0, 0}, {10, 1} };
        for (auto instruction : data)
        {
            navigateWaypoint(instruction, location2);
        }

        std::cout << "Ship's Manhattan distance is " << std::to_string((abs(location2.ship.east) + abs(location2.ship.north))) << std::endl;

    }
}
