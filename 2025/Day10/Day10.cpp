#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <numeric>
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef long long BigNumber;

typedef std::vector<bool> Lights;

typedef std::vector<BigNumber> Button;
typedef std::vector<Button> Buttons;

typedef std::vector<BigNumber> Joltages;

class Machine
{
    public:
        Machine(Lights lights, Buttons buttons, Joltages joltages)
            : lights_(std::move(lights))
            , buttons_(std::move(buttons))
            , joltages_(std::move(joltages))
        { }

        const Lights lights() const { return lights_; }
        const Buttons buttons() const { return buttons_; }
        const Joltages joltages() const { return joltages_; }

        const BigNumber solveP1();
        const BigNumber solveP2();


    private:
        Lights lights_;
        Buttons buttons_;
        Joltages joltages_;
};


typedef std::vector<Machine> Machines;

typedef std::set<std::string> Visited;

// Split a line by delimiter
std::vector<std::string> split(std::string line, std::vector<std::string> delimiters)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;
    if (delimiters.empty())
    {
        return result;
    }

    int delimIndex = 0;

    while (!line.empty())
    {
        std::string delimiter = delimiters[delimIndex];

        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());

                delimIndex = delimIndex < delimiters.size() - 1 ? delimIndex + 1 : delimIndex;
                delimiter = delimiters[delimIndex];
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


std::string trimLeft(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the first non-whitespace character
    size_t pos = str.find_first_not_of(pattern);

    // Extract the substring from the first non-whitespace to the end
    return (pos == std::string::npos) ? "" : str.substr(pos);
}


std::string trimRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    // Find the last non-whitespace character
    size_t pos = str.find_last_not_of(pattern);

    // Extract the substring from the beginning to the last non-whitespace 
    return (pos == std::string::npos) ? "" : str.substr(0, pos + 1);
}


std::string trimLeftRight(const std::string& str, const std::string pattern = " \t\n\r\f\v")
{
    return trimLeft(trimRight(str, pattern), pattern);
}


void readInputFile(std::string fileName, Machines& machines)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch (state)
            {
                case 1:
                {
                    // Read ranges
                    if (!line.empty())
                    {
                        Lights lights;
                        Buttons buttons;
                        Joltages joltages;

                        // Each line represents a machine
                        auto elements1 = split(line, { "[", "]" });
                        if (elements1.size() == 2)
                        {
                            // Decode the lights
                            for (auto& ch : elements1[0])
                            {
                                lights.emplace_back(ch == '#');
                            }

                            // Decode the rest
                            auto elements2 = split(elements1[1], { "{", "}" });
                            if (elements2.size() == 2)
                            {
                                // Decode the buttons
                                auto elements3 = split(elements2[0], { " " });
                                if (elements3.size() > 0)
                                {
                                    for (auto& element : elements3)
                                    {
                                        std::vector<BigNumber> button;
                                        auto elements5 = split(trimRight(trimLeft(element, "("), ")"), { "," });
                                        for (auto& element : elements5)
                                        {
                                            button.emplace_back(std::stoll(element));
                                        }
                                        buttons.emplace_back(button);
                                    }
                                }

                                // Decode the joltages
                                auto elements4 = split(elements2[1], { "," });
                                for (auto& element : elements4)
                                {
                                    joltages.emplace_back(std::stoll(element));
                                }
                            }
                        }
                        machines.emplace_back(lights, buttons, joltages);
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    break;
                }
            }
        }
        myFile.close();
    }
}


class NodeP1
{
    public:
        NodeP1(BigNumber score, Lights lights, std::string path)
            : score_(score)
            , lights_(lights)
            , path_(path)
        {
        }

        Lights lights() { return lights_; }

        BigNumber score() { return score_; }

        std::string path() { return path_; }

        std::string asString()
        {
            // Provide a unique representation of the current state represented by lights_
            std::string state;
            for (auto light : lights_)
            {
                if (!state.empty())
                {
                    state += ":";
                }
                state += std::to_string(light);
            }
            return state;
        }

