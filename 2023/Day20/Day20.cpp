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
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";

static const std::string delimiterBroadcaster = "broadcaster";
static const std::string delimiterFlipFlop = "%";
static const std::string delimiterConjunction = "&";
static const std::string delimiterConnection = " -> ";
static const std::string delimiterSeparator = ", ";

typedef long long BigNumber;


enum ModuleType
{
    Broadcaster = 'B',
    FlipFlop = '%',
    Conjunction = '&',
    Unknown ='?'
};

enum PulseType
{
    HighPulse = 1,
    LowPulse = 0,
};

class Module;

typedef std::map<std::string, Module> Modules;

class Module
{
public:
    Module()
        : moduleType_(Unknown)
        , stateFlipFlop_(false)
    {
        if (moduleType_ != Conjunction)
        {
            setInput(false);
        }
    }

    Module(ModuleType moduleType, std::string id, std::vector<std::string> outputConnections)
        : moduleType_(moduleType)
        , id_(id)
        , outputConnections_(outputConnections)
        , stateFlipFlop_(false)
    {
        if (moduleType_ != Conjunction)
        {
            setInput(false);
        }
    }

    void connect(std::string id, bool input = false)
    {
        auto it = inputConnections_.find(id);
        if (it != inputConnections_.end())
        {
            it->second = input;
        }
        else
        {
            inputConnections_.emplace(id, input);
        }
    }

    void setInput(bool input, std::string fromId = "default")
    {
        connect(fromId, input);
    }

    bool simulate(Modules& modules, bool& output)
    {
        bool outputGenerated = true;

        switch (moduleType_)
        {
            case Broadcaster:
            {
                output_ = inputConnections_["default"];
                break;
            }
            case FlipFlop:
            {
                if (!inputConnections_["default"])
                {
                    stateFlipFlop_ = !stateFlipFlop_;

                    if (stateFlipFlop_)
                    {
                        output_ = true;
                    }
                    else
                    {
                        output_ = false;
                    }
                }
                else
                {
                    outputGenerated = false;
                }

                break;
            }
            case Conjunction:
            {
                bool summary = true;
                for (auto& conjunction : inputConnections_)
                {
                    summary &= conjunction.second;
                }

                if (summary)
                {
                    //send low pulse
                    output_ = false;
                }
                else
                {
                    // send high pulse
                    output_ = true;
                }

                break;
            }
            default:
            {
                // should never get here
                break;
            }
        }

        output = output_;
        return outputGenerated;
    }


    ModuleType moduleType_;
    std::string id_;
    std::vector<std::string> outputConnections_;
    bool output_;
    bool stateFlipFlop_;
    std::map<std::string, bool> inputConnections_;
};

class Period
{
public:
    Period()
    {}

    Period(std::string id)
        : id_(id)
    {

    }

    std::string id_;
    std::vector<BigNumber> counts;
};

typedef std::map<std::string, Period> Periods;

void establishConnections(Modules& modules)
{
    for (auto& module : modules)
    {
        for (auto& connection : module.second.outputConnections_)
        {
            if (modules[connection].moduleType_ == Conjunction)
            {
                modules[connection].connect(module.second.id_);
            }
        }
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

BigNumber lcm(std::vector<BigNumber> values)
{
    // Requires C++17 that provides std::gcd()
    // calculate the LCM of values in a vector
    // lcm(a, b) = (a*b)/gcd(a,b)
    BigNumber lcmTmp = values.back();
    values.pop_back();

    for (auto value : values)
    {
        lcmTmp = lcmTmp * value / std::gcd(lcmTmp, value);
    }

    return lcmTmp;
}


void readInputFile(std::string fileName, Modules& modules)
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
                    // Read Workflows
                    if (!line.empty())
                    {
                        size_t tokenPosition = 0;

                        ModuleType moduleType;
                        std::string id;

                        tokenPosition = line.find(delimiterConnection, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            id = line.substr(0, tokenPosition);
                            line.erase(0, tokenPosition + delimiterConnection.length());

                            if (id == delimiterBroadcaster)
                            {
                                moduleType = Broadcaster;
                            }
                            else
                            {
                                if (id[0] == delimiterFlipFlop[0])
                                {
                                    moduleType = FlipFlop;
                                }
                                else if (id[0] == delimiterConjunction[0])
                                {
                                    moduleType = Conjunction;
                                }
                                id.erase(0, 1);
                            }

                        }

                        std::vector<std::string> connections = split(line, delimiterSeparator);

                        modules.emplace(id, Module(moduleType, id, connections));
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }

            }
        }
        myFile.close();
    }
}

void updateCounters(bool input, BigNumber& lowCount, BigNumber& highCount)
{
    if (input)
    {
        highCount++;
    }
    else
    {
        lowCount++;
    }
}

