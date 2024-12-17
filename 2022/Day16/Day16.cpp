#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

//static const char inputFileName[] = "input.txt";
static const char inputFileName[] = "inputTest.txt";


static const std::string delimiterValve = "Valve ";
static const std::string delimiterRate = " has flow rate=";
static const std::string delimiterConnections = " tunnels lead to valves ";
static const std::string delimiterConnection = " tunnel leads to valve ";
static const std::string delimiterColon = ":";
static const std::string delimiterComma = ", ";
static const std::string delimiterSemiColon = ";";

#define MAX_TIME 30
static const std::string START_VALVE("AA");

class Valve
{
public:
    Valve()
        : rate_(0)
        , isOpen_(false)
    {}

    Valve(std::string& name, int rate, std::vector<std::string> connections)
        : name_(name)
        , rate_(rate)
        , connections_(connections)
        , isOpen_(false)
    {}

    std::string name_;
    int rate_;
    std::vector<std::string> connections_;
    bool isOpen_;
};

typedef std::map<std::string, Valve> Valves;


std::vector<std::string> parseConnections(std::string& line)
{
    std::vector<std::string> connections;

    while (!line.empty())
    {
        size_t tokenPositionComma = line.find(delimiterComma, 0);

        std::string name = line.substr(0, tokenPositionComma);
        connections.emplace_back(name);
        if (tokenPositionComma == std::string::npos)
        {
            line.erase(0);
        }
        else
        {
            line.erase(0, tokenPositionComma + delimiterComma.length());
        }
    }

    return connections;
}

void readInputFile(std::string fileName, Valves& valves)
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
                        std::string name;
                        int rate;
                        std::vector<std::string> connections;

                        size_t tokenPositionValve = line.find(delimiterValve, 0);
                        if (tokenPositionValve != std::string::npos)
                        {
                            line.erase(0, tokenPositionValve + delimiterValve.length());

                            size_t tokenPositionRate = line.find(delimiterRate, 0);
                            if (tokenPositionRate != std::string::npos)
                            {
                                name = line.substr(0, tokenPositionRate);
                                line.erase(0, tokenPositionRate + delimiterRate.length());

                                size_t tokenPositionSemiColon = line.find(delimiterSemiColon, 0);
                                if (tokenPositionSemiColon != std::string::npos)
                                {
                                    rate = std::stoi(line.substr(0, tokenPositionSemiColon));
                                    line.erase(0, tokenPositionSemiColon + delimiterSemiColon.length());

                                    size_t tokenPositionConnections = line.find(delimiterConnections, 0);
                                    if (tokenPositionConnections != std::string::npos)
                                    {
                                        line.erase(0, tokenPositionConnections + delimiterConnections.length());

                                        connections = parseConnections(line);
                                    }
                                    else
                                    {
                                        size_t tokenPositionConnection = line.find(delimiterConnection, 0);
                                        if (tokenPositionConnection != std::string::npos)
                                        {
                                            line.erase(0, tokenPositionConnection + delimiterConnection.length());

                                            connections = parseConnections(line);
                                        }
                                    }
                                }
                            }
                        }

                        valves.emplace(name, Valve(name, rate, connections));
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


int getFlowRate(Valves valves)
{
    int total = 0;

    std::cout << "Valves open: ";

    for (auto& valve : valves)
    {
        if (valve.second.isOpen_)
        {
            total += valve.second.rate_;

            std::cout << valve.second.name_ << " ";
        }
    }

    if (total == 0)
    {
        std::cout << " none." << std::endl;
    }
    else
    {
        std::cout << ", releasing " << total << " pressure." << std::endl;
    }

    return total;
}

std::string findMove(Valves& valves, std::string currentValve)
{
    int maxRate = 0;
    std::string newValve;
    Valve current = valves[currentValve];

    if (current.rate_ > 0 && !current.isOpen_)
    {
        current.isOpen_ = true;
        valves[currentValve] = current;
        newValve = currentValve;
        std::cout << "You open valve " << valves[currentValve].name_ << std::endl;
    }
    else
    {
        for (auto& valve : current.connections_)
        {
            if (valves[valve].rate_ >= maxRate && !valves[valve].isOpen_)
            {
                maxRate = valves[valve].rate_;
                newValve = valve;
            }
        }
        std::cout << "You move to valve " << newValve << std::endl;
    }

    return newValve;
}


int getMostPressure(Valves& valves)
{
    int total = 0;
    int time = 0;

    std::string currentValve(START_VALVE);

    while (time < MAX_TIME)
    {
        std::cout << std::endl << "== Minute " << time + 1 << " ==" << std::endl;
        total += getFlowRate(valves);

        //std::cout << "At valve " << currentValve.name_ << " rate: " << currentValve.rate_ << (currentValve.isOpen_ ? " open" : " closed") << " " << std::endl;

        std::string newValve = findMove(valves, currentValve);

        currentValve = newValve;

        time++;
    }

    return total;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Valves valves;

    readInputFile(inputFileName, valves);


    if (valves.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        {
            std::cout << "Part 1:" << std::endl;

            std::cout << "Most pressure: " << getMostPressure(valves);

            std::cout << std::endl;
        }

        {
            std::cout << "Part 2:" << std::endl;

            std::cout << std::endl;
        }
    }
}