    private:
        BigNumber score_;
        Lights lights_;
        std::string path_;
};


const BigNumber Machine::solveP1()
{
    int score = 0;

    Lights lightsState = lights();

    // Set all initial light values to off (false)
    for (auto it = lightsState.begin(); it < lightsState.end(); it++)
    {
        *it = false;
    }

    NodeP1 node( 0, lightsState, "");

    std::deque<NodeP1> q;
    Visited visited;

    // Place the starting point on the Q
    q.emplace_back(node);

    while (!q.empty())
    {
        auto current = q.front();
        q.pop_front();

        std::string key = current.asString();
        auto seen = visited.find(key);

        if (seen != visited.end())
        {
            // if it has been seen before it is certainly not the shortest solution
            continue;
        }

        visited.emplace(key);

        if (current.lights() == lights())
        {
            // Solved!
            return current.score();
        }

      
        int buttonId = 0;
        // Iterate through all the buttons and apply what they do to the lights
        for (auto button : buttons())
        {
            auto lights = current.lights();
            for (auto& lightId : button)
            {
                // toggle the lights that are controlled by the button
                lights[lightId] = !lights[lightId];
            }

            // Add new node
            NodeP1 node(current.score() + 1, lights, current.path() +":" + std::to_string(buttonId));
            q.emplace_back(node);

            ++buttonId;
        }
    }


    return 0;
}


class NodeP2
{
    public:
        NodeP2(BigNumber score, Joltages joltages, std::string path)
            : score_(score)
            , joltages_(joltages)
            , path_(path)
        {
        }

        Joltages joltages() { return joltages_; }

        BigNumber score() { return score_; }

        std::string path() { return path_; }

        std::string asString()
        {
            // Provide a unique representation of the current state represented by joltages_
            std::string state;
            for (auto& joltage : joltages_)
            {
                if (!state.empty())
                {
                    state += ":";
                }
                state += std::to_string(joltage);
            }
            return state;
        }

    private:
        BigNumber score_;
        Joltages joltages_;
        std::string path_;
};


const BigNumber Machine::solveP2()
{
    int score = 0;

    Joltages joltagesState = joltages();

    // Set all initial light values to off (false)
    for (auto it = joltagesState.begin(); it < joltagesState.end(); it++)
    {
        *it = 0;
    }

    NodeP2 node(0, joltagesState, "");

    std::deque<NodeP2> q;
    Visited visited;

    // Place the starting point on the Q
    q.emplace_back(node);

    while (!q.empty())
    {
        auto current = q.front();
        q.pop_front();

        std::string key = current.asString();
        auto seen = visited.find(key);

        if (seen != visited.end())
        {
            // if it has been seen before it is certainly not the shortest solution
            continue;
        }

        visited.emplace(key);

        if (current.joltages() == joltages())
        {
            // Solved!
            return current.score();
        }


        int buttonId = 0;
        // Iterate through all the buttons and apply what they do to the lights
        for (auto button : buttons())
        {
            auto joltages = current.joltages();
            for (auto& joltageId : button)
            {
                // toggle the lights that are controlled by the button
                joltages[joltageId] += 1;
            }

            // Add new node
            NodeP2 node(current.score() + 1, joltages, current.path() + ":" + std::to_string(buttonId));
            q.emplace_back(node);

            ++buttonId;
        }
    }


    return 0;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Machines machines;

    readInputFile(inputFileName, machines);

    if (machines.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber result = 0;

            auto machinesP1(machines);

            int loopcount = 0;
            for (auto& machine : machinesP1)
            {
                result += machine.solveP1();
                loopcount++;
                std::cout << "Loop count :" << std::to_string(loopcount) << std::endl;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber result = 0;

            auto machinesP2(machines);

            int loopcount = 0;
            for (auto& machine : machinesP2)
            {
                result += machine.solveP2();
                loopcount++;
                std::cout << "Loop count :" << std::to_string(loopcount) << std::endl;
            }

            std::cout << "Total: " << result;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}