class QItem
{
public:
    QItem()
    {}

    QItem(std::string source, std::string destination, bool input)
        : source_(source)
        , destination_(destination)
        , input_(input)
    {}
    std::string source_;
    std::string destination_;
    bool input_;
};

void simulate1(Modules& modules, bool input, BigNumber& lowCount, BigNumber& highCount)
{
    std::deque<QItem> q;
    std::string sourceId = "switch";
    std::string destinationId = "broadcaster";

    updateCounters(input, lowCount, highCount);

    QItem qItem(sourceId, destinationId, input);
    q.emplace_back(qItem);

    while (!q.empty())
    {
        // Simulate the current module
        QItem qItemIncoming = q.front();
        q.pop_front();

        Module& currentModule = modules[qItemIncoming.destination_];
        bool output = false;

        if (currentModule.moduleType_ != Conjunction)
        {
            currentModule.setInput(qItemIncoming.input_);
        }
        else
        {
            currentModule.setInput(qItemIncoming.input_, qItemIncoming.source_);
        }

        bool outputGenerated = currentModule.simulate(modules, output);

        if (outputGenerated)
        {
            // Queue up connected modules with new input
            for (auto& outputConnection : currentModule.outputConnections_)
            {
                updateCounters(output, lowCount, highCount);
                QItem qItemOutgoing(currentModule.id_, outputConnection, output);
                q.emplace_back(qItemOutgoing);
            }
        }
    }
}

void simulate2(Modules& modules, bool input, std::set<std::string>& sources, std::set<std::string>& visited, Periods& periods, BigNumber count)
{
    std::deque<QItem> q;
    std::string sourceId = "switch";
    std::string destinationId = "broadcaster";

    QItem qItem(sourceId, destinationId, input);
    q.emplace_back(qItem);

    while (!q.empty())
    {
        // Simulate the current module
        QItem qItemIncoming = q.front();
        q.pop_front();

        Module& currentModule = modules[qItemIncoming.destination_];
        bool output = false;

        if (currentModule.moduleType_ != Conjunction)
        {
            currentModule.setInput(qItemIncoming.input_);
        }
        else
        {
            currentModule.setInput(qItemIncoming.input_, qItemIncoming.source_);
        }

        bool outputGenerated = currentModule.simulate(modules, output);

        if (outputGenerated)
        {
            // Keep track of the modules of interest
            if (output && sources.find(currentModule.id_) != sources.end())
            {
                if (visited.find(currentModule.id_) != visited.end())
                {
                    periods[currentModule.id_].counts.emplace_back(count);
                }
                else
                {
                    visited.emplace(currentModule.id_);

                    periods.emplace(currentModule.id_, currentModule.id_);
                    periods[currentModule.id_].counts.emplace_back(count);
                }
            }

            // Queue up connected modules with new input
            for (auto& outputConnection : currentModule.outputConnections_)
            {
                QItem qItemOutgoing(currentModule.id_, outputConnection, output);
                q.emplace_back(qItemOutgoing);
            }
        }
    }
}

std::set<std::string> findSources(Modules& modules, std::string target)
{
    std::set<std::string> result;

    for (auto& module : modules)
    {
        for (auto& connection : module.second.outputConnections_)
        {
            if (connection == target)
            {
                if (module.second.moduleType_ != Conjunction)
                {
                    // Is it a conjunction?
                    connection = connection;
                }
                result.emplace(module.second.id_);
            }
        }
    }

    return result;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Modules modules1;

    readInputFile(inputFileName, modules1);

    Modules modules2 = modules1;

    // Connect ALL inputs to conjunction modules
    // (i.e. make all conjunction boxes aware of their inputs
    establishConnections(modules1);
    establishConnections(modules2);

    if (modules1.empty() || modules2.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber lowCount = 0;
            BigNumber highCount = 0;
            bool input = false;

            for (int i = 0; i < 1000; i++)
            {
                simulate1(modules1, input, lowCount, highCount);
            }

            std::cout << "The total is: " << lowCount * highCount;
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            bool input = false;
            BigNumber count = 0;
            std::set<std::string> visited;
            Periods periods;

            bool done = false;

            std::string target = "rx";
            std::set<std::string> sources;
            while (sources.size() < 2 )
            {
                sources = findSources(modules2, target);
                target = *sources.begin(); // In case we need to keep looking
            }

            // Run it long enough to hit periods multiple times to inspect them
            for (BigNumber i = 1; i <= 15000; i++)
            {
                simulate2(modules2, input, sources, visited, periods, i);
            }

            // Pull out the first occurence of repeats
            std::vector<BigNumber> values;

            for (auto& period : periods)
            {
                values.emplace_back(period.second.counts[0]);
            }

            std::cout << "The total is: " << lcm(values);
        }

        std::cout << std::endl;

    }
}
